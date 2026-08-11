namespace ClickBait.Platform
{
    public struct Point<T> where T : unmanaged
    {
        public T X;
        public T Y;
    };

    internal ref struct AABB<T> where T : unmanaged
    {
        internal Point<T> TopLeft;
        internal Point<T> Dimensions;
    };
}
