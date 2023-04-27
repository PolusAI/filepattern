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

//namespace fs = std::filesystem;

class ExternalFilePattern : public ExternalPattern {
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

        ExternalFilePattern(const std::string& path, const std::string& file_pattern, const std::string& block_size="50 MB", bool recursive=false, bool suppress_warnings=false);

        ExternalFilePattern(){}

        /**
         * @brief Destroy the External File Pattern object: Deletes all files in temp directory
         * 
         */
        ~ExternalFilePattern();

        /**
         * @brief Match files to pattern.
         * 
         * Iterates over directory, matching filenames to the user provided pattern.
         * If the file matches the pattern, the variables and filepath are written to 
         * a temporary .txt file. 
         */
        void matchFiles();

        /**
         * @brief Prints matched files to the console
         * 
         * Prints the maps for each matched file to the console.
         */
        void printFiles();

        /**
         * @brief Get all files that match a specified value(s) of variable(s).
         * 
         * @return std::vector<Tuple> Matched files.  
         */
        //std::vector<Tuple> getMatching(std::string&);

    private: 
        std::string path_; // path to directory
        std::filesystem::directory_iterator iterator_; // File iterator for given path
        std::filesystem::recursive_directory_iterator recursive_iterator_; // Recursive iterator
        bool end_of_file_; // True if end of temp file is reached
        bool recursive_; // True if recursive iterator through subdirectories
        int total_files_; // Total number of matched files (will be deleted)

        /**
         * @brief Matches files from a directory iterator. 
         * 
         * Iterates over directory using a filesystem directory iterator and finds 
         * all files in the directory that match the filepattern. matchFilesLoop is
         * called to generate a mapping between the variables and values. The mapping is
         * stored in the first member of the resulting tuple. The filepath is stored in the second 
         * member of the tuple.
         */
        void matchFilesOneDir();

        /**
         * @brief Iterates over a directory and its subdirectories, matching files 
         * to the pattern. If two files in different subdirectories have the same name, 
         * the filepath to both files is stored in the array in the second member of the tuple.
         * 
         */
        void matchFilesMultDir();

};
