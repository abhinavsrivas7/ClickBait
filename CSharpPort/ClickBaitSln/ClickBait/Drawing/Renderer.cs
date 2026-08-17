using ClickBait.Buffers;

namespace ClickBait.Drawing
{
    public static unsafe class Renderer
    {
        public static void NavigateToCell(Context* context, Point<short> cell)
        {
            context->CommandBuffer.SetMode(in context->Platform, CommandBufferMode.Ansi);
        }
    }
}
