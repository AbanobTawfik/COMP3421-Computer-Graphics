# Assignment 3 Justification

If you think any of the questions within the sections are not applicable, please write "N/A". If you are answering a
criteria in a different section, please describe that like: "4a. This criteria is met by the implementation described in
2b."

## Section 1: Design Pitch. Describe a possible setting and what kind of graphics technology would be necessary to implement this. These components do not have to be implemented in this project. Suggestion: 200 words should be more than enough for this.

please read the first descriptive paragraph in an epic TRAILER NARRATOR VOICE FOR MAXIMUM AWESOMENESS AND IMMERSION
(play inception trailer music for ultimate epicness). https://www.youtube.com/watch?v=dRnneA-y7Q4&ab_channel=TCfoxs <-
soundtrack use while reading

Slenderman 2. you're thrown all alone in the forest with no clue why you're there. It's a dark night and nothing seems
to make sense. All around you are large trees and laying in the middle are two giant reflective spheres that power this
world. Can you escape? Will you find your way back home? Who brought you here? Your job is to run through the forest
evading Mr. Slenderman whilst searching for your exit out. During gameplay certain environmental effects can take place,
you may feel incredibly light-headed and the screen will blur as a result. Mr. Slenderman is quite a unique, never seen
before villain who might decide to change your environment through supernatural power by darkening your light sources or
even removing colour from the world you see. Why is he after you? Face your fears.

To achieve this game outcome technologically, we will need multiple post-processing effects applied after initial
rendering upon toggle such as blur, greyscale, High Dynamic Range lighting, and potentially kernel effects. For the
reflective spheres, I expect to use a skybox alongside real-time cube map generation to create realistic reflections.
Motion blur would be required to simulate the intense nature of escaping and outrunning Mr. Slenderman. To simulate a
realistic model chasing you, we require a rigged 3D model with skeletal animations. In order to traverse the screen an
MMO walking camera would add intuitive controls that most users are already familair with. This assignment has no
intention of implementing skeletal animations or motion blur, however, they would make amazing features. This world
would be used as the graphics engine for the new horror game Slenderman 2.

## Section 2: Advanced Lighting

### 2a: Blinn-Phong Lighting. Include code references to the implementation of lighting in your project.

#### How have you fulfilled this requirement?

When the code is run, you will see an environment that is lit using the Blinn-Phong lighting algorithm. The diffuse and
ambient lighting remains the same, specular lighting now is applied however the specular component can be observed
without artifacts even when the reflected light is over 90 degrees from the camera's view.

A point light and directional light are both used in the scene. The key "R" will teleport you in front of the point
light (extremely bright).

#### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

The entire scene and the idea of lighting, darker areas, a general ambience to the environment. Specifically if they "
fly" using spacebar they can see some objects have specular highlights, they should observe the specular lighting in
particular around the environment. The leaves of the tree at the "R" key show this effect

#### Where in the code should tutors look to see that you've fulfilled this requirement?

- res/obj folder for models
- res/shaders/shader.frag: lines 48-78 specifically lines: 48-51 (calculate halfway), 76 (blinn pong magic)
- res/shaders/shader.vert: lines 23-32 specifically lines 27-29


- res/include/ass3/Light.hpp: lines 9-21 for light structs
- src/Scene.cpp: lines 115-128 for light creation
- src/Renderer.cpp: lines 103-140 for specifically lines 107-118 for light binding to renderer

### 2b: High Dynamic Range. Show use of an intermediate frame buffer to store light values > 1.0 and how that is converted to Low Dynamic Range. Include code references.

#### How have you fulfilled this requirement?

I have implemented HDR using a frame buffer as a post-processing effect applied by a different shader, that takes the
already gamma correct SRGB scene, applies it to a quad, then tone maps using a tone mapping acquired from learn-OpenGL.

credits for tone map: https://learnopengl.com/Advanced-Lighting/HDR.

There is a point light in the scene that has all RGB values as 1 (pure white) and further is combined with the
directional lighting component. When the user presses "R" to get to that tree they can see the effect take RGB values
greater than 1 and converts to a lower dynamic range. It is worth noting that I do gamma correction BEFORE HDR. and this
is done in the default lighting.

#### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

By default, HDR is turned off (this is to show the impact when turning it on). The tutor should press the "H" key to
toggle on and off the effect of HDR. to see it more evidently on lighting values larger than 1.0, simply press the "R"
key to teleport in front of the point light that has light values larger than 1. You can move towards and away from the
tree, whilst toggling HDR on and off to see the effect. In general, you will see it as more of a dimming effect that
nicely adds to the atmosphere of the environment.

#### Where in the code should tutors look to see that you've fulfilled this requirement?

- res/shaders/HDR.frag: lines 11-17 specifically lines: 13-14 (tone mapping)
- res/shaders/HDR.vert: lines 9-11


- src/main.cpp: lines 49-50, 106-107 (framebuffer creation), 147-161
- src/Renderer.cpp: lines 264-313 specifically lines 265-267 for rendering to specified framebuffer for post-proc.
- src/Framebuffer.cpp: lines 5-33, 80-81

## Section 3: Reflections

There are 2 spheres in this enviornment, 1 uses skybox reflection, the other uses realtime cubemap reflection.

### 3a: Environment Mapped Reflections. Show at least one object can reflect a cube map. Cube map can be a preprepared skybox and the object doesn't need to reflect other objects in the scene. Include code references to implementation.

#### How have you fulfilled this requirement?

I have created a sphere object that can reflect a pre-loaded texture by sampling from a cube map texture. cube-map that
is created using the environment skybox texture. and attached to the sphere. A reflective shader is used similair to the
one in tute09 that is able to render based on the sampled reflection target from the skybox.

#### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

For ease of use, I made a key to teleport the user to this specific reflective sphere. Simply press the "Y" key to
teleport to the reflective orb and you will see the starry night reflected onto a sphere, moving around the sphere you
will notice the different details of the reflected skybox. Some post-processing effects make this more obvious such as
Edge Detection, due to the dark nature of the sphere alongside the dark skybox, press "K"
and you will see the outlines more clearly.

#### Where in the code should tutors look to see that you've fulfilled this requirement?

- res/shaders/reflective.frag: line 10
- res/shaders/reflective.vert: lines 13-18


- src/Model.cpp: lines 184-209
- src/Scene.cpp: lines 98, 157-167
- src/Renderer.cpp: lines 71-93, 205-220, 253-255

### 3b: Realtime Cube Map. Describe how the cube map is created during the frame render and then used for a reflective object. Include code references to the generation of the cube map (3a should already describe the reflection process).

#### How have you fulfilled this requirement?

For this requirement, I have made a real-time cube map reflection (this is only done once in real-time at the start of
the program to go easy on the frame rate since the object is static). To do this requirement I created a frame buffer
that uses an empty cube map as the texture. I make sure that the dimensions of this cube map texture are 1024x1024 (
matching the previous skybox dimensions). The first time I generate this cube map, I create a frame buffer. I then
change the viewport to 1024x1024 and insert a second camera into the scene with a FOV of 90 degrees. Next, I make a
duplicate scene using the second camera that has everything except the reflective orb we are rendering, and I bind the
frame buffer texture. I then render 6 times to the scene, rotating the camera each time, using a FOV of 90 degrees and
an aspect ratio of 1, and the resulting dynamic cube map texture is bound to the reflective sphere node. This texture is
then sampled to perform the reflections. It is worth noting the second camera reflection is from a different part of the
screen, but the reflections are from the scene.

alot of the logic and code came from these resources:

credit thinmatrix: https://www.youtube.com/watch?v=lW_iqrtJORc&ab_channel=ThinMatrix

credit khronos: https://www.khronos.org/opengl/wiki/Framebuffer_Object_Extension_Examples

#### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

To see this reflection may be difficult due to a large number of models on the screen, so the tutor should press the "T"
key to teleport to this reflective orb. They should then slowly move left/right and they will see the real-time
reflections. You will see the view of the tree in front of it is obstructed by the reflections from a different area of
the scene. To go back at any time to the sphere just use the "T" key

#### Where in the code should tutors look to see that you've fulfilled this requirement?

- res/shaders/reflective.frag: line 10
- res/shaders/reflective.vert: lines 13-18


- src/Model.cpp: lines 213-227 (create empty cube map)
- src/Scene.cpp: lines 98, 142-153
- src/Camera.cpp: lines 216-241 (camera switching positions for cubemap)
- src/Renderer.cpp: lines 148-200, 251-252, specifically 169-176 for drawing to the cubemap

### 3c: In-World Camera/Magic Mirror. Describe how you have placed a camera in a scene that renders to a texture. Show where both of these objects are and how they function to show a different viewpoint of the scene. Include references to code.

#### How have you fulfilled this requirement?

This requirement was fulfilled in 3b using another placed camera in the scene, that is used as the view to render, using
a different viewport, and the result is rendered to a cube map texture that is then displayed on the screen as a
reflection on the sphere.

#### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

see 3b

#### Where in the code should tutors look to see that you've fulfilled this requirement?

- src/Renderer.cpp: lines 150 (new viewport), 158-160 (create new scene and cam), 168 (bind new cam to new scene), 170 (
  bind to new texture), 173-175 (render using that cam to the texture), 177-179 (back to original viewport + rebinds)

### 3d: Planar Reflections. Describe the system used to generate a planar reflection, including any techniques for perspective and/or near plane correction. Include references to code.

N/A

## Section 4: Post Processing

### 4a: Simple Colour Change. Describe how this project uses a framebuffer and what change is made to the colour data, including references to where this is implemented in code.

#### How have you fulfilled this requirement?

I created multiple post-processing effects for a colour change, simple examples of non-kernel post-processing
implemented include, greyscale, inverse colour, and HDR. These post-processing effects load the rendered scene into a
frame buffer, load the render program that creates said effects and modifies the colour value, and draws a new quad
using the render program. These results are then passed into the screen to be displayed.

#### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

The tutor can choose which effect they want to use to see this effect in play

- "H": toggles HDR post-processing
- "I": toggles colour inversion post-processing
- "G": toggles greyscale post-processing

#### Where in the code should tutors look to see that you've fulfilled this requirement?

- res/shaders/HDR.frag: lines 11-17 specifically lines: 13-14 (tone mapping)
- res/shaders/HDR.vert: lines 9-11
- res/shaders/Inverse.frag: line 10
- res/shaders/Inverse.vert: lines 9-11
- res/shaders/Greyscale.frag: lines 11-13
- res/shaders/Greyscale.vert: lines 9-11


- src/main.cpp: lines 49-63, 103-119, 131-160
- src/Renderer.cpp: lines 264-313 specifically lines 265-267 for rendering to specified framebuffer for post-proc.
- src/Framebuffer.cpp: lines 5-33, 80-81

### 4b: Use of Kernels. Describe the kernel sampling technique in use, including code references.

#### How have you fulfilled this requirement?

A few kernel methods that sample pixels around them to determine the final colour were used in this program. These
methods include edge detection, and blur. see 4a for more specifics of how I did this post-processing.

#### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

The tutor can choose which effect they want to use to see this effect in play

- "K": toggles edge-detection post-processing
- "B": toggles blur post-processing

#### Where in the code should tutors look to see that you've fulfilled this requirement?

- res/shaders/Kernel.frag: lines 9-38 specifically lines: 11-27 (kernel for edge detection)
- res/shaders/Kernel.vert: lines 9-11
- res/shaders/Blur.frag: lines 9-38 specifically lines: 11-27 (kernel for blurring)
- res/shaders/Blur.vert: lines 9-11


- src/main.cpp: lines 49-63, 103-119, 131-160
- src/Renderer.cpp: lines 264-313 specifically lines 265-267 for rendering to specified framebuffer for post-proc.
- src/Framebuffer.cpp: lines 5-33, 80-81

### 4c: Multiple Intermediate Framebuffers. Show use of more than one framebuffer for post processing (like chaining two post processing effects in a row). Include references to code.

#### How have you fulfilled this requirement?

Post-processing effects can be combined together by toggling multiple at the same time, and the resulting output would
be the combined effect of all toggled on post-processing effects. This is done by creating a queue of post-processing
effects and rendering the scene to the frame buffer of the first post-processing effects, the intermediate calls are
chained by drawing the quad linked to the current post-processing effect to the frame buffer of the next effect until we
get to the very last post-processing effect. On the last effect, we simply draw that effects quad using the shader of
that effect onto the screen buffer.

#### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

Please note ESC key will clear all the post-processing effects, this is so you can try all whacky combinations without
having to worry about which is toggled on or off

- "K": toggles edge-detection post-processing
- "B": toggles blur post-processing
- "H": toggles HDR post-processing
- "I": toggles colour inversion post-processing
- "G": toggles greyscale post-processing

To see this in effect, simply use any combination of post-processing effects and you will see the gradual combinations.
My favourite combinates are the following

- KIGH (press these keys in order to see the effect change)
- IHG
- KB
- KBI you can use ESC to easily clear all effects to try any combination.

#### Where in the code should tutors look to see that you've fulfilled this requirement?

- res/shaders/Kernel.frag: lines 9-38 specifically lines: 11-27 (kernel for edge detection)
- res/shaders/Kernel.vert: lines 9-11
- res/shaders/Blur.frag: lines 9-38 specifically lines: 11-27 (kernel for blurring)
- res/shaders/Blur.vert: lines 9-11
- res/shaders/HDR.frag: lines 11-17 specifically lines: 13-14 (tone mapping)
- res/shaders/HDR.vert: lines 9-11
- res/shaders/Inverse.frag: line 10
- res/shaders/Inverse.vert: lines 9-11
- res/shaders/Greyscale.frag: lines 11-13
- res/shaders/Greyscale.vert: lines 9-11

- src/main.cpp: lines 49-63, 103-119, 131-160 specifically lines 154-160 to see the chaining to framebuffers
- src/Renderer.cpp: lines 264-313 specifically lines 272-289 to see the chaining of frame buffers and quads.
- src/Framebuffer.cpp: lines 5-33, 80-81

### 4d: Multiple Render Targets. Describe how this project uses multiple frame buffers to write information from the initial fragment shader pass and how it uses them in post processing. Include references to fragment shader output code as well as post processing code.

N/A

### 4e: Temporal Post Processing. Show storage of framebuffers from previous frame(s) and how they are being used in post processing. Include references to where this is happening in code.

N/A

## Section 5: Show Me What You Got. Implement any techniques that are not part of previous criteria for assignments in this course. This is open, so must be documented clearly with code references to show what has been created to add to the visual effects of the scene or objects in it.

N/A

## Section 6: Subjective Mark

### How have you fulfilled this requirement?

I thought this assignment would be a nice homage to one of the most popular "horror" games I played and watched
YouTubers like Pewdiepie play during my years in high school which was Slenderman. I always found the idea of being lost
in a forest and chased terrifying and my goal was to recreate that feeling of launching Slenderman for the first time (
along with some post-processing of course). I feel like if you played Slenderman back in the days, you would find this a
cool homage made even slightly creepier with the right post-processing effects.

### What is your vision for the scene you are showing?

The vision I had for this scene is that you are all alone in a forest, confused why you're there and the environment is
eerie and creepy. I had envisioned it as the starting point of your Slenderman 2 game, and you have to find the pages on
the random trees before he catches you, and post-processing would be applied by some game logic to randomly startle the
player. Within the span of 2 seconds 2 seconds, the colour in the scene becomes inverted making a terrifying effect. The ability
to apply post-processing effects by the engine would add to the gameplay and feel of the environment.

### Are you referencing anything in this scene? Is this a quote or homage to any other art? Please describe it.

I am making a clear reference to the 2012 hit horror game "Slenderman" where it is a similair scene of being in a forest
chased by some entity, and having to find pages on random trees. This was the first time I was actually ever terrified
using a computer and the rush and thrill I had when I first played this game made me want to recreate that memory.
Whilst not being perfect, It is a nice homage to the game.

gameplay: https://www.youtube.com/watch?v=OZMBG4Pn3Sg&ab_channel=PewDiePie

### Please give instructions for how a viewer/player should interact with this scene to see all the content you have created.

Keybinds for post-processing:

- H: HDR
- K: edge detection
- B: blur
- I: colour inversion
- G: greyscale
- ESC: clear all post-processing toggles

Keybinds for gameplay:

- R: return to starting point
- Y: teleport to skybox reflected orb
- T: teleport to realtime cubemap reflected orb
- C: cutscene camera from assignment 2 (ESC to turn off, will turn off post-processing too)
- W: move forward
- A: move left
- S: move backward
- D: move right
- Spacebar: move upward (fly)
- shift (Left shift): move downward (descend)

When the user first loads in they will be surrounded in the middle of the forest (directly next to a very bright point
light) with a gorgeous skybox representing a full moon night. The user should first enable HDR by pressing the "H" key
to see the immediate low dynamic lighting range effect and the desired gameplay lighting. The user is encouraged to
explore using the MMO camera (left/right-click to pan the screen), they can hold down both left and right-click to move
and pan, alongside traditional WASD controls. In order to see the reflective spheres the user should press the "T" key
to see the real-time reflections from a different screen space mapped onto a sphere, the "T" key will act as a teleport
back to that sphere. The "Y" key will instead take the user to a skybox reflected sphere which acts as a creepy orb when
seen from below. The user can fly around using the space bar key, and return to the ground using the shift key. If the
user ever finds themself lost and wants to return to the start they can simply press the "R" key. Whilst exploring the
user is encouraged to turn on different post-processing effects to see how the game should be played, for example using
blur for 10 seconds to simulate nausea, or greyscale and inversion to simulate a chilling effect. The user is also
encouraged to combine post-processing effects to see a cool result. To clear all post-processing effects simply press
the "ESC" key. The user is encouraged to go around the whole forest and perhaps even take a flying view of the forest at
one point. To see specular highlights in particular it is encouraged to look at the landscape or the leaves of the trees
from a flying view.

## Section 9 REFERENCES AND SHOUTOUTS

MAJOR SHOUTOUT TO Saskia, she actually drew the 3D models of the tree for me on blender, absolute legend, deserves
credit where its due. Another major shoutout to Sam the tutor for the extra help session it was much appreciated.

Alot of this code was built off the tutorials written by all the amazing tutors and i would like to thank and credit
them for giving a great foundation to work with.

Shoutouts to understanding concepts + code

- https://learnopengl.com/Advanced-Lighting/HDR         (tone map is from here too)
- https://learnopengl.com/Advanced-OpenGL/Framebuffers
- https://www.youtube.com/watch?v=lW_iqrtJORc&ab_channel=ThinMatrix
- https://www.khronos.org/opengl/wiki/Framebuffer_Object_Extension_Examples

reference for skybox + 3d model for landscape + 1 tree i got online

- https://gamebanana.com/mods/download/7920 (skybox)
- https://free3d.com/3d-model/-rectangular-grass-patch--205749.html
- https://free3d.com/3d-model/birch-tree-48016.html

reference for all other trees in the scene

- SASKIA

Shoutout to Shangjie Zhou the frame buffer warlord himself helped me discover the bug in my chaining of frame buffers
BLESS UP TO HIM

Thank you so much for this course and experience, i've learnt so much and realise how much more there is left to learn,
I am grateful for this experience, it's been the most enjoyable course i've done at uni despite how difficult it was. 