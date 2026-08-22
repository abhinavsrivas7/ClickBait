using ClickBait.Color;
using ClickBait.Draw;
using ClickBait.Commands;
using System.Runtime.InteropServices;
using PLATFORM = ClickBait.Platform.Platform;

namespace ClickBait.Context
{
    [StructLayout(LayoutKind.Sequential)]
    public unsafe struct Context
    {
        internal PLATFORM Platform;
        internal CommandBuffer CommandBuffer;
        internal PaletteBuffer PaletteBuffer;
        internal FrameBuffer FrameBuffer;
        internal ScreenSize ScreenSize;

        public static Context* Startup()
        {
            Context* context = (Context*)NativeMemory.Alloc((nuint)sizeof(Context));
            PLATFORM.Init(context);
            PLATFORM.UpdateScreenSize(context);
            CommandBuffer.Init(context);
            PaletteBuffer.Init(context);
            FrameBuffer.Init(context);
            return context;
        }

        public static void ShutDown(Context* context)
        {
            PLATFORM.Shutdown(context);
            NativeMemory.Free(context);
        }
    }
}
