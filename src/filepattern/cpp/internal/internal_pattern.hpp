#pragma once
#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <regex>
#include <map>
#include "../pattern.hpp"
#include "../util/util.hpp"

class InternalPattern : public Pattern {

    private:
        std::vector<Tuple> matching_; // store files from a call to getMatching
        std::vector<std::pair<std::string, std::vector<Types>>> variable_mapping_;

        /**
         * @brief Sets up variables from getMatching to call the main loop of getMatching (getMatchingLoop)
         * 
         * @param variable_map Mapping of variables to values from getMatching
         */
        void getMatchingHelper(const std::tuple<std::string, std::vector<Types>>& variable_map);

        /**
         * @brief Main loop of the getMatching function.
         * 
         * Iterates over a vector of matched files and stores the files that match the variable's value
         * 
         * @param iter Vector of files that match the pattern
         * @param variable Variable to get the matching values of
         * @param values Value of the variable to match
         * @param temp Temporary Tuple to use in the loop
         */
        void getMatchingLoop(std::vector<Tuple>& iter, 
                             const std::string& variable, 
                             const std::vector<Types>& values, 
                             Types& temp);
    
    public:
        void next();

        void nextGroup();

        void sortFiles();

        /**
         * @brief Group matched files by a variable.
         * 
         * Sorts files by provided variable to return matching files in groups where the
         * parameter variable is constant.
         *
         * @param group Variable to group files by
         */
        void groupBy(std::vector<std::string>& groups);

        void groupByHelper(const std::vector<std::string>& groups);

        /**
         * @brief Returns files that match the value of variable. Needs to be updated to match old version input. 
         * 
         * @param variables Variables with value to match (ex. "x=1, y=2")
         * @return std::vector<Tuple> Vector of files where the variable(s) match the value.
         */
        std::vector<Tuple> getMatching(const std::vector<std::tuple<std::string, std::vector<Types>>>& variables);

        /**
         * @brief Returns a single filename from a group of files that captures the variables from a list of files.
         *
         * Given a the list of files [img_r001_c001.tif, img_r001_c002.tif, img_r001_c003.tif], the filename
         * img_r001_c(001-003).tif is returned.
         * 
         * @param vec 
         * @return std::string 
         */
        std::string outputName(std::vector<Tuple>& vec);

        Tuple getItem(int key);

        std::vector<Tuple> getItemList(std::vector<int>& key);

        std::vector<Tuple> getSlice(std::vector<Types>& key);
        
        /**
         * @brief Returns the guess of a pattern given a path to a directory or text file.
         * 
         * @param path Path to directory or text file
         * @param variables Name of variables to use. Optional
         * @return std::string Guess of the pattern
         */
        std::string inferPattern(const std::string& path, std::string& variables, const std::string& block_size);
        
        /**
         * @brief Returns the guess of a pattern given vector of filenames.
         * 
         * @param vec Vector of filenames
         * @param variables Name of variables to use. Optional
         * @return std::string Guess of the pattern
         */
        std::string inferPattern(std::vector<std::string>& vec, std::string& variables);

        int currentBlockLength();

        std::vector<Tuple> getMatchingBlock();

};
