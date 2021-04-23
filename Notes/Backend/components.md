# Basic Component structure (for PoC)

- This may be changed during development.

## Input from Frontend

- We will make heavy use of emscripten/html5.h to register callbacks for certain events (e.g. clicks on buttons or canvas).
- The callback function will then receive a struct containing all necessary information.

## Structure in Backend

- There will be a singleton class to structure all current options (e.g. active color, tool, ...)
- Later on there will be at least to different classes to deal with WebGL.
    - One to apply color using polygons and one to perform rake transformations.
- One component will be used to store the canvas content as an image file.
- In detail architecture will be developed during project progress.