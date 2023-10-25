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
#ifndef _PATTERNOBJECT
#define _PATTERNOBJECT

class PatternObject {
    public:
        bool external = false;

        std::vector<Tuple> valid_files_; // Store files that match given regex

        std::string path_;
        std::string file_pattern_;

        std::vector<std::pair<std::vector<std::pair<std::string, Types>> , std::vector<Tuple>>> valid_grouped_files_; // 2D vector to store grouped files
        std::vector<std::string> group_; // current groupBy variable

        std::vector<std::string> variables_; // Store the names of variables from the pattern
        std::map<std::string, std::map<Types, int>> variable_occurrences_; // store the number of times a variable value occurs
        std::map<std::string, std::set<Types>> unique_values_; // store each unique value for every variable

        std::vector<std::string> named_groups_;
        std::vector<std::string> tmp_directories_; // store paths to all temporary directories used

        std::vector<Tuple> current_block_; // Store current block of files
        //std::vector<std::pair<std::pair<std::string, Types>, std::vector<Tuple>>> currentGroup; //Store current block of grouped files
        std::vector<std::pair<std::vector<std::pair<std::string, Types>> , std::vector<Tuple>>> current_group_;

        virtual ~PatternObject() {}

        virtual std::vector<Tuple> getMatching(const std::vector<std::tuple<std::string, std::vector<Types>>>& variables) = 0;

        virtual void groupBy(std::vector<std::string>& groups) = 0;

        virtual std::map<std::string, std::map<Types, int>> getOccurrences(const std::vector<std::tuple<std::string, std::vector<Types>>>& mapping) = 0;

        virtual std::map<std::string, std::set<Types>> getUniqueValues(const std::vector<std::string>& mapping) = 0;

        virtual std::string outputName(std::vector<Tuple>& vec) = 0;

        virtual std::vector<std::string> getVariables() = 0;

        //virtual void getNewNaming(std::string& pattern, bool suppressWarnings) = 0;

        virtual void next() = 0;

        virtual void nextGroup() = 0;

        virtual int currentBlockLength() = 0;

        virtual void setGroup(const std::vector<std::string>& groups) = 0;

        virtual std::vector<Tuple> getSlice(std::vector<Types>& key) = 0;

        virtual std::string inferPattern(const std::string& path, std::string& variables, const std::string& block_size) = 0;

        virtual std::string inferPattern(std::vector<std::string>& vec, std::string& variables) = 0;

        virtual std::vector<Tuple> getMatchingBlock() = 0;

        virtual Tuple getItem(int key) = 0;

        virtual std::vector<Tuple> getItemList(std::vector<int>& key) = 0;

        size_t length() const {return valid_files_.size();};

        const std::pair<std::vector<std::pair<std::string, Types>> , std::vector<Tuple>>& get_grouped_file_by_idx(int idx) {

            if (idx < 0 || idx >= this->valid_grouped_files_.size()) {

                throw std::out_of_range("Invalid index " + std::to_string(idx) + " for file vector size of " + std::to_string(valid_grouped_files_.size()));
           
            }

            return valid_grouped_files_[idx];
        }
};
#endif

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
