#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>

using namespace std;

// ===== TRADITIONAL DFS =====
void DFS_Traditional(int start, vector<vector<int>>& adjList, vector<bool>& visited) {
    stack<int> s;
    s.push(start);

    while (!s.empty()) {
        int node = s.top();
        s.pop();

        if (!visited[node]) {
            cout << node << " ";
            visited[node] = true;

            for (int i = adjList[node].size() - 1; i >= 0; --i) {
                int neighbor = adjList[node][i];
                if (!visited[neighbor]) {
                    s.push(neighbor);
                }
            }
        }
    }
}

// ===== OMP DFS =====
void DFS_OMP(int start, vector<vector<int>>& adjList, vector<bool>& visited) {
    stack<int> s;
    s.push(start);

    while (!s.empty()) {
        int node = s.top();
        s.pop();

        if (!visited[node]) {
            cout << node << " ";
            visited[node] = true;

            #pragma omp parallel for
            for (int i = adjList[node].size() - 1; i >= 0; --i) {
                int neighbor = adjList[node][i];
                if (!visited[neighbor]) {
                    #pragma omp critical
                    s.push(neighbor);
                }
            }
        }
    }
}

// ===== TRADITIONAL BFS =====
void BFS_Traditional(int start, vector<vector<int>>& adjList, vector<bool>& visited) {
    queue<int> q;
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        cout << node << " ";

        for (int i = 0; i < adjList[node].size(); ++i) {
            int neighbor = adjList[node][i];
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
}

// ===== OMP BFS =====
void BFS_OMP(int start, vector<vector<int>>& adjList, vector<bool>& visited) {
    queue<int> q;
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        cout << node << " ";

        #pragma omp parallel for
        for (int i = 0; i < adjList[node].size(); ++i) {
            int neighbor = adjList[node][i];
            if (!visited[neighbor]) {
                #pragma omp critical
                {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }
        }
    }
}

// ===== MAIN FUNCTION =====
int main() {
    int vertices, edges;
    cout << "Enter number of vertices: ";
    cin >> vertices;
    cout << "Enter number of edges: ";
    cin >> edges;

    vector<vector<int>> adjList(vertices);

    cout << "Enter edges (format: u v for edge from u to v):\n";
    for (int i = 0; i < edges; ++i) {
        int u, v;
        cin >> u >> v;
        adjList[u].push_back(v);
        adjList[v].push_back(u); // undirected graph
    }

    int start;
    cout << "Enter starting node: ";
    cin >> start;

    cout << "\nChoose traversal algorithm:\n";
    cout << "1. Depth-First Search (DFS)\n";
    cout << "2. Breadth-First Search (BFS)\n";
    cout << "Enter choice: ";
    int choice;
    cin >> choice;

    double startTime, endTime;
    vector<bool> visited(vertices, false);

    switch (choice) {
        case 1:
            cout << "\n=== Traditional DFS traversal ===\n";
            startTime = omp_get_wtime();
            DFS_Traditional(start, adjList, visited);
            endTime = omp_get_wtime();
            cout << "\nExecution Time (Traditional DFS): " << (endTime - startTime) << " seconds\n";

            // Reset visited
            fill(visited.begin(), visited.end(), false);

            cout << "\n=== OMP Parallel DFS traversal ===\n";
            startTime = omp_get_wtime();
            DFS_OMP(start, adjList, visited);
            endTime = omp_get_wtime();
            cout << "\nExecution Time (OMP DFS): " << (endTime - startTime) << " seconds\n";

            break;

        case 2:
            cout << "\n=== Traditional BFS traversal ===\n";
            startTime = omp_get_wtime();
            BFS_Traditional(start, adjList, visited);
            endTime = omp_get_wtime();
            cout << "\nExecution Time (Traditional BFS): " << (endTime - startTime) << " seconds\n";

            // Reset visited
            fill(visited.begin(), visited.end(), false);

            cout << "\n=== OMP Parallel BFS traversal ===\n";
            startTime = omp_get_wtime();
            BFS_OMP(start, adjList, visited);
            endTime = omp_get_wtime();
            cout << "\nExecution Time (OMP BFS): " << (endTime - startTime) << " seconds\n";

            break;

        default:
            cout << "Invalid choice!\n";
            return 1;
    }

    cout << endl;
    return 0;
}
