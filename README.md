# Campus Shortest Route Finder – Dijkstra's Algorithm

A menu-driven C program that finds the shortest distance and route from a chosen source location to every other location on a college campus, using Dijkstra's algorithm.

## About

The campus is modelled as a weighted graph. Locations (Main Gate, Library, Computer Department, Laboratory, Auditorium) are vertices, and roads are edges with distances. The graph is stored as an adjacency matrix, with `0` on the diagonal and `INF` where there is no direct road. Roads are two-way, and distances must be non-negative.

## Features

- Enter your own campus graph, or load the built-in 5-location sample
- Display the adjacency matrix
- Select any location as the source
- Run Dijkstra's algorithm with a step-by-step table showing how distances change
- Display the shortest path to every destination
- Display the distance from the source to all locations
- Input validation and clear error messages

## Files

| File | Description |
|---|---|
| `main.c` | Complete C source code |
| `README.md` | Project documentation |

## Build and Run

```bash
gcc main.c -o campus
./campus
```

On Windows (MinGW): `gcc main.c -o campus.exe` then `campus.exe`

## Menu

```
1. Enter Campus Graph
2. Display Adjacency Matrix
3. Select Source Location
4. Find Shortest Distance
5. Display Shortest Paths
6. Display Distance from Source to All Locations
7. Exit
```

The menu repeats until option 7 is chosen. An invalid choice shows an error and returns to the menu.

**Typical order of use:** 1 → 3 → 4 → 5 (or 6). The program warns you if a step is attempted before the previous one is done.

## Entering a Custom Graph (Option 1)

1. Enter the number of locations (2–20).
2. Enter the name of each location.
3. Enter the number of roads.
4. Enter each road as `<from no.> <to no.> <distance>` (location numbers start at 1).

## Sample Test Case

**Roads:**

| From | To | Distance |
|---|---|---|
| 1 (Main Gate) | 2 (Library) | 4 |
| 1 (Main Gate) | 3 (Computer Dept) | 8 |
| 2 (Library) | 3 (Computer Dept) | 3 |
| 2 (Library) | 4 (Laboratory) | 6 |
| 3 (Computer Dept) | 4 (Laboratory) | 2 |
| 3 (Computer Dept) | 5 (Auditorium) | 7 |
| 4 (Laboratory) | 5 (Auditorium) | 3 |

**Source:** Main Gate

**Intermediate steps (option 4):**

```
Step / Vertex picked           1     2     3     4     5
Initial                        0   INF   INF   INF   INF
1. Picked 1 (d=0)              0     4     8   INF   INF
2. Picked 2 (d=4)              0     4     7    10   INF
3. Picked 3 (d=7)              0     4     7     9    14
4. Picked 4 (d=9)              0     4     7     9    12
5. Picked 5 (d=12)             0     4     7     9    12
```

**Result (option 5):**

| Destination | Distance | Shortest Path |
|---|---|---|
| Library | 4 | Main Gate → Library |
| Computer Department | 7 | Main Gate → Library → Computer Department |
| Laboratory | 9 | Main Gate → Library → Computer Department → Laboratory |
| Auditorium | 12 | Main Gate → Library → Computer Department → Laboratory → Auditorium |

## How It Works

Three arrays drive the algorithm:

- `dist[]` – current minimum distance of each location from the source
- `visited[]` – marks locations whose shortest distance is final
- `parent[]` – the previous location on the shortest path, used to rebuild routes

Algorithm:

1. Set every distance to `INF` and the source distance to 0.
2. Pick the unvisited location with the smallest distance and mark it visited.
3. For each of its neighbours, update the distance if the route through this location is shorter, and record the parent.
4. Repeat steps 2–3 until all reachable locations are visited.
5. Rebuild each path by following `parent[]` back to the source.

## Complexity

**Time: O(V²)**, where V is the number of locations.
The outer loop runs V times. In each pass, finding the nearest unvisited vertex takes O(V), and updating its neighbours (one matrix row) takes O(V). So the total is V × O(V) = O(V²).

**Space: O(V²)** for the adjacency matrix, plus O(V) for the `dist`, `parent` and `visited` arrays.

## Limits and Notes

- Maximum 20 locations (`MAX`), names up to 39 characters
- `INF` is defined as `999999`
- Unreachable locations are shown as `INF` with "No path"
- Roads are treated as undirected (two-way)
