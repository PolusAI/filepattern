//package filepatternbindings;

package filepattern2.java_bindings;

import java.util.HashMap;
import java.util.Map;
import java.util.ArrayList;
import java.util.Iterator;
//import java.util.AbstractMap;
import java.nio.file.*;
import java.io.*;

import org.bytedeco.javacpp.*;
import org.bytedeco.javacpp.annotation.*;
/*
import org.bytedeco.javacpp.tools.Info;
import org.bytedeco.javacpp.tools.InfoMap;
import org.bytedeco.javacpp.tools.InfoMapper;
*/

import org.bytedeco.javacpp.BytePointer;
import org.bytedeco.javacpp.ClassProperties;
import org.bytedeco.javacpp.FunctionPointer;
import org.bytedeco.javacpp.LoadEnabled;
import org.bytedeco.javacpp.Loader;
import org.bytedeco.javacpp.Pointer;
import org.bytedeco.javacpp.PointerPointer;
import org.bytedeco.javacpp.annotation.Adapter;
import org.bytedeco.javacpp.annotation.ByRef;
import org.bytedeco.javacpp.annotation.ByVal;
import org.bytedeco.javacpp.annotation.Cast;
import org.bytedeco.javacpp.annotation.Platform;
import org.bytedeco.javacpp.annotation.Properties;
import org.bytedeco.javacpp.annotation.StdString;
import org.bytedeco.javacpp.presets.javacpp;
import org.bytedeco.javacpp.tools.BuildEnabled;
import org.bytedeco.javacpp.tools.Info;
import org.bytedeco.javacpp.tools.InfoMap;
import org.bytedeco.javacpp.tools.InfoMapper;
import org.bytedeco.javacpp.tools.Logger;

/*
@Platform(compiler="cpp17",
        include={ "<vector>",
                "<map>",
                "<tuple>",
                "<variant>",
                "<string>",
                "<filesystem>",
                "../cpp/interface/filepattern.h",
                "../cpp/interface/filepattern.cpp",
                "../cpp/util/util.hpp"},
        link="stdc++fs")
*/

@Platform(compiler="cpp17",
        include={ "<vector>",
                "<map>",
                "<tuple>",
                "<variant>",
                "<string>",
                "<filesystem>",
                "../cpp/pattern.cpp",
                "../cpp/pattern.hpp",
                "../cpp/internal/filepattern.cpp",
                "../cpp/internal/filepattern.hpp",
                "../cpp/internal/stringpattern.cpp",
                "../cpp/internal/stringpattern.hpp",
                "../cpp/internal/vectorpattern.cpp",
                "../cpp/internal/vectorpattern.hpp",
                "../cpp/internal/internal_pattern.hpp",
                "../cpp/internal/internal_pattern.cpp",
                "../cpp/internal/internal_pattern.cpp",
                "../cpp/internal/internal_pattern.hpp",
                "../cpp/external/external_pattern.cpp",
                "../cpp/external/external_pattern.hpp",
                "../cpp/external/external_filepattern.cpp",
                "../cpp/external/external_filepattern.hpp",
                "../cpp/external/external_stringpattern.cpp",
                "../cpp/external/external_stringpattern.hpp",
                "../cpp/external/external_vectorpattern.cpp",
                "../cpp/external/external_vectorpattern.hpp",
                "../cpp/interface/filepattern.h",
                "../cpp/interface/filepattern.cpp",
                "../cpp/pattern_object.hpp",
                "../cpp/util/util.hpp"},
        link="stdc++fs")
public class FilePatternBindings implements InfoMapper {

    public void map(InfoMap infoMap) {

       infoMap.put(new Info("std::vector<std::tuple<std::map<std::string, std::variant<int, std::string, double>>, std::vector<std::string>>>").pointerTypes("FilePatternVector").define());
       infoMap.put(new Info("std::tuple<std::map<std::string, std::variant<int, std::string, double>>, std::vector<std::string>>").pointerTypes("Tuple").define());
       infoMap.put(new Info("std::map<std::string, std::variant<int, std::string, double>>").pointerTypes("Map").define());
       infoMap.put(new Info("std::vector<std::string>").pointerTypes("FileVector").define());
       infoMap.put(new Info("std::variant<int, std::string, double>>").pointerTypes("Variant").define());
       infoMap.put(new Info("std::vector<std::string>").pointerTypes("StringVector").define());
       infoMap.put(new Info("std::map<std::string, std::variant<int, std::string, double>>").pointerTypes("StringVariantMap").define());
       infoMap.put(new Info("std::string").pointerTypes("Path").define());

    }

    //@Name("int")
    //public static class Int extends Pointer {

    //}

    @Name("std::map<Types, int>")
    public static class TypesIntMap extends Pointer {
        static { Loader.load(); }
        /** Pointer cast constructor. Invokes {@link Pointer#Pointer(Pointer)}. */
        public TypesIntMap(Pointer p) { super(p); }
        public TypesIntMap()       { allocate();  }
        private native void allocate();

        public native long size();

        //@Index public native @ByRef int get(Variant i);

        public native @ByVal Iterator begin();
        public native @ByVal Iterator end();
        @NoOffset @Name("iterator") public static class Iterator extends Pointer {
            public Iterator(Pointer p) { super(p); }
            public Iterator() { }

            public native @Name("operator++") @ByRef Iterator increment();
            public native @Name("operator==") boolean equals(@ByRef Iterator it);
            public native @Name("operator*().first") @MemberGetter @ByVal Variant first();
            public native @Name("operator*().second") @MemberGetter @ByVal int second();
        }

        public static HashMap<Object, Integer> cast(TypesIntMap map) {
            Iterator mapIter = map.begin();
            Iterator mapEnd = map.end();

            HashMap<Object, Integer> casted = new HashMap<Object, Integer>();
            while(mapIter.increment() != mapEnd) {
                casted.put(mapIter.first(), new Integer(mapIter.second()));
            }

            return casted;
        }

    }

    /*
    @Name("std::set<Types>")
    public static class TypeSet extends Pointer {
        static { Loader.load(); }
        public TypeSet()       { allocate();  }
        public TypeSet(long n) { allocate(n); }

        private native void allocate();
        private native void allocate(long n);

        @Name("operator[]")
        public native @ByRef Variant get(long n);
        public native @ByRef Variant at(long n);

        public native long size();
        public native @Cast("bool") boolean empty();

        public static ArrayList<Object> cast(TypeSet set) {
            ArrayList<Object> casted = new ArrayList<Object>();
            for(int i = 0; i < set.size(); ++i) {
                casted.add(set.get(i));
            }

            return casted;
        }
    }

    @Name("std::map<std::string, std::set<Types>")
    public static class StringSetMap extends Pointer {
        static { Loader.load(); }

        public StringSetMap(Pointer p) { super(p); }
        public StringSetMap()       { allocate();  }
        private native void allocate();

        public native long size();

        @Index public native @ByRef int get(Variant i);

        public native @ByVal Iterator begin();
        public native @ByVal Iterator end();
        @NoOffset @Name("iterator") public static class Iterator extends Pointer {
            public Iterator(Pointer p) { super(p); }
            public Iterator() { }

            public native @Name("operator++") @ByRef Iterator increment();
            public native @Name("operator==") boolean equals(@ByRef Iterator it);
            public native @Name("operator*().first") @MemberGetter @ByVal @StdString String first();
            public native @Name("operator*().second") @MemberGetter @ByVal TypeSet second();
        }

        public static HashMap<String, ArrayList<Object>> cast(StringSetMap map) {
            Iterator mapIter = map.begin();
            Iterator mapEnd = map.end();

            HashMap<String, ArrayList<Object>> casted = new HashMap<String, ArrayList<Object>>();
            while(mapIter.increment() != mapEnd) {
                casted.put(mapIter.first(), TypeSet.cast(mapIter.second()));
            }

            return casted;
        }
    }
    */

    @Name("std::map<std::string, std::map<Types, int>>")
    public static class StringMapMap extends Pointer {
        static { Loader.load(); }

        public StringMapMap(Pointer p) { super(p); }
        public StringMapMap()       { allocate();  }
        private native void allocate();

        public native long size();

        @Index public native @ByRef TypesIntMap get(@StdString String i);

        public native @ByVal Iterator begin();
        public native @ByVal Iterator end();
        @NoOffset @Name("iterator") public static class Iterator extends Pointer {
            public Iterator(Pointer p) { super(p); }
            public Iterator() { }

            public native @Name("operator++") @ByRef Iterator increment();
            public native @Name("operator==") boolean equals(@ByRef Iterator it);
            public native @Name("operator*().first") @MemberGetter @ByVal @StdString String first();
            public native @Name("operator*().second") @MemberGetter @ByVal TypesIntMap second();
        }

        public static HashMap<String, HashMap<Object, Integer>> cast(StringMapMap map) {
            Iterator mapIter = map.begin();
            Iterator mapEnd = map.end();

            HashMap<String, HashMap<Object, Integer>> casted = new HashMap<String, HashMap<Object, Integer>>();
            while(mapIter.increment() != mapEnd) {
                casted.put(mapIter.first(), TypesIntMap.cast(mapIter.second()));
            }

            return casted;
        }

    }

    @Name("std::variant<int, std::string, double>")
    public static class Variant extends Pointer {
        static { Loader.load(); }

        public Variant(Pointer p) { super(p); }
        public Variant()       { allocate();  }
        private native void allocate();

        @Name("operator=")
        public native @ByVal Variant put(@ByRef int value);

        @Name("operator=")
        public native @ByVal Variant put(@ByRef @StdString String value);

        @Name("operator=")
        public native @ByVal Variant put(@ByRef double value);

    }

    @Name("std::map<std::string, std::variant<int, std::string, double>>")
    public static class StringVariantMap extends Pointer {
        static { Loader.load(); }

        public StringVariantMap(Pointer p) { super(p); }
        public StringVariantMap()       { allocate();  }
        private native void allocate();

        public native long size();

        @Index public native @ByRef Variant get(@StdString String i);
        public native StringVariantMap put(@StdString String i, Variant value);

        public native @ByVal Iterator begin();
        public native @ByVal Iterator end();
        @NoOffset @Name("iterator") public static class Iterator extends Pointer {
            public Iterator(Pointer p) { super(p); }
            public Iterator() { }

            public native @Name("operator++") @ByRef Iterator increment();
            public native @Name("operator==") boolean equals(@ByRef Iterator it);
            public native @Name("operator*().first") @MemberGetter @ByRef @StdString String first();
            public native @Name("operator*().second") @MemberGetter @ByRef Variant second();
        }

        public static HashMap<String, Object> cast(StringVariantMap map) {
            Iterator mapIter = map.begin();
            Iterator mapEnd = map.end();
            //System.out.println("stringvairant cast1");
            HashMap<String, Object> casted = new HashMap<String, Object>();
            while(true) {
                if (mapIter.equals(mapEnd)) {
                    break;
                }
                //System.out.println("stringvairant cast2");
                casted.put(mapIter.first(), mapIter.second());
                mapIter.increment();
            }
            //System.out.println("stringvairant cast3");
            return casted;
        }

        public static StringVariantMap cast(HashMap<String, Object> map) {
            StringVariantMap casted = new StringVariantMap();
            for (Map.Entry<String, Object> entry : map.entrySet()) {
                Variant var = new Variant();

                if (entry.getValue() instanceof Integer) {
                    casted.put(entry.getKey(), var.put((int) entry.getValue()));
                } else if (entry.getValue() instanceof String) {
                    casted.put(entry.getKey(), var.put(String.valueOf(entry.getValue())));
                } else if (entry.getValue() instanceof Double) {
                    casted.put(entry.getKey(), var.put((double) entry.getValue()));
                } else {
                    throw new IllegalArgumentException("Value must be Integer or String.");
                }
            }
            return casted;
        }

    }

    @Name("std::vector<std::string>")
    public static class FileVector extends Pointer {
        static { Loader.load(); }
        public FileVector()       { allocate();  }
        public FileVector(long n) { allocate(n); }

        private native void allocate();
        private native void allocate(long n);

        @Name("operator[]")
        public native @ByRef String get(long n);
        public native @ByRef String at(long n);

        @ValueSetter @Index(function = "at") public native FileVector put(@Cast("size_t") long i, @StdString String value);

        public native long size();
        public native @Cast("bool") boolean empty();

        public static ArrayList<Path> cast(FileVector vec) {
            ArrayList<Path> casted = new ArrayList<Path>();
            for(int i = 0; i < vec.size(); ++i) {
                casted.add(Paths.get(vec.get(i)));
            }

            return casted;
        }

        public static FileVector cast(ArrayList<Path> input) {
            FileVector casted = new FileVector();

            for(long i = 0; i < input.size(); ++i) {
                casted.put(i, input.get((int) i).toString());
            }

            return casted;
        }
    }

    @NoOffset
    @Name("std::tuple<std::map<std::string, std::variant<int, std::string, double>>, std::vector<std::string>>")
    public static class Tuple extends Pointer {
        static { Loader.load(); }

        public Tuple() { super(); }
        public Tuple(Pointer p) { super(p); }


        public @ByRef StringVariantMap get0() {return get0(this);}

        @Namespace @Name("std::get<0>")
        public static native @ByRef StringVariantMap get0(@ByRef Tuple container);

        public @ByRef FileVector get1() {return get1(this);}

        @Namespace @Name("std::get<1>")
        public static native @ByRef FileVector get1(@ByRef Tuple container);


        @Namespace @Name("std::make_tuple")
        public static native @ByVal Tuple put(@ByRef StringVariantMap i, @ByRef FileVector j);

        public static Pair<HashMap<String, Object>, ArrayList<Path>> cast(Tuple tuple) {

            //System.out.println("in tuple cast");
            Pair<HashMap<String, Object>, ArrayList<Path>> casted = new Pair<HashMap<String, Object>, ArrayList<Path>>();
            //System.out.println("in tuple cast2");
            casted.first = StringVariantMap.cast(tuple.get0());
            //System.out.println("in tuple cast3");
            casted.second = FileVector.cast(tuple.get1());
            //System.out.println("in tuple cast4");

            return casted;
        }

        public static Tuple cast(Pair<HashMap<String, Object>, ArrayList<Path>> input) {

            StringVariantMap casted1 = new StringVariantMap();
            FileVector casted2 = new FileVector();

            /*
            for (Map.Entry<String, Object> entry : input.first) {
                casted1.put(entry.getKey(), entry.getValue().entrySet());
            }
            */

            casted1 = StringVariantMap.cast(input.first);

            for(long i = 0; i < input.second.size(); ++i) {
                casted2.put(i, input.second.get((int)i).toString());
            }

            Tuple tuple = new Tuple();

            return Tuple.put(casted1, casted2);
        }

        //public native long size();
       // public native @Cast("bool") boolean empty();

    }

    @Name("std::vector<std::tuple<std::map<std::string, std::variant<int, std::string, double>>, std::vector<std::string>>>")
    public static class TupleVector extends Pointer {
        static { Loader.load(); }
        public TupleVector()       { allocate();  }
        public TupleVector(long n) { allocate(n); }

        private native void allocate();
        private native void allocate(long n);
        //@Name("operator=")
        //public native @ByRef TupleVector put(@ByRef TupleVector x);

        //public native TupleVector put(@Cast("size_t") long i, Tuple tuple);

        @Name("operator[]")
        public native @ByRef Tuple get(long n);
        public native @ByRef Tuple at(long n);

        public native long size();
        public native @Cast("bool") boolean empty();

        @ValueSetter @Index(function = "at") public native TupleVector put(@Cast("size_t") long i, @ByRef Tuple value);

        public native void resize(@Cast("size_t") long n);

        public static TupleVector cast(ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> input) {

            //System.out.println("in cast1");
            TupleVector casted = new TupleVector();

            Pair<HashMap<String, Object>, ArrayList<Path>> pair = new Pair<HashMap<String, Object>, ArrayList<Path>>();
            Tuple tuple = new Tuple();

            for(long i = 0; i < input.size(); ++i) {
                pair = input.get((int) i);
                tuple.put(StringVariantMap.cast(pair.first), FileVector.cast(pair.second));
                casted.put(i, tuple);

            }

            return casted;
        }

        public static ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> cast(TupleVector input) {

            System.out.println("in cast2");

            ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> casted = new ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>>();

            for(int i = 0; i < input.size(); ++i) {
                System.out.println("in loop");
                casted.add(i, Tuple.cast(input.get(i)));
                System.out.println("in loop2");
            }

            System.out.println("end cast2");

            return casted;
        }

        /*
        public TupleVector cast( vec) {
            TupleVector tupVec = new TupleVector();
            tupVec.resize()
        }
        */
    }

    @Name("std::vector<std::tuple<std::map<std::string, std::variant<int, std::string, double>>, std::vector<std::string>>>")
    public static class FilePatternVector extends Pointer {
        static { Loader.load(); }
        public FilePatternVector()       { allocate();  }
        public FilePatternVector(long n) { allocate(n); }

        private native void allocate();
        private native void allocate(long n);
        @Name("operator=")
        public native @ByRef FilePatternVector put(@ByRef FilePatternVector x);

        @Name("operator[]")
        public native @ByRef Tuple get(long n);
        public native @ByRef Tuple at(long n);

        public native long size();
        public native @Cast("bool") boolean empty();

        public static ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> cast(FilePatternVector vec) {

            ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> casted = new ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>>();
            for(int i = 0; i < vec.size(); ++i) {
                casted.add(Tuple.cast(vec.get(i)));
            }

            return casted;
        }
    }

    @Name("std::vector<std::string>")
    public static class StringVector extends Pointer {
        static { Loader.load(); }
        public StringVector()       { allocate();  }
        public StringVector(long n) { allocate(n); }

        private native void allocate();
        private native void allocate(long n);
        @Name("operator=")
        public native @ByRef StringVector put(@ByRef StringVector x);

        @Name("operator[]")
        public native @ByRef String get(long n);
        public native @ByRef String at(long n);

        public native long size();
        public native @Cast("bool") boolean empty();
    }

    @Name("std::vector<std::tuple<std::string, std::vector<Types>>>")
    public static class TypesTupleVector extends Pointer {
        static { Loader.load(); }
        public TypesTupleVector()       { allocate();  }
        public TypesTupleVector(long n) { allocate(n); }
        private native void allocate();
        private native void allocate(long n);
        @Name("operator=")
        public native @ByRef TypesTupleVector put(@ByRef TypesTupleVector x);

        @Name("operator[]")
        public native @ByRef TypesTuple get(long n);
        public native @ByRef TypesTuple at(long n);

        public native long size();
        public native @Cast("bool") boolean empty();
    }

    @Name("std::tuple<std::string, std::vector<Types>>")
    public static class TypesTuple extends Pointer {
        static { Loader.load(); }
        public TypesTuple(Pointer p) { super(p); }


        public @ByRef @StdString String get0() {return get0(this);}

        @Namespace @Name("std::get<0>")
        public static native @ByRef @StdString String get0(@ByRef TypesTuple container);

        public @ByRef VariantVector get1() {return get1(this);}

        @Namespace @Name("std::get<1>")
        public static native @ByRef VariantVector get1(@ByRef TypesTuple container);

        //public native long size();
       // public native @Cast("bool") boolean empty();
    }

    @Name("std::vector<Types>")
    public static class VariantVector extends Pointer {
        static { Loader.load(); }
        public VariantVector()       { allocate();  }
        public VariantVector(long n) { allocate(n); }

        private native void allocate();
        private native void allocate(long n);

        @Name("operator=")
        public native @ByRef VariantVector put(@ByRef VariantVector x);

        @Name("operator[]")
        public native @ByRef Variant get(long n);
        public native @ByRef Variant at(long n);

        public native long size();
        public native @Cast("bool") boolean empty();
    }

    public static class FilePattern extends Pointer{

        static { Loader.load(); }

        public FilePattern(String path, String filePattern, String blockSize, boolean recursive, boolean suppress_warnings) {
            allocate(path, filePattern, blockSize, recursive, suppress_warnings);
        }
        private native void allocate(String path, String filePattern, String blockSize, boolean recursive, boolean suppress_warnings);

        public native @StdString String getPattern();
        public native @StdString String getPath();

        public native void setPattern(@StdString String pattern);

        public native @ByVal StringVector getVariables();

        public native void setGroup(@ByRef StringVector groups);

        public native @ByVal TupleVector getFiles();

        public native @ByVal FilePatternVector getMatchingByMap(@ByRef StringVariantMap variables);

        //public abstract @ByVal FilePatternVector getMatching(@ByRef TypesTupleVector variables);

        public native void groupBy(@ByRef StringVector groups);

        //public abstract void init(String path, String filePattern, boolean recursive, boolean suppress_warnings);

        //public abstract void printFiles();

        //public native @ByVal FilePatternVector getFiles();

        public native @ByVal StringMapMap getOccurrencesByMap(@ByRef StringVariantMap mapping);

        //public abstract @ByVal StringMapMap getUniqueValues(@ByRef StringVector vec);

        //public native void setMatchingVariables(@ByRef TypesTupleVector matchingVariables);

        //public abstract void groupBy(StringVector groups);

        public native @ByVal @StdString String outputName(@ByRef TupleVector vec);

        public native @ByVal int getSize();

        public native @ByVal TupleVector getSliceByIdx(int index);
    }

    /*
    public static abstract class PatternObject extends Pointer{

        public abstract @StdString String getPattern();
        public abstract @StdString String getPath();

        public abstract void setPattern(@StdString String pattern);

        public abstract @ByVal StringVector getVariables();

        public abstract void setGroup(@ByRef StringVector groups);

        public abstract @ByVal FilePatternVector getMatching(@ByRef TypesTupleVector variables);

        //public abstract @ByVal FilePatternVector getMatching(@ByRef TypesTupleVector variables);

        public abstract void groupBy(@ByRef StringVector groups);

        //public abstract void init(String path, String filePattern, boolean recursive, boolean suppress_warnings);

        //public abstract void printFiles();

        public abstract @ByVal FilePatternVector getFiles();

        public abstract @ByVal StringMapMap getOccurrences(@ByRef TypesTupleVector mapping);

        //public abstract @ByVal StringMapMap getUniqueValues(@ByRef StringVector vec);

        public abstract void setMatchingVariables(@ByRef TypesTupleVector matchingVariables);

        //public abstract void groupBy(StringVector groups);

        public abstract @ByVal @StdString String outputName(@ByRef TupleVector vec);
    }

    public static abstract class Pattern extends PatternObject {

        static { Loader.load(); }
        public Pattern() { allocate(); }
        private native void allocate();

        public native @ByVal @StdString String getPattern();

        public native @ByVal @StdString String getPath();

        public native void setPattern(@StdString String pattern);

        public native @ByVal StringVector getVariables();

        public native void setGroup(@ByRef StringVector groups);

        public native @ByVal FilePatternVector getFiles();

        public native @ByVal StringMapMap getOccurrences(@ByRef TypesTupleVector mapping);

        //public native @ByVal StringMapMap getUniqueValues(@ByRef StringVector vec);

        //public native void setGroup(StringVector groups);

        public native void setMatchingVariables(@ByRef TypesTupleVector matchingVariables);

        public abstract void groupBy(@ByRef StringVector groups);

        //public abstract void groupBy(StringVector groups);

        public abstract @ByVal FilePatternVector getMatching(@ByRef TypesTupleVector variables);

        //public abstract @ByVal FilePatternVector getMatching(@ByRef TypesTupleVector variables);

        public abstract @ByVal @StdString String outputName(@ByRef TupleVector vec);

        //public native @ByVal StringVariantMap getUniqueValues(@ByRef StringVector mapping);

        //public native FilePatternVector getMatching();


    }


    public static abstract class InternalPattern extends Pattern {

        public native void groupBy(@ByRef StringVector groups);

        public native @ByVal FilePatternVector getMatching(@ByRef TypesTupleVector variables);

        public native @ByVal @StdString String outputName(@ByRef TupleVector vec);

    }


    public static abstract class ExternalPattern extends Pattern {
        public FilePatternVector getMatching(TypesTupleVector variables) {
            return this.getMatchingBlock();
        }

        private native @ByVal FilePatternVector getMatchingBlock();

        public native void groupBy(@ByRef StringVector groups);

        public native @ByVal @StdString String outputName(@ByRef TupleVector vec);
    }



    public static class FilePattern extends InternalPattern {
        static { Loader.load(); }
        public FilePattern(String path, String filePattern, boolean recursive, boolean suppress_warnings) {
            allocate(path, filePattern, recursive, suppress_warnings);
        }
        private native void allocate(String path, String filePattern, boolean recursive, boolean suppress_warnings);

        //public native void init(String path, String filePattern, boolean recursive, boolean suppress_warnings);

        //public native void printFiles();

        //@Cast("std::vector<std::tuple<std::map<std::string, std::variant<int, std::string, double>>, std::vector<std::string>>>*")
        // public native FilePatternVector getMatching();
    }


    public static class StringPattern extends InternalPattern {
        static { Loader.load(); }
        public StringPattern(String path, String filePattern, boolean suppress_warnings) {
            allocate(path, filePattern, suppress_warnings);
        }
        private native void allocate(String path, String filePattern, boolean suppress_warnings);

    }

    public static class VectorPattern extends InternalPattern {
        static { Loader.load(); }
        public VectorPattern(String path, String filePattern, boolean suppress_warnings) {
            allocate(path, filePattern, suppress_warnings);
        }
        private native void allocate(String path, String filePattern, boolean suppress_warnings);
    }

    public static class ExternalFilePattern extends ExternalPattern {
        static { Loader.load(); }
        public ExternalFilePattern(String path, String filePattern, String block_size, boolean recursive, boolean suppressWarnings) {
            allocate(path, filePattern, block_size, recursive, suppressWarnings);
        }
        private native void allocate(String path, String filePattern, String block_size, boolean recursive, boolean suppressWarnings);

        //public native void init(String path, String filePattern, boolean recursive, boolean suppress_warnings);

        //public native void printFiles();

        //public native FilePatternVector getFiles();

        //public native FilePatternVector getMatching();
    }

    public static class ExternalStringPattern extends ExternalPattern {
        static { Loader.load(); }
        public ExternalStringPattern(String path, String filePattern, String block_size, boolean suppressWarnings) { allocate(path, filePattern, block_size, suppressWarnings); }
        private native void allocate(String path, String filePattern, String block_size, boolean suppressWarnings);


    }

    public static class ExternalVectorPattern extends ExternalPattern {
        static { Loader.load(); }
        public ExternalVectorPattern(String path, String filePattern, String block_size, boolean suppressWarnings) { allocate(path, filePattern, block_size, suppressWarnings); }
        private native void allocate(String path, String filePattern, String block_size, boolean suppressWarnings);
    }
    */
}
