using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;
using CONTEXT = ClickBait.Context.Context;

namespace ClickBait.Draw
{
    [StructLayout(LayoutKind.Sequential)]
    public unsafe struct FrameBuffer
    {
        internal fixed byte Frame[_area];
        internal const int Width      = 3840;
        private const int _height     = 2160;
        private const int _area       = Width * _height;
        private const byte _undefined = 216;

        internal byte this[int x, int y]
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get => Frame[y * Width + x];

            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            set => Frame[y * Width + x] = value;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static void Init(CONTEXT* context) => Unsafe.InitBlock(&context->FrameBuffer, _undefined, _area);
    }
}
