// #include "examples.h"

// int experiment()
// {
//     DWORD read;
//     INPUT_RECORD rec;
//     STD std = getHandles();
//     MouseResetEvent mouseEventCallback = { .assigned = false };
//     COORD screenSize = prepare(&std);
//     printf("Window sizekkk: (%d,%d)\n", screenSize.X, screenSize.Y);

//     while(true)
//     {
//         ReadConsoleInputW(std.In, &rec, 1, &read); 

//         switch(rec.EventType)
//         {
//             case KEY_EVENT: 
//             {
//                 KEY_EVENT_RECORD keyEvent = rec.Event.KeyEvent;
//                 if(keyEvent.bKeyDown && keyEvent.uChar.AsciiChar == QUIT_COMMAND) 
//                 {
//                     cleanup(&std, &screenSize, &mouseEventCallback);
//                     return 0;
//                 }
//                 handleKeyEvent(&keyEvent);
//                 break;
//             }
//             case MOUSE_EVENT: 
//                 handleMouseEvent(std.Out, &rec.Event.MouseEvent, &mouseEventCallback);
//                 break;
//             case WINDOW_BUFFER_SIZE_EVENT: 
//                 handleWindowResizeEvent(&rec.Event.WindowBufferSizeEvent, &screenSize);
//                 break;
//         }
//     }
// }