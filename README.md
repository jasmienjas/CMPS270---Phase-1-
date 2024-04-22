# CMPS270---project
Autonomous Character Movement (A* Algorithm)
Assignment Description: 

Phase Overview:

The phase aims to develop an autonomous character capable of navigating through a maze
using the A* algorithm.
The character moves within the maze on a grid-based system, using a 2D grid, where each
cell can be either a passage or an obstacle. Movement occurs from one cell to an adjacent
cell horizontally or vertically, but not diagonally.
The character can only move to adjacent cells that are not blocked by obstacles. Attempting
to move into an obstacle should result in the character remaining stationary.

Maze Generation:

Implement a method to randomly generate mazes using Prim's algorithm to ensure maze
connectivity and prevent cycles. The maze generation should adhere to the following
requirements:

Cell Representation: 

Represent the maze using a 2D grid, where each cell can be either a
passage or an obstacle.
Randomness: 

The maze generation process should be random, ensuring diverse maze
configurations for each playthrough.

Scalability:

Allow flexibility in maze size, enabling the generation of mazes with varying
dimensions based on user-defined parameters.

Connectivity: 

Utilize Prim's algorithm to generate mazes that are fully connected,
guaranteeing a path from every cell to every other cell. This prevents isolated sections of
the maze that cannot be reached.
Start and End Points: Ensure that the generated maze includes clearly defined start and
end points, with a clear path between them. This ensures the maze is solvable.
Maze Navigation (A* Algorithm Integration):
Implement the A* algorithm for pathfinding. The algorithm should take the maze graph
and start/end points as input and return the optimal path.
Integrate the maze generation process seamlessly with the rest of the project, allowing for
easy retrieval and utilization of the generated maze within the A* algorithm for pathfinding.

SFML GRAPHICS: 
Add graphics to draw and visualize navigation and the maze using SFML library then merge to your main branch. 
