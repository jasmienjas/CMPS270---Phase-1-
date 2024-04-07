#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <unordered_set>
#include <unordered_map>
#include <cstdlib>
#include <ctime>
#include <functional>

using namespace std;

// Define a pair to represent a cell in the maze
typedef pair<int, int> Cell;

// Custom hash function for std::pair
namespace std {
    template<typename T, typename U>
    struct hash<pair<T, U>> {
        size_t operator()(const pair<T, U>& p) const {
            hash<T> hasher1;
            hash<U> hasher2;
            size_t hash1 = hasher1(p.first);
            size_t hash2 = hasher2(p.second);
            return hash1 ^ (hash2 << 1);
        }
    };


}

// Manhattan distance heuristic
int heuristic(const Cell& a, const Cell& b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
}

int main() {
    // Maze generation using Prim's algorithm
vector<vector<bool>> generateMaze(int width, int height) {
    vector<vector<bool>> maze(height, vector<bool>(width, false));
    Cell start_cell = make_pair(1, 1); // Start from (1, 1) to create borders
    maze[1][1] = true;
}

    vector<Cell> frontier = {start_cell};
    while (!frontier.empty()) {
        int random_index = rand() % frontier.size();
        Cell current_cell = frontier[random_index];
        frontier.erase(frontier.begin() + random_index);

        vector<Cell> neighbors = getUnvisitedNeighbors(maze, current_cell);
        if (!neighbors.empty()) {
            int random_neighbor_index = rand() % neighbors.size();
            Cell neighbor = neighbors[random_neighbor_index];
            int x1 = current_cell.first, y1 = current_cell.second;
            int x2 = neighbor.first, y2 = neighbor.second;

            // Mark the cells as visited
            maze[y1][x1] = true;
            maze[y2][x2] = true;

            // Add the remaining unvisited neighbors to the frontier
            vector<Cell> new_neighbors = getUnvisitedNeighbors(maze, neighbor);
            frontier.insert(frontier.end(), new_neighbors.begin(), new_neighbors.end());
        }
    }

    // Create borders
    for (int x = 0; x < width; x++) {
        maze[0][x] = true; // Top border
        maze[height - 1][x] = true; // Bottom border
    }
    for (int y = 0; y < height; y++) {
        maze[y][0] = true; // Left border
        maze[y][width - 1] = true; // Right border
    }

    return maze;
}

// Helper function to get unvisited neighbors for maze generation
vector<Cell> getUnvisitedNeighbors(const vector<vector<bool>>& maze, const Cell& cell) {
    vector<Cell> neighbors;
    int x = cell.first, y = cell.second;
    int rows = maze.size(), cols = maze[0].size();

    // Check left neighbor
    if (x > 0 && !maze[y][x - 1])
        neighbors.push_back(make_pair(x - 1, y));

    // Check right neighbor
    if (x < cols - 1 && !maze[y][x + 1])
        neighbors.push_back(make_pair(x + 1, y));

    // Check top neighbor
    if (y > 0 && !maze[y - 1][x])
        neighbors.push_back(make_pair(x, y - 1));

    // Check bottom neighbor
    if (y < rows - 1 && !maze[y + 1][x])
        neighbors.push_back(make_pair(x, y + 1));

    return neighbors;
}



