using System.Runtime.CompilerServices;
using CONTEXT = ClickBait.Context.Context;
using COLOR = ClickBait.Color.Color;
using CMD = ClickBait.Commands.Commands;
using ClickBait.Commands;
using ClickBait.Extensions;

namespace ClickBait.Draw
{
    public static unsafe class Drawing
    {
        #region Point

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void NavigateToCell(CONTEXT* context, Point<short> cell)
        {
            CommandBuffer.SetMode(context, CommandBufferMode.Ansi);
            CMD.NavigateToCell(context, cell);
        }

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

        #endregion

        #region Line

        private const byte _inside = 0; // 0000
        private const byte _left   = 1; // 0001
        private const byte _right  = 2; // 0010
        private const byte _bottom = 4; // 0100
        private const byte _top    = 8; // 1000

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void DrawLine(CONTEXT* context, LineSegment line, COLOR color)
        {
            if (!ClipLine(context, &line)) return;
            if (line.A.X == line.B.X && line.A.Y == line.B.Y) DrawPoint(context, line.A, color);
            else if (line.A.X == line.B.X) DrawVerticalLine(context, line, color);
            else if (line.A.Y == line.B.Y) DrawHorizontalLine(context, line, color);
            else DrawDiagonalLine(context, line, color);
        }

        private static void DrawHorizontalLine(CONTEXT* context, LineSegment line, COLOR color) => Unsafe.InitBlock(
            context->FrameBuffer.Frame + (line.A.Y * FrameBuffer.Width) + Math.Min(line.A.X, line.B.X),
            color.GetColorPaletteIndex(),
            (uint)(Math.Abs(line.A.X - line.B.X) + 1)
        );

        private static void DrawVerticalLine(CONTEXT* context, LineSegment line, COLOR color)
        {
            short start = Math.Min(line.A.Y, line.B.Y);
            short end   = Math.Max(line.A.Y, line.B.Y);
            byte index  = color.GetColorPaletteIndex();
            int offset  = (start * FrameBuffer.Width) + line.A.X;

            for(short i = start; i <= end; i++, offset += FrameBuffer.Width)
            {
                context->FrameBuffer.Frame[offset] = index;
            }
        }

        //Bressenham's line drawing algorithm
        private static void DrawDiagonalLine(CONTEXT* context, LineSegment line, COLOR color)
        {
            Point<short> step = new()
            {
                X = (short)(line.A.X < line.B.X ? 1 : -1),
                Y = (short)(line.A.Y < line.B.Y ? 1 : -1)
            };

            Point<short> delta = new()
            {
                X = (short) Math.Abs(line.A.X - line.B.X),
                Y = (short)-Math.Abs(line.A.Y - line.B.Y)
            };

            short decision;
            short error     = (short)(delta.X + delta.Y);
            byte index      = color.GetColorPaletteIndex();
            int offsetStepY = step.Y * FrameBuffer.Width;
            int offset      = (line.A.Y * FrameBuffer.Width) + line.A.X;

            while(true)
            {
                context->FrameBuffer.Frame[offset] = index;
                decision                           = (short)(2 * error);

                if (line.A.X == line.B.X || line.A.Y == line.B.Y) break;

                if(decision >= delta.Y)
                {
                    error    += delta.Y;
                    line.A.X += step.X;
                    offset   += step.X;
                }

                if(decision <= delta.X)
                {
                    error    += delta.X;
                    line.A.Y += step.Y;
                    offset   += offsetStepY;
                }
            }
        }

        //Cohen-Sutherland line clipping algotithm
        private static bool ClipLine(CONTEXT* context, LineSegment* line)
        {
            short edge;
            short outsideCode;
            Point<short> delta = new();
            Point<short> intersection = new();
            short height = (short)(context->ScreenSize.Pixels.Y - 1);
            short width = (short)(context->ScreenSize.Pixels.X - 1);
            byte codeA = ComputeCode(line->A, context->ScreenSize.Pixels);
            byte codeB = ComputeCode(line->B, context->ScreenSize.Pixels);

            while(true)
            {
                if (!(codeA | codeB).ToBool()) return true;
                if ((codeA & codeB).ToBool()) return false;

                outsideCode = codeA.ToBool() ? codeA : codeB;
                delta.X = (short)(line->B.X - line->A.X);
                delta.Y = (short)(line->B.Y - line->A.Y);

                if((outsideCode & (_top | _bottom)).ToBool())
                {
                    edge = (outsideCode & _top).ToBool() ? (short)0 : height;
                    intersection.X = (short)(line->A.X + (edge - line->A.Y) * delta.X / delta.Y);
                    intersection.Y = edge;
                }
                else if ((outsideCode & (_left | _right)).ToBool())
                {
                    edge = (outsideCode & _left).ToBool() ? (short)0 : width;
                    intersection.Y = (short)(line->A.Y + (edge - line->A.X) * delta.Y / delta.X);
                    intersection.X = edge;
                }

                if(outsideCode == codeA)
                {
                    line->A.X = intersection.X;
                    line->A.Y = intersection.Y;
                    codeA = ComputeCode(line->A, context->ScreenSize.Pixels);
                }
                else
                {
                    line->B.X = intersection.X;
                    line->B.Y = intersection.Y;
                    codeB = ComputeCode(line->B, context->ScreenSize.Pixels);
                }
            }
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private static byte ComputeCode(Point<short> point, Point<short> screen)
        {
            byte code = _inside;

            if      (point.X < 0)         code |= _left;
            else if (point.X >= screen.X) code |= _right;
            if      (point.Y < 0)         code |= _top;
            else if (point.Y >= screen.Y) code |= _bottom;

            return code;
        }

        #endregion
    }
}
