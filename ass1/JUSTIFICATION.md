# Assignment 1 Justification

PLEASE NOTE IMPORTANT:<br/>
Due to a large number of textures being used, over 1000, the first run might be slow/look choppy, if this is the case,
unlikely to occur, then please re-run the program and it will likely be smooth. To prevent this from happening textures
are preloaded which takes roughly 5-7 seconds. If you press run you should see the console/terminal print to standard
output the process of preloading the images for the gifs. After loading all gifs once, a window will appear showing the
project in full. These images are preloaded to cache them within the operating system so that the render loop doesn't
slow down whilst loading a large number of textures.<br/><br/><br/>

If you think any of the questions within the sections are not applicable, please write "N/A".

## Section 1: It runs

### How have you fulfilled this requirement?

Yes, when the code is run, there are no errors or warnings (on clion at least). When you press run the application will
continue to run until the user either aborts the program or presses x on the application window.

### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

This depends on the system the tutor is using. If they are using clion, then they can simply check that the messages
window will display the output of compiling, and the run message will display the output of running. If the tutor is
using vscode I assume they would check the output in the terminal (I didn't use vscode.)
If the tutor is using a terminal they can simply check the terminal output. When the program successfully builds and
compiles, the tutor should look at standard output to see that textures are being appropriately pre-loaded. Once
textures have been pre-loaded a window should appear on the screen with animations and images.

### Where in the code should tutors look to see that you've fulfilled this requirement?

N/A (all the code.)

## Section 2: Visibility & Shaders

### How have you fulfilled this requirement?

The visibility and shaders requirements were completed using the chicken3421 library in conjunction with .glsl shader
files for vertexes and fragments. Visibility in specific was done by displaying a window on the screen that has multiple
textures. I have made sure that the shaders cause no errors.

### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

To see visibility is implemented they just need to see at least a window that is 1920 x 1080 display. Please note that
this will only appear after all textures have been pre-loaded (wait 1-10 seconds based on the system). To see shader
usage, they can see that vertex's and coloured objects exist on the screen.

### Where in the code should tutors look to see that you've fulfilled this requirement?

in the files

- res/shaders/frag.glsl
- res/shaders/vert.glsl
- src/main.cpp in lines 70-80, 159-168

## Section 3: Creation and Use of 2D Shapes

### How have you fulfilled this requirement?

Yes, many 2D shapes of different sizes and coordinates were used in this assignment. The vertexes form the outline for
the images used. All the shapes used were squares (consisting of 6 verts each), however, multiple squares exist. The
shapes would move around the screen inside the render loop producing multiple different animations, and this was done
through matrix transformations.

###### 3a. (10%) Your application is able to create a 2D shape and it is visible in the application window.

This is clearly fulfilled, multiple objects can be seen on the screen at the same time, all in different local
coordinate systems using different shapes.

###### 3b. (5%) Your application is able to create multiple 2D shapes that are visible in the application window.

As stated in 3a, there were multiple objects created, all in different local coordinate systems and using different
shapes.

###### 3c. (5%) Your application uses Matrix Transforms to position and/or alter a 2D shape and this is visible in the application window.

Matrix transforms were applied to the objects to alter the positions of the 2D shapes. This was done for multiple 2D
objects. The tutor should see multiple gifs moving around in the application window.

###### 3d. (5%) Your application uses the render loop to animate transforms of shape(s) so that they move while the application is running.

In the application render loop, multiple matrix transforms are applied onto different objects using different
mathematical formulas to get specified movement. An example would be one object moving in a linear direction, whilst the
other performs circular movements.

### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

When the tutor runs the application they should see ATLEAST a bunch of 2d objects moving around the application window
in different motions, all moving separately in their own coordinate system. To be specific they should see the "coffin
dance" gif moving from its starting position to the top left, back and forth. They should also see the "catjam" gif
rotating in its circle, amongst many other gifs moving in different motions.

### Where in the code should tutors look to see that you've fulfilled this requirement?

- For requirement 3a and 3b: src/main.cpp in Lines 70-80, 87-97, 99-118, 188-316
- For requirement 3c and 3d: src/main.cpp in Lines 337-444 specifically lines where the transform matrix is being loaded
- For calls invoking openGL to draw the shapes: src/main.cpp in Lines 92-96

## Section 4: Use of Textures

### How have you fulfilled this requirement?

I used over 1000 image textures to animate gifs out of images in this assignment. To recreate the idea of a gif from
images, textures were changed with specific timing to imitate an animated object. The textures were also being cleared
once used to prevent memory leaks.

###### 4a. (10%) Your application is able to load texture a texture file and apply it to an object. Evidence of this is visible in the application window.

It is evident that I have loaded a texture from a texture file and apply it to the objects in Section 3. This can be
seen by inspection in the application.

###### 4b. (10%) Your application is able to load multiple textures and apply them to different objects. Evidence of this is visible in the application window.

Textures were being loaded from multiple texture files and applied to multiple objects from Section 3. This can be seen
in the application as multiple gifs on-screen separated from each other.

###### 4c. (5%) Your application is able to alter or change textures on objects during runtime. Evidence of this is visible in the application window.

Textures are changed on all objects inside the application render loop, this occurs at specific to recreate the smooth
animation of a gif out of still images.

### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

When the tutor runs the code they should specifically see the following (Please note these are all animated from still
images):

- Pink and purple disco floor background static
- Orange justice dance from Fortnite at the bottom of the screen (surrounded by spotlights) at the bottom center of the
  screen
- Frog eyes emote at the center of the screen
- Car shearer dances (surrounded by spotlights) at the top of the screen.
- Drake dancing whilst moving around the screen in an up/down movement at the bottom left corner of the screen
- Catjam emote rotating around in a circular motion, at a similair rate to which it bops its head
- Coffin dance emote moving from the top left to the center in a diagonal movement
- Another dance occurring in the bottom right of the screen

It is worth noting as well, these gifs are not only recreated from still images but also run on different timers to
create different FPS for each gif.

### Where in the code should tutors look to see that you've fulfilled this requirement?

In general, the tutor should look at the render loop, however, to be specific:

- For requirement 4a and 4b: src/main.cpp in Lines 318-320, 342, 350, 358, 366-367, 381-382, 398-399, 411-412, 420-422,
  432-433, 442-443
- For requirement 4c: src/main.cpp in Lines 366, 381, 398, 411, 420-421, 432, 442, 447-515
- For the timers used in the gif: src/main.cpp in Lines 170-176, 447-507
- For all the images used in the gifs (Note all gifs were ran through Ezgif to either remove greenscreen or split into
  images)
    - res/img/car_shearer/*.png
        - https://giphy.com/stickers/jason-clarke-dance-meme-car-shearer-fYqQBDobkehZCsi7cL
        - https://giphy.com/stickers/jason-clarke-dance-meme-car-shearer-IeQZkk1isTSYqxecqS
        - https://giphy.com/stickers/jason-clarke-dance-meme-car-shearer-KBUzVDmMxB4X9Ks7uK
    - res/img/catjam/*.png
        - https://tenor.com/view/catjam-gif-18900709
    - res/img/coffin_dance/*.png
        - https://www.youtube.com/watch?v=Gip_sQT6e0g (video was turned into a gif then edited)
    - res/img/dance/*.png
        - https://www.youtube.com/watch?v=-THEXZSh1Ek (video was trimmed, turned into a gif then edited)
    - res/img/drake/*.png
        - https://www.youtube.com/watch?v=Lp6D0Q9qQ2M (video was trimmed, turned into a gif then edited)
    - res/img/froge/*.png
        - https://tenor.com/view/booba-gif-20640480
    - res/img/orange_justice/*.png
        - https://tenor.com/view/fortnite-dance-fortnite-rust-lord-orange-justice-gif-13197773
    - res/img/disco2.png
        - https://wallpapercave.com/w/wp2785304
    - res/img/spotlight.png
        - https://www.pikpng.com/transpng/hohoxTT/
    - res/img/spotlight2.png
        - https://lovepik.com/image-401413807/yellow-stage-lighting.html

### Section 5: Subjective Mark

### How have you fulfilled this requirement?

I think that I made something funny, using multiple popular memes, the theme that I specifically went for was a dance
party as a legacy to all the popular memes from 2015-2021. These gifs that I recreated are popular "meme" dances that
were commonly shared between friends. I think through the skills I displayed in the assignment, someone looking at it
would probably think it was just me loading gif files and moving them around, whereas, in reality, each gif they see is
created from the still frames at specific frame rates. Preferably to enjoy this, even more, perhaps a "meme" song can be
played on top such as:
https://www.youtube.com/watch?v=L_jWHffIx5E. <br/> I think whether or not someone enjoys this depends on their sense of
humor, it's was created with the intent to make the viewer smile, laugh or even reminisce about some old forgotten memes
such as the Drake dance. Below is a list of the memes in their purest form, recommended to watch these, to better
understand.

- Car shearer
    - https://www.youtube.com/watch?v=v9QD25nakt8
- Catjam
    - https://www.youtube.com/watch?v=mQif8VDt7nE
- Coffin dance
    - https://www.youtube.com/watch?v=O9ejXv5Er6M
- Guy dancing his heart out
    - https://www.youtube.com/watch?v=YbFuAlFA1HM
- Drake hotline bling:
    - https://www.youtube.com/watch?v=etoZoJd2oXw
- Twitch emote:
    - https://streamerfacts.com/booba-meaning/
        - A very commonly used twitch chat meme emote
- Orange justice:
    - https://www.youtube.com/watch?v=g_SIPWSJIiU
    - This was a submission to a real Fortnite dance competition and was voted the most wanted dance to be put into the
      game by the community, after the epic games said they refuse, a major uproar came from the community, and thus the
      name orange justice was born, and the dance was put into the game.

### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

When the tutor runs the code they should specifically see the following (Please note these are all animated from still
images):

- Pink and purple disco floor background static.
- Orange justice dance from fortnite at the bottom of the screen (sorrounded by spotlights) at the bottom center of the
  screen.
- Frog eyes emote at the center of the screen .
- Car shearer dance (sorrounded by spotlights) at the top of the screen.
- Drake dancing whilst moving around the screen in an up/down movement at the bottom left corner of the screen.
- Catjam emote rotating around in a circular motion, at a similair rate to which it bops its head.
- Coffin dance emote moving from the top left to the center in a diagonal movement.
- Another dance occuring in the bottom right of the screen.

### Where in the code should tutors look to see that you've fulfilled this requirement?

- src/main.cpp
- res/img/*
- res/shaders/*.glsl

## Use of External Libraries

Credit wheres it due

- ezgif application: https://ezgif.com/split and https://ezgif.com/effects

### What does the library do?

Ezgif in particular has the ability to split a gif into still images in png file format. This tool also have the
capability to remove green screens/background colour and create png backgrounds.

### Why did you decide to use it?

I decided to do use this tool instead of manually creating code to do this, as this behaviour was out of the scope of
this assignment, what i wanted to display was the ability to recreate a gif from the images, or in this case, create any
gif given its still set of images, and move it around the screen.