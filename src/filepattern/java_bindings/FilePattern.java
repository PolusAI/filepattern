package filepattern2.java_bindings;

import java.util.HashMap;
import java.util.Map;
import java.util.ArrayList;
import java.util.Iterator;
//import java.util.AbstractMap;
import java.nio.file.*;
import java.io.*;

//std::vector<std::tuple<std::map<std::string, std::variant<int, std::string>>, std::vector<std::filesystem::path>>>
// ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>

public class FilePattern implements Iterable {
    
    private FilePatternBindings.FilePattern fp;
    private FilePatternBuilder builder;
    private boolean external;

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
            
        /*
        FilePatternFactory patternFactory = new FilePatternFactory();

        this.fp = patternFactory.getObject(builder);
        */
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

    /*
    public ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> getUniqueOccurrences(HashMap<String, Object> keywordArgs) throws IllegalArgumentException {
        this.checkKeywordArgs(keywordArgs);

        return fp.getUniqueOccurrences(keywordArgs);
    }
    */

    public String outputName(ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> files) {

        return this.fp.outputName(FilePatternBindings.TupleVector.cast(files));
    }

    public String getPath() {

        return this.fp.getPath();
    }

    public ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> getFiles() {
        return FilePatternBindings.TupleVector.cast(this.fp.getFiles());
    }

    public Iterator<Pair<HashMap<String, Object>, ArrayList<Path>>> iterator() {
        return new FilePatternIterator(this);
    }

    public Pair<HashMap<String, Object>, ArrayList<Path>> getAt(int index) {
        return FilePatternBindings.TupleVector.cast(this.fp.getSliceByIdx(index)).get(0);
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
    /*
    private class FilePatternFactory {

        public FilePatternBindings.PatternObject getObject(FilePatternBuilder builder) {
            if (builder.blockSize.equals("")) {
                if(builder.path.endsWith(".txt")) {
                    FileReader infile = new FileReader(builder.path);
                    BufferedReader buffer = new BufferedReader(infile);
    
                    String line = buffer.readLine();
    
                    if(line.matches("file\\: .+?; corr\\: .+?; position\\: .+?; grid\\: .+?;")) {
                        return new FilePatternBindings.VectorPatternObject(builder.path, builder.filePattern); // need to add builder to FPOjbect
                    }
                    
                    return new FilePatternBindings.StringPatternObject(builder.path, builder.filePattern); // need to add builder to FPOjbect
                }
    
                return new FilePatternBindings.FilePatternObject(builder.path); // need to add builder to FPOjbect
            }
    
            if(builder.path.endsWith(".txt")) {
                FileReader infile = new FileReader(builder.path);
                BufferedReader buffer = new BufferedReader(infile);
    
                String line = buffer.readLine();
    
                if(line.matches("file\\: .+?; corr\\: .+?; position\\: .+?; grid\\: .+?;")) {
                    return new FilePatternBindings.ExternalVectorPatternObject(builder.path, builder.filePattern); // need to add builder to FPOjbect
                }
                
                return new FilePatternBindings.ExternalStringPatternObject(builder.path, builder.filePattern, builder.blockSize); // need to add builder to FPOjbect
            }
    
            return new FilePatternBindings.ExternalFilePatternObject(builder.path, builder.blockSize); // need to add builder to FPOjbect
        }
    }
    */

    /*
    private static class Cast {
        public ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> cast(FilePatternVector vec) {
            ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> casted = new ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>>();

            for (int i = 0; i < vec.size(); ++i) {
                FileVector path = vec[i].get1();
                StringVariant map = vec[i].get0();

            }

        }

    }
    */
    public static void main(String[] args) throws IOException {
        // Pointer objects allocated in Java get deallocated once they become unreachable,
        // but C++ destructors can still be called in a timely fashion with Pointer.deallocate()
        //FilePattern fp = new FilePattern("/home/ec2-user/Dev/Demo/test_data/data", "img_r{r:ddd}_c{c:ddd}_{channel:c+}.tif", false, true);
        //p.init("/home/ec2-user/Dev/Demo/test_data/data", "img_r{r:ddd}_c{c:ddd}_{channel:c+}.tif", false, true);
        //fp.printFiles();

        //FilePatternVector files = fp.getFiles();

        //ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> vec = FilePatternVector.cast(files);

        FilePattern fp = new FilePattern.FilePatternBuilder("/home/jessemckinzie/Dev/data").recursive(false)
                                        .filePattern("img_r00{r:d}_c00{c:d}_{channel:c+}.tif")
                                        .suppressWarnings(false)
                                        .blockSize("")
                                        .recursive(false).build();

        System.out.println("Path: ");
        System.out.println(fp.getPattern());

        ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> result = new ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>>();

        //result = fp.getFiles();

        //System.out.println("success");

        /*
        HashMap<String, Object> match = new HashMap<String, Object>();

        match.put("r",  (int) 1);

        result = fp.getMatching(match);

        System.out.println(result.size());

        for(Pair<HashMap<String, Object>, ArrayList<Path>> pair: result) {
            System.out.println(pair.first);
            System.out.print(pair.second);
            System.out.println("");
        }
        */
        

        Iterator<Pair<HashMap<String, Object>, ArrayList<Path>>> iter = fp.iterator();

        for (Iterator i = fp.iterator(); i.hasNext(); ) {
            System.out.println(i.next());
        }
    }
}