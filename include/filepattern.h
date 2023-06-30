#pragma once

#include<map>
#include<string>
#include<variant>
#include<vector>

using Types = std::variant<int, std::string, double>;
using Map = std::map<std::string, Types>;
using Tuple = std::tuple<Map, std::vector<std::string>>;

class FilePattern {

    public:
    
        //std::unique_ptr<PatternObject> fp_;

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

        size_t length();

        std::string getPattern();
        void setPattern(std::string& pattern);
        std::string getPath();

};


