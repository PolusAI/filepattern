package filepattern.test;

import static org.junit.Assert.*;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.HashMap;
import java.io.File;
import java.nio.file.*;
import java.io.*;
import java.util.Iterator;

import filepattern.java.*;


public class TestFilePattern {

    @BeforeAll
    public static void setUp() {
        TestData data = new TestData("test_fp_data");

        try {
            data.generateData();
        } catch (Exception e) {
            fail("Error creating test data." + e);
        }
        
        ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> fp_result = data.getResults();
    }
    
    @Test
    public void testFilePattern() {

        FilePattern fp = null;
        try {
             fp = new FilePattern.FilePatternBuilder("test_fp_data")
                    .recursive(false)
                    .filePattern("img_r00{r:d}_c00{c:d}_{channel:c+}.tif")
                    .suppressWarnings(false)
                    .blockSize("")
                    .recursive(false).build();
        } catch (Exception e) {
            fail("Error creating FilePattern object.");
        }

        ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> result = new ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>>();

        for (Iterator<?> i = fp.iterator(); i.hasNext(); ) {
            result.add((Pair<HashMap<String, Object>, ArrayList<Path>>) i.next());
        }

        ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> truth = TestData.getResults();

        assertEquals(result.size(), truth.size());
        assertEquals(result, truth);
    }

    @Test
    public void testGetMatching() {

        FilePattern fp = null;
        try {
            fp = new FilePattern.FilePatternBuilder("test_fp_data")
                    .recursive(false)
                    .filePattern("img_r00{r:d}_c00{c:d}_{channel:c+}.tif")
                    .suppressWarnings(false)
                    .blockSize("")
                    .recursive(false).build();
        } catch (Exception e) {
            fail("Error creating FilePattern object.");
        }

        ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> result = new ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>>();

        HashMap<String, Object> matching = new HashMap<>();
        matching.put("r", 1);

        for (Iterator<?> i = fp.iterator(matching); i.hasNext(); ) {
            result.add((Pair<HashMap<String, Object>, ArrayList<Path>>) i.next());
        }

        ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> truth = TestData.getMatchingResults();

        assertEquals(result.size(), truth.size());
        assertEquals(result, truth);
    }

    @Test
    public void testGroupBy() {

        FilePattern fp = null;
        try {
            fp = new FilePattern.FilePatternBuilder("test_fp_data")
                    .recursive(false)
                    .filePattern("img_r00{r:d}_c00{c:d}_{channel:c+}.tif")
                    .suppressWarnings(false)
                    .blockSize("")
                    .recursive(false).build();
        } catch (Exception e) {
            fail("Error creating FilePattern object.");
        }

        ArrayList<Pair<ArrayList<Pair<String, Object>>, ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>>>> result = new ArrayList<>();

        fp.setGroup("r");

        for (Iterator<?> i = fp.iterator(); i.hasNext(); ) {
            result.add((Pair<ArrayList<Pair<String, Object>>, ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>>>) i.next());
        }

        ArrayList<Pair<ArrayList<Pair<String, Object>>, ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>>>> truth = TestData.getGroupedResults();

        System.out.println(result.size());
        System.out.println(truth.size());

        assertEquals(result.size(), truth.size());
        assertEquals(result, truth);
        
    }

}
