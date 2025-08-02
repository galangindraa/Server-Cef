stock showSkinMenu(playerid, bool:whitskin)
{
    new jsonFormat[128];
    format(jsonFormat, sizeof(jsonFormat),
        "{\"whitskin\":%s,\"gender\":\"%s\"}",
        whitskin ? "true" : "false",
        "Male"
    );
    cef_emit_event(playerid, "ui:showSkinMenu", CEFSTR(jsonFormat));
    cef_focus_browser(playerid, INTERFACE_BROWSER_ID, true);
    return 1;
}

CMD:skin(playerid, params[])
{
    showSkinMenu(playerid, true);
    return 1;
}

CMD:skindev(playerid, params[])
{
    cef_toggle_dev_tools(playerid, INTERFACE_BROWSER_ID, true);
    return 1;
}