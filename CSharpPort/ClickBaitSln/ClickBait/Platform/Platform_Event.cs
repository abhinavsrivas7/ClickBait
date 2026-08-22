using ClickBait.Events;
using System.Runtime.CompilerServices;
using CMD = ClickBait.Commands.Commands;
using CONTEXT = ClickBait.Context.Context;

namespace ClickBait.Platform
{
    internal unsafe partial struct Platform
    {
        private const ushort _keyEvent              = 0x0001;
        private const ushort _windowBufferSizeEvent = 0x0004;
        private const ushort _mouseEvent            = 0x0002;
        private const uint _leftMouseButton         = 0x0001;
        private const uint _rightMouseButton        = 0x0002;
        private const uint _mouseWheeled            = 0x0004;
        private const ushort _eventBufferSize       = 256;

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
    }
}
