using ClickBait.Buffers;
using ClickBait.Color;
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
        public PaletteCache PaletteCache;
        public DrawBuffer DrawBuffer;

        public static Context* Startup()
        {
            Context* context = (Context*)NativeMemory.Alloc((nuint)sizeof(Context));
            context->Platform.InitForStartup();
            context->CommandBuffer.Init();
            context->PaletteCache.Init();
            context->DrawBuffer.Init();
            return context;
        }

        public void ShutDown()
        {
            Platform.RestoreForShutdown();
            NativeMemory.Free(Unsafe.AsPointer(ref this));
        }
    }
}
