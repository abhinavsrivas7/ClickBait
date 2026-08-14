using System.Runtime.InteropServices;

namespace ClickBait.Platform
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
}
