using ClickBait.Extensions;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using CONTEXT = ClickBait.Context.Context;

namespace ClickBait.Platform
{
    [StructLayout(LayoutKind.Sequential)]
    internal unsafe partial struct Platform
    {
        internal HANDLE InHandle;
        internal HANDLE OutHandle;
        internal CONSOLE_MODE InMode;
        internal CONSOLE_MODE OutMode;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static void Dump(CONTEXT* context, ReadOnlySpan<byte> command) => Dump(
            context,
            (byte*)Unsafe.AsPointer(ref MemoryMarshal.GetReference(command)),
            (uint)command.Length
        );

        internal static void Dump(CONTEXT* context, byte* command, uint length)
        {
            Unsafe.SkipInit(out uint written);
            int result = WriteConsoleA(context->Platform.OutHandle, command, length, &written, null);
            if (!result.ToBool()) Environment.Exit(6969);
        }
    };
}
