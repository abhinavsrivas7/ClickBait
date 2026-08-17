using ClickBait.Drawing;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace ClickBait.Platform
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

    [StructLayout(LayoutKind.Explicit)]
    public struct EventData
    {
        [FieldOffset(0)] public Point<short> Position;
        [FieldOffset(0)] public sbyte Character;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public unsafe struct Event
    {
        public EventData EventData;
        public EventTypes EventType;
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
            (byte)EventType >= (byte)EventTypes.LeftMouseClicked &&
            (byte)EventType <= (byte)EventTypes.RightMouseClicked;

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
                EventType = EventTypes.None;
                return;
            }
            if (@event.EventType == _windowBufferSizeEvent)
            {
                EventType = EventTypes.WindowResized;
                return;
            }
            if (IsKeyDownEvent(in @event))
            {
                if (@event.Event.KeyEvent.uChar.AsciiChar == _quitCommand)
                {
                    EventType = EventTypes.Exit;
                    return;
                }
                else
                {
                    EventType = EventTypes.KeyPressed;
                    EventData.Character = @event.Event.KeyEvent.uChar.AsciiChar;
                    return;
                }
            }
            if (@event.EventType == _mouseEvent)
            {
                EventData.Position.X = @event.Event.MouseEvent.dwMousePosition.X;
                EventData.Position.Y = @event.Event.MouseEvent.dwMousePosition.Y;

                if ((@event.Event.MouseEvent.dwButtonState & _leftMouseButton) != 0)
                {
                    EventType = EventTypes.LeftMouseClicked;
                    return;
                }
                if ((@event.Event.MouseEvent.dwButtonState & _rightMouseButton) != 0)
                {
                    EventType = EventTypes.RightMouseClicked;
                    return;
                }
                if (@event.Event.MouseEvent.dwEventFlags == _mouseWheeled)
                {
                    short state = (short)HiWord(@event.Event.MouseEvent.dwButtonState);
                    
                    EventType = state > 0
                        ? EventTypes.MouseWheeledUp
                        : EventTypes.MouseWheeledDown;

                    return;
                }
            }

            EventType = EventTypes.None;
            return;
        }
    };
}
