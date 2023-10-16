package filepattern.java;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.ArrayList;
import java.util.Iterator;
import java.nio.file.*;
import java.io.*;

//std::vector<std::tuple<std::map<std::string, std::variant<int, std::string>>, std::vector<std::filesystem::path>>>
// ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>

public class FilePattern implements Iterable{

    private FilePatternBindings.FilePattern fp;
    private FilePatternBuilder builder;
    private boolean external;
    private String[] groups;

    public static class FilePatternBuilder {
        private String path;
        private String filePattern = "";
        private boolean recursive = false;
        private boolean suppressWarnings = false;
        private String blockSize = "";

        public FilePatternBuilder(String path) {
            this.path = path;
        }

        public FilePatternBuilder filePattern(String filePattern) {
            this.filePattern = filePattern;
            return this;
        }

        public FilePatternBuilder recursive(boolean recursive) {
            this.recursive = recursive;
            return this;
        }

        public FilePatternBuilder suppressWarnings(boolean suppressWarnings) {
            this.suppressWarnings = suppressWarnings;
            return this;
        }

        public FilePatternBuilder blockSize(String blockSize) {
            this.blockSize = blockSize;
            return this;
        }

        public FilePattern build() throws IOException {
            FilePattern fp = new FilePattern(this);
            return fp;
        }

    }

    private FilePattern(FilePatternBuilder builder) throws IOException {

        this.builder = builder;

        this.fp = new FilePatternBindings.FilePattern(builder.path, builder.filePattern, builder.blockSize, builder.recursive, builder.suppressWarnings); // need to add builder to FPOjbect

    }

    public String getPattern() {
        return this.fp.getPattern();
    }

    public ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> getMatching(HashMap<String, Object> keywordArgs) throws IllegalArgumentException {

        this.checkKeywordArgs(keywordArgs);

        return FilePatternBindings.FilePatternVector.cast(fp.getMatchingByMap(FilePatternBindings.StringVariantMap.cast(keywordArgs)));
    }

    public HashMap<String, HashMap<Object, Integer>> getOccurrences(HashMap<String, Object> keywordArgs) throws IllegalArgumentException {
        this.checkKeywordArgs(keywordArgs);

        return FilePatternBindings.StringMapMap.cast(this.fp.getOccurrencesByMap(FilePatternBindings.StringVariantMap.cast(keywordArgs)));
    }

    
    public HashMap<String, HashSet<Object>> getUniqueValues(String ... variables) throws IllegalArgumentException {

        return FilePatternBindings.StringSetMap.cast(this.fp.getUniqueValues(new FilePatternBindings.StringVector(variables)));

    }
    

    public String outputName(ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> files) {

        return this.fp.outputName(FilePatternBindings.TupleVector.cast(files));
    }

    public ArrayList<String> getVariables() {

        return FilePatternBindings.StringVector.cast(new FilePatternBindings.StringVector(this.fp.getVariables()));

    }

    public void getNewNaming(String pattern, boolean suppressWarnings) {

        this.fp.getNewNaming(pattern, suppressWarnings);

    }

 
    public String getPath() {

        return this.fp.getPath();
    }

    public ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> getFiles() {
        return FilePatternBindings.TupleVector.cast(this.fp.getFiles());
    }

    public Iterator<?> iterator() {
        
        if (this.groups.length == 0) {
            return new FilePatternIterator(this);
        } else {
            return new FilePatternGroupedIterator(this);
        }
    }

    public Pair<HashMap<String, Object>, ArrayList<Path>> getAt(int index) {
        return FilePatternBindings.TupleVector.cast(this.fp.getSliceByIdx(index)).get(0);
    }

    
    public Pair<ArrayList<Pair<String, Object>>, ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>>> getAtGrouped(int index) {
        return FilePatternBindings.PairTupleVector.cast(this.fp.getGroupedSliceByIdx(index));
    }
    

    private void checkKeywordArgs(HashMap<String, Object> keywordArgs) throws IllegalArgumentException {
        for (Map.Entry<String, Object> mapElement : keywordArgs.entrySet()) {

            Object value = mapElement.getValue();

            if(!(value instanceof Integer || value instanceof String)) {
                throw new IllegalArgumentException("Value of keywordArgs must be Integer or String.");
            }
        }
    }

    public int getSize() {
        return this.fp.getSize();
    }

    public void setGroup(String ... groups) {

        this.groups = groups;
        
        this.fp.setGroup(new FilePatternBindings.StringVector(groups));

    }
 
    private class FilePatternIterator implements Iterator<Pair<HashMap<String, Object>, ArrayList<Path>>> {

        private int current;
        private FilePattern fp;

        public FilePatternIterator(FilePattern obj) {
            fp = obj;
            current = 0;
        }

        public boolean hasNext() {
            return (current + 1) <= this.fp.getSize();
        }

        public Pair<HashMap<String, Object>, ArrayList<Path>> next() {
            return this.fp.getAt(current++);
        }

    }

    private class FilePatternGroupedIterator implements Iterator<Pair<ArrayList<Pair<String, Object>>, ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>>>> {

        private int current;
        private FilePattern fp;

        public FilePatternGroupedIterator(FilePattern obj) {
            fp = obj;
            current = 0;
        }

        public boolean hasNext() {
            return (current + 1) <= this.fp.getSize();
        }

        
        public Pair<ArrayList<Pair<String, Object>>, ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>>> next() {
            return this.fp.getAtGrouped(current++);
        }
    

    }

    public static void main(String[] args) throws IOException {

        FilePattern fp = new FilePattern.FilePatternBuilder("/Users/jmckinzie/Documents/GitHub/filepattern-1/data").recursive(false)
                                        .filePattern("img_r00{r:d}_c00{c:d}.tif")
                                        .suppressWarnings(false)
                                        .blockSize("")
                                        .recursive(false).build();

        ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> result = new ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>>();

        Iterator<?> iter = fp.iterator();

        for (Iterator<?> i = fp.iterator(); i.hasNext(); ) {
            System.out.println(i.next());
        }
    }
}
