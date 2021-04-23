# How to apply paint to canvas

- All mentioned tools will use the active color.

## Paint dropper:

- Paint dropper paints at mouse position to the canvas.
- Holding the mouse button extends the size of the drop.
- Nearby drops will be displaced by new ones. (only when using polygons, see next point)
- Drops are represented by lists of edges of a polygon.
    - For the PoC drops are directly generated at pixel level.

## Paint grid:

- Grids are used to create regular drop patterns.

## Paint sprinkler:

- Apply several drops of paint with slightly varying sizes simultaneously at random positions. 
- Holding the mouse button down will apply more drops over time.
