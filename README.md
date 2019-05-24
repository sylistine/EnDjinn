# EnDjinn
A DirectX 12 Rendering Engine

Originally a research project, EnDjinn is a 3d renderer built in the style of a video game engine with no concern for legacy graphics APIs.

## Guidelines

PRs are welcome to add and extend the features of this engine. However, a goal of this project is to achieve understanding. Consequently, this repository will not take on other third party dependencies, licensed or non (except, perhaps, in a *strictly* temporary way).
Of course, since we leverage API's like DX12 and Vulkan, and must run upon platforms like Windows, this restriction doesn't include platform or hardware APIs and drivers.

## Conventions

**Throw-based Error Handling** - Prefer `throw` and `catch` to error codes. The Djn::Exception class builds a stack trace when it is constructed to make this error handling more useful.

**Smart pointers (almost) always** - Simplify your code and make the application clean itself up when things go awry. This is also critical because we use throw. If `unique_ptr` or some other smart pointer doesn't do what you need, I encourage you to create your own RAII-based NOTHROW smart pointer.

## Non-Graphics Engine Features

Because this is modeled after game engines
- "Robust" Logging
- Custom exception handling w/ stack traces
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
