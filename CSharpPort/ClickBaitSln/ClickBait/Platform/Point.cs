using System.Runtime.InteropServices;

namespace ClickBait.Platform
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
}
