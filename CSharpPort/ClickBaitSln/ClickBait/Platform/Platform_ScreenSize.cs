using ClickBait.Draw;
using System.Runtime.CompilerServices;
using CMD = ClickBait.Commands.Commands;
using CONTEXT = ClickBait.Context.Context;

namespace ClickBait.Platform
{
    internal unsafe partial struct Platform
    {
        private const ushort _responseBufferSize              = 1024;
        private const ushort _updateScreenSizeEventBufferSize = 256;
        private const ushort _pollingRetryDelay               = 10;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static void ClearScreen(CONTEXT* context)
        {
            Dump(context, CMD.ExitSixelMode());
            Dump(context, CMD.ClearScreen());
        }

        [SkipLocalsInit]
        internal static void UpdateScreenSize(CONTEXT* context)
        {
            FlushConsoleInputBuffer(context->Platform.InHandle);
            Dump(context, CMD.ExitSixelMode());
            Dump(context, CMD.RequestScreenSize());
            char* responseBuffer = stackalloc char[_responseBufferSize];
            int responseBufferIndex = 0;
            Unsafe.SkipInit(out uint eventsRead);
            INPUT_RECORD* eventBuffer = stackalloc INPUT_RECORD[_updateScreenSizeEventBufferSize];

            while (true)
            {
                if (HasEvents(context, eventBuffer, &eventsRead))
                {
                    for (uint i = 0; i < eventsRead; i++)
                    {
                        if (IsKeyDown(&eventBuffer[i]))
                        {
                            char c = eventBuffer[i].Event.KeyEvent.uChar.UnicodeChar;
                            if (responseBufferIndex >= _responseBufferSize - 1) Environment.Exit(6969);
                            if (c != '\0') responseBuffer[responseBufferIndex++] = c;
                        }
                    }

                    ReadOnlySpan<char> response = new(responseBuffer, responseBufferIndex);

                    if (Extract(response, "[4;", out var pixels) && Extract(response, "[8;", out var cells))
                    {
                        context->ScreenSize.Pixels = pixels;
                        context->ScreenSize.Cells = cells;
                        context->ScreenSize.CellsPerPixel.X = (float)cells.X / pixels.X;
                        context->ScreenSize.CellsPerPixel.Y = (float)cells.Y / pixels.Y;
                        context->ScreenSize.PixelsPerCell.X = 1.0f / context->ScreenSize.CellsPerPixel.X;
                        context->ScreenSize.PixelsPerCell.Y = 1.0f / context->ScreenSize.CellsPerPixel.Y;
                        break;
                    }
                }

                Thread.Sleep(_pollingRetryDelay);
            }
        }

        private static bool Extract(ReadOnlySpan<char> buffer, ReadOnlySpan<char> sequence, out Point<short> size)
        {
            var index = buffer.IndexOf(sequence);

            if (index != -1)
            {
                var data = buffer.Slice(index + 3);
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

    }
}
