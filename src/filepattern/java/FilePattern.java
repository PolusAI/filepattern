package filepattern.java;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Arrays;
import java.nio.file.*;
import java.io.*;

/**
 * This class provides use of the filepattern package.
 * filepattern  is used to store files that follow a pattern, where the pattern is 
 * analogous to a simplified regular expression. This class allows a user to specify
 * a filepattern, and then iterate over the FilePattern object to obtain files matching
 * the given filepattern
 * 
 * For example, consider the follwoing files in a directory:
 * 
 * data/
 *      img_r01_c01.tif
 *      img_r01_c02.tif
 *      img_r02_c01.tif
 *      img_r02_c02.tif
 * 
 * A filepattern to capture these file would be "img_r{r:dd}_c{c:dd}.tif",
 * where "dd" captures two digits and assigns the value to a variable named "r" another "c".
 * 
 * For more information on how to use this class, please see the ReadtheDocs or the Github for filepattern.
 * 
 * Github: https://github.com/PolusAI/filepattern
 * rtd: https://filepattern.readthedocs.io/en/latest/?badge=latest
 *  
 */
public class FilePattern implements Iterable{

    private FilePatternBindings.FilePattern fp;
    private FilePatternBuilder builder;
    private boolean external;
    private ArrayList<String> groups = new ArrayList<>();

    /**
     * Builder class for FilePattern to provide optional variables when constructing
     * a FilePattern.
     */
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

    /**
     * Construct a FilePattern object from a Builder
     * @param builder Builder containing the FilePattern parameters
     * @throws IOException Throws IOException when supplied directory path is not found.
     */
    private FilePattern(FilePatternBuilder builder) throws IOException {

        this.builder = builder;
        this.fp = new FilePatternBindings.FilePattern(builder.path, builder.filePattern, builder.blockSize, builder.recursive, builder.suppressWarnings); // need to add builder to FPOjbect

    }

    /**
     * Get the filepattern that is currently being used.
     * 
     * @return The filepattern.
     */
    public String getPattern() {
        return this.fp.getPattern();
    }

    /**
     * This method will return a list containing all files where the variable matches the supplied. For example,
     * if the HashMap `{"x", 1}` is passed to get matching, all files where x is 1 will be returned. A list of values
     * can also be passed, such as `{"x", {1,2,3}}`. Furthermore, an arbitrary number of variables and values can be passed,
     * such as {{x, 1}, {y, 2}, {z, 3}}`. 
     * 
     * @param keywordArgs HashMap mapping variable names to the values to be matched.
     * @return An ArrayList of files matching the variable values requested.
     * @throws IllegalArgumentException An exception will be thrown if invalid variables are used as keys in the HashMap.
     */
    public ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> getMatching(HashMap<String, Object> keywordArgs) throws IllegalArgumentException {

        this.checkKeywordArgs(keywordArgs);

        return FilePatternBindings.FilePatternVector.cast(fp.getMatchingByMap(FilePatternBindings.StringVariantMap.cast(keywordArgs)));
    }


    /**
     * Takes in a variable as the key and a list of values as the value and returns the a HashMap
     * mapping the variable to a HashMap of the values mapped to the number of occurrences of the variable
     * value.

     * For example, if the filepattern is `img_r{r:ddd}_c{r:ddd}.tif` and r=1 occurs 20 times in the path,
     * then the passing `{"r", 1}` will return `{"r", {1: 20}}`.
     * @param keywordArgs
     * @return
     * @throws IllegalArgumentException
     */
    public HashMap<String, HashMap<Object, Integer>> getOccurrences(HashMap<String, Object> keywordArgs) throws IllegalArgumentException {
        this.checkKeywordArgs(keywordArgs);

        return FilePatternBindings.StringMapMap.cast(this.fp.getOccurrencesByMap(FilePatternBindings.StringVariantMap.cast(keywordArgs)));
    }

    /**
     * Given variable names from the filepattern as arguments, this method returns a HashMap
     * of mapping the variable names to a set of the unique values for each variable. If no variables are
     * provided, all variables will be returned.
     *
     * For example if the filepattern is `img_r{r:ddd}_c{r:ddd}.tif` and `r` ranges from 1 to 3 and c ranges from 1 to 2,
     * then fp_object.getUniqueValues("r", "c") will return `{"r", {1,2,3}, "c", {1,2}}`.
     * 
     * @param variables Name of variables that exist in the filepattern.
     * @return The unique values for the supplied variables.
     * @throws IllegalArgumentException
     */
    public HashMap<String, HashSet<Object>> getUniqueValues(String ... variables) throws IllegalArgumentException {

        return FilePatternBindings.StringSetMap.cast(this.fp.getUniqueValues(new FilePatternBindings.StringVector(variables)));

    }
    
    /**
     * Returns a single filename that captures variables from a list of files.
     * 
     * Given a list of files, this method will return a single filename that captures the variables from each
     * file in the list. If a variable is constant through the list, the variable value will be in the returned
     * name. If a variable is not constant, the minimum and maximum values will appear in the returned name in
     * the form "(min-max)".

     * @param files List of FilePattern file mapping to get the output name of.
     * @return The outputname for the ArrayList of files.
     */
    public String outputName(ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> files) {

        return this.fp.outputName(FilePatternBindings.TupleVector.cast(files));
    }


    /**
     * Returns a list of variables that are contained in the filepattern

     * For example, if the filepattern is `img_x{x:d}_y{y:d}_c{c:c+}.tif`, get_variables will return
     * the list `{x, y, c}`.
     * 
     * @return ArrayList of variables contained in the filepattern.
     */
    public ArrayList<String> getVariables() {

        return FilePatternBindings.StringVector.cast(new FilePatternBindings.StringVector(this.fp.getVariables()));

    }

    /**
     * Replace the first version of filepattern naming style with the updated version. This method
     * will update the filepattern being used in the FilePattern object.
     * 
     * @param pattern A String containing the old style of filepattern.
     * @param suppressWarnings True to suppress warnings for using deprecated style
     */
    public void getNewNaming(String pattern, boolean suppressWarnings) {

        this.fp.getNewNaming(pattern, suppressWarnings);

    }

    
    /**
     * Get the path supplied to the FilePattern object
     * 
     * @return The path the FilePattern object is using.
     */
    public String getPath() {

        return this.fp.getPath();
    }

    /**
     * Get the nested structure of file variable values and path. This returns what the iterator
     * would return but in a single ArrayList.
     * 
     * @return ArrayList of variable values and paths for all files found by the filepattern.
     */
    public ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> getFiles() {
        return FilePatternBindings.TupleVector.cast(this.fp.getFiles());
    }

    /**
     * Get the iterator for iterating over the files found by the filepattern.
     */
    public Iterator<?> iterator() {
        
        if (this.groups.size() == 0) {
            return new FilePatternIterator(this);
        } else {
            return new FilePatternGroupedIterator(this);
        }
    }

    /**
     * Get the iterator for iterating over grouped files found by the filepattern. 
     * 
     * For example, if groups="r", then the files will be returned in groups for each value of "r".
     * 
     * @param groups Names of variables contained in the filepattern to group by.
     * @return An iterator for the grouped files.
     */
    public Iterator<?> iterator(String ... groups) {
        
        this.setGroup(groups);

        return new FilePatternGroupedIterator(this);
        
    }

    /**
     * Get iterator for files matching specific values.
     * 
     * This will return lists containing all files where the variable matches the supplied. For example,
     * if the argument `{"x", 1}` is passed, all files where x is 1 will be returned.
     *
     * @param keywordArgs A hashmap containg variables in the filepattern mapped to values to get the matching files for.
     * @return The files matching the values specified in the arguments.
     */
    public Iterator<?> iterator(HashMap<String, Object> keywordArgs) {
        return this.getMatching(keywordArgs).iterator();
    }

    /**
     * Randomly access files found by the filepattern by index.
     * 
     * @param index Index of the file to be accessed.
     * @return The file stored at `index`.
     */
    public Pair<HashMap<String, Object>, ArrayList<Path>> getAt(int index) {
        return FilePatternBindings.TupleVector.cast(this.fp.getSliceByIdx(index)).get(0);
    }

    /**
     * Randomly access of grouped files found by the filepattern by index.
     * 
     * @param index Index of the file to be accessed.
     * @return The grouped files stored at `index`.
     */
    public Pair<ArrayList<Pair<String, Object>>, ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>>> getAtGrouped(int index) {
        return FilePatternBindings.PairTupleVector.cast(this.fp.getGroupedSliceByIdx(index));
    }
    
    /**
     * Check if the arguments passed contain valid data.
     * 
     * @param keywordArgs HashMap mapping variable names to values.
     * @throws IllegalArgumentException
     */
    private void checkKeywordArgs(HashMap<String, Object> keywordArgs) throws IllegalArgumentException {
        for (Map.Entry<String, Object> mapElement : keywordArgs.entrySet()) {

            Object value = mapElement.getValue();

            if(!(value instanceof Integer || value instanceof Double || value instanceof String)) {
                throw new IllegalArgumentException("Value of keywordArgs must be Integer or String.");
            }
        }
    }

    /**
     * Get the number of files found by the filepattern.
     * 
     * @return The number of files found by the filepattern.
     */
    public int getSize() {
        return this.fp.getSize();
    }

    /**
     * Get the number of groups found by the filepattern.
     * 
     * @return The number of groups found by the filepattern.
     */
    public int getGroupedSize() {
        return this.fp.getGroupedSize();
    }

    /**
     * Set the variables to group the found files by.
     * 
     * @param groups Name of variables in the filepattern to group the files by.
     */
    public void setGroup(String ... groups) {

        this.groups = new ArrayList<String>(Arrays.asList(groups));
        
        this.fp.setGroup(new FilePatternBindings.StringVector(groups));

        this.fp.group(new FilePatternBindings.StringVector(groups));

    }
    
    /**
     * Iterator for the files found by the filepattern.
     */
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

    /**
     * Iterator for groups found by the file pattern and grouped by the groups specified in setGroups().
     */
    private class FilePatternGroupedIterator implements Iterator<Pair<ArrayList<Pair<String, Object>>, ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>>>> {

        private int current;
        private FilePattern fp;

        public FilePatternGroupedIterator(FilePattern obj) {
            fp = obj;
            current = 0;
        }

        public boolean hasNext() {
            return (current + 1) <= this.fp.getGroupedSize();
        }
        
        public Pair<ArrayList<Pair<String, Object>>, ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>>> next() {
            return this.fp.getAtGrouped(current++);
        }
    }
}
