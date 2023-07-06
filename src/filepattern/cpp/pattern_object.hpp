#pragma once

#include<map>
#include <set>
#include<string>
#include <tuple>
#include<variant>
#include<vector>

#include "util/util.hpp"

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
       
        virtual void getNewNaming(std::string& pattern, bool suppressWarnings) = 0;

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

};

 