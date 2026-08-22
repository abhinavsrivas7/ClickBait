using ClickBait.Draw;
using System.Runtime.InteropServices;

namespace ClickBait.Events
{
    [StructLayout(LayoutKind.Explicit)]
    public ref struct EventData
    {
        [FieldOffset(0)] public Point<short> Position;
        [FieldOffset(0)] public sbyte Character;
    };
}
