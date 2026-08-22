using System.Runtime.CompilerServices;

namespace ClickBait.Extensions
{
    internal static class NumericExtensions
    {
        extension(byte value)
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            internal bool ToBool() => value != 0;
        }

        extension(int value)
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            internal bool ToBool() => value != 0;
        }
    }
}
