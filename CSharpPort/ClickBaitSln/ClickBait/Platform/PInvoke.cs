using System.Runtime.InteropServices;

namespace Windows.Win32
{
    internal static unsafe partial class PInvoke
    {
        [LibraryImport("kernel32.dll")]
        internal static partial int WriteConsoleA(
            void* console_output,
            byte* buffer,
            uint number_of_chars_to_write,
            uint* number_of_chars_written,
            void* reserved
        );   
    }
}
