using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using PLATFORM = ClickBait.Platform.Platform;

namespace ClickBait.Context
{
    [StructLayout(LayoutKind.Sequential)]
    public unsafe struct Context
    {
        public PLATFORM Platform;

        public static Context* Startup()
        {
            Context* context = (Context*)NativeMemory.Alloc((nuint)sizeof(Context));
            context->Platform.InitForStartup();
            return context;
        }

        public void ShutDown()
        {
            Platform.RestoreForShutdown();
            NativeMemory.Free(Unsafe.AsPointer(ref this));
        }
    }
}
