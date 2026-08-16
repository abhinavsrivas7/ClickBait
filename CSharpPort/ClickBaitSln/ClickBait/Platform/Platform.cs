using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace ClickBait.Platform
{
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public unsafe struct Platform
    {
        internal HANDLE In;
        internal HANDLE Out;
        internal CONSOLE_MODE InMode;
        internal CONSOLE_MODE OutMode;
        internal ScreenSize ScreenSize;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal readonly bool HasEvents(INPUT_RECORD* eventBuffer, uint* eventsRead) =>
            Event.HasEvents(in this, eventBuffer, eventsRead);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal void UpdateScreenSize() => ScreenSize.UpdateScreenSize(in this);

        internal void InitForStartup()
        {
            In = GetStdHandle(STD_HANDLE.STD_INPUT_HANDLE);
            Out = GetStdHandle(STD_HANDLE.STD_OUTPUT_HANDLE);
            GetConsoleMode(In,  (CONSOLE_MODE*)Unsafe.AsPointer(ref InMode));
            GetConsoleMode(Out, (CONSOLE_MODE*)Unsafe.AsPointer(ref OutMode));
            SwitchTerminalBuffer(true);
            ChangeIoModes(true);
            UpdateScreenSize();
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal readonly void RestoreForShutdown()
        {
            ChangeIoModes(false);
            SwitchTerminalBuffer(false);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal readonly void DumpToConsole(ReadOnlySpan<byte> command) => DumpToConsole(
            (byte*)Unsafe.AsPointer(ref MemoryMarshal.GetReference(command)),
            (uint)command.Length
        );

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal readonly void DumpToConsole(byte* command, uint length)
        {
            Unsafe.SkipInit(out uint written);
            WriteConsoleA(Out, command, length, &written, null);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal readonly void ClearScreen()
        {
            DumpToConsole(Commands.ExitSixelMode());
            DumpToConsole(Commands.ClearScreen());
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private readonly void SwitchTerminalBuffer(bool isStartup)
        {
            DumpToConsole(Commands.ExitSixelMode());
            
            DumpToConsole(isStartup 
                ? Commands.SwitchToAlternateBuffer() 
                : Commands.SwitchToOriginalBuffer()
            );
        }

        private readonly void ChangeIoModes(bool isStartup)
        {
            var inMode = InMode;
            var outMode = OutMode;

            if(isStartup)
            {
                inMode |= CONSOLE_MODE.ENABLE_MOUSE_INPUT 
                       |  CONSOLE_MODE.ENABLE_WINDOW_INPUT 
                       |  CONSOLE_MODE.ENABLE_EXTENDED_FLAGS;

                inMode &= ~(
                    CONSOLE_MODE.ENABLE_QUICK_EDIT_MODE |
                    CONSOLE_MODE.ENABLE_LINE_INPUT      |
                    CONSOLE_MODE.ENABLE_ECHO_INPUT      |
                    CONSOLE_MODE.ENABLE_VIRTUAL_TERMINAL_INPUT
                );

                outMode |= CONSOLE_MODE.ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            }
            
            SetConsoleMode(In, inMode);
            SetConsoleMode(Out, outMode);
            ToggleCursor(!isStartup);
            ClearScreen();
        }

        private readonly void ToggleCursor(bool enable)
        {
            Unsafe.SkipInit(out CONSOLE_CURSOR_INFO cursorInfo);
            GetConsoleCursorInfo(Out, &cursorInfo);
            cursorInfo.bVisible = new BOOL(enable);
            SetConsoleCursorInfo(Out, &cursorInfo);
        }
    };
}
