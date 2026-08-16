using ClickBait.Platform;
using CONTEXT = ClickBait.Context;

namespace ClickBait
{
    public static unsafe class EntryPoint
    {
        public static int EventLoop()
        {
            CONTEXT* context = CONTEXT.Startup();
            Event @event = new();

            while (true)
            {
                @event.PollForEvents(in context->Platform);
                if (@event.EventType == EventTypes.EXIT_EVENT)
                {
                    context->ShutDown();
                    break;
                }
            }
            return 0;
        }
    }
}
