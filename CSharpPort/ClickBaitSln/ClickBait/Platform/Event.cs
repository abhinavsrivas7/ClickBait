using System.Runtime.CompilerServices;

namespace ClickBait.Platform
{
    public enum EventTypes : byte
    {
        NO_EVENT                  = 0,
        LEFT_MOUSE_CLICKED_EVENT  = 1,
        RIGHT_MOUSE_CLICKED_EVENT = 2,
        MOUSE_WHEELED_UP_EVENT    = 3,
        MOUSE_WHEELED_DOWN_EVENT  = 4,
        KEY_PRESSED_EVENT         = 5,
        WINDOW_RESIZED_EVENT      = 6,
        EXIT_EVENT                = 7
    };

    public unsafe struct Event
    {
        public EventTypes EventType;
        public sbyte Character;
        public Point<short> Position;

        private const sbyte  _quitCommand           = 27;
        private const ushort _keyEvent              = 0x0001;
        private const ushort _windowBufferSizeEvent = 0x0004;
        private const ushort _mouseEvent            = 0x0002;
        private const uint   _leftMouseButton       = 0x0001;
        private const uint   _rightMouseButton      = 0x0002;
        private const uint   _mouseWheeled          = 0x0004;
        private const ushort _eventBufferSize       = 256;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public readonly bool IsMouseClickedEvent() =>
            (byte)EventType >= (byte)EventTypes.LEFT_MOUSE_CLICKED_EVENT &&
            (byte)EventType <= (byte)EventTypes.RIGHT_MOUSE_CLICKED_EVENT;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static bool IsKeyDownEvent(in INPUT_RECORD record) =>
            record.EventType == _keyEvent && record.Event.KeyEvent.bKeyDown;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private static ushort HiWord(uint state) =>
            (ushort)((((nuint)state) >> 16) & 0xffff);

        internal static bool HasEvents(
            in Platform platform, 
            INPUT_RECORD* eventBuffer, 
            uint* eventsRead)
        {
            Unsafe.SkipInit(out uint pendingEvents);
            
            return GetNumberOfConsoleInputEvents(platform.In, &pendingEvents)
                && pendingEvents > 0
                && ReadConsoleInput(platform.In, eventBuffer, _eventBufferSize, eventsRead)
                && *eventsRead > 0;
        }

        internal void PollForEvents(in Platform platform)
        {
            Unsafe.SkipInit(out INPUT_RECORD @event);
            Unsafe.SkipInit(out uint read);

            if (!ReadConsoleInput(platform.In, &@event, 1, &read))
            {
                EventType = EventTypes.NO_EVENT;
                return;
            }
            if (@event.EventType == _windowBufferSizeEvent)
            {
                EventType = EventTypes.WINDOW_RESIZED_EVENT;
                return;
            }
            if (IsKeyDownEvent(in @event))
            {
                if (@event.Event.KeyEvent.uChar.AsciiChar == _quitCommand)
                {
                    EventType = EventTypes.EXIT_EVENT;
                    return;
                }
                else
                {
                    EventType = EventTypes.KEY_PRESSED_EVENT;
                    Character = @event.Event.KeyEvent.uChar.AsciiChar;
                    return;
                }
            }
            if (@event.EventType == _mouseEvent)
            {
                Position.X = @event.Event.MouseEvent.dwMousePosition.X;
                Position.Y = @event.Event.MouseEvent.dwMousePosition.Y;

                if ((@event.Event.MouseEvent.dwButtonState & _leftMouseButton) != 0)
                {
                    EventType = EventTypes.LEFT_MOUSE_CLICKED_EVENT;
                    return;
                }
                if ((@event.Event.MouseEvent.dwButtonState & _rightMouseButton) != 0)
                {
                    EventType = EventTypes.RIGHT_MOUSE_CLICKED_EVENT;
                    return;
                }
                if (@event.Event.MouseEvent.dwEventFlags == _mouseWheeled)
                {
                    short state = (short)HiWord(@event.Event.MouseEvent.dwButtonState);
                    
                    EventType = state > 0
                        ? EventTypes.MOUSE_WHEELED_UP_EVENT
                        : EventTypes.MOUSE_WHEELED_DOWN_EVENT;

                    return;
                }
            }

            EventType = EventTypes.NO_EVENT;
            return;
        }
    };
}
