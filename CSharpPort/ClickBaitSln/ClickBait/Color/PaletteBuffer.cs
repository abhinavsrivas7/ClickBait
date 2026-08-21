using CONTEXT = ClickBait.Context.Context;
using System.Runtime.InteropServices;
using System.Text.Unicode;

namespace ClickBait.Color
{
    [StructLayout(LayoutKind.Sequential)]
    internal unsafe struct PaletteBuffer
    {
        internal int Size;
        internal fixed byte Cache[_cacheSize];

        private const int _cacheSize = 8192;
        private const byte _gamutSpread = 20; 

        internal static void Init(CONTEXT* context)
        {
            byte index = 0;
            context->PaletteBuffer.Size = 0;
            Span<byte> cacheSpan = new(&context->PaletteBuffer.Cache, _cacheSize);

            for(byte r = 0; r < Color.ChannelSize; r++)
            {
                for(byte g = 0; g < Color.ChannelSize; g++)
                {
                    for (byte b = 0; b < Color.ChannelSize; b++, index++)
                    {
                        if (Utf8.TryWrite(
                            cacheSpan,
                            $"#{index};2;{r * _gamutSpread};{g * _gamutSpread};{b * _gamutSpread}",
                            out int written
                        ))
                        {
                            context->PaletteBuffer.Size += written;
                            cacheSpan = cacheSpan.Slice(written);
                        }
                        else
                        {
                            Environment.Exit(6969);
                        }
                    }
                }
            }
        }
    }
}
