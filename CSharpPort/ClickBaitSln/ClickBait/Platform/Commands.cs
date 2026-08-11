using System.Runtime.CompilerServices;

namespace ClickBait.Platform
{
    internal static class Commands
    {
        internal const byte Sixel_Height                           = 6;
        internal const byte Sixel_Offset                           = 63; //2^6 - 1
        internal const byte MoveToStripStart                       = (byte) '$';
        internal const byte MoveDownByOneStrip                     = (byte)'-';

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static ReadOnlySpan<byte> EnterSixelMode() => "\033Pq\"1;1"u8;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static ReadOnlySpan<byte> ExitSixelMode() => "\033\\"u8;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static ReadOnlySpan<byte> RequestScreenSize() => "\033[14t\033[18t"u8;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static ReadOnlySpan<byte> SwitchToAlternateBuffer() => "\033[?1049h"u8;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static ReadOnlySpan<byte> SwitchToOriginalBuffer() => "\033[?1049l"u8;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        internal static ReadOnlySpan<byte> ClearScreen() => "\x1b[0m\x1b[2J\x1b[1;1H"u8;
    }
}
