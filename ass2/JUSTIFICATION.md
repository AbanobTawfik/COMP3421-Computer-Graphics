# Assignment 2 Justification

If you think any of the questions within the sections are not applicable, please write "N/A".

## Section 1: Creation and Use of 3D Objects

### 1a: Show that you have created or imported at least one 3D object that is visible in your application. Include references to where this is implemented in your code.

#### How have you fulfilled this requirement?

When the code is run, multiple 3d objects will appear on the screen in the form of a pokemon, this can be evidently
seen  
by observing the output, all references for all 3d objects used are at the end

#### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

the 6 pokemon on screen or the background arena

#### Where in the code should tutors look to see that you've fulfilled this requirement?

- res/obj folder
- Scene.cpp lines: 21-129, functions: make_scene
- Model.cpp lines: 11-46, 65-116 functions: make_mesh, load_obj

### 1b: Describe your scene graph and the hierarchy within the scene. Include references to where this is implemented in your code.

#### How have you fulfilled this requirement?

There is a scene hierarchy that has the background arena, inside is the center of the arena and star pokemon eevee (best
pokemon). All pokemon circling eevee are children of eevee. In particular vaporeon and bulbusaur have a water droplet
and leaf rotating around them and are children of these pokemon.

- Background
    - Eevee
        - Bulbusaur
            - leaf
        - Vaporeon
            - water drop
        - Charizard
        - Snorlax
        - Pikachu
        - Jigglypuff

#### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

The 6 on screen circling eevee (pay attention to how the water drop follows the vaporeon whilst also being animated).

#### Where in the code should tutors look to see that you've fulfilled this requirement?

- res/obj folder
- Scene.cpp lines: 21-129, functions: make_scene, specifically look inside the children, if you want to follow the chain
  of the heirarchy
- Model.cpp lines: 11-46, 65-116 functions: make_mesh, load_obj
-

### 1c: Describe how your objects are moving or animating in realtime. Include code references (line numbers and function names) to use of delta time and also how this affects the scene graph.

#### How have you fulfilled this requirement?

The 6 pokemon on screen are circling the eevee in a circular movement (sort of like a battle) aswell as this, the water
drop and leaf are also circling vaporeon and bulbusaur use splines in particular to have a heart like shape around the
pokemon. Delta time was also used to make the animations consistent regardless of frame rates.

#### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

look for the seven pokemon circling the eevee

#### Where in the code should tutors look to see that you've fulfilled this requirement?

- Main.cpp lines: 91-101, functions: time_delta, time_now
- Animator.cpp lines: 128-192, functions: void animate

### 1d: Describe how you have used parametric splines in your application. Include code references to the implementation of control point based curves in your project.

#### How have you fulfilled this requirement?

the water drop circling vaporeon and the leaf circling bulbasaur make use of parametric splines using bezier curves and
control points, to animate a circular smooth movement around these.

#### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

look for the water drop circling vaporeon and the leaf circling bulbasaur as the pokemon circle the arena.

#### Where in the code should tutors look to see that you've fulfilled this requirement?

- Main.cpp lines: 70, 91-101, functions: time_delta, time_now
- Animator.cpp lines: 75-126, 128-192, functions: make_animator, animate, specifically make_animator to find the control
  points for the bezier curves. 2 seperate curves were made to account for the different shapes of the pokemon and
  customised tighter fit for both.

## Section 2: Camera

### 2a: Describe what controls the player should use to move the camera in your scene. Include code references to the implementation of delta time control of speed and use of user input in your project.

#### How have you fulfilled this requirement?

The camera in the scene allows movement with regular binding rules,

- w : moves character forward in the x/z plane (no height change)
- a : moves character to the left
- s : moves the character backwards in the x/z plane (no height change)
- d : moves character to the right
- r : reset cameras position and view (incase you get lost and don't want to restart)
- c : cutscene view (can pan and move camera around during this too)
- esc: exit cutscene view
- spacebar : moves character up
- left shift : moves character down
- left mouse click : camera pan movement
- right mouse click : camera pan movement

#### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

- Use wasd to move around (height should never change if only using wasd)
- Use mouse to pan around, should notice can never gimble lock as you cannot look up past 90 degrees and down below 90
  degrees
- use spacebar and shift to "fly around and modify height" (no gravity was added)

#### Where in the code should tutors look to see that you've fulfilled this requirement?

- Main.cpp lines: 59, 91-101, functions: make_scene
- Scene.cpp lines: 19, functions: make_camera
- Camera.cpp lines: 75-131, 71-73, 47-69, 180-188, functions: scroll_callback, update_cam_pos, update_cam_fov,
  update_cam_angles

### 2b: Describe how your camera interacts with the scene. Is it able to "walk" on the Z-X plane or follow terrain? Include code references.

#### How have you fulfilled this requirement?

The camera is able to walk on the X-Z plane by removing movement in the y dimension from wasd, this is similair to how
games have wasd movement. alongside this the speed of movement is also more realistically as when you look up you also
move slower. The camera height can be modified by using spacebar and left shift to move up and down the y dimension. The
user aswell can scroll in and out modifying the FOV of the scene directly by using the scroll wheel to zoom in and out.

#### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

- Use wasd to move around (height should never change if only using wasd)
- Use mouse to pan around, should notice can never gimble lock as you cannot look up past 90 degrees and down below 90
  degrees
- use spacebar and shift to "fly around and modify height" (no gravity was added)
- find a height of preference, and use wasd while moving the camera around and you will see your only moving in the x-z
  plane.
- use scroll wheel to zoom in and out and notice fov change

#### Where in the code should tutors look to see that you've fulfilled this requirement?

- Main.cpp lines: 59, 91-101, functions: make_scene
- Scene.cpp lines: 19, functions: make_camera
- Camera.cpp lines: 75-131, 71-73, 47-69, 180-188, functions: scroll_callback, update_cam_pos, update_cam_angles
    - specifically to see bounding on x-z plane only front or side view is modified when using wasd.
    - to handle avoidance of gimble lock, lines 62-65 check the cameras pitch to make sure it does not go above 90
      degrees. This is similair to how games don't let you look directly up/down.

### 2c: Describe how the view of your scene can change from player controlled to application controlled (specify user input that makes this change happen). Include code references to use of parametric curves and delta time in the application controlled camera.

#### How have you fulfilled this requirement?

when the user presses the button to switch from manual control to cutscene view, the camera will move back and forth
whilst rotating to pan around the arena, this will continue until the user presses the exit key which exits this
cutscene mode.

#### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

- use the c key to enable cutscene mode from wherever you are (preferrably the middle)
- use the r key to reset your position if required (application controlled reset)
- sit back and watch as the camera moves around moving and displaying the world changing the view as it does
- when you want to exit press the "esc" key to exit back to full manual mode

#### Where in the code should tutors look to see that you've fulfilled this requirement?

- Main.cpp lines: 59, 91-101, functions: make_scene
- Scene.cpp lines: 19, functions: make_camera
- Camera.cpp lines: 83-112, functions: update_cam_pos
    - the code for this is specifically done in the if conditional that toggles cutscene mode on and off
    - line 79 in particular uses delta time to calculate the step in order to make movement consistent regardless of
      frame rate

## Section 3: Lighting

### 3a: Describe where Diffuse Lighting is clear to see in your application. Include code references to your implemented lighting (including where this is implemented in your shaders).

#### How have you fulfilled this requirement?

Diffuse lighting is evidently used to provide object color to the scene based on the light source. I have added a
directional sunlight that omnidirectionally lights up the objects.

#### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

look around at the pokemon/arena

#### Where in the code should tutors look to see that you've fulfilled this requirement?

- Renderer.cpp lines: 25-44, 73-76, functions: make_renderer, void render(const renderer_t &renderer, const scene_t
  &scene)
- Model.cpp lines: 22-41, 81-98, functions: mesh_t make_mesh, model_t load_obj(const std::string &path)
- shader.vert lines: 25-29
- shader.frag lines: 25-27, 33, 37

### 3b: Describe where Specular Lighting is clear to see in your application, including how the camera position affects the lighting. Include code references to your implemented lighting (including where this is implemented in your shaders).

#### How have you fulfilled this requirement?

it is quite clear to see a white specular light moving as objects move, since the normal surfaces change as objects
move. you can see white reflection causing this specular lighting.

#### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

look around at the pokemon in particular eevee, charizard as you move the camera around them you will notice the light
reflection changing as white highlights on parts of them.

#### Where in the code should tutors look to see that you've fulfilled this requirement?

- Renderer.cpp lines: 25-39, 86-94, 99-107, functions: make_renderer, render, draw
- Model.cpp lines: 22-41, 81-98, functions:  make_mesh, load_obj
- shader.vert lines: 23-32
- shader.frag lines: 32-34, 40-45, 48, 50, 53, 56

### 3c: Describe which of your light(s) in your scene are point light(s) and how you have placed objects in the scene to clearly show the point lights functioning. Include code references to point light implementation.

#### How have you fulfilled this requirement?

I used Eevee as my point light as Eevee is the star of the show, it is quite clear to see that eevee is brighter than
the other Pokemon, and as you move away from Eeevee the entire scene gets darker, as it loses the potency of the point
light due to attenuation.

#### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

The tutor should look at the eevee, and move the camera towards and away from eevee to clearly see the point light
effect. ideally you would begin the camera flying a little bit above Eevee, and move away to see the clear impact of the
point lighting.

#### Where in the code should tutors look to see that you've fulfilled this requirement?

- Renderer.cpp lines: 25-39, 86-94, 99-107, functions: make_renderer, render, draw
- Model.cpp lines: 22-41, 81-98, functions:  make_mesh, load_obj
- shader.vert lines: 23-32
- shader.frag lines: 32-38, 40-41, 49, 51, 55, 56

### 3d: Describe all the lights in your scene and what type they are. Include code references to implementation of the capability to work with an arbitrary number of lights.

I did not complete part 3d. (If you would like your 3d to be marked, you will need to replace this line)

### 3e: Explain what maps you have used and which objects they are on. Your application should clearly show the effect these maps are having on lighting. Include code references to maps on objects and vertices as well as lighting code that uses the maps.

I did not complete part 3e. (If you would like your 3e to be marked, you will need to replace this line)

## Section 4: Subjective Mark

### How have you fulfilled this requirement?

To make something cool I used a pokemon inspired "battle" to the best of my ability. I found really cool 3D models of my
favorite pokemon, especially my favorite, Eevee. these 6 pokemon are circling Eevee "preparing an attack". I feel like
if your a pokemon fan and you appreciate the cuteness that is Eevee you would find this cute and cool.

### What is your vision for the scene you are showing?

The vision for this scene is that the aftermath of a pokemon tournament is being shown. Evee, and its trainer having won
the entire tournament. The winning pokemon is taking center stage so it may be applauded by the crowd while the losing
pokemon are circling the Evee in awe, as the unassuming Eevee managed to defeat more powerful pokemon (such as
Charizard) in a pokemon battle.

### Are you referencing anything in this scene? Is this a quote or homage to any other art? Please describe it.

I'm using this assignment to pay tribute to one of my favourite animations/game series growing up as a kid (Pokemon).
Specifically, I am paying tribute to one of the Pokemon that I find the cutest, Eevee. I found 3d models of some of my
other favourite Pokemon and even a suitable arena and tried to the best of my abilities to use graphics to show off my
favourite Pokemon. The arena is a homage to the Pokémon colosseum arenas which you could use when challenging your
friends in battles. Additionaly, The camera work was inspired by Final Fantasy 14's camera (my main MMO). I spent a
great deal of time moving around weirdly, rotating the camera while moving, trying to gimble lock, in order to try and
replicate the same control and feel of a camera as best I could manage.

### Please give instructions for how a viewer/player should interact with this scene to see all the content you have created.

1. start application
2. use shift/space to find your preferred height if needed
3. use scroll wheel to find preferred FOV for the scene
4. move around the scene, have a good look, look at rotation of pokemon and notice how they turn as the spin in the
   circle
    1. pay attention to the scene heirachy and the water/grass bezier curve movements
    2. try to gimble lock or move in the y plane without space/shift keys
    3. move away from and towards Eevee and check the impact of the Eevee point light.
    4. get up and close to the pokemon to notice the nice specular highlights, try to follow them as they rotate
5. when done moving around preferrably move to the middle of the arena above eevee and look slightly towards the crowd
6. press "c" key to begin a cutscene view
7. press "esc" key to exit cutscene view when you want to

## section 4.5: References

- tute05 + tute08, special thanks to simon haddad and all the tutors who worked very hard on these amazing tutes
- Bulbasaur model: https://free3d.com/3d-model/bulbasaur-pokemon-57218.html
- Eevee model: https://www.rigmodels.com/model.php?view=Eevee-3d-model__TISEZJP552P343BIZZGL36AZX
- Charizard model: https://clara.io/view/b0bd6e30-bc44-4806-877a-407f9dc40f1a#
- Jigglypuff model: https://free3d.com/3d-model/jigglypuff-pokemon-20012.html
- Snorlax model: https://free3d.com/3d-model/snorlax-pokemon-54436.html
- Pikachu model: https://free3d.com/3d-model/pikachu-pokemon-87270.html
- Vaporeon model: https://free3d.com/3d-model/vaporeon-pokemon-91724.html
- Plant model: https://free3d.com/3d-model/plant-model-352.html
- water drop model: https://sketchfab.com/3d-models/cc0-drops-f8f24228aebf41f297c878ad82ea7db0
- arena model: https://free3d.com/3d-model/training-stadium-pokemon-9861.html

## Section 5: Use of External Libraries

### What does the library do?

N/A

### Why did you decide to use it?

N/A

### How does this integrate with the assignment project build system? Please include any instructions and requirements if a marker needs to integrate an external library.

N/A
