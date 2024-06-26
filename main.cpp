#include <SFML/Graphics.hpp>
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
using namespace sf;

const int CELL_SIZE = 20; // Size of each cell in pixels

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

// Get unvisited neighbors of a cell
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

// Get unvisited neighbors for maze generation
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

    priority_queue<pair<int, Cell>, vector<pair<int, Cell>>, greater<pair<int, Cell>>> frontier;
    unordered_set<Cell> visited;
    for (const Cell& neighbor : getUnvisitedNeighbors(maze, start_cell)) {
        frontier.push(make_pair(heuristic(start_cell, make_pair(width / 2, height / 2)), neighbor));
        visited.insert(neighbor);
    }

    while (!frontier.empty()) {
        Cell current_cell = frontier.top().second;
        frontier.pop();
        visited.insert(current_cell);

        vector<Cell> neighbors = getUnvisitedNeighbors(maze, current_cell);
        // Randomly shuffle the neighbors
        random_shuffle(neighbors.begin(), neighbors.end());
        int numNeighborsToAdd = neighbors.size() / 2; // Add only half of the neighbors to the frontier
        for (int i = 0; i < numNeighborsToAdd; i++) {
            const Cell& neighbor = neighbors[i];
            int x1 = current_cell.first, y1 = current_cell.second;
            int x2 = neighbor.first, y2 = neighbor.second;

            // Mark the cells as visited
            maze[y1][x1] = true;
            maze[y2][x2] = true;

            // Add the unvisited neighbors of the neighbor to the frontier
            for (const Cell& n : getUnvisitedNeighbors(maze, neighbor)) {
                if (visited.find(n) == visited.end()) {
                    frontier.push(make_pair(heuristic(n, make_pair(width / 2, height / 2)), n));
                    visited.insert(n);
                }
            }
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

    int width, height;
    cout << "Enter the width of the maze: ";
    cin >> width;
    cout << "Enter the height of the maze: ";
    cin >> height;

    vector<vector<bool>> maze = generateMaze(width, height);

    Cell start = make_pair(1, 1);
    Cell end = make_pair(width - 2, height - 2);

    vector<Cell> path = aStar(maze, start, end);

    // Create an SFML window
    RenderWindow window(VideoMode(width * CELL_SIZE, height * CELL_SIZE), "Maze Navigation");

    // Create a circle to represent the character
    CircleShape character(CELL_SIZE / 3.0f);
    character.setFillColor(Color::Green);
    character.setPosition(start.first * CELL_SIZE, start.second * CELL_SIZE);

    int pathIndex = 0; // Index to keep track of the current position in the path

    Clock clock;
    float moveInterval = 0.5f; // Time interval between character moves
    float elapsedTime = 0.0f;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        elapsedTime += clock.restart().asSeconds();

        if (elapsedTime >= moveInterval && pathIndex < path.size()) {
            // Move the character to the next position in the path
            Cell nextCell = path[pathIndex];
            character.setPosition(nextCell.first * CELL_SIZE, nextCell.second * CELL_SIZE);
            pathIndex++;
            elapsedTime = 0.0f;
        }

        window.clear(Color::White);

        // Draw the maze
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (maze[y][x]) {
                    RectangleShape wall(Vector2f(CELL_SIZE, CELL_SIZE));
                    wall.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                    wall.setFillColor(Color(220, 220, 220)); // Light gray color
                    window.draw(wall);
                }
            }
        }

        // Draw the path
        for (int i = pathIndex; i < path.size(); i++) {
            CircleShape pathCell(CELL_SIZE / 5.0f);
            pathCell.setPosition(path[i].first * CELL_SIZE + CELL_SIZE / 2.0f - CELL_SIZE / 10.0f, path[i].second * CELL_SIZE + CELL_SIZE / 2.0f - CELL_SIZE / 10.0f);
            pathCell.setFillColor(Color::Yellow);
            window.draw(pathCell);
        }

        // Draw the character
        window.draw(character);

        window.display();
    }

    return 0;
}





