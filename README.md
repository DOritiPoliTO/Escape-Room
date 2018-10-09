# Escape Room

**Escape Room is a videogame developed as a university project for Computer Graphics Course**

## Developers:

- Damiano Oriti (Programming, 3D Modeling and Animation)
- Sara Battelini (3D Modeling and Animation)

## Description

In this videogame, you have to solve some very simple puzzles in order to find the key and escape the castle room you are in.

### Game Features

- Freely move in a 3D environment
- Crouch and jump to reach objects which rest on the floor or on very height places
- Interact with objects using the physics
- Rotate and move objects to examine them closely and from every angle
- Combine objects to use them (e.g. put the key in the keyhole so you may open the door)
- Diary with notes updated when solving puzzles
- Animated sky with clouds

### Graphics Features

- Phong shading
- Environment map reflections
- Real-time shadows with shadow map technique (2D map for directional light and cube map for point light)
- Cascaded shadow maps technique (for directional light)
- Soft shadows
- Font rendering based on [signed distance field](http://www.valvesoftware.com/publications/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf)
- Render to texture

### Other Features

- Multi-threaded video decoding
- Configurator class with related config.ini file
- Batch Allocator class for fast allocation
- Asset Manager to manage assets and avoid duplication of assets in memory (e.g. textures)

[Video]()

## Tools

The game has been developed using [Microsoft Visual Studio](https://visualstudio.microsoft.com/), [Blender](https://www.blender.org/), [GIMP](https://www.gimp.org/) and [Inkscape](https://inkscape.org/).

## Libraries

### Graphics, Window Creation and Input Handling

This software is based on [OpenGL](http://www.opengl.org/) for drawing 2D and 3D graphics.

For window creation and input handling [GLUT](https://www.opengl.org/resources/libraries/glut/) has been used.

### Physics, Video and Image Decoding, etc.

These other libraries have been used:

- [Bullet](https://github.com/bulletphysics/bullet3) for physics calculations
- [libpng](https://github.com/glennrp/libpng) for loading png images
- [FFmpeg](https://github.com/FFmpeg/FFmpeg) for video decoding
- [Assimp](https://github.com/assimp/assimp) for loading 3D model file formats such as fbx

To install the above libraries, you could use [Vcpkg](https://github.com/Microsoft/vcpkg).

### Audio

You also need [fmod](https://en.wikipedia.org/wiki/FMOD) for audio file formats loading and sound reproduction. To install fmod, you need to register on http://www.fmod.com.