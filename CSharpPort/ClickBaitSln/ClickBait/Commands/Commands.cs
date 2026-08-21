using ClickBait.Drawing;
using COLOR = ClickBait.Color.Color;
using System.Runtime.CompilerServices;
using CONTEXT = ClickBait.Context.Context;

namespace ClickBait.Commands
{
    internal static unsafe class Commands
    {
        internal const sbyte Quit                                    = 27;
        internal const byte  MoveToStripStart                        = (byte) '$';
        internal const byte  MoveDownByOneStrip                      = (byte) '-';
                                                                     
        [MethodImpl(MethodImplOptions.AggressiveInlining)]           
        internal static ReadOnlySpan<byte> EnterSixelMode()          => "\x1bPq\"1;1"u8;
                                                                     
        [MethodImpl(MethodImplOptions.AggressiveInlining)]           
        internal static ReadOnlySpan<byte> ExitSixelMode()           => "\x1b\\"u8;
                                                                     
        [MethodImpl(MethodImplOptions.AggressiveInlining)]           
        internal static ReadOnlySpan<byte> RequestScreenSize()       => "\x1b[14t\x1b[18t"u8;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static ReadOnlySpan<byte> SwitchToAlternateBuffer() => "\x1b[?1049h"u8;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static ReadOnlySpan<byte> SwitchToOriginalBuffer()  => "\x1b[?1049l"u8;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static ReadOnlySpan<byte> ClearScreen()             => "\x1b[0m\x1b[2J\x1b[1;1H"u8;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static void NavigateToCell(CONTEXT* context, Point<short> cell)
        {
            CommandBuffer.Append(context, "\x1b["u8, true);
            CommandBuffer.Append(context, cell.Y, true);
            CommandBuffer.Append(context, (byte)';', true);
            CommandBuffer.Append(context, cell.X, true);
            CommandBuffer.Append(context, (byte)'H');
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static void MoveRightBy(CONTEXT* context, short columns)
        {
            CommandBuffer.Append(context, (byte)'!', true);
            CommandBuffer.Append(context, columns, true);
            CommandBuffer.Append(context, (byte)'?');
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static void SetColor(CONTEXT* context, byte index, COLOR color)
        {
            UseColor(context, index);
            CommandBuffer.Append(context, ";2;"u8, true);
            CommandBuffer.Append<byte>(context, color.R, true);
            CommandBuffer.Append(context, (byte)';', true);
            CommandBuffer.Append<byte>(context, color.G, true);
            CommandBuffer.Append(context, (byte)';', true);
            CommandBuffer.Append<byte>(context, color.B);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static void UseColor(CONTEXT* context, byte index)
        {
            CommandBuffer.Append(context, (byte)'#', true);
            CommandBuffer.Append<byte>(context, index);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static void RleCommand(CONTEXT* context, short count, byte command)
        {
            CommandBuffer.Append(context, (byte)'!', true);
            CommandBuffer.Append(context, count, true);
            CommandBuffer.Append(context, command);
        }
    }  
}