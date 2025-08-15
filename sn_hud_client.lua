local ESX = exports['es_extended']:getSharedObject()
local speedMultiplier = Config.useMPH and 2.23694 or 3.6
local showingHUD = true
local inPauseMenu = false
local health = 0
local armor = 0
local isTalking = false
local talkingOnRadio = false
local onRadio = false
local onPhone = false
local voiceRange = 2
local stats = {}
local vehicleStats = {}
local lastFuelUpdate = 0
local lastFuelCheck = nil
local lastCrossroadUpdate = 0
local lastCrossroadCheck = nil
-- Seatbelt logic variables
local previousSpeed = 0
local previousBodyHealth = 0

-- UI Update Functions
local function updatePlayerStats()
    SendNUIMessage({
        action = 'updateStats', 
        data = {
            showing = inPauseMenu == false and showingHUD or false,
            health = health,
            armor = armor,
            isTalking = isTalking,
            talkingOnRadio = talkingOnRadio,
            onRadio = onRadio,
            onPhone = onPhone,
            voiceRange = voiceRange,
            stats = stats
        }
    })
end

local function updateVehicleHUD()
    if not cache.vehicle then return end
    local veh = cache.vehicle
    SendNUIMessage({
        action = 'updateVehicle', 
        data = {
            showing = inPauseMenu == false and showingHUD or false,
            rpm = GetVehicleCurrentRpm(veh),
            speed = math.ceil(GetEntitySpeed(veh) * speedMultiplier),
            fuel = vehicleStats.fuel,
            engineOn = vehicleStats.engineOn,
            beltOn = vehicleStats.beltOn,
        }
    })
end

-- Utility Functions
local function getVehicleFuelLevel(vehicle)
    local updateTick = GetGameTimer()
    if (updateTick - lastFuelUpdate) > 2000 then
        if Entity(vehicle) then
            lastFuelCheck = math.floor(Entity(vehicle).state.fuel or 0)
        else
            lastFuelCheck = math.floor(GetVehicleFuelLevel(vehicle))
        end
    end
    return lastFuelCheck
end

local function getCurrentLocation()
    local updateTick = GetGameTimer()
    if updateTick - lastCrossroadUpdate > 1500 then
        local pos = GetEntityCoords(cache.ped)
        local street1, street2 = GetStreetNameAtCoord(pos.x, pos.y, pos.z)
        lastCrossroadUpdate = updateTick
        local street1 = GetStreetNameFromHashKey(street1)
        local street2 = GetStreetNameFromHashKey(street2)
        if street2 then
            lastCrossroadCheck = street1..' x '..street2
        else
            lastCrossroadCheck = street1
        end
    end
    return lastCrossroadCheck
end

local directions = {"N", "NE", "E", "SE", "S", "SW", "W", "NW", "N"}
local function getCompassDirection(heading)
    local index = math.floor(((heading % 360) + 22.5) / 45) + 1
    return directions[index]
end

-- Modified seatbelt ejection function for easier ejection at lower speeds
local function handleSeatbeltEjection(vehicle, currentSpeed)
    if vehicle and not IsThisModelABike(vehicle) then
        local currentBodyHealth = GetVehicleBodyHealth(vehicle)
        local playerPedId = PlayerPedId()
        
        if vehicleStats.beltOn then
            SetPedConfigFlag(playerPedId, 32, true)
        else
            SetPedConfigFlag(playerPedId, 32, false)
        end
        
        local isVehicleMovingForward = GetEntitySpeedVector(vehicle, true).y > 0.5  -- Reduced from 1.0
        local vehicleAcceleration = (previousSpeed - currentSpeed) / GetFrameTime()
        local speedThreshold = previousSpeed > (20 / 2.237)  -- Reduced from 40 to 20 km/h
        local accelerationThreshold = vehicleAcceleration > 200  -- Reduced from 500 to 200
        local isVehicleDamaged = false
        local bodyHealthChange = previousBodyHealth - currentBodyHealth
        
        if currentBodyHealth < 1000 and bodyHealthChange > 5.0 then  -- Reduced from 10.0 to 5.0
            isVehicleDamaged = true
        end
        
        if not vehicleStats.beltOn and (isVehicleMovingForward and speedThreshold and accelerationThreshold and isVehicleDamaged) then
            local playerPed = PlayerPedId()
            local veh = GetVehiclePedIsIn(playerPed, false)
            local coords = GetOffsetFromEntityInWorldCoords(veh, 1.0, 0.0, 1.0)
            SetEntityCoords(playerPed, coords.x, coords.y, coords.z, true, true, true, false)
            Wait(1)
            SetPedToRagdoll(playerPed, 1000, 1000, 0, false, false, false)
            SetEntityVelocity(playerPed, 0.0, 0.0, 0.0)
        end
        
        previousSpeed = currentSpeed
        previousBodyHealth = currentBodyHealth
    end
end

-- Vehicle Loop
local function startVehicleHUDLoop(veh)
    if Config.noHudVehicles[GetEntityModel(veh)] then return end
    CreateThread(function()
        while veh == cache.vehicle do
            SendNUIMessage({
                action = 'compasstick',
                data  = {
                    direction = getCompassDirection(GetGameplayCamRot(0).z),
                    roads = getCurrentLocation(),
                    zone = GetLabelText(GetNameOfZone(GetEntityCoords(cache.ped))),
                },
            })
            local speed = GetEntitySpeed(veh)
            local stressSpeed = 150
            if speed * 3.6 >= stressSpeed then
                TriggerEvent('esx_status:add', 'stress', math.random(5000, 8000))
            end
            handleSeatbeltEjection(veh, speed)
            updateVehicleHUD()
            Wait(50)
        end
        SendNUIMessage({
            action = 'updateVehicle', 
            data = {showing = false, rpm = 0, speed = 0}
        })
    end)
end

lib.onCache('vehicle', function(value) if value then startVehicleHUDLoop(value) end end)

-- Pause Menu Handler
RegisterNetEvent("esx:pauseMenuActive", function(state)
    Wait(1000)
    inPauseMenu = state
end)

-- Shooting Stress System
CreateThread(function()
    local sleepTime = 1000
    while true do
        sleepTime = 1000
        if ESX.IsPlayerLoaded() then
            local ped = cache.ped
            local weapon = cache.weapon
            if weapon ~= false then
                sleepTime = 100
                if IsPedShooting(ped) then
                    TriggerEvent('esx_status:add', 'stress', math.random(10000, 15000))
                end
            end
        end
        Wait(sleepTime)
    end
end)

-- Stress Effect Functions
local function getBlurIntensity(stressLevel)
    for _, v in pairs(Config.Intensity['blur']) do
        if stressLevel >= v.min and stressLevel <= v.max then
            return v.intensity
        end
    end
    return 1500
end

local function getStressEffectInterval(stressLevel)
    for _, v in pairs(Config.EffectInterval) do
        if stressLevel >= v.min and stressLevel <= v.max then
            return v.timeout
        end
    end
    return 60000
end

local function playStressSound()
    exports.xsound:PlayUrl("./sounds/beat", "./sounds/beat.mp3", 0.3, true)
end

local function stopStressSound()
    exports.xsound:Destroy("./sounds/beat")
end

local function toggleStressEffects(enable)
    if enable then
        StartScreenEffect("DrugsMichaelAliensFightIn", 3.0, 0)
        Wait(1000)
        StartScreenEffect("DrugsMichaelAliensFight", 3.0, 0)
    else
        StopScreenEffect("DrugsMichaelAliensFightIn")
        StopScreenEffect("DrugsMichaelAliensFight")
        SetTimecycleModifier('default')
    end
end

-- Main Status Update Handler
RegisterNetEvent("esx_status:onTick", function(data)
    local ped = cache.ped
    local playerId = cache.playerId
    health = math.floor((GetEntityHealth(ped) - 100)/(GetEntityMaxHealth(ped) - 100)*100)
    armor = GetPedArmour(ped)
    isTalking = NetworkIsPlayerTalking(playerId) == 1
    onRadio = LocalPlayer.state['radioChannel'] > 0
    onPhone = LocalPlayer.state['callChannel'] > 0        

    local hunger, thirst, stress
    for k, v in pairs(data) do
        if k == "thirst" then
            thirst = (v / 1000000) * 100
        end
        if k == "hunger" then
            hunger = (v / 1000000) * 100
        end
        if k == "stress" then
            stress = (v / 1000000) * 100
        end
    end
    stats.health = health
    stats.hunger = hunger
    stats.thirst = thirst
    stats.stress = stress
    
    if cache.vehicle and not IsThisModelABicycle(cache.vehicle) then
        vehicleStats.fuel = getVehicleFuelLevel(cache.vehicle)
        vehicleStats.engine = (GetVehicleEngineHealth(cache.vehicle) / 10) < 50
        vehicleStats.engineOn = GetIsVehicleEngineRunning(cache.vehicle)
        DisplayRadar(true)
    else
        DisplayRadar(Config.minimapWalking)
    end
    updatePlayerStats()
end)

-- Stress Effect Loop
CreateThread(function()
    while ESX.IsPlayerLoaded() do
        local ped = cache.ped
        if stats.stress == nil then
            Wait(1000)
            goto continue
        end
        
        local effectInterval = getStressEffectInterval(stats.stress)
        
        if stats.stress >= Config.MinimumStress then
            local blurIntensity = getBlurIntensity(stats.stress)
            local fallRepeat = math.random(2, 4)
            local ragdollTimeout = fallRepeat * 1750
            TriggerScreenblurFadeIn(1000.0)
            ShakeGameplayCam('SMALL_EXPLOSION_SHAKE', 0.08)
            Wait(blurIntensity)
            TriggerScreenblurFadeOut(1000.0)
            
            if not IsPedRagdoll(ped) and IsPedOnFoot(ped) and not IsPedSwimming(ped) then
                SetPedToRagdollWithFall(ped, ragdollTimeout, ragdollTimeout, 1, GetEntityForwardVector(ped), 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0)
            end
            
            playStressSound()
            toggleStressEffects(true)
        elseif stats.stress <= Config.MinimumStress then
            stopStressSound()
            toggleStressEffects(false)
        end
        Wait(effectInterval)
        ::continue::
    end
end)

-- Voice System Events
AddEventHandler("pma-voice:setTalkingMode", function(mode)
    voiceRange = tonumber(mode)
    updatePlayerStats()
end)

AddEventHandler("pma-voice:radioActive", function(radioTalking)
    talkingOnRadio = radioTalking
    updatePlayerStats()
end)

-- Seatbelt Functions with mandatory usage
local function toggleSeatbelt()
    if cache.vehicle and IsThisModelABike(cache.vehicle) then
        return
    end
    -- Make seatbelt mandatory - always keep it on when in vehicle
    if cache.vehicle then
        vehicleStats.beltOn = true
        lib.notify({title = 'Seatbelt is mandatory in vehicles', type = 'inform'})
    else
        vehicleStats.beltOn = false
        lib.notify({title = 'Seatbelt Off', type = 'error'})
    end
    updateVehicleHUD()
end

exports("SeatbeltState", function(...)
    toggleSeatbelt()
end)

RegisterNetEvent('seatbelt:client:ToggleSeatbelt', function()
    if cache.vehicle and IsThisModelABike(cache.vehicle) then
        return
    end
    -- Make seatbelt mandatory - always keep it on when in vehicle
    if cache.vehicle then
        vehicleStats.beltOn = true
        lib.notify({title = 'Seatbelt is mandatory in vehicles', type = 'inform'})
    else
        vehicleStats.beltOn = false
        lib.notify({title = 'Seatbelt Off', type = 'error'})
    end
    TriggerEvent("InteractSound_CL:PlayOnOne", "seatbelt", 1)
    updateVehicleHUD()
end)

RegisterNetEvent('hud:client:ToggleShowSeatbelt', function()
    if cache.vehicle and IsThisModelABike(cache.vehicle) then
        return
    end
    -- Make seatbelt mandatory - always keep it on when in vehicle
    if cache.vehicle then
        vehicleStats.beltOn = true
    else
        vehicleStats.beltOn = false
    end
    TriggerEvent("InteractSound_CL:PlayOnOne", "seatbelt", 1)
    updateVehicleHUD()
end)

RegisterCommand("0r-hud:ToggleSeatbelt", function()
    if cache.vehicle and IsThisModelABike(cache.vehicle) then
        return
    end
    -- Make seatbelt mandatory - always keep it on when in vehicle
    if cache.vehicle then
        vehicleStats.beltOn = true
        lib.notify({title = 'Seatbelt is mandatory in vehicles', type = 'inform'})
    else
        vehicleStats.beltOn = false
        lib.notify({title = 'Seatbelt Off', type = 'error'})
    end
    TriggerEvent("InteractSound_CL:PlayOnOne", "seatbelt", 1)
    updateVehicleHUD()
end, false)

RegisterKeyMapping("0r-hud:ToggleSeatbelt", "Toggle Seatbelt", 'keyboard', "B")

-- Auto-enable seatbelt when entering vehicle
lib.onCache('vehicle', function(value)
    if value and not IsThisModelABike(value) then
        -- Automatically enable seatbelt when entering a vehicle
        vehicleStats.beltOn = true
        lib.notify({title = 'Seatbelt automatically enabled', type = 'success'})
        updateVehicleHUD()
    end
end) 