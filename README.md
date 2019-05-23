# EnDjinn
A DirectX 12 Rendering Engine

Originally a research project, EnDjinn is a 3d renderer built in the style of a video game engine with no concern for legacy graphics APIs.

## Engine Features

Because this is modeled after game engines, I am welcoming PR's for additions and improvements to the following features:
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
