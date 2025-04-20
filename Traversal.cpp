#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>

using namespace std;

// ===== DFS FUNCTION =====
void DFS(int start, vector<vector<int>>& adjList, vector<bool>& visited) {
    stack<int> s;
    s.push(start);

    while (!s.empty()) {
        int node = s.top();
        s.pop();

        if (!visited[node]) {
            cout << node << " ";
            visited[node] = true;

            // Push neighbors (can be parallel if needed)
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

// ===== BFS FUNCTION =====
void BFS(int start, vector<vector<int>>& adjList, vector<bool>& visited) {
    queue<int> q;
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        cout << node << " ";

        // Enqueue neighbors
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
        adjList[v].push_back(u);  // For undirected graph
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

    vector<bool> visited(vertices, false);

    switch (choice) {
        case 1:
            cout << "\nDFS traversal:\n";
            DFS(start, adjList, visited);
            break;

        case 2:
            cout << "\nBFS traversal:\n";
            BFS(start, adjList, visited);
            break;

        default:
            cout << "Invalid choice!\n";
            return 1;
    }

    cout << endl;
    return 0;
}
