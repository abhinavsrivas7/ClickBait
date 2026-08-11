namespace ClickBait.Platform
{
    internal unsafe struct Platform
    {
        internal HANDLE In;
        internal HANDLE Out;
        internal nuint InMode;
        internal nuint OutMode;
        internal ScreenSize ScreenSize;

        internal void InitForStartup()
        {

        }

        internal void RestoreForShutdown()
        {
            ChangeIoModes(false);
            SwitchTerminalBuffer(false);
        }

        internal void DumpToConsole(ReadOnlySpan<byte> str)
        {

        }

        internal void ClearScreen()
        {
            DumpToConsole(Commands.ExitSixelMode());
            DumpToConsole(Commands.ClearScreen());
        }

        private void SwitchTerminalBuffer(bool isStartup)
        {
            DumpToConsole(Commands.ExitSixelMode());
            
            DumpToConsole(isStartup 
                ? Commands.SwitchToAlternateBuffer() 
                : Commands.SwitchToOriginalBuffer()
            );
        }

        private void ChangeIoModes(bool isStartup)
        {

        }

        private void ToggleCursor(bool enable)
        {

        }
    };
}
