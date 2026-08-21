using ClickBait.Events;
using CONTEXT = ClickBait.Context.Context;
using PLATFORM = ClickBait.Platform.Platform;

namespace ClickBait
{
    public static unsafe class EntryPoint
    {
        public static int EventLoop()
        {
            CONTEXT* context = CONTEXT.Startup();
            Event cbEvent = new();

            while (true)
            {
                PLATFORM.PollForEvents(context, &cbEvent);

                if (cbEvent.EventType == EventTypes.Exit)
                {
                    CONTEXT.ShutDown(context);
                    break;
                }
            }
            
            return 0;
        }
    }
}
