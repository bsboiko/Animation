# Animation

This software will create an animation that follows keyframes set on bezier splines.

## Bezier:

By default the width/height of the window is 512 x 512. To see better, this can be adjusted by stretching the window size. If you want to keep it power of two (which I recommend), the initial values are in the c file and can be modified.

Initially it runs at 30 FPS. If you wish to see the animation slower and in more detail, this value is in the top of the Bezier.c file and can be adjusted. I've found the 240 gives pretty good detail.

## Compiling:

Run the make utility in the project directory. The executable program will be called Bezier.

## Running:

Run ./Bezier to start the program.

## Interaction:

The program starts with the bouncing ball in initial control points showing the animation curves.

To select a point, move the mouse over it. It will be highlighted. At that point you can click it and move it around. The curve will update in real time. Release the curve to deselect.

The manipulation of the point is bounded when you drag the cursor outside the window, This prevents you from losing access to a control point and keeping everything within the scene.

These are the following keyboard inputs:

- Esc	- End the program

- 'e'	- Turn the easing function on/off
	
- 's'	- Switch between the two animations (ball/lamp)

- 'h'	- Hide/show the animation curves and control points
