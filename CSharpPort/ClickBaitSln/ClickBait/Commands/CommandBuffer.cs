using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using CONTEXT = ClickBait.Context.Context;
using PLATFORM = ClickBait.Platform.Platform;

namespace ClickBait.Commands
{
    [StructLayout(LayoutKind.Sequential)]
    internal unsafe struct CommandBuffer
    {
        internal CommandBufferMode Mode;
        internal int Count;
        internal fixed byte Buffer[_commandBufferSize];
        private const int _commandBufferSize = 256 * 1024 * 1024; //256MB
        private const int _commandBufferThreshold = 64;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static void Init(CONTEXT* context)
        {
            context->CommandBuffer.Count = 0;
            context->CommandBuffer.Mode = CommandBufferMode.Ansi;
        }

        internal static void Flush(CONTEXT* context)
        {
            if (context->CommandBuffer.Count == 0) return;
            PLATFORM.Dump(context, context->CommandBuffer.Buffer, (uint)context->CommandBuffer.Count);
            context->CommandBuffer.Count = 0;
        }

        internal static void Append(CONTEXT* context, ReadOnlySpan<byte> command, bool skipCheck = false)
        {
            if (command.Length > _commandBufferSize) Environment.Exit(6969);
            CheckCapacityAndFlush(context, command.Length, skipCheck);
            
            Unsafe.CopyBlock(
                ref context->CommandBuffer.Buffer[context->CommandBuffer.Count], 
                ref MemoryMarshal.GetReference(command), 
                (uint)command.Length
            );
            
            context->CommandBuffer.Count += command.Length;
            CheckCapacityAndFlush(context, _commandBufferThreshold, skipCheck);
        }

        internal static void Append(CONTEXT* context, byte c, bool skipCheck = false)
        {
            context->CommandBuffer.Buffer[context->CommandBuffer.Count++] = c;
            CheckCapacityAndFlush(context, _commandBufferThreshold, skipCheck);
        }

        internal static void Append<T>(CONTEXT* context, T value, bool skipCheck = false) where T : IUtf8SpanFormattable
        {
            Span<byte> destination = new(
                context->CommandBuffer.Buffer + context->CommandBuffer.Count, 
                _commandBufferSize - context->CommandBuffer.Count
            );

            if(value.TryFormat(destination, out int written, default, null))
            {
                context->CommandBuffer.Count += written;
                CheckCapacityAndFlush(context, _commandBufferThreshold, skipCheck);
            }
            else
            {
                Environment.Exit(6969);
            }
        }

        internal static void SetMode(CONTEXT* context, CommandBufferMode mode)
        {
            if (context->CommandBuffer.Mode == mode) return;
            if (context->CommandBuffer.Mode == CommandBufferMode.Sixel) Append(context, Commands.ExitSixelMode());
            else Append(context, Commands.EnterSixelMode());
            context->CommandBuffer.Mode = mode;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        private static void CheckCapacityAndFlush(CONTEXT* context, int min, bool skipCheck)
        {
            if (!skipCheck && _commandBufferSize - context->CommandBuffer.Count < min) Flush(context);
        }
    }
}
