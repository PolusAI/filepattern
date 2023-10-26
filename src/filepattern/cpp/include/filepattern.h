#pragma once

#include<map>
#include <set>
#include<string>
#include <tuple>
#include<variant>
#include<vector>
#include <memory>

#ifdef FP_CPP_LIB_EXPORT
#include "filepattern_export.h"
#else
#define FILEPATTERN_EXPORT
#endif

#if __has_include(<filesystem>)
  #include <filesystem>
  namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
  #include <experimental/filesystem>
  namespace fs = std::experimental::filesystem;
#else
  error "Missing the <filesystem> header."
#endif

using Types = std::variant<int, std::string, double>;
using Map = std::map<std::string, Types>;
#ifdef JAVA_BINDING
using Tuple = std::tuple<Map, std::vector<std::string>>;
#else
using Tuple = std::tuple<Map, std::vector<fs::path>>;
#endif

class PatternObject; // forward declaration
class FILEPATTERN_EXPORT FilePattern {

    public:

        FilePattern(const std::string& path, const std::string& filePattern="", const std::string& block_size="", bool recursive=false, bool suppressWarnings=false);

        ~FilePattern();

        std::vector<Tuple> getMatchingByMap (Map& variables);

        std::vector<Tuple> getFiles();

        std::vector<Tuple> getMatching(const std::vector<std::tuple<std::string, std::vector<Types>>>& variables);

        std::map<std::string, std::map<Types, int>> getOccurrences(const std::vector<std::tuple<std::string, std::vector<Types>>>& mapping);

        std::map<std::string, std::map<Types, int>> getOccurrencesByMap(Map& mapping);

        std::map<std::string, std::set<Types>> getUniqueValues(const std::vector<std::string>& mapping);

        std::string outputName(std::vector<Tuple>& vec);

        std::vector<std::string> getVariables();

        std::vector<std::pair<std::vector<std::pair<std::string, Types>> , std::vector<Tuple>>> groupBy(std::vector<std::string>& groups);

        void group(std::vector<std::string>& groups);

        void setGroup(std::string& groups);

        void setGroup(const std::vector<std::string>& groups);

        void next();

        void nextGroup();

        int currentBlockLength();

        void getNewNaming(std::string& pattern, bool suppressWarnings);

        std::vector<Tuple> getSlice(std::vector<Types>& key);

        std::vector<Tuple> getSliceByIdx(int key);

        static std::string inferPattern(const std::string& path, std::string& variables, const std::string& block_size="");

        static std::string inferPattern(std::vector<std::string>& vec, std::string& variables);

        std::vector<Tuple> getMatchingBlock();

        Tuple getItem(int key);

        std::vector<Tuple> getItemList(std::vector<int>& key);

        std::vector<Tuple>::iterator begin();

        std::vector<Tuple>::iterator end() ;

        int getSize();

        int getGroupedSize();

        size_t length();

        std::pair<std::vector<std::pair<std::string, Types>> , std::vector<Tuple>> getGroupedSliceByIdx(int idx);

        std::string getPattern();
        void setPattern(std::string& pattern);
        std::string getPath();
        const std::unique_ptr<PatternObject>& getPatternObject() const;
    private:
        std::unique_ptr<PatternObject> fp_;
};
