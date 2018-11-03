# Escape Room

**Escape Room is a videogame developed in OpenGL**

### Developers:

- Damiano Oriti (Programming, 3D Modeling and Animation)
- Sara Battelini (3D Modeling and Animation)

## Description

In this videogame, developed for the Computer Graphics course at Politecnico of Torino, you have to solve some very simple puzzles in order to find the key and escape the room you are in.

[Video](https://www.youtube.com/watch?v=IKH6xkf1tJQ)

### Game Features:

- Freely move in a 3D environment
- Crouch and jump to reach objects which rest on the floor or on height places
- Interact with objects using the physics
- Rotate and move objects to examine them closely and from every angle
- Combine objects to use them (e.g. put the key in the keyhole so you may open the door)
- Diary with notes updated when solving puzzles
- Animated sky with clouds
- Sprite animation

### Graphics Features:

- Phong shading
- Support for normal, diffuse and specular maps
- Environment map reflections
- Real-time shadows with shadow map technique (2D map for directional light and cube map for point light)
- Cascaded shadow maps technique (for directional light)
- Filtered shadows for soft edges
- Font rendering based on [signed distance field](http://www.valvesoftware.com/publications/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf)
- Text effects such as drop shadows and glow
- Billboard rendering with transparency
- Render to texture technique

**Note**
Some media files (specifically textures, models and videos) necessary to build this project are not present in this repository and are not available anywhere else. However, if you want to use only the renderer, you can find it, and all you need to build it (except for the external libraries), in this [repository](https://github.com/DamianoOriti/OpenGL-Renderer).

### Other Features:

- Multi-threaded video decoding
- Configurator class with related config.ini file
- Batch Allocator class for fast allocation
- Asset Manager to manage assets and avoid duplication of assets in memory (e.g. textures)
- Integrated FPS (Frame Per Second) counter
- Singleton classes for easy access to Renderer, Audio System, Physics Engine, Configurator, and Asset Manager

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


### [License](https://github.com/DamianoOriti/Escape-Room/blob/master/LICENSE.txt)

**Important note**
Since this software uses fmod, which is a commercial product, you should check its [license](https://fmod.com/licensing) and [EULA](https://fmod.com/resources/eula) too.