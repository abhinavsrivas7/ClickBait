using System.Runtime.InteropServices;

namespace ClickBait.Draw
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Point<T> where T : unmanaged
    {
        public T X;
        public T Y;
    };
    
    [StructLayout(LayoutKind.Sequential)]
    internal ref struct AABB<T> where T : unmanaged
    {
        internal Point<T> TopLeft;
        internal Point<T> Dimensions;
    };

    [StructLayout(LayoutKind.Sequential)]
    public ref struct LineSegment
    {
        public Point<short> A;
        public Point<short> B;
    };
}
