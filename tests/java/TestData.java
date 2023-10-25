package filepattern.test;

import java.util.ArrayList;
import java.util.HashMap;
import java.nio.file.*;
import java.io.*;

import filepattern.java.*;

public class TestData {
    
    private static final int max_r = 3;
    private static final int max_c = 3;
    private static ArrayList<String> channels = new ArrayList<>();
    private static String path;
    private static ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> fp_result = new ArrayList<>();
    private static ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> get_matching_result = new ArrayList<>();
    private static ArrayList<Pair<ArrayList<Pair<String, Object>>, ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>>>> groupedData = new ArrayList<>();
    
    
    private static boolean initialized = false;


    public TestData(String path) {

        TestData.path = path;

        TestData.channels.add("DAPI");
        TestData.channels.add("GFP");
        TestData.channels.add("TXREAD");

        File dataDirectory = new File(path);
        if (!dataDirectory.exists()){
            dataDirectory.mkdirs();
        }

    }

    public void generateData() throws IOException {

        if (TestData.initialized) {
            return;
        }
        
        for (int r = 1; r <= max_r; ++r) {
            ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> fileGroup = new ArrayList<>();
            for (int c = 1; c <= max_c; ++c) {
                for (String channel : channels) {

                    String filename = "img_r00" + Integer.toString(r) + "_c00" + Integer.toString(c) + "_" + channel + ".tif";
                    
                    String filePath = TestData.path + "/" + filename;

                    File img = new File(filePath);
                    
                    if (img.createNewFile()) {  
                        System.out.println("Create file " + filename);
                    } else {
                        System.out.println("Error creating file " + filename);
                    }

                    HashMap<String, Object> map = new HashMap<>();

                    map.put("r", r);
                    map.put("c", c);
                    map.put("channel", channel);
                    
                    ArrayList<Path> paths = new ArrayList<>();
                    paths.add(Paths.get(filePath));

                    Pair<HashMap<String, Object>, ArrayList<Path>> pair = new Pair<>();
                    
                    pair.first = map;
                    pair.second = paths;

                    TestData.fp_result.add(pair);

                    if (r == 1) {
                        TestData.get_matching_result.add(pair);
                    }

                    fileGroup.add(pair);
                }
            }
            
            Pair<ArrayList<Pair<String, Object>>, ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>>> currentGroup = new Pair<>();
            
            Pair<String, Object> group = new Pair<>();
            group.first = "r";
            group.second = r;

            ArrayList<Pair<String, Object>> arrayGroup = new ArrayList<>();
            arrayGroup.add(group);

            currentGroup.first = arrayGroup;
            currentGroup.second = fileGroup;    

            groupedData.add(currentGroup);
        }

        initialized = true;
    }

    public static ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> getResults() {
        return TestData.fp_result;
    }

    public static ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> getMatchingResults() {
        return TestData.get_matching_result;
    }

    public static ArrayList<Pair<ArrayList<Pair<String, Object>>, ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>>>> getGroupedResults() {
        return TestData.groupedData;
    }


}
