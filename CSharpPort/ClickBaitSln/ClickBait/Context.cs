using ClickBait.Buffers;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using PLATFORM = ClickBait.Platform.Platform;

namespace ClickBait
{
    [StructLayout(LayoutKind.Sequential)]
    public unsafe struct Context
    {
        public PLATFORM Platform;
        public CommandBuffer CommandBuffer;
        public PaletteBuffer PaletteBuffer;
        public DrawBuffer DrawBuffer;

        public static Context* Startup()
        {
            Context* context = (Context*)NativeMemory.Alloc((nuint)sizeof(Context));
            context->Platform.Init();
            context->CommandBuffer.Init();
            context->PaletteBuffer.Init();
            context->DrawBuffer.Init();
            return context;
        }

        public void ShutDown()
        {
            Platform.Shutdown();
            NativeMemory.Free(Unsafe.AsPointer(ref this));
        }
    }
}
