#include <stdio.h>
#include <string.h>
 
#define MAX   20
#define NAMELEN 40
#define INF   999999
 
int  n = 0;                         /* number of locations            */
char names[MAX][NAMELEN];           /* location names                 */
int  adj[MAX][MAX];                 /* adjacency matrix               */
int  dist[MAX];                     /* shortest distance from source  */
int  parent[MAX];                   /* predecessor on shortest path   */
int  source = -1;                   /* selected source (0-based)      */
int  graphReady = 0;                /* graph entered?                 */
int  computed   = 0;                /* Dijkstra already run?          */
 
/* ---------- helpers ---------- */
 
/* Read an integer safely; returns 1 on success, 0 on bad input. */
int readInt(int *x)
{
    char buf[64];
    if (fgets(buf, sizeof buf, stdin) == NULL) return 0;
    return sscanf(buf, "%d", x) == 1;
}
 
void readLine(char *s, int len)
{
    if (fgets(s, len, stdin) == NULL) { s[0] = '\0'; return; }
    s[strcspn(s, "\n")] = '\0';
}
 
void initMatrix(void)
{
    for (int i = 0; i < MAX; i++)
        for (int j = 0; j < MAX; j++)
            adj[i][j] = (i == j) ? 0 : INF;
}
 
void loadSample(void)
{
    const char *sample[5] = {"Main Gate", "Library", "Computer Department",
                             "Laboratory", "Auditorium"};
    n = 5;
    initMatrix();
    for (int i = 0; i < n; i++) strcpy(names[i], sample[i]);
 
    int edges[][3] = {          /* u, v, distance (1-based) */
        {1, 2, 4}, {1, 3, 8}, {2, 3, 3}, {2, 4, 6},
        {3, 4, 2}, {3, 5, 7}, {4, 5, 3}
    };
    int m = sizeof edges / sizeof edges[0];
    for (int k = 0; k < m; k++) {
        int u = edges[k][0] - 1, v = edges[k][1] - 1, w = edges[k][2];
        adj[u][v] = adj[v][u] = w;
    }
    graphReady = 1;
    source = -1;
    computed = 0;
}
 
/* ---------- menu operations ---------- */
 
void enterGraph(void)
{
    char ans[16];
    printf("Load sample campus data (5 locations)? (y/n): ");
    readLine(ans, sizeof ans);
    if (ans[0] == 'y' || ans[0] == 'Y') {
        loadSample();
        printf("Sample campus graph loaded.\n");
        return;
    }
 
    int cnt;
    printf("Enter number of locations (2-%d): ", MAX);
    if (!readInt(&cnt) || cnt < 2 || cnt > MAX) {
        printf("Invalid number of locations.\n");
        return;
    }
 
    n = cnt;
    initMatrix();
    for (int i = 0; i < n; i++) {
        printf("Name of location %d: ", i + 1);
        readLine(names[i], NAMELEN);
        if (names[i][0] == '\0') sprintf(names[i], "Location %d", i + 1);
    }
 
    int m, maxEdges = n * (n - 1) / 2;
    printf("Enter number of roads (0-%d): ", maxEdges);
    if (!readInt(&m) || m < 0 || m > maxEdges) {
        printf("Invalid number of roads. Graph has no roads.\n");
        m = 0;
    }
 
    printf("Enter each road as: <from no.> <to no.> <distance>\n");
    for (int k = 0; k < m; ) {
        int u, v, w;
        char buf[100];
        printf("Road %d: ", k + 1);
        readLine(buf, sizeof buf);
        if (sscanf(buf, "%d %d %d", &u, &v, &w) != 3 ||
            u < 1 || u > n || v < 1 || v > n || u == v) {
            printf("  Invalid road. Try again.\n");
            continue;
        }
        if (w < 0) {
            printf("  Distance must be non-negative. Try again.\n");
            continue;
        }
        adj[u - 1][v - 1] = adj[v - 1][u - 1] = w;   /* two-way road */
        k++;
    }
 
    graphReady = 1;
    source = -1;
    computed = 0;
    printf("Campus graph entered successfully.\n");
}
 
void displayMatrix(void)
{
    if (!graphReady) { printf("Please enter the campus graph first (option 1).\n"); return; }
 
    printf("\nLocations:\n");
    for (int i = 0; i < n; i++) printf("  %2d. %s\n", i + 1, names[i]);
 
    printf("\nAdjacency Matrix (INF = no direct road):\n\n     ");
    for (int j = 0; j < n; j++) printf("%5d", j + 1);
    printf("\n     ");
    for (int j = 0; j < n; j++) printf("-----");
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("%3d |", i + 1);
        for (int j = 0; j < n; j++) {
            if (adj[i][j] == INF) printf("  INF");
            else                  printf("%5d", adj[i][j]);
        }
        printf("\n");
    }
}
 
void selectSource(void)
{
    if (!graphReady) { printf("Please enter the campus graph first (option 1).\n"); return; }
 
    printf("\nLocations:\n");
    for (int i = 0; i < n; i++) printf("  %2d. %s\n", i + 1, names[i]);
 
    int s;
    printf("Select source location number: ");
    if (!readInt(&s) || s < 1 || s > n) {
        printf("Invalid location number.\n");
        return;
    }
    source = s - 1;
    computed = 0;           /* old results are no longer valid */
    printf("Source selected: %s\n", names[source]);
}
 
/* Print current distance array (intermediate result) */
void printDistRow(void)
{
    for (int i = 0; i < n; i++) {
        if (dist[i] == INF) printf("%6s", "INF");
        else                printf("%6d", dist[i]);
    }
    printf("\n");
}
 
/* Dijkstra's algorithm with arrays: O(V^2) */
void dijkstra(int src)
{
    int visited[MAX] = {0};
 
    for (int i = 0; i < n; i++) {
        dist[i]   = INF;
        parent[i] = -1;
    }
    dist[src] = 0;
 
    /* header for the intermediate-results table */
    printf("\nStep-by-step execution (distance of each location after each step):\n");
    printf("%-26s", "Step / Vertex picked");
    for (int i = 0; i < n; i++) printf("%6d", i + 1);
    printf("\n%-26s", "Initial");
    printDistRow();
 
    for (int count = 0; count < n; count++) {
        /* pick unvisited vertex with minimum distance */
        int u = -1, min = INF;
        for (int i = 0; i < n; i++)
            if (!visited[i] && dist[i] < min) { min = dist[i]; u = i; }
 
        if (u == -1) break;         /* remaining vertices unreachable */
        visited[u] = 1;
 
        /* relax edges leaving u */
        for (int v = 0; v < n; v++)
            if (!visited[v] && adj[u][v] != INF && dist[u] + adj[u][v] < dist[v]) {
                dist[v]   = dist[u] + adj[u][v];
                parent[v] = u;
            }
 
        char label[NAMELEN + 10];
        sprintf(label, "%d. Picked %d (d=%d)", count + 1, u + 1, dist[u]);
        printf("%-26s", label);
        printDistRow();
    }
}
 
void findShortestDistance(void)
{
    if (!graphReady) { printf("Please enter the campus graph first (option 1).\n"); return; }
    if (source < 0)  { printf("Please select a source location first (option 3).\n"); return; }
 
    dijkstra(source);
    computed = 1;
    printf("Dijkstra's algorithm completed from source: %s\n", names[source]);
    printf("Use option 5 or 6 to view the results.\n");
}
 
/* Recursively print the path using the parent array */
void printPath(int v)
{
    if (parent[v] == -1) {
        printf("%s", names[v]);
        return;
    }
    printPath(parent[v]);
    printf(" -> %s", names[v]);
}
 
int checkComputed(void)
{
    if (!graphReady) { printf("Please enter the campus graph first (option 1).\n"); return 0; }
    if (source < 0)  { printf("Please select a source location first (option 3).\n"); return 0; }
    if (!computed)   { printf("Please run option 4 (Find Shortest Distance) first.\n"); return 0; }
    return 1;
}
 
void displayPaths(void)
{
    if (!checkComputed()) return;
 
    printf("\nSource: %s\n\n", names[source]);
    printf("%-22s %-10s %s\n", "Destination", "Distance", "Shortest Path");
    printf("--------------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        if (i == source) continue;
        printf("%-22s ", names[i]);
        if (dist[i] == INF) {
            printf("%-10s No path\n", "INF");
        } else {
            printf("%-10d ", dist[i]);
            printPath(i);
            printf("\n");
        }
    }
}
 
void displayDistances(void)
{
    if (!checkComputed()) return;
 
    printf("\nShortest distance from %s:\n\n", names[source]);
    printf("%-5s %-22s %s\n", "No.", "Location", "Distance");
    printf("------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%-5d %-22s ", i + 1, names[i]);
        if (dist[i] == INF) printf("INF (unreachable)\n");
        else                printf("%d\n", dist[i]);
    }
}
 
/* ---------- main ---------- */
 
int main(void)
{
    int choice;
    initMatrix();
 
    do {
        printf("\n===== Campus Shortest Route Finder (Dijkstra) =====\n");
        printf("1. Enter Campus Graph\n");
        printf("2. Display Adjacency Matrix\n");
        printf("3. Select Source Location\n");
        printf("4. Find Shortest Distance\n");
        printf("5. Display Shortest Paths\n");
        printf("6. Display Distance from Source to All Locations\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
 
        if (!readInt(&choice)) choice = 0;
 
        switch (choice) {
            case 1: enterGraph();            break;
            case 2: displayMatrix();         break;
            case 3: selectSource();          break;
            case 4: findShortestDistance();  break;
            case 5: displayPaths();          break;
            case 6: displayDistances();      break;
            case 7: printf("Exiting program. Goodbye!\n"); break;
            default: printf("Invalid choice. Please enter 1-7.\n");
        }
    } while (choice != 7);
 
    return 0;
}
 