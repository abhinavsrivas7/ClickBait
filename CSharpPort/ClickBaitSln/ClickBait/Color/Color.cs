using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace ClickBait.Color
{
    [StructLayout(LayoutKind.Sequential)]
    public ref struct Color
    {
        public byte R;
        public byte G;
        public byte B;

        internal const byte ChannelSize = 6;
        private const byte _channelSquare = ChannelSize * ChannelSize;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public readonly byte GetColorPaletteIndex() => (byte)(R * _channelSquare + G * ChannelSize + B);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Color FromShades(Shade r, Shade g, Shade b) => new() { R = (byte)r, G = (byte)g, B = (byte)b };

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Color Black() => FromShades(Shade.Off, Shade.Off, Shade.Off);
       
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Color White() => FromShades(Shade.Full, Shade.Full, Shade.Full);
        
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Color Red(Shade shade) => FromShades(shade, Shade.Off, Shade.Off);
        
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Color Green(Shade shade) => FromShades(Shade.Off, shade, Shade.Off);
        
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Color Blue(Shade shade) => FromShades(Shade.Off, Shade.Off, shade);
    }
}
