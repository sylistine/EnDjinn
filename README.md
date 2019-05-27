# EnDjinn - A DirectX 12 Rendering Engine

Originally a research project, EnDjinn is a 3d renderer built in the style of a video game engine with no concern for legacy graphics APIs.

## Guidelines

PRs are welcome to add and extend the features of this engine. However, a goal of this project is to achieve understanding. Consequently, this repository will not take on other third party dependencies, licensed or non (except, perhaps, in a *strictly* temporary way).
Of course, since we leverage API's like DX12 and Vulkan, and must run upon platforms like Windows, this restriction doesn't include platform or hardware APIs and drivers.

## Conventions

**Throw-based Error Handling** - Prefer `throw` and `catch` to error codes. The `Djn::Exception` class builds a stack trace when it is constructed to make this error handling more useful.

**Smart pointers (almost) always** - Simplify your code and make the application clean itself up when things go awry. This is also critical because we use `throw`. If `unique_ptr` or some other smart pointer doesn't do what you need, I encourage you to create your own RAII-based NOTHROW smart pointer.

## Non-Graphics Engine Features

- "Robust" Logging
- Custom exception handling w/ stack traces
Sample exception logging with stack trace.
```
EnDjinn v3.3

Enumerating Graphics Hardware Results...
Adapter: NVIDIA GeForce GTX 1080
- Output: \\.\DISPLAY1
Adapter: Microsoft Basic Render Driver

Djinn Exception caught --------------------------------

Sample exception

Stack Trace:
d:\aaron\dev\src\01solutions\endjinn\djinn\                          debug.h:86    Djn::Exception::GetStackTrace
d:\aaron\dev\src\01solutions\endjinn\djinn\                          debug.h:48    Djn::Exception::Exception
d:\aaron\dev\src\01solutions\endjinn\djinn\                          gfx.cpp:108   Djn::Gfx::Gfx
d:\aaron\dev\src\01solutions\endjinn\djinn\                            gfx.h:30    Djn::Gfx::Instance
d:\aaron\dev\src\01solutions\endjinn\djinn\                          app.cpp:43    Djn::App::App
d:\aaron\dev\src\01solutions\endjinn\djinn\                        djinn.cpp:22    WinMain
d:\agent\_work\1\s\src\vctools\crt\vcstartup\src\startup\     exe_common.inl:107   invoke_main
d:\agent\_work\1\s\src\vctools\crt\vcstartup\src\startup\     exe_common.inl:288   __scrt_common_main_seh
d:\agent\_work\1\s\src\vctools\crt\vcstartup\src\startup\     exe_common.inl:331   __scrt_common_main
d:\agent\_work\1\s\src\vctools\crt\vcstartup\src\startup\    exe_winmain.cpp:17    WinMainCRTStartup

-------------------------------------------------------

Closing Djinn.
```

- A template-based c#-style event and action system

## Development Schedule

### High Priority
- Quaternion Logic, and a complete Transform class.
- Complete a full, simple render pipeline.
  - Scene-based VBO generation and camera constant buffer definition
  - Simple vertex and fragment shading.

### Next
- Create model and texture importers
- Implement Vulkan

### Future
- VR Support
- Mobile support

### Someday

- Audio
- Physics
