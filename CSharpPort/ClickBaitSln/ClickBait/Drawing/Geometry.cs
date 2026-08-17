using System.Runtime.InteropServices;

namespace ClickBait.Drawing
{
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct Point<T> where T : unmanaged
    {
        public T X;
        public T Y;
    };
    
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    internal ref struct AABB<T> where T : unmanaged
    {
        internal Point<T> TopLeft;
        internal Point<T> Dimensions;
    };

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public ref struct LineSegment
    {
        public Point<short> A;
        public Point<short> B;
    };
}
