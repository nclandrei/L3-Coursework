/**
 *   Dependency discoverer in a multi-threaded environment
 *   Name: Andrei-Mihai Nicolae
 *   GUID: 2147392n
 *   This is my own work as defined in the Academic Ethics agreement I have
 *   signed.
 *       
*/

import java.util.*;
import java.util.concurrent.*;
import java.io.*;
import java.lang.*;

public class singleDepDiscoverer {

    private static ArrayList<String> dir;
    private static ConcurrentHashMap<String, LinkedList<String>> hashM;
    private static LinkedBlockingQueue<String> workQueue;

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

    private static String getFileRoot(String file) {
        return file.substring(0, file.indexOf('.'));
    }

    private static char getFileExt(String file) {
        return file.charAt(file.indexOf('.')+1);
    }

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


    private static void process(String file, LinkedList<String> ll) {
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
                if (line.compareTo("") == 0) {
                    break;
                }
                int index = 0;
                LinkedList<String> newLL;
                while (line.charAt(index) == ' ') {
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

    private static String getEnvironVar(String env) {
        return System.getenv(env);
    }

    public static void main(String[] args) {
        int n = 0, i;
        String cpath = getEnvironVar("CPATH");
        int argsLen = args.length;
        String workQueueIter;

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
        dir = new ArrayList<String>(m+n+2);
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

        while ((workQueueIter = workQueue.poll()) != null) {
            LinkedList<String> ll = hashM.get(workQueueIter);
            process(workQueueIter, ll);
            hashM.put(workQueueIter, ll);
        }
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
}

