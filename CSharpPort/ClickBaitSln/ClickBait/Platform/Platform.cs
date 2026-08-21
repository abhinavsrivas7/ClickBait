using ClickBait.Drawing;
using ClickBait.Events;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using CMD = ClickBait.Commands.Commands;
using CONTEXT = ClickBait.Context.Context;

namespace ClickBait.Platform
{
    [StructLayout(LayoutKind.Sequential)]
    internal unsafe struct Platform
    {
        internal HANDLE InHandle;
        internal HANDLE OutHandle;
        internal CONSOLE_MODE InMode;
        internal CONSOLE_MODE OutMode;

        private const ushort _responseBufferSize = 1024;
        private const ushort _updateScreenSizeEventBufferSize = 256;
        private const ushort _pollingRetryDelay = 10;

        private const ushort _keyEvent = 0x0001;
        private const ushort _windowBufferSizeEvent = 0x0004;
        private const ushort _mouseEvent = 0x0002;
        private const uint _leftMouseButton = 0x0001;
        private const uint _rightMouseButton = 0x0002;
        private const uint _mouseWheeled = 0x0004;
        private const ushort _eventBufferSize = 256;

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
        internal static void Dump(CONTEXT* context, ReadOnlySpan<byte> command) => Dump(
            context,
            (byte*)Unsafe.AsPointer(ref MemoryMarshal.GetReference(command)),
            (uint)command.Length
        );

        internal static void Dump(CONTEXT* context, byte* command, uint length)
        {
            Unsafe.SkipInit(out uint written);
            int result = WriteConsoleA(context->Platform.OutHandle, command, length, &written, null);
            if (result == 0) Environment.Exit(6969);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static void SwitchTerminalBuffer(CONTEXT* context, bool isStartup)
        {
            Dump(context, CMD.ExitSixelMode());
            Dump(context, isStartup ? CMD.SwitchToAlternateBuffer() : CMD.SwitchToOriginalBuffer());
        }

        internal static void ToggleCursor(CONTEXT* context, bool enable)
        {
            Unsafe.SkipInit(out CONSOLE_CURSOR_INFO cursorInfo);
            GetConsoleCursorInfo(context->Platform.OutHandle, &cursorInfo);
            cursorInfo.bVisible = new BOOL(enable);
            SetConsoleCursorInfo(context->Platform.OutHandle, &cursorInfo);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static void ClearScreen(CONTEXT* context)
        {
            Dump(context, CMD.ExitSixelMode());
            Dump(context, CMD.ClearScreen());
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

        private static bool Extract(ReadOnlySpan<char> buffer, ReadOnlySpan<char> sequence, out Point<short> size)
        {
            var index = buffer.IndexOf(sequence);

            if (index != -1)
            {
                var data = buffer.Slice(index + 3);
                var delimeterIndex = data.IndexOf(";");
                var endIndex = data.IndexOf("t");

                if (delimeterIndex != -1 && endIndex != -1)
                {
                    var y = data.Slice(0, delimeterIndex);
                    var x = data.Slice(delimeterIndex + 1, endIndex - delimeterIndex - 1);

                    if (short.TryParse(y, out var yValue) && short.TryParse(x, out var xValue))
                    {
                        size.Y = yValue;
                        size.X = xValue;
                        return true;
                    }
                }
            }

            size.Y = short.MinValue;
            size.X = short.MinValue;
            return false;
        }

        internal static void UpdateScreenSize(CONTEXT* context)
        {
            FlushConsoleInputBuffer(context->Platform.InHandle);
            Dump(context, CMD.ExitSixelMode());
            Dump(context, CMD.RequestScreenSize());
            char* responseBuffer = stackalloc char[_responseBufferSize];
            int responseBufferIndex = 0;
            Unsafe.SkipInit(out uint eventsRead);
            INPUT_RECORD* eventBuffer = stackalloc INPUT_RECORD[_updateScreenSizeEventBufferSize];

            while (true)
            {
                if (HasEvents(context, eventBuffer, &eventsRead))
                {
                    for (uint i = 0; i < eventsRead; i++)
                    {
                        if (IsKeyDown(&eventBuffer[i]))
                        {
                            char c = eventBuffer[i].Event.KeyEvent.uChar.UnicodeChar;
                            if (responseBufferIndex >= _responseBufferSize - 1) Environment.Exit(6969);
                            if (c != '\0') responseBuffer[responseBufferIndex++] = c;
                        }
                    }

                    ReadOnlySpan<char> response = new(responseBuffer, responseBufferIndex);

                    if (Extract(response, "[4;", out var pixels) && Extract(response, "[8;", out var cells))
                    {
                        context->ScreenSize.Pixels = pixels;
                        context->ScreenSize.Cells = cells;
                        context->ScreenSize.CellsPerPixel.X = (float)cells.X / pixels.X;
                        context->ScreenSize.CellsPerPixel.Y = (float)cells.Y / pixels.Y;
                        context->ScreenSize.PixelsPerCell.X = 1.0f / context->ScreenSize.CellsPerPixel.X;
                        context->ScreenSize.PixelsPerCell.Y = 1.0f / context->ScreenSize.CellsPerPixel.Y;
                        break;
                    }
                }

                Thread.Sleep(_pollingRetryDelay);
            }
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static bool IsKeyDown(INPUT_RECORD* record) =>
            record->EventType == _keyEvent && record->Event.KeyEvent.bKeyDown;

        internal static bool HasEvents(CONTEXT* context, INPUT_RECORD* buffer, uint* read)
        {
            Unsafe.SkipInit(out uint pendingEvents);

            return GetNumberOfConsoleInputEvents(context->Platform.InHandle, &pendingEvents)
                && pendingEvents > 0
                && ReadConsoleInput(context->Platform.InHandle, buffer, _eventBufferSize, read)
                && *read > 0;
        }

        internal static void PollForEvents(CONTEXT* context, Event* cbEvent)
        {
            Unsafe.SkipInit(out INPUT_RECORD @event);
            Unsafe.SkipInit(out uint read);

            if (!ReadConsoleInput(context->Platform.InHandle, &@event, 1, &read))
            {
                cbEvent->EventType = EventTypes.None;
                return;
            }

            if (@event.EventType == _windowBufferSizeEvent)
            {
                cbEvent->EventType = EventTypes.WindowResized;
                return;
            }

            if (IsKeyDown(&@event))
            {
                if (@event.Event.KeyEvent.uChar.AsciiChar == CMD.Quit)
                {
                    cbEvent->EventType = EventTypes.Exit;
                    return;
                }
                else
                {
                    cbEvent->EventType = EventTypes.KeyPressed;
                    cbEvent->EventData.Character = @event.Event.KeyEvent.uChar.AsciiChar;
                    return;
                }
            }

            if (@event.EventType == _mouseEvent)
            {
                cbEvent->EventData.Position.X = @event.Event.MouseEvent.dwMousePosition.X;
                cbEvent->EventData.Position.Y = @event.Event.MouseEvent.dwMousePosition.Y;

                if ((@event.Event.MouseEvent.dwButtonState & _leftMouseButton) != 0)
                {
                    cbEvent->EventType = EventTypes.LeftMouseClicked;
                    return;
                }

                if ((@event.Event.MouseEvent.dwButtonState & _rightMouseButton) != 0)
                {
                    cbEvent->EventType = EventTypes.RightMouseClicked;
                    return;
                }

                if (@event.Event.MouseEvent.dwEventFlags == _mouseWheeled)
                {
                    short state = (short)(@event.Event.MouseEvent.dwButtonState >> 16);
                    cbEvent->EventType = state > 0 ? EventTypes.MouseWheeledUp : EventTypes.MouseWheeledDown;
                    return;
                }
            }

            cbEvent->EventType = EventTypes.None;
            return;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static void Shutdown(CONTEXT* context)
        {
            ChangeIoModes(context, false);
            SwitchTerminalBuffer(context, false);
        }
    };
}
