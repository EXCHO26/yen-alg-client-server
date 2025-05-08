# Yen’s Algorithm

## 📝 Description  
This project implements Yen’s algorithm for finding the K shortest loopless paths in a weighted directed graph. The implementation is based on a Client-Server architecture that uses TCP sockets for data communication between the client and the server.
* 🧾 The client sends a request that includes:
    * the name of the file containing the graph
    * the start and end node
    * a value for K (number of paths)
* 🖥️ The server receives the request, executes Yen’s algorithm, and returns the K shortest paths from the source to the target node.

---

## 📂 Project Structure
The project is logically divided into two main parts:

1. **Algorithm** — Yen's K-Shortest Paths
    * Uses **Dijkstra’s algorithm** for both the initial path and spur paths.
    * Maintains two lists:
        * A – already found shortest paths
        * B – candidate (temporary) paths
    * Pseudocode
```python 
function YenKSP(Graph, source, sink, K):
    A[0] = Dijkstra(Graph, source, sink)
    B = []
    for k from 1 to K:
        for i from 0 to size(A[k − 1]) − 2:
            spurNode = A[k-1].node(i)
            rootPath = A[k-1].nodes(0, i)
            for each path p in A:
                if rootPath == p.nodes(0, i):
                    remove p.edge(i, i+1) from Graph
            for each node in rootPath except spurNode:
                remove node from Graph
            spurPath = Dijkstra(Graph, spurNode, sink)
            totalPath = rootPath + spurPath
            if totalPath not in B:
                B.append(totalPath)
            restore graph state
        if B is empty:
            break
        B.sort()
        A[k] = B[0]
        B.pop()
    return A 
```

2. **Client–Server Communication (Sockets)**
    * 🧵 Multithreaded server with a maximum of 4 worker threads
    * 🧮 Uses a thread pool to process up to 4 client requests concurrently
    * Pseudocode:
```python 
(server)
Initialize thread pool with max 4 threads
Start server on port 55555
while true:
    clientSocket = accept()
    pool.enqueue(handleClient, clientSocket)
closesocket(server)

(client)
Connect to server at port 55555
while true:
    if user input is "exit": break
    send(start, end, K)
    receive path
    display path
closesocket(client)
```
---


### ▶️ Running the Program

#### 📋 Requirements:
    * ✅ MinGW-w64 with g++ and mingw32-make
    * ✅ CMake

#### 🛠️ Build Instructions:
```bash 
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
```

#### 🖥️ Running the Server:
```bash 
./server/server.exe
```

#### 💻 Running the Client:
```bash 
./client/client.exe
```
Once the connection is established, the client sends graph and query data, and the server responds with the result.
To terminate the session, the client sends the keyword **exit**.

### 📄 Graph File Format:
```python
<number_of_nodes>
(start end weight)
(start end weight)
...
```
