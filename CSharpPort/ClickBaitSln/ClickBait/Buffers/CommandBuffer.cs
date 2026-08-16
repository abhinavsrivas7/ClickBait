using ClickBait.Platform;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using PLATFORM = ClickBait.Platform.Platform;

namespace ClickBait.Buffers
{
    public enum CommandBufferMode: byte { Ansi = 0, Sixel = 1 }

    [StructLayout(LayoutKind.Sequential)]
    public unsafe struct CommandBuffer
    {
        public CommandBufferMode Mode;
        public int Count;
        public fixed byte Buffer[_commandBufferSize];
        private const int _commandBufferSize = 256 * 1024 * 1024;
        private const int _commandBufferThreshold = 64;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal void Init()
        {
            Count = 0;
            Mode = CommandBufferMode.Ansi;
        }

        internal void Flush(in PLATFORM platform)
        {
            if (Count == 0) return;
            platform.DumpToConsole((byte*)Unsafe.AsPointer(ref Buffer[0]), (uint)Count);
            Count = 0;
        }

        internal void AppendS(in PLATFORM platform, ReadOnlySpan<byte> command)
        {
            if (command.Length > _commandBufferSize) Environment.Exit(6969);
            CheckCapacityAndFlush(in platform, command.Length);
            Unsafe.CopyBlock(ref Buffer[Count], ref MemoryMarshal.GetReference(command), (uint)command.Length);
            Count += command.Length;
            CheckCapacityAndFlush(in platform, _commandBufferThreshold);
        }

        internal void AppendC(in PLATFORM platform, byte c)
        {
            Buffer[Count++] = c;
            CheckCapacityAndFlush(in platform, _commandBufferThreshold);
        }

        internal void SetMode(in PLATFORM platform, CommandBufferMode mode)
        {
            if (Mode == mode) return;
            if (Mode == CommandBufferMode.Sixel) AppendS(in platform, Commands.ExitSixelMode());
            else AppendS(in platform, Commands.EnterSixelMode());
            Mode = mode;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private void CheckCapacityAndFlush(in PLATFORM platform, int min)
        {
            if (_commandBufferSize - Count < min) Flush(in platform);
        }
    }
}
