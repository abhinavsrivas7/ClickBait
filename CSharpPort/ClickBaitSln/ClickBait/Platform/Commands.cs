using System.Runtime.CompilerServices;

namespace ClickBait.Platform
{
    internal static class Commands
    {
        internal const byte Sixel_Height                             = 6;
        internal const byte Sixel_Offset                             = 63; //2^6 - 1
        internal const byte MoveToStripStart                         = (byte) '$';
        internal const byte MoveDownByOneStrip                       = (byte) '-';
                                                                     
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
    }  
}