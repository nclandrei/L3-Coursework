import java.util.*;


/**
 *class to represent an undirected graph using adjacency lists
 */
public class Graph {

    private Vertex[] vertices; // the (array of) vertices
    private int numVertices = 0; // number of vertices
    private int[] pathList; // array containing the indexes traversed in the shortest path
    private int[] distances; // array used for storing distances in the backtrack method


    /**
     * creates a new instance of Graph with n vertices
     */
    public Graph(int n) {
        numVertices = n;
        vertices = new Vertex[n];
        pathList = new int[n];
        distances = new int[n];
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
      * returns the current shortest path between 2 nodes
      */
    public int[] getPath () {
        return this.pathList;
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

    // implementation of the Dijkstra algorithm
    public int Dijkstra (int startVertex, int endVertex) {
        int graphSize = this.size();
        boolean[] visited = new boolean[graphSize];

        // initialising distances
        for (int i = 0; i < graphSize; ++i) {
            distances[i] = Integer.MAX_VALUE;
            pathList[i] = -1;
            visited[i] = false;
        }

        // setting distance to sourceVertex equal to 0
        distances[startVertex] = 0;

        // going to visit all the nodes
        for (int i = 0; i < graphSize; ++i) {
            
            // finding out the minimum distanced and unvisited element
            int min = -1;
            for (int j = 0; j < graphSize; ++j)
                if (!visited[j] && ((min == -1) || (distances[j] < distances[min]))) {
                    min = j;
                }

                // marking the element as visited
                visited[min] = true;

                // as soon as we hit the destination vertex, we break out of the loop
                if (min == endVertex) {
                    break;
                }

                // performing relaxation
                for (int k = 0; k < graphSize; ++k) {
                    int weight = this.getVertex(min).getWeightsList().get(k);
                    if (weight != 0) {
                        relax(min, k, weight);
                    }
                }
        }
        return distances[endVertex];
    }

    // this is the relax function called in dijkstra method to update distances and shortest path to the node
    public void relax (int min, int currVertex, int weight) {
        if (distances[min] + weight < distances[currVertex]) {
            distances[currVertex] = distances[min] + weight;
            pathList[currVertex] = min;
        }
    }

    // this function will recursively print out the path between the startVertex and the endVertex
    public void printPath (int endVertex, int startVertex) {
        if (endVertex != startVertex) {
            printPath(pathList[endVertex], startVertex);
        }
        System.out.print(endVertex + " ");
    }
}
