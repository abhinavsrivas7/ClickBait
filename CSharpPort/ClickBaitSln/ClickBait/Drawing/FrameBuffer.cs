using COLOR = ClickBait.Color.Color;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;
using CONTEXT = ClickBait.Context.Context;

namespace ClickBait.Drawing
{
    [StructLayout(LayoutKind.Sequential)]
    public unsafe struct FrameBuffer
    {
        internal fixed byte Frame[_frameArea];
        private const int _frameWidth = 3840;
        private const int _frameHeight = 2160;
        private const int _frameArea = _frameWidth * _frameHeight;
        private const byte _undefined = 216;

        internal byte this[int x, int y]
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get => Frame[y * _frameWidth + x];

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            set => Frame[y * _frameWidth + x] = value;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static void Init(CONTEXT* context) => Unsafe.InitBlock(&context->FrameBuffer, _undefined, _frameArea);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void DrawPoint(CONTEXT* context, Point<short> point, COLOR color)
        {
            if (point.X >= 0 &&
               point.X < context->ScreenSize.Pixels.X &&
               point.Y >= 0 &&
               point.Y < context->ScreenSize.Pixels.Y)
            {
                context->FrameBuffer[point.X, point.Y] = color.GetColorPaletteIndex();
            }
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void DrawLine(CONTEXT* context, LineSegment line, COLOR color)
        {
            if (line.A.X == line.B.X && line.A.Y == line.B.Y)
            {
                DrawPoint(context, line.A, color);
            }
            else if (line.A.X == line.B.X)
            {
                DrawVerticalLine(context, line, color);
            }
            else if (line.A.Y == line.B.Y)
            {
                DrawHorizontalLine(context, line, color);
            }
        }

        private static void DrawHorizontalLine(CONTEXT* context, LineSegment line, COLOR color)
        {
            short start = Math.Min(line.A.X, line.B.X), end = Math.Max(line.A.X, line.B.X);
            byte index = color.GetColorPaletteIndex();

            for (short i = start; i <= end; i++)
            {
                context->FrameBuffer[i, line.A.Y] = index;
            }
        }

        private static void DrawVerticalLine(CONTEXT* context, LineSegment line, COLOR color)
        {
            short start = Math.Min(line.A.Y, line.B.Y), end = Math.Max(line.A.Y, line.B.Y);
            byte index = color.GetColorPaletteIndex();

            for (short i = start; i <= end; i++)
            {
                context->FrameBuffer[line.A.X, i] = index;
            }
        }
    }
}
