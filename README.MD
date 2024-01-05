# Wells - 3D simulation engine
A low-level 3D engine with basic physics simulation and debugging features. Written in C++ using the [OpenGL library](https://www.opengl.org/). The shader language used is GLSL. The program uses its own vertex, fragment and geometry shaders and all geometries are defined in code (hard-coded or for more complicated shapes parametric surfaces are used). The lighting is achieved by having a light source defined in the virtual world. The basic fragment shader implements the [Blinn-Phong model](https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model).

<video width="1280" height="720" controls>
  <source src="docs/preview.mp4" type="video/mp4">
</video>

## Features
| Key / Button      | Feature |
| ----------- | ----------- |
|`w`, `a`, `s`, `d`, `c`, `SPACE`| Move the player around the virtual world |
| `y`      | Start physics simulation   |
| `n`   | Toggle normal vector display     |
| `f`   | Toggle wireframe display     |
| `b`   | Toggle back-face culling     |
| `g` | Change the direction of gravity (only applies to the rectangular body) |
| `p` | Place static sphere |
| `middle mouse button` | Place moving sphere |
| `right mouse button` | Grab static sphere (move / look around to change its position) |
| `left mouse button` | Click + drag to move camera |
| `scroll wheel` | While grabbing a static sphere (holding down left click), move it further away / closer to the camera by scrolling up / down |

### Wireframe display
By pressing the `f` button, the wireframe display can be toggled. When enabled, the world's triangles are not filled, only their edges are drawn. This is useful for debugging and demonstrating how complex and smooth looking objects are made of triangles. The feature is achived using the following calls:
```C++
glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) // wireframe only
glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) // fill all triangles
```
<video width="640" height="360" controls>
  <source src="docs/wireframe_only.mp4" type="video/mp4">
</video>

### Back-face culling
Back-Face Culling is a performance optimization that avoids drawing polygons that are determined to be facing away from the camera. In the simulation engine, this feature can be toggled by pressing `b`. Below is the demonstration of what this looks like with the triangles in the virtual world. This feature is toggled by a built in function call.
```C++
glEnable(GL_CULL_FACE);
glDisable(GL_CULL_FACE);
```

<video width="640" height="360" controls>
  <source src="docs/back-face_culling.mp4" type="video/mp4">
</video>

### Display normal vectors (using geometry shader)
The normal vectors of each triangle can be drawn by pressing `n`. This will draw a small yellow line from each vertex of each triangle on the scene and the direction of the normal vector is parallel to this line and points away from the vertex. This can be especially useful for debugging lighting problems and helps construct more complicated geometries. The feature is implemented using a geometry shader, that emits a vertex at each original vertex location and another one offset by a certain magnitude in the normal vector's direction. Then these two vertices can be connected and drawn as a primitive (line). Below is the GLSL code responsible for the effect and a video of the feature.
```GLSL
void drawNormal(int i) {
    gl_Position = gl_in[i].gl_Position * V * P;
    EmitVertex();
    
    gl_Position = (gl_in[i].gl_Position + vec4(gs_in[i].normal, 0) * MAGNITUDE) * V * P;
    EmitVertex();
    
    EndPrimitive();
}
```
<video width="640" height="360" controls>
  <source src="docs/display_normals.mp4" type="video/mp4">
</video>

### The terrain
The terrain (below the player when the program starts) is a parametric surface. The amplitude ```a[i][j]``` and phase values ```phi[i][j]``` are generated by a pink noise generator for each `(i,j)` vertex of the parametric surface. The vertices are then colored depending on where they are from the bottom of the terrain (greener at the base and browner at the top).

### Basic gravity simulation
Moving (orange) spheres: 2 kg mass

Static (blue) spheres: 3.51×10^10 kg mass (~Itokawa asteroid)

The moving spheres will orbit the center of mass of all the static spheres. They obey the gravitational law and are affected by air resistance, meaning the system eventually reaches its equilibrium. 

### Interacting with the objects in the virtual world
A subset of the world's objects can be interacted with. The blue spheres (placed by pressing `p` on the keyboard) can be grabbed the following way:
   1. Look at any blue sphere (center of the screen has to overlap with a blue sphere)
   2. Press & hold left click

Once grabbed, the following interactions are possible:

 1. Move the sphere and the player. While grabbed, the sphere will keep its distance from the player's camera as we move it (`w`, `a`, `s`, `d`, `c`, `SPACE`)
 2. By moving the camera around, the sphere will keep its distance and position from the player (meaning it will occupy the same place on the screen).
 3. Using the scroll wheel on the mouse, the sphere will get farther away or closer to the player.

### Simulation of a body on a stretchy rope

In the virtual world, a rectangular body is attached to a rope (invisible) and the other end of the rope is attached to a point marked by a blue sphere. When the physical simulation is started (by pressing the `y` button), Newton's laws of motion start applying to it. It interacts with a gravitational force that is in magnitude much like on the surface of the Earth, and air resistance. By pressing `g` the direction of the gravitational force can be changed by 90 degrees. 

<video width="640" height="360" controls>
  <source src="docs/body.mp4" type="video/mp4">
</video>