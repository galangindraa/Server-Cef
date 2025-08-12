CMD:skin(playerid, params[])
{
    showSkinMenu(playerid);
    return 1;
}

CMD:editprop(playerid, params[])
{
    Dialog_Show(playerid, EditPropMenu, DIALOG_STYLE_LIST, "Edit Prop", "Hat\nGlasses\nMask\nBackpack", "Select", "Cancel");
    return 1;
}