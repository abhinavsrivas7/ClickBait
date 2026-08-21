using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace ClickBait.Events
{
    [StructLayout(LayoutKind.Sequential)]
    public unsafe ref struct Event
    {
        public EventData EventData;
        public EventTypes EventType;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool IsMouseClicked(Event* cbEvent) =>
            (byte)cbEvent->EventType >= (byte)EventTypes.LeftMouseClicked &&
            (byte)cbEvent->EventType <= (byte)EventTypes.RightMouseClicked;
    };
}
