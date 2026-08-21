namespace ClickBait.Events
{
    public enum EventTypes : byte
    {
        None              = 0,
        LeftMouseClicked  = 1,
        RightMouseClicked = 2,
        MouseWheeledUp    = 3,
        MouseWheeledDown  = 4,
        KeyPressed        = 5,
        WindowResized     = 6,
        Exit              = 7
    };
}
