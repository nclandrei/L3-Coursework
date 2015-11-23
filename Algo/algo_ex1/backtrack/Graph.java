import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Queue;

/**
  * class to represent an undirected graph using adjacency lists
 */
public class Graph {

    private Vertex[] vertices; // the (array of) vertices
    private int numVertices = 0; // number of vertices
    private Path currentPath; // storing the 2 paths, a current one and the best path overall
    private Path bestPath;
    private int endVertex;
    private int startVertex; 
    private boolean[] visited; // improved efficiency with using visited array than the getVisited method at every iteration

    /**
     * creates a new instance of Graph with n vertices
     */
    public Graph(int n, int startVertex, int endVertex) {
        numVertices = n;
        vertices = new Vertex[n];
        this.endVertex = endVertex;
        this.startVertex = startVertex;
        this.currentPath = new Path(this);
        this.bestPath = new Path(this);
        this.visited = new boolean[n];
        for (int i = 0; i < n; i++)
            vertices[i] = new Vertex(i);
    }

    public int size() {
        return numVertices;
    }

    public Vertex getVertex(int i) {
        return vertices[i];
    }

    public void setVertex(int i) {
        vertices[i] = new Vertex(i);
    }

    /**
     * visit vertex v, with predecessor index p,
     * during a depth first traversal of the graph
     */
    private void Visit(Vertex v, int p) {
        v.setVisited(true);
        v.setPredecessor(p);
        LinkedList<AdjListNode> L = v.getAdjList();
        for (AdjListNode node : L) {
            int n = node.getVertexNumber();
            if (!vertices[n].getVisited()) {
                Visit(vertices[n], v.getIndex());
            }
        }
    }

    /**
     * carry out a depth first search/traversal of the graph
     */
    public void dfs() {
        for (Vertex v : vertices)
            v.setVisited(false);
        for (Vertex v : vertices)
            if (!v.getVisited())
                Visit(v, -1);
    }

    public int getWeight (int first, int second) {
        return vertices[first].getWeightsList().get(second);
    }


    /**
     * carry out a breadth first search/traversal of the graph
     * psedocode version
     */
    public void bfs() {
        for (Vertex v : vertices) {
            v.setVisited(false);
        }

        Queue<Vertex> queue = new LinkedList<Vertex>();

        for (Vertex v : vertices) {
            if (!v.getVisited()) {
                v.setVisited(true);
                v.setPredecessor(v.getPredecessor());
                queue.add(v);
                while (!queue.isEmpty()) {
                    Vertex u = queue.remove();
                    LinkedList<AdjListNode> L = u.getAdjList();
                    for (AdjListNode w : L) {
                        int node = w.getVertexNumber();
                        if (!vertices[node].getVisited()) {
                            vertices[node].setVisited(true);
                            vertices[node].setPredecessor(u.getIndex());
                            queue.add(vertices[node]);		     
                        }
                    }
                }
            }
        }
    }

    /** 
      * This will initialize the currentPath with adding the startVertex in the path, then calls the 
      * main backtracking method
     */

    public void callBackTracking () {
        currentPath.addFirstVertex(startVertex);
        bestPath.setDistance(Long.MAX_VALUE);
        for (boolean b : visited) {
            b = false;
        }
        Backtrack(1);
    }

    /**
      * main method for performing backtracking on finding the shortest path
    **/

    public void Backtrack (int n) {
        LinkedList<AdjListNode> adjList = vertices[currentPath.getLastVertex()].getAdjList();
        for (AdjListNode node : adjList) {
            int vertexIndex = node.getVertexNumber();
            if (!visited[vertexIndex]) {
                currentPath.addToPath(vertexIndex);
                visited[vertexIndex] = true;
                if (currentPath.getDistance() < bestPath.getDistance()) {
                    if (vertexIndex == endVertex) {
                        bestPath.assignPath(currentPath);
                    }
                    else {
                        Backtrack (n+1);
                    }
                }
                currentPath.removeLastVertexFromPath();
                visited[vertexIndex] = false;
            }
        }
    }

    /** this will print out the vertices in the best path
      *
     */
    public void printBestPath () {
        ArrayList<Integer> verticesPath = bestPath.getPath();
        System.out.print("Shortest path is ");
        for (int x : verticesPath) {
            System.out.print(x + " ");
        }
        System.out.println();
    }

    /** 
      * This will print out the distance of the best path
    **/

    public void printBestDistance () {
        System.out.println("Shortest distance from vertex " + startVertex +
                           " to vertex " + endVertex + " is " + this.bestPath.getDistance());
    }
}
