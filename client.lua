-- Seatbelt Functions
local function toggleSeatbelt()
    if not cache.vehicle or IsThisModelABike(cache.vehicle) then
        return
    end
    vehicleStats.beltOn = not vehicleStats.beltOn
    if vehicleStats.beltOn then
        lib.notify({title = 'Seatbelt On', type = 'success'})
    else
        lib.notify({title = 'Seatbelt Off', type = 'error'})
    end
    updateVehicleHUD()
end

exports("SeatbeltState", function(...)
    toggleSeatbelt()
end) 
RegisterCommand("0r-hud:ToggleSeatbelt", function()
    -- Check if player is in a vehicle first
    if not cache.vehicle then
        lib.notify({title = 'You must be in a vehicle to use seatbelt', type = 'error'})
        return
    end
    
    -- Check if it's a bike (no seatbelt for bikes)
    if IsThisModelABike(cache.vehicle) then
        lib.notify({title = 'Seatbelts are not available on bikes', type = 'inform'})
        return
    end
    
    -- Toggle seatbelt state
    vehicleStats.beltOn = not vehicleStats.beltOn
    
    if vehicleStats.beltOn then
        lib.notify({title = 'Seatbelt On', type = 'success'})
    else
        lib.notify({title = 'Seatbelt Off', type = 'error'})
    end
    
    TriggerEvent("InteractSound_CL:PlayOnOne", "seatbelt", 1)
    updateVehicleHUD()
end, false) 