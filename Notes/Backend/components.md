# Basic Component structure (for PoC)

- This may be changed during development.

## Input from Frontend

- PoC:
  - There will be a function which takes a position in the canvas, a radius and a color. It will then render the circle to the canvas.
- First thoughts for later versions:
  - Functions for color application:
    - One will take a position, some sort of size and a color and will generate a new "smooth" polygon, render it to the canvas and return some kind of identifier to the polygon.
    - The other one can be used to expand a polygon of known identifier which only takes the id and an expansion parameter.
    - Color grids will be realized in an similar way.
    - The color sprinkler will be realized by one function which takes the average size for drops, the density of drops, the current mouse pointer position and a radius. Random drops will be generated on a circle with the given radius around the mouse position.
  - Functions for rakes:
    - TODO

## Structure in Backend

- There will be a singleton class to structure all current options (e.g. active color, tool, ...)
- Later on there will be at least to different classes to deal with WebGL.
- One to apply color using polygons and one to perform rake transformations.
- One component will be used to store the canvas content as an image file.
- In detail architecture will be developed during project progress.
