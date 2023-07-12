/**
 * @file Pattern.hpp
 * @author Jesse McKinzie (Jesse.McKinzie@axleinfo.com)
 * @brief Parent class of FilePattern and ExternalFilePattern
 * @version 0.1
 * @date 2021-12-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include <map>
#include <set>
#include "util/fs_stream.hpp"
#include "util/util.hpp"
#include "pattern_object.hpp"

class Pattern : public PatternObject {
    private:
    
        std::regex regex_expression_; // Regex expression
        //std::string file_pattern_; // Pattern to match files to
        std::string path_pattern_;
        std::string regex_file_pattern_; // Pattern with capture groups
        //std::string path_;
        bool just_path_;
        bool suppress_warnings_;

        std::string VARIABLES_; 

    protected:
        void setRegexExpression(std::regex);
        void setFilePattern(std::string);
        void setPathPattern(std::string);
        void setRegexFilePattern(std::string);
        void setPath(std::string);
        void setJustPath(bool);
        void setSuppressWarnings(bool);

        std::regex getRegexExpression();
        std::string getFilePattern();
        std::string getPathPattern();
        std::string getRegexFilePattern();
        std::string getPath();
        bool getJustPath();
        bool getSuppressWarnings();

        
        /**
         * @brief Returns a guess of the filepattern using internal memory.
         * 
         * Helper function for child classes. Contains the main loop of inferPattern. Called
         * after parsing directory or text file input into main memory. Used to process blocks of filenames
         * when called from external memory classes.
         * 
         * @param files List of filenames to guess pattern of
         * @param variables Name of variables to use. Optional
         * @param startingPattern Initial pattern. Optional. Used for external memory version
         * @return std::string Guess of the pattern
         */
        std::string inferPatternInternal(std::vector<std::string>& files, std::string& variables, const std::string& startingPattern="");
        
        void getPathFromPattern(const std::string& path);

    public:
        

        /**
         * @brief Convert to pattern to regex and update class variables from the returned 
         * tuple of getRegex.
         * 
         * Creates a version of the pattern with regex to match files to. For example, 
         * if the pattern contains {variable:d}, this is changed to [0-9] in the regex pattern.
         */
        void filePatternToRegex();

        /**
         * @brief Get the regex version of the filepattern. 
         * 
         * Converts filePattern to new naming style if the old version is used.
         * 
         * @param pattern Pattern to get regex version of
         * @return std::tuple<std::string, std::vector<std::string>, std::vector<std::string>> Tuple containing the
         * the regex version of the file pattern, the variables found, and the named groups.
         */
        std::tuple<std::string, std::vector<std::string>, std::vector<std::string>> getRegex(std::string& pattern, bool suppressWarning=false);

        /**
         * @brief Get the mapping of variables to values for a matching file. Used with a recursive directory iterator. 
         * 
         * Uses the regex version of the pattern from filePatternToRegex() to extract capture groups from
         * a basename. Returns a tuple of variable matched to capture group and the file path if no file with 
         * the same basename has already been matched and an empty tuple otherwise. In the later case, the 
         * basename is appending to the second member of the existing tuple.
         * 
         * @param filePattern filePath that matches the pattern
         * @param sm captured groups
         * @return Tuple A tuple with the mapping in first and the file path in second. An empty tuple is returned
         * if the basename of filePath has already been matched
         */
        Tuple getVariableMapMultDir(const std::string& filePath, const std::smatch& sm);


         /**
         * @brief Get the mapping of variables to values for a matching file. Used with a directory iterator.
         * 
         * Uses the regex version of the pattern from filePatternToRegex() to extract capture groups from
         * a basename. Returns a tuple of variable matched to capture group.
         * 
         * @param filePattern filePath that matches the pattern
         * @param sm captured groups
         * @return Tuple A tuple with the mapping in first and the file path in second 
         */
        Tuple getVariableMap(const std::string& filePath, const std::smatch& sm);

        /**
         * @brief Get the pattern to match files to.
         * 
         * @return std::string The pattern that files are matched to
         */
        std::string getPattern();

        /**
         * @brief Set the pattern.
         * 
         * @param pattern New pattern
         */
        void setPattern(const std::string& pattern);

        /**
         * @brief Get the pattern with regex capture groups
         * 
         * @return std::string The pattern with regex capture groups
         */
        std::string getRegexPattern();
        
        /**
         * @brief Get the variable names
         * 
         * @return std::vector<std::string> Vector of variable names
         */
        std::vector<std::string> getVariables();

        /**
         * @brief Prints the variables to the console.
         * 
         */
        void printVariables();

        /**
         * @brief Sets the variable to be grouped in groupBy()
         * 
         * @param group Variable to group the matched files by 
         */
        void setGroup(const std::vector<std::string>& groups);

        /**
         * @brief Returns the number of times each unique value for each variable occurs.
         * 
         * @param mapping Variable(s) mapped to value(s) to get the number of occurrences
         * @return std::map<std::string, std::map<Types, int>> Number of occurrences for each variable
         */
        std::map<std::string, std::map<Types, int>> getOccurrences(const std::vector<std::tuple<std::string, std::vector<Types>>>& mapping);

        /**
         * @brief Returns the unique values for each variables.
         * 
         * @param mapping Vector of variables to get the unique values of. If the vector is empty, all variables will be returned.
         * @return std::map<std::string, std::set<Types>>  Unique values of the variables.
         */
        std::map<std::string, std::set<Types>> getUniqueValues(const std::vector<std::string>& mapping);

        /**
         * @brief Get the new naming style for an old pattern from version 1 of filepattern.
         * 
         * @param pattern Pattern to get the new naming style of.
         */
        void getNewNaming(std::string& pattern, bool suppressWarnings);

        /**
         * @brief Main loop of outputName. Finds the output name for a vector of files.
         * 
         * @param vec Vector of files.
         * @return std::string A name which captures the variable values of all the files in vec.
         */
        std::string outputNameHelper(std::vector<Tuple>& vec);

        /**
         * @brief Creates the output name for the outputName method.
         * 
         * Creates the output name using the min and max values found by outputNameHelper.
         * 
         * @param min Minimum value of the variable
         * @param max Maximum value of the variable
         * @param var Name of the variable
         * @param outputName Current output name
         * @param idx Index of the namedGroup
         * @param temp temporary string to use
         * @param patternRegex pattern to match the filename to
         */
        void replaceOutputName(Tuple& min, 
                               Tuple& max, 
                               const std::string& var, 
                               std::string& outputName, 
                               const int idx, 
                               std::string& temp, 
                               const std::regex& patternRegex);

        /**
         * @brief Smith-Waterson search used in inferPattern.
         * 
         * This function uses a modified
         * `Smith-Waterman <https://en.wikipedia.org/wiki/Smith%E2%80%93Waterman_algorithm>`_
         * to update the input pattern so that it will operate on the filename. This
         * algorithm assumes that only numeric and alphabetic values might change and all text in the
         * filename should be identical.
         * 
         * This function should not be called directly, but from inferPatternHelper.
         * 
         * @param pattern 
         * @param filename 
         * @param variables 
         * @return std::string 
         */
        std::string swSearch(std::string& pattern, std::string& filename, const std::string& variables);

        /**
         * @brief Returns paths to all the temporary directories used in external memory classes.
         * 
         * @return std::vector<std::string> Vector of paths to temporary directories used.
         */
        std::vector<std::string> getTmpDirs();

};
