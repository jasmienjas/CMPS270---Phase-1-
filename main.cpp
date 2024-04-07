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

// Maze generation using Prim's algorithm
vector<vector<bool>> generateMaze(int width, int height) {
    vector<vector<bool>> maze(height, vector<bool>(width, false));
    Cell start_cell = make_pair(1, 1); // Start from (1, 1) to create borders
    maze[1][1] = true;

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

int main() {
    srand(time(nullptr));  // Seed the random number generator

    // Generate maze
    int width, height;
    cout << "Enter the width of the maze: ";
    cin >> width;
    cout << "Enter the height of the maze: ";
    cin >> height;

    vector<vector<bool>> maze = generateMaze(width, height);

    // TODO: Implement A* algorithm to find the shortest path
    / Helper function to get neighbors for A* algorithm
vector<Cell> getNeighbors(const vector<vector<bool>>& maze, const Cell& cell) {
    vector<Cell> neighbors;
    int x = cell.first, y = cell.second;
    int rows = maze.size(), cols = maze[0].size();

    // Check left neighbor
    if (x > 0 && maze[y][x - 1])
        neighbors.push_back(make_pair(x - 1, y));

    // Check right neighbor
    if (x < cols - 1 && maze[y][x + 1])
        neighbors.push_back(make_pair(x + 1, y));

    // Check top neighbor
    if (y > 0 && maze[y - 1][x])
        neighbors.push_back(make_pair(x, y - 1));

    // Check bottom neighbor
    if (y < rows - 1 && maze[y + 1][x])
        neighbors.push_back(make_pair(x, y + 1));

    return neighbors;
}

// Reconstruct path from came_from map
vector<Cell> reconstructPath(const unordered_map<Cell, Cell>& came_from, const Cell& end) {
    vector<Cell> path;
    Cell current = end;
    while (came_from.count(current)) {
        path.push_back(current);
        current = came_from.at(current);
    }
    path.push_back(current);
    reverse(path.begin(), path.end());
    return path;
}

// A* algorithm
vector<Cell> aStar(const vector<vector<bool>>& maze, const Cell& start, const Cell& end) {
    priority_queue<pair<int, Cell>, vector<pair<int, Cell>>, greater<pair<int, Cell>>> open_set;
    open_set.push(make_pair(0, start));

    unordered_map<Cell, int> g_score;
    unordered_map<Cell, Cell> came_from;

    g_score[start] = 0;

    while (!open_set.empty()) {
        Cell current = open_set.top().second;
        open_set.pop();

        if (current == end)
            return reconstructPath(came_from, end);

        vector<Cell> neighbors = getNeighbors(maze, current);
        for (const Cell& neighbor : neighbors) {
            int tentative_g_score = g_score[current] + 1;
            if (!g_score.count(neighbor) || tentative_g_score < g_score[neighbor]) {
                came_from[neighbor] = current;
                g_score[neighbor] = tentative_g_score;
                int f_score = tentative_g_score + heuristic(neighbor, end);
                open_set.push(make_pair(f_score, neighbor));
            }
        }
    }

    return vector<Cell>();
}

    // Print the maze and the path
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (maze[y][x])
                cout << "#";
            else
                cout << " ";
        }
        cout << endl;
    }

    return 0;
}




