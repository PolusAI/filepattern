package filepattern.java;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.ArrayList;
import java.util.Iterator;
import java.nio.file.*;
import java.io.*;

import org.bytedeco.javacpp.*;
import org.bytedeco.javacpp.annotation.*;
import org.bytedeco.javacpp.BytePointer;
import org.bytedeco.javacpp.ClassProperties;
import org.bytedeco.javacpp.FunctionPointer;
import org.bytedeco.javacpp.LoadEnabled;
import org.bytedeco.javacpp.Loader;
import org.bytedeco.javacpp.Pointer;
import org.bytedeco.javacpp.PointerPointer;
import org.bytedeco.javacpp.presets.javacpp;
import org.bytedeco.javacpp.tools.BuildEnabled;
import org.bytedeco.javacpp.tools.Info;
import org.bytedeco.javacpp.tools.InfoMap;
import org.bytedeco.javacpp.tools.InfoMapper;
import org.bytedeco.javacpp.tools.Logger;

@Platform(compiler="cpp17",
        include={ "<vector>",
                "<map>",
                "<tuple>",
                "<variant>",
                "<string>",
                "<filesystem>",
                "java.h",
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
                "../cpp/include/filepattern.h",
                "../cpp/interface/filepattern.cpp",
                "../cpp/pattern_object.hpp",
                "../cpp/util/util.hpp"}
        )
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

    @Name("std::map<Types, int>")
    public static class TypesIntMap extends Pointer {

        static { Loader.load(); }

        /** Pointer cast constructor. Invokes {@link Pointer#Pointer(Pointer)}. */
        public TypesIntMap(Pointer p) { super(p); }
        public TypesIntMap()       { allocate();  }

        private native void allocate();

        public native long size();

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

    @Name("std::set<Types>")
    public static class TypesSet extends Pointer {
        static { Loader.load(); }

        public TypesSet(Pointer p) { super(p); }
        public TypesSet()       { allocate();  }

        private native void allocate();

        public boolean empty() { return size() == 0; }
        public native long size();

        public native @ByVal Iterator begin();
        public native @ByVal Iterator end();

        @NoOffset @Name("iterator") public static class Iterator extends Pointer {
            public Iterator(Pointer p) { super(p); }
            public Iterator() { }

            public native @Name("operator ++") @ByRef Iterator increment();
            public native @Name("operator ==") boolean equals(@ByRef Iterator it);
            public native @Name("operator *") @ByVal Variant get();
        }

        public static HashSet<Object> cast(TypesSet set) {

            Iterator setIter = set.begin();
            Iterator setEnd = set.end();

            HashSet<Object> casted = new HashSet<Object>();

            while(setIter.increment() != setEnd) {

                casted.add(Variant.cast(setIter.get()));

            }

            return casted;

        }
    }

    @Name("std::map<std::string, std::set<Types>>")
    public static class StringSetMap extends Pointer {
        static { Loader.load(); }

        public StringSetMap(Pointer p) { super(p); }
        public StringSetMap()       { allocate();  }

        private native void allocate();

        public native long size();

        @Index public native @ByRef TypesSet get(@StdString String i);

        public native @ByVal Iterator begin();
        public native @ByVal Iterator end();

        @NoOffset @Name("iterator") public static class Iterator extends Pointer {

            public Iterator(Pointer p) { super(p); }
            public Iterator() { }

            public native @Name("operator++") @ByRef Iterator increment();
            public native @Name("operator==") boolean equals(@ByRef Iterator it);
            public native @Name("operator*().first") @MemberGetter @ByVal @StdString String first();
            public native @Name("operator*().second") @MemberGetter @ByVal TypesSet second();
        }

        public static HashMap<String, HashSet<Object>> cast(StringSetMap map) {

            Iterator mapIter = map.begin();
            Iterator mapEnd = map.end();

            HashMap<String, HashSet<Object>> casted = new HashMap<String, HashSet<Object>>();

            while(mapIter.increment() != mapEnd) {

                casted.put(mapIter.first(), TypesSet.cast(mapIter.second()));

            }

            return casted;
        }

    }

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

        @Namespace @Name("std::get<int>")
        public static native @ByRef int getInt(@ByRef Variant container);

        @Namespace @Name("std::get<double>")
        public static native @ByRef double getDouble(@ByRef Variant container);

        @Namespace @Name("std::get<std::string>")
        public static native @ByRef String getString(@ByRef Variant container);

        public static Object cast(Variant variant) {
            Object value;
            
            try {

                value = Integer.valueOf(Variant.getInt(variant));

            } catch (Exception e) {

                try {

                    value = Double.valueOf(Variant.getDouble(variant));

                } catch (Exception e2) {

                    try {

                        value = Double.valueOf(Variant.getDouble(variant));

                    } catch (Exception e3) {

                        value = Variant.getString(variant);

                    }
                }
            }

            return value;
        }

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

            HashMap<String, Object> casted = new HashMap<String, Object>();

            while(true) {
                if (mapIter.equals(mapEnd)) {
                    break;
                }

                // Get the value from the Variant
                Object value = Variant.cast(mapIter.second());

                casted.put(mapIter.first(), value);
                mapIter.increment();
            }

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

            Pair<HashMap<String, Object>, ArrayList<Path>> casted = new Pair<HashMap<String, Object>, ArrayList<Path>>();

            casted.first = StringVariantMap.cast(tuple.get0());

            casted.second = FileVector.cast(tuple.get1());

            return casted;
        }

        public static Tuple cast(Pair<HashMap<String, Object>, ArrayList<Path>> input) {

            StringVariantMap casted1 = new StringVariantMap();

            FileVector casted2 = new FileVector();

            casted1 = StringVariantMap.cast(input.first);

            for(long i = 0; i < input.second.size(); ++i) {
                casted2.put(i, input.second.get((int)i).toString());
            }

            Tuple tuple = new Tuple();

            return Tuple.put(casted1, casted2);
        }
    }

    @Name("std::vector<std::tuple<std::map<std::string, std::variant<int, std::string, double>>, std::vector<std::string>>>")
    public static class TupleVector extends Pointer {
        static { Loader.load(); }
        public TupleVector()       { allocate();  }
        public TupleVector(long n) { allocate(n); }

        private native void allocate();
        private native void allocate(long n);

        @Name("operator[]")
        public native @ByRef Tuple get(long n);
        public native @ByRef Tuple at(long n);

        public native long size();
        public native @Cast("bool") boolean empty();

        @ValueSetter @Index(function = "at") public native TupleVector put(@Cast("size_t") long i, @ByRef Tuple value);

        public native void resize(@Cast("size_t") long n);

        public static TupleVector cast(ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> input) {

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

            ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> casted = new ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>>();

            for(int i = 0; i < input.size(); ++i) {
                casted.add(i, Tuple.cast(input.get(i)));

            }

            return casted;
        }
    }
    
    
    @Name("std::pair<std::string, std::variant<int, std::string, double>>")
    public static class StringVariantTuple extends Pointer {

        static { Loader.load(); }

        public StringVariantTuple() { super(); }
        public StringVariantTuple(Pointer p) { super(p); }

        public @ByRef @StdString String get0() {return get0(this);}

        @Namespace @Name("std::get<0>")
        public static native @ByRef @StdString String get0(@ByRef StringVariantTuple container);

        public @ByRef Variant get1() {return get1(this);}

        @Namespace @Name("std::get<1>")
        public static native @ByRef Variant get1(@ByRef StringVariantTuple container);
;

        @Namespace @Name("std::make_pair")
        public static native @ByVal StringVariantTuple put(@ByRef String i, @ByRef Variant j);

        public static Pair<String, Object> cast(StringVariantTuple tuple) {
            Pair<String, Object> pair = new Pair<String, Object>();

            pair.first = StringVariantTuple.get0(tuple);

            pair.second = Variant.cast(StringVariantTuple.get1(tuple));

            return pair;
        }
    }
    
    @Name("std::vector<std::pair<std::string, std::variant<int, std::string, double>>>")
    public static class PairVector extends Pointer {

        static { Loader.load(); }
        public PairVector()       { allocate();  }
        public PairVector(long n) { allocate(n); }

        private native void allocate();
        private native void allocate(long n);

        @Name("operator[]")
        public native @ByRef StringVariantTuple get(long n);
        public native @ByRef StringVariantTuple at(long n);

        public native long size();
        public native @Cast("bool") boolean empty();

        @ValueSetter @Index(function = "at") public native PairVector put(@Cast("size_t") long i, @ByRef StringVariantTuple value);

        public native void resize(@Cast("size_t") long n);
        
        
        public static ArrayList<Pair<String, Object>> cast(PairVector input) {

            ArrayList<Pair<String, Object>> casted = new ArrayList<Pair<String, Object>>();

            for (int i = 0; i < input.size(); ++i) {
                casted.add(StringVariantTuple.cast(input.get(i)));
            }

            return casted;
        }

    }
    
    @Name("std::pair<std::vector<std::pair<std::string, std::variant<int, std::string, double>>> , std::vector<std::tuple<std::map<std::string, std::variant<int, std::string, double>>, std::vector<std::string>>>>")
    public static class PairTupleVector extends Pointer {

        static { Loader.load(); }

        public PairTupleVector()       { allocate();  }

        private native void allocate();

        public @ByRef PairVector get0() {return get0(this);}

        @Namespace @Name("std::get<0>")
        public static native @ByRef PairVector get0(@ByRef PairTupleVector container);

        public @ByRef FilePatternVector get1() {return get1(this);}

        @Namespace @Name("std::get<1>")
        public static native @ByRef FilePatternVector get1(@ByRef PairTupleVector container);

        @Namespace @Name("std::make_pair")
        public static native @ByVal PairTupleVector put(@ByRef PairVector i, @ByRef FilePatternVector j);

        public static Pair<ArrayList<Pair<String, Object>>, ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>>> cast(PairTupleVector input) {

            ArrayList<Pair<String, Object>> first = new ArrayList<Pair<String, Object>>();

            first = PairVector.cast(input.get0());
            
            ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>> second = new ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>>();

            second = FilePatternVector.cast(input.get1());

            Pair<ArrayList<Pair<String, Object>>, ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>>> pair = new Pair<ArrayList<Pair<String, Object>>, ArrayList<Pair<HashMap<String, Object>, ArrayList<Path>>>>();

            pair.first = first;
            pair.second = second;
        
            return pair;
        }
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
        /** Pointer cast constructor. Invokes {@link Pointer#Pointer(Pointer)}. */
        public StringVector(Pointer p) { super(p); }
        public StringVector(BytePointer value) { this(1); put(0, value); }
        public StringVector(BytePointer ... array) { this(array.length); put(array); }
        public StringVector(@StdString String value) { this(1); put(0, value); }
        public StringVector(@StdString String ... array) { this(array.length); put(array); }

        public StringVector()       { allocate();  }
        public StringVector(long n) { allocate(n); }
        private native void allocate();
        private native void allocate(@Cast("size_t") long n);
        public native @Name("operator =") @ByRef StringVector put(@ByRef StringVector x);

        public boolean empty() { return size() == 0; }
        public native long size();
        public void clear() { resize(0); }
        public native void resize(@Cast("size_t") long n);

        @Index(function = "at") public native @StdString BytePointer get(@Cast("size_t") long i);
        public native StringVector put(@Cast("size_t") long i, BytePointer value);
        @ValueSetter @Index(function = "at") public native StringVector put(@Cast("size_t") long i, @StdString String value);

        public native @ByVal Iterator insert(@ByVal Iterator pos, @StdString BytePointer value);
        public native @ByVal Iterator erase(@ByVal Iterator pos);
        public native @ByVal Iterator begin();
        public native @ByVal Iterator end();
        @NoOffset @Name("iterator") public static class Iterator extends Pointer {
            public Iterator(Pointer p) { super(p); }
            public Iterator() { }

            public native @Name("operator ++") @ByRef Iterator increment();
            public native @Name("operator ==") boolean equals(@ByRef Iterator it);
            public native @Name("operator *") @StdString BytePointer get();
        }

        public BytePointer[] get() {
            BytePointer[] array = new BytePointer[size() < Integer.MAX_VALUE ? (int)size() : Integer.MAX_VALUE];
            for (int i = 0; i < array.length; i++) {
                array[i] = get(i);
            }
            return array;
        }
        @Override public String toString() {
            return java.util.Arrays.toString(get());
        }

        public BytePointer pop_back() {
            long size = size();
            BytePointer value = get(size - 1);
            resize(size - 1);
            return value;
        }
        public StringVector push_back(BytePointer value) {
            long size = size();
            resize(size + 1);
            return put(size, value);
        }
        public StringVector put(BytePointer value) {
            if (size() != 1) { resize(1); }
            return put(0, value);
        }
        public StringVector put(BytePointer ... array) {
            if (size() != array.length) { resize(array.length); }
            for (int i = 0; i < array.length; i++) {
                put(i, array[i]);
            }
            return this;
        }

        public StringVector push_back(String value) {
            long size = size();
            resize(size + 1);
            return put(size, value);
        }
        public StringVector put(String value) {
            if (size() != 1) { resize(1); }
            return put(0, value);
        }
        public StringVector put(String ... array) {
            if (size() != array.length) { resize(array.length); }
            for (int i = 0; i < array.length; i++) {
                put(i, array[i]);
            }
            return this;
        }

        public static ArrayList<String> cast(StringVector vec) {

            ArrayList<String> arrayList = new ArrayList<String>();

            for (int i = 0; i < vec.size(); ++i) {
                arrayList.add(vec.get(i).getString());
            }

            return arrayList;
        }
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

        public native void group(@ByRef StringVector groups);

        public native @ByVal StringMapMap getOccurrencesByMap(@ByRef StringVariantMap mapping);

        public native @ByVal StringSetMap getUniqueValues(@ByRef StringVector vec);

        public native @ByVal @StdString String outputName(@ByRef TupleVector vec);

        public native @ByVal int getSize();

        public native @ByVal int getGroupedSize();

        public native @ByVal TupleVector getSliceByIdx(int index);

        public native void getNewNaming(@StdString String pattern, boolean suppressWarnings);

        public native @ByVal PairTupleVector getGroupedSliceByIdx(int index);
    }
}
