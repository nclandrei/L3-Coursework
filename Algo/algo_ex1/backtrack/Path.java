import java.util.*;


/**
  * Class for representing a path used in the backtracking algorithm
**/

public class Path {
	
	private ArrayList<Integer> verticesPath;    // storing the vertices in the path as integers
	private long distance;     // storing the distance until 
	private Graph graph;	   // having a Graph field because the Path is specific to that graph; moreover, we need access to the vertices

	// constructor
	public Path (Graph graph) {
		this.verticesPath = new ArrayList<Integer>();
		this.distance = 0;
		this.graph = graph;
	}

	// assigning the data from a path to another
	public void assignPath (Path path) {
		this.verticesPath = new ArrayList<Integer>(path.getPath());
		this.distance = path.getDistance();
	}

	public ArrayList<Integer> getPath () {
		return this.verticesPath;
	}

	public void setPath (Path path) {
		this.verticesPath = new ArrayList<Integer>(path.getPath());
	}

	public long getDistance () {
		return this.distance;
	}

	public void setDistance (long distance) {
		this.distance = distance;
	}

	public int getSize () {
		return verticesPath.size();
	}

	// for efficiency, we compute the distance when we remove the item, substracting the weight between
	// the last 2 elements
	public void removeLastVertexFromPath () {
		int weight = graph.getWeight(verticesPath.get(getSize() - 2),getLastVertex());
		this.distance -= weight;
		this.verticesPath.remove(getSize() - 1);
	}

	public void addFirstVertex (int vertex) {
		this.verticesPath.add(vertex);
	}

	// adding a vertex to the path includes a distance computation inside, thus improving time efficiency
	public void addToPath (int vertex) {
		int weight = graph.getWeight(getLastVertex(),vertex);
		this.distance += weight;
		this.verticesPath.add(vertex);
	}

	public int getLastVertex () {
		return verticesPath.get(getSize() - 1);
	}
}