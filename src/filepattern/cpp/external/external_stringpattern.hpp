/**
 * @file ExternalFilePattern.hpp
 * @author Jesse McKinzie (Jesse.McKinzie@axleinfo.com)
 * @brief External memory version of filepattern. Matches files in a directory to a pattern using
 * a user specified amount of memory.
 * @version 0.1
 * @date 2021-12-21
 *
 *
 *
 */

#pragma once
#include "external_pattern.hpp"


class ExternalStringPattern : public ExternalPattern {
    public:

        /**
         * @brief Construct a new External File Pattern object
         *
         * Valid patterns are d, c, and +, where d is a digit, c is
         * a character, and + means an arbitrary number of the pattern it
         * acts on.

         * Example: The pattern files_x{row:ddd}_y{col:ddd}_{channel: c+}.ome.tif
         * would match files with 3 digits after x, 3 digits after y, and then an
         * arbitrary number of characters.
         *
         * @param path Path to directory
         * @param file_pattern Pattern to compare filenames to
         * @param block_size Amount of memory to process at a time
         * @param recursive Iterate over all subdirectories if true
         */

        ExternalStringPattern(const std::string& path, const std::string& file_pattern, const std::string& block_size="50 MB", bool suppress_warnings=false);

        ~ExternalStringPattern();

        /**
         * @brief Match files to pattern.
         *
         * Iterates over directory, matching filenames to the user provided pattern.
         * If the file matches the pattern, the variables and filepath are written to
         * a temporary .txt file.
         */
        void matchFiles();

        /**
         * @brief Get all files that match a specified value(s) of variable(s).
         *
         * @return std::vector<Tuple> Matched files.
         */
        //std::vector<Tuple> getMatching(std::string&);

    private:
        std::string path_; // path to directory
        fs::directory_iterator iterator_; // File iterator for given path
        fs::recursive_directory_iterator recursive_iterator_; // Recursive iterator
        bool end_of_file_; // True if end of temp file is reached
        int total_files_; // Total number of matched files (will be deleted)


        /**
         * @brief Get a map from a .txt file
         *
         * Gets a map from a txt file that contains variables mapped to values.
         *
         * @param infile Input stream
         * @param mapping Map to be modified
         * @return true The end of the file has not been reached and the map is modified
         * @return false The end of the file has been reached and the mao is not modified
         */
        //bool getMap(std::fstream& infile, Tuple& mapping);

        /**
         * @brief Finds values of variables given a file that matches the pattern.
         *
         * Finds the value of variables given a matching filepath and returns a map of the
         * variables to the respective value. The type of the variable is preserved.
         *
         * @return A map of variables to values from the basename of the filepath.
         */
        //Map matchFilesLoop(Map&, const std::string&, const std::regex&, std::vector<std::string>&);


};
