using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace ClickBait.Buffers
{
    [StructLayout(LayoutKind.Sequential, Pack  = 1)]
    public unsafe struct DrawBuffer
    {        
        public fixed byte Frame[_frameDimensions];
        private const int _frameWidth = 3840;
        private const int _frameHeight = 2160;
        private const int _frameDimensions = _frameWidth * _frameHeight;
        internal const byte _undefinedPixel = 216;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal void Init() => Unsafe.InitBlock(ref Frame[0], _undefinedPixel, _frameDimensions);
        
        public byte this[int x, int y]
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get => Frame[y * _frameWidth + x];

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            set => Frame[y * _frameWidth + x] = value;
        }
    }
}
