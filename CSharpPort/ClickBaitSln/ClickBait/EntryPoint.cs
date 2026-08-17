using ClickBait.Platform;

namespace ClickBait
{
    public static unsafe class EntryPoint
    {
        public static int EventLoop()
        {
            Context* context = Context.Startup();
            Event @event = new();

            while (true)
            {
                @event.PollForEvents(in context->Platform);
                if (@event.EventType == EventTypes.Exit)
                {
                    context->ShutDown();
                    break;
                }
            }
            return 0;
        }
    }
}
