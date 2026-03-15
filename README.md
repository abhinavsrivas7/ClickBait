# Run locally on windoows
- install mysys2 and mingw64
- rename file: `./msys64/ucrt64/bin/%make%.exe` to `./msys64/ucrt64/bin/%make%.exe` 
- make run



# Platform layer:
- Control terminal window and modify modes.
- Persist original modes.
- Get dimensions of safe area for rendering.

# DTO Layer:
- Define user dtos
- performs validations on user dtos
    - subtrees should be within the bounds of their local roots
    - supplied dimensions are within max range
- map user inputs to locked screen dimensions
- map user dtos to internal structs

# Events Layer:
- tracks and dispatches events to respective handlers
- allow support for binding event listeners to specific sections of screen

# Change Tracking Layer:
- On the occurrance of events, track the components that became dirty
- Based on the FPS goals, re-encode the dirty components once per frame

# Drawing Layer:
- Uses the shape structs constructed from DTO layer
- Interfaces with Direct2D to create them
- Includes bridge for mapping Direct2D bitmap into pixel pointer
- DX12, DMA copy to readback buffer
- Use Scissor rect to redraw only dirty regions
- Still asks DMA to copy entire target buffer to readback buffer

# Encoding Layer:
- SIMD mode and hardware thread detection
- defines AVX 256 and AVX 512 bindings based on detection results
- Uses SIMD + multi-threading to convert Direct2D buffer into sixel buffer
- Uses Run Length Encoding for sixel buffer compression
- Should allow strip level encoding to allow partial re-renders

# Transmission Layer:
- Maintains a buffer of instructions to be dumped to conPTY
- exposes append and flush abstractions
- uses WriteConsoleA instead of printf for performance
