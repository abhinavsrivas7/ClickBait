using System.Runtime.InteropServices;
using System.Text.Unicode;

namespace ClickBait.Color
{
    [StructLayout(LayoutKind.Sequential)]
    public unsafe struct PaletteCache
    {
        internal int Size;

        internal fixed byte Cache[_cacheSize];

        private const int _cacheSize = 8192;
        private const byte _gamutSpread = 20; 

        internal void Init()
        {
            Size = 0;
            byte index = 0;
            Span<byte> cacheSpan = MemoryMarshal.CreateSpan(ref Cache[0], _cacheSize);

            for(byte r = 0; r < Color.ChannelSize; r++)
            {
                for(byte g = 0; g < Color.ChannelSize; g++)
                {
                    for (byte b = 0; b < Color.ChannelSize; b++, index++)
                    {
                        bool success = Utf8.TryWrite(
                            cacheSpan,
                            $"#{index};2;{r * _gamutSpread};{g * _gamutSpread};{b * _gamutSpread}",
                            out int written
                        );

                        if (success)
                        {
                            Size += written;
                            cacheSpan = cacheSpan.Slice(written);
                        }
                    }
                }
            }
            
        }
    }
}
