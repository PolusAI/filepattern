#pragma once
#include "internal_pattern.hpp"

class StringPattern : public InternalPattern {
    public:
        
        /**
         * @brief Constructor of StringPattern. 
         * 
         * @param file_name Path to text file
         * @param file_pattern Pattern to match line of text file to
         * @param suppress_warnings True will disable the warnings about using the old style of pattern. Default is false.
         */
        StringPattern(const std::string& file_name, const std::string& file_pattern, bool suppress_warnings=false);

        /**
         * @brief Match lines of text file to the pattern.
         * 
         */
        void matchFiles();

        /**
         * @brief Read file into main memory.
         * 
         */
        void readFile();

    private:
        std::vector<std::string> files_; // Vector of filenames
        std::string file_name_; // path to txt file
};