using ClickBait.Drawing;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace ClickBait.Platform
{
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    internal unsafe struct ScreenSize
    {
        internal Point<short> Cells;
        internal Point<short> Pixels;
        internal Point<float> CellsPerPixel;
        internal Point<float> PixelsPerCell;
        private const ushort _ansiStart = 1;
        private const ushort _responseBufferSize = 1024;
        private const ushort _eventBufferSize = 256;
        private const ushort _pollingsRetryDelay = 10;


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

        private static bool ExtractSize(
            ReadOnlySpan<char> responseBuffer, ReadOnlySpan<char> sequence, out Point<short> size)
        {
            var index = responseBuffer.IndexOf(sequence);
            if (index != -1)
            {
                var data = responseBuffer.Slice(index + 3);
                var delimeterIndex = data.IndexOf(";");
                var endIndex = data.IndexOf("t");

                if (delimeterIndex != -1 && endIndex != -1)
                {
                    var y = data.Slice(0, delimeterIndex);
                    var x = data.Slice(delimeterIndex + 1, endIndex - delimeterIndex - 1);

                    if (short.TryParse(y, out var yValue) && short.TryParse(x, out var xValue))
                    {
                        size.Y = yValue;
                        size.X = xValue;
                        return true;
                    }
                }
            }
            size.Y = short.MinValue;
            size.X = short.MinValue;
            return false;
        }

        internal void UpdateScreenSize(in Platform platform)
        {
            FlushConsoleInputBuffer(platform.In);
            platform.DumpToConsole(Commands.ExitSixelMode());
            platform.DumpToConsole(Commands.RequestScreenSize());
            char* responseBuffer = stackalloc char[_responseBufferSize];
            int responseBufferIndex = 0;
            Unsafe.SkipInit(out uint eventsRead);
            INPUT_RECORD* eventBuffer = stackalloc INPUT_RECORD[_eventBufferSize];

            while (true)
            {
                if (platform.HasEvents(eventBuffer, &eventsRead))
                {
                    for (uint i = 0; i < eventsRead; i++)
                    {
                        if (Event.IsKeyDownEvent(in eventBuffer[i]))
                        {
                            char c = eventBuffer[i].Event.KeyEvent.uChar.UnicodeChar;
                            if (responseBufferIndex >= _responseBufferSize - 1)
                            {
                                Environment.Exit(6969);
                            }
                            if (c != '\0')
                            {
                                responseBuffer[responseBufferIndex++] = c;
                            }
                        }
                    }

                    ReadOnlySpan<char> response = new(responseBuffer, responseBufferIndex);
                    
                    if (ExtractSize(response, "[4;", out var pixels) && 
                        ExtractSize(response, "[8;", out var cells))
                    {
                        Pixels = pixels;
                        Cells = cells;
                        CellsPerPixel.X = (float)cells.X / pixels.X;
                        CellsPerPixel.Y = (float)cells.Y / pixels.Y;
                        PixelsPerCell.X = 1.0f / CellsPerPixel.X;
                        PixelsPerCell.Y = 1.0f / CellsPerPixel.Y;
                        break;
                    }
                }

                Thread.Sleep(_pollingsRetryDelay);
            }
        }
    };

}
