# EnDjinn
A DirectX 12 Rendering Engine

Originally a research project, EnDjinn is a 3d renderer built in the style of a video game engine with no concern for legacy graphics APIs.

PRs are welcome to add and extend the features of this engine. However, a goal of this project is to achieve understanding. Consequently, this repository will not take on other third party dependencies, licensed or non (except, perhaps, in a *strictly* temporary way).

Since we leverage API's like DX12 and Vulkan, and must run upon platforms like Windows, this restriction doesn't include platform or hardware APIs and drivers.

## Non-Graphics Engine Features

Because this is modeled after game engines
- "Robust" Logging
- Custom exception handling w/ stack traces (EnDjinn leverages throw-based error handling)
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
