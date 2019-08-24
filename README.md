# Asteroids

This is a remake of the classic arcade game using the
[SDL](https://www.libsdl.org/) libray.

### Controls

- *Up arrow*: thrust
- *Left arrow*: rotate left
- *Right arrow*: rotate right
- *Space bar*: shoot

### Reference
This game solves some simple geometric problems:

- Generation of random irregular polygons (like the asteroids in the game)
- Generation of regular polygons (by removing randomness)
- Intersection between two circles
- Intersection between two line segments
- Intersection between two polygonal shapes (side by side intersection)
- Rotation of a polygon around a point

It also presents a basic pipeline for a game with simple physics:
- Initialization
- Event handling
- Update
    - Objects movement (apply acceleration, velocity and rotation)
    - Collision handling
    - Rendering
