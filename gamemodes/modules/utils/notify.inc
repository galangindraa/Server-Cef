stock ShowNotification(playerid, const id[] = "default", const title[], const description[], const type[] = "info", duration = 3000, bool:showDuration = false, const position[] = "top-right", const icon[] = "", const iconColor[] = "")
{    
    new jsonFormat[512];
    new showDurationStr[6];
    format(showDurationStr, sizeof(showDurationStr), "%s", showDuration ? "true" : "false");
    
    format(jsonFormat, sizeof(jsonFormat),
        "{\"id\":\"%s\",\"type\":\"%s\",\"title\":\"%s\",\"description\":\"%s\",\"duration\":%d,\"position\":\"%s\",\"icon\":\"%s\",\"iconColor\":\"%s\",\"showDuration\":%s}",
        id,
        type,
        title,
        description,
        duration,
        position,
        icon,
        iconColor,
        showDurationStr
    );
    return cef_emit_event(playerid, "ui:notify", CEFSTR(jsonFormat));
}