# Pacman Game in C

This is a classic Pacman game implementation in C using the CSFML graphics library. The project was developed as a final assignment for CS 2600.

## Contributors

- 

## Implementation Details

### Game Map

The game map is represented using a 2D array where different integer values represent different elements:
- 1: Empty space
- 2: Pellet (small dot)
- 3: Power-up (larger dot)
- 4: Wall
- 5: Ghost home

The map layout is designed to mimic the classic Pacman maze with walls, corridors, and pellets strategically placed throughout the game board.

### Graphics Rendering

The game uses CSFML to handle graphics rendering:

- Walls are rendered as blue rectangles
- Pellets are rendered as small white circles
- Power-ups are rendered as larger white circles
- The game window is scaled based on the map dimensions

### Game Loop

The main game loop handles:
1. Event processing (window closing)
2. Clearing the screen
3. Drawing the map elements (walls, pellets, power-ups)
4. Displaying the rendered frame

### Hit Detection and Movement

The game implements hit detection and movement using a structured approach:

- Pac-Man and ghosts are represented as structs containing x and y coordinates and current direction
- Movement logic is processed in the game loop:
  1. For Pac-Man:
     - Check the space in front based on current direction
     - If a ghost is there and aggressive, Pac-Man moves and dies
     - If a ghost is there and afraid, Pac-Man moves and the ghost is eaten
     - If a wall is there (map[x][y] = 5), Pac-Man will not move
     - If a pellet is there, Pac-Man moves and eats the pellet
     - If a power-up is there, Pac-Man eats it and ghosts' status changes
  2. For each ghost:
     - Check the space in front based on current direction
     - If Pac-Man is there and the ghost is aggressive, it moves and Pac-Man dies
     - If Pac-Man is there and the ghost is afraid, it moves and gets eaten

### User Input

The game captures user input through CSFML:

- Arrow keys change Pac-Man's direction
- Input is processed each frame to determine Pac-Man's next move

### Power-Up System

Power-ups affect ghost behavior through state management:

- Ghosts have three possible states:
  1. Aggressive: Can damage Pac-Man
  2. Afraid: Vulnerable to Pac-Man
  3. Running: Returning home after being eaten
- When Pac-Man eats a power-up:
  - All ghosts enter the afraid state
  - A global timer tracks the duration of the effect
  - Each frame decrements the timer until it reaches zero
  - When the timer expires, afraid ghosts return to aggressive mode
- If Pac-Man eats a ghost:
  - The ghost enters running mode
  - When the ghost reaches home, it becomes aggressive again

### Ghost AI

Ghost movement is determined by their current state and the maze structure:

- Basic movement rules:
  - If no space exists to the left/right of current direction, move forward
  - At intersections (spaces to left/right):
    1. Aggressive: Choose an open direction closest to Pac-Man (except backward)
       - If multiple directions are equally close, choose randomly
    2. Afraid: Choose a random open direction
    3. Running: Move toward home (similar to aggressive but targeting home)
       - Running ghosts move twice as fast as other ghosts

## Challenges

Developing this Pacman game presented several challenges:

1. **CSFML Integration**: Working with the CSFML library required understanding its architecture and API, which differs from standard C libraries. Setting up the proper include paths and linking was particularly challenging across different operating systems.

2. **Map Representation**: Designing a map representation that was both efficient and easy to work with required careful planning. The 2D array approach was chosen for its simplicity and direct mapping to the visual grid.

3. **Collision Detection**: Implementing proper collision detection between Pacman and walls, as well as Pacman and collectible items (pellets and power-ups) required precise position tracking and boundary checking.

4. **Game Mechanics**: Implementing the core game mechanics such as pellet collection, power-up effects, and ghost AI behavior required careful state management and game logic.

5. **Cross-Platform Compatibility**: Ensuring the game works consistently across different operating systems (Windows and macOS) required platform-specific compilation instructions and careful management of dependencies.

## Building and Running

### macOS

1.

### Windows

1.

