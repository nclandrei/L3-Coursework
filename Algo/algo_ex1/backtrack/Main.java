import java.io.*;
import java.util.*;

/**
  * program to find shortest path using the backtrack search algorithm
 */
public class Main {

	public static void main(String[] args) throws IOException {

		int startVertex;
		int endVertex;

		// read in the data here
		String inputFileName = args[0]; // input file name
		Scanner numberScanner = new Scanner(new File(inputFileName));

		int numVertices = numberScanner.nextInt();
		ArrayList<Integer> weights = new ArrayList<Integer>();
		int index = 0;
		int numberOfNodes = numVertices * numVertices;    // so that we don't calculate the square at each step
		while (index < numberOfNodes) {
		    weights.add(numberScanner.nextInt());
			++index;
		}
		startVertex = numberScanner.nextInt();
		endVertex = numberScanner.nextInt();
		numberScanner.close();

		// create graph here
		Graph graph = new Graph(numVertices, startVertex, endVertex);
		for (int i = 0; i < numVertices; ++i) {
			for (int j = 0; j < numVertices; ++j) {
				int weight = weights.get(i * numVertices + j);
				if (weight != 0) {
					graph.getVertex(i).addToAdjList(j);
				}
				graph.getVertex(i).setWeight(j, weight); // setting the weight for each vertex
			}
		}
		
		// do the work here
		long start = System.currentTimeMillis();
		graph.callBackTracking();
		graph.printBestDistance();
		graph.printBestPath();
		long end = System.currentTimeMillis();
		// end timer and print total time
		System.out.println("Elapsed time: " + (end - start) + " milliseconds");
	}

}
