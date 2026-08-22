using System.Runtime.CompilerServices;
using CMD = ClickBait.Commands.Commands;
using CONTEXT = ClickBait.Context.Context;

namespace ClickBait.Platform
{
    internal unsafe partial struct Platform
    {
        internal static void Init(CONTEXT* context)
        {
            context->Platform.InHandle = GetStdHandle(STD_HANDLE.STD_INPUT_HANDLE);
            context->Platform.OutHandle = GetStdHandle(STD_HANDLE.STD_OUTPUT_HANDLE);
            GetConsoleMode(context->Platform.InHandle, &context->Platform.InMode);
            GetConsoleMode(context->Platform.OutHandle, &context->Platform.OutMode);
            SwitchTerminalBuffer(context, true);
            ChangeIoModes(context, true);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static void SwitchTerminalBuffer(CONTEXT* context, bool isStartup)
        {
            Dump(context, CMD.ExitSixelMode());
            Dump(context, isStartup ? CMD.SwitchToAlternateBuffer() : CMD.SwitchToOriginalBuffer());
        }

        internal static void ChangeIoModes(CONTEXT* context, bool isStartup)
        {
            var inMode = context->Platform.InMode;
            var outMode = context->Platform.OutMode;

            if (isStartup)
            {
                inMode |= CONSOLE_MODE.ENABLE_MOUSE_INPUT
                       | CONSOLE_MODE.ENABLE_WINDOW_INPUT
                       | CONSOLE_MODE.ENABLE_EXTENDED_FLAGS;

                inMode &= ~(
                    CONSOLE_MODE.ENABLE_QUICK_EDIT_MODE |
                    CONSOLE_MODE.ENABLE_LINE_INPUT |
                    CONSOLE_MODE.ENABLE_ECHO_INPUT |
                    CONSOLE_MODE.ENABLE_VIRTUAL_TERMINAL_INPUT
                );

                outMode |= CONSOLE_MODE.ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            }

            SetConsoleMode(context->Platform.InHandle, inMode);
            SetConsoleMode(context->Platform.OutHandle, outMode);
            ToggleCursor(context, !isStartup);
            ClearScreen(context);
        }

        internal static void ToggleCursor(CONTEXT* context, bool enable)
        {
            Unsafe.SkipInit(out CONSOLE_CURSOR_INFO cursorInfo);
            GetConsoleCursorInfo(context->Platform.OutHandle, &cursorInfo);
            cursorInfo.bVisible = new BOOL(enable);
            SetConsoleCursorInfo(context->Platform.OutHandle, &cursorInfo);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static void Shutdown(CONTEXT* context)
        {
            ChangeIoModes(context, false);
            SwitchTerminalBuffer(context, false);
        }
    }
}
