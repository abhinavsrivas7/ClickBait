using System.Runtime.CompilerServices;

namespace ClickBait.Platform
{
    internal unsafe struct Platform
    {
        internal HANDLE In;
        internal HANDLE Out;
        internal nuint InMode;
        internal nuint OutMode;
        internal ScreenSize ScreenSize;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal readonly bool HasEvents(INPUT_RECORD* eventBuffer, uint* eventsRead) =>
            Event.HasEvents(in this, eventBuffer, eventsRead);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal void UpdateScreenSize() => ScreenSize.UpdateScreenSize(in this);

        internal void InitForStartup()
        {

        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal void RestoreForShutdown()
        {
            ChangeIoModes(false);
            SwitchTerminalBuffer(false);
        }

        internal void DumpToConsole(ReadOnlySpan<byte> str)
        {

        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal void ClearScreen()
        {
            DumpToConsole(Commands.ExitSixelMode());
            DumpToConsole(Commands.ClearScreen());
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
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
