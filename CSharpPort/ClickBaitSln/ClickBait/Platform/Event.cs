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

        private const ushort _keyEvent = 0x0001;
        private const ushort _mouseEvent = 0x0002;
        private const ushort _windowBufferSizeEvent = 0x0004;
        private static INPUT_RECORD _privateEvent;
        private static uint _read;


        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public readonly bool IsMouseClickedEvent() =>
            (byte)EventType >= (byte)EventTypes.LEFT_MOUSE_CLICKED_EVENT &&
            (byte)EventType <= (byte)EventTypes.RIGHT_MOUSE_CLICKED_EVENT;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static bool IsKeyDownEvent(in INPUT_RECORD record) =>
            record.EventType == _keyEvent && record.Event.KeyEvent.bKeyDown;

        internal void PollForEvents(in Platform that)
        {
            fixed (INPUT_RECORD* pEvent = &_privateEvent)
            {
                fixed (uint* pRead = &_read)
                {
                    if (!ReadConsoleInput(that.In, pEvent, 1, pRead))
                    {
                        EventType = EventTypes.NO_EVENT;
                        return;
                    }
                    if(pEvent->EventType == _windowBufferSizeEvent)
                    {
                        EventType = EventTypes.WINDOW_RESIZED_EVENT;
                        return;
                    }
                    else if(pEvent->EventType == _keyEvent && pEvent->Event.KeyEvent.bKeyDown) 
                    {
                        if (pEvent->Event.KeyEvent.uChar.AsciiChar == Commands.QuitCommand)
                        {
                            EventType = EventTypes.EXIT_EVENT;
                            return;
                        }
                        else
                        {
                            EventType = EventTypes.KEY_PRESSED_EVENT;
                            Character = pEvent->Event.KeyEvent.uChar.AsciiChar;
                            return;
                        }
                    }
                    else if(pEvent->EventType == _mouseEvent)
                    {

                    }
                }
            }
        }
    };
}
