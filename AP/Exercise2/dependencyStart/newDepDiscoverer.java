/**
  * Title of assignment: APH Exercise 2
  * Name: Andrei-Mihai Nicolae
  * GUID: 2147392n
  * This is my own work as defined in the Academic Ethics agreement I have signed.
 */

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.StringJoiner;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.Condition;

public class newDepDiscoverer {

    private static ArrayList<String> dir;   // will store the directories in which we search for includes
    private static ConcurrentHashMap<String, LinkedList<String>> hashM;  // this is the "another structure", the master hashMap
    private static LinkedBlockingQueue<String> workQueue;  // the workQueue that will contain all files needed to have work done on
    private static int numberOfThreads;  // variable that will store the number of threads
    private static CyclicBarrier timer;  // we will use this to start all threads at approximately the same time
    private static boolean done;
    private static ReentrantLock lock;
    private static Condition queueEmpty;

    /**
      * This method creates a directory path, adding / if necessary
     */
    private static String createDir(String s) {
    	int len = s.length() - 1;
    	String tempBuff;
    	String result;
    	if (s.charAt(len) != '/') {
    		tempBuff = String.format("%s/", s);
    	}
    	else {
    		tempBuff = s;
    	}
    	result = tempBuff;
    	return result;
    }

    /**
      * Utility to get a file's root
     */
    private static String getFileRoot(String file) {
    	return file.substring(0, file.indexOf('.'));
    }

    /**
      * Utility function to get a file's extension
     */
    private static char getFileExt(String file) {
    	return file.charAt(file.indexOf('.')+1);
    }

    /**
      * Another utility function retrieving a file given its path as a string
     */
    private static File openFile(String file) {
    	File currFile;

    	for (String directory : dir) {
    		currFile = new File(directory.concat(file));
    		if (currFile.exists() && !currFile.isDirectory()) {
    			return currFile;
    		}
    	}
    	return null;
    }

    /**
      * Recursively print all the dependencies for the processed files
     */
    private static void printDependencies(ConcurrentHashMap<String,LinkedList<String>> map, LinkedList<String> toProcess) {
    	LinkedList<String> ll;
    	String p;

    	while (toProcess.size() != 0) {
    		p = toProcess.removeFirst();
    		ll = hashM.get(p);
    		for (String name : ll) {
    			if (map.containsKey(name)) {
    				continue;
    			}
    			System.out.print(" " + name);
    			map.put(name,new LinkedList<String>());
    			toProcess.add(name);
    		}
    	}
    }

    /** 
      * Utility function to retrieve environment variables
     */
    private static String getEnvironVar(String env) {
    	return System.getenv(env);
    }

    public static void main(String[] args) {
    	int n = 0, i;
    	String cpath = getEnvironVar("CPATH");
    	String crawlerThreadsEnv = getEnvironVar("CRAWLER_THREADS");
    	Thread[] workers;
    	int argsLen = args.length;

        // we start getting all the directories
    	if (cpath != null) {
    		n = cpath.split(":").length;
    	}

    	for (i = 0; i < argsLen; ++i) {
    		if (args[i].substring(0,2).compareTo("-I") != 0) {
    			break;
    		}
    	}

    	int start = i;
    	int m = start - 1;

    	dir = new ArrayList<String>(m + n + 2);
    	dir.add("./");

    	for (i = 0; i < start; ++i) {
    		dir.add(createDir(args[i].substring(2)));
    	}

    	if (n > 0) {
    		String[] cpathDirs = cpath.split(":");
    		for (String direc : cpathDirs){
    			dir.add(createDir(direc));
    		}
    	}

        // initializing the 2 main thread-safe data structures
    	hashM = new ConcurrentHashMap<String, LinkedList<String>>();
    	workQueue = new LinkedBlockingQueue<String>();

    	for (i = start; i < argsLen; ++i) {
    		LinkedList<String> ll;
    		String root = getFileRoot(args[i]);
    		char ext = getFileExt(args[i]);
    		if (ext != 'y' && ext != 'c' && ext != 'l') {
    			System.err.println("Illegal extenstion: " + args[i] + " - must be .c, .y or .l");
    		}
    		String obj = root + ".o";
    		ll = new LinkedList<String>();
    		ll.add(args[i]);
    		hashM.put(obj, ll);
    		workQueue.add(args[i]);
    		ll = new LinkedList<String>();
    		hashM.put(args[i], ll);
    	}

        // we create a certain number of threads using the CRAWLER_THREADS env variable 
    	if (crawlerThreadsEnv != null) {
    		numberOfThreads = Integer.parseInt(crawlerThreadsEnv);
    	}
    	else {
    		numberOfThreads = 2;
    	}
    	workers = new Thread[numberOfThreads];

        // we initialize all our objects that will be used by the workers
    	timer = new CyclicBarrier(numberOfThreads + 1);
    	lock = new ReentrantLock();
    	queueEmpty = lock.newCondition();
    	done = false;

    	for (i = 0; i < numberOfThreads; ++i) {
    		workers[i] = new Thread(new Worker(workQueue, hashM));
    		workers[i].start();
    	}

        // here we trigger the actual beginning of the program
    	try {
    		timer.await();
    	}
    	catch (Exception e) {
    		e.printStackTrace();
    	}

        // we wait for all threads to finish and then continue to printing
    	try {
    		for (i = 0; i < numberOfThreads; ++i) {
    			workers[i].join();
    		}
    	}
    	catch (InterruptedException e) {
    		System.err.println("Error trying to join the thread.");
    	}

        // here we print out the dependencies for all the files we have passed on
        // as arguments when running the program
    	for (i = start; i < argsLen; ++i) {
    		ConcurrentHashMap<String, LinkedList<String>> printed;
    		LinkedList<String> toProcess;
    		String root = "";
    		char ext = ' ';
    		root = getFileRoot(args[i]);
    		ext = getFileExt(args[i]);
    		String obj = root + ".o";
    		System.out.print(obj + ":");
    		printed = new ConcurrentHashMap<String, LinkedList<String>>();
    		printed.put(obj, new LinkedList<String>());
    		toProcess = new LinkedList<String>();
    		toProcess.add(obj);
    		printDependencies(printed, toProcess);
    		System.out.println();
    	}
    }

    private static class Worker implements Runnable {

    	private LinkedBlockingQueue<String> queue;
    	private ConcurrentHashMap<String, LinkedList<String>> map;
    	private int index;

    	public Worker (LinkedBlockingQueue<String> queue, ConcurrentHashMap<String, LinkedList<String>> map) {
    		this.map = map;
    		this.queue = queue;
    	}

    	@Override
    	public void run() {
    		String workQueueIter;

            // this tries to make all threads start at almost the same time
            // such that we use them at their full power
    		try {
    			timer.await();
    		}
    		catch (Exception e) {
    			e.printStackTrace();
    		}

            // we do this while done is false
    		while (!done) {

                // we take the first element from the queue (removing it from there as well);
                // (the ConcurrentHashMap and LinkedBlockingQueue are created to be thread-safe,
                // thus getting, removing and all other operations are synchronized)
    			workQueueIter = queue.poll();
    			if (workQueueIter != null) {
    				LinkedList<String> ll = hashM.get(workQueueIter);
    				process(workQueueIter, ll);
    				lock.lock();
    				try {
    					queueEmpty.signalAll();
    				}
    				finally {
    					lock.unlock();
    				}
    			}

                // if the first element in the queue is empty, then we need to make sure that this thread
                // WAITS effectively until some other thread adds something in the queue
    			else {
    				lock.lock();
    				try {
    					if (lock.getWaitQueueLength(queueEmpty) == numberOfThreads - 1) {
    						queueEmpty.signalAll();
    						done = true;
    					}
    					else {
    						while (true) {
    							queueEmpty.await();
    						}
    					}
    				}
    				catch (InterruptedException e) {
    					e.printStackTrace();
    				}
    				finally {
    					lock.unlock();
    				}
    			}
    		}
    	}

        /*
         * This method processes the file given as an argument and finds all includes
         * inside it. Then, adds them to the hashMap and workQueue defined globally
        */
        private void process(String file, LinkedList<String> ll) {
        	try {
        		File currFile = openFile(file);
        		StringBuilder sb;

        		if (currFile == null) {
        			return;
        		}

        		BufferedReader br = new BufferedReader(new FileReader(currFile));
        		String line = null;
        		while ((line = br.readLine()) != null) {
        			int length = line.length();
        			if (length == 0) {
        				continue;
        			}
        			int index = 0;
        			LinkedList<String> newLL;
        			while (index < length && line.charAt(index) == ' ') {
        				++index;
        			}
        			if (index > length-8 || line.substring(index, index+8).compareTo("#include") != 0) {
        				continue;
        			}
        			index += 8;
        			while (line.charAt(index) == ' ') {
        				++index;
        			}
        			if (line.charAt(index) != '"') {
        				continue;
        			}
        			index++;
        			int len = line.length();
        			sb = new StringBuilder();
        			while (index < len) {
        				if (line.charAt(index) == '"') {
        					break;
        				}
        				sb.append(line.charAt(index));
        				++index;
        			}
        			String currName = sb.toString();
        			ll.add(currName);
        			if (hashM.containsKey(currName)) {
        				continue;
        			}
        			newLL = new LinkedList<String>();
        			hashM.put(currName, newLL);
        			workQueue.add(currName);
        		}
        	}
        	catch (Exception x) {
        		x.printStackTrace();
        	}
        }
    }
}