using ClickBait.Drawing;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace ClickBait.Context
{
    [StructLayout(LayoutKind.Sequential)]
    internal struct ScreenSize
    {
        internal Point<short> Cells;
        internal Point<short> Pixels;
        internal Point<float> CellsPerPixel;
        internal Point<float> PixelsPerCell;
        private  const ushort _ansiStart = 1;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal readonly Point<short> PixelToCell(Point<short> pixel) => new()
        {
            X = (short)(MathF.Floor(pixel.X * CellsPerPixel.X) + _ansiStart),
            Y = (short)(MathF.Floor(pixel.Y * CellsPerPixel.Y) + _ansiStart)
        };

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal readonly Point<short> CellToTopLeftPixel(Point<short> cell) => new()
        {
            X = (short)((cell.X - _ansiStart) * PixelsPerCell.X),
            Y = (short)((cell.Y - _ansiStart) * PixelsPerCell.Y),
        };

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal readonly Point<short> FindScreenCenter() => new()
        {
            X = (short)(Pixels.X >> 1),
            Y = (short)(Pixels.Y >> 1)
        };
    };
}
