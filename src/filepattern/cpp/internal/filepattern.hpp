/**
 * @file FilePatternObject.hpp
 * @author Jesse McKinzie (Jesse.McKinzie@axleinfo.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-22
 * 
 * 
 */

#pragma once 
#include "internal_pattern.hpp"

#if __has_include(<filesystem>)
  #include <filesystem>
  namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
  #include <experimental/filesystem> 
  namespace fs = std::experimental::filesystem;
#else
  error "Missing the <filesystem> header."
#endif

class FilePatternObject : public InternalPattern {
    public:
        FilePatternObject(const std::string& path, const std::string& pattern, bool recursive=false, bool suppress_warnings=false);

        /**
         * @brief Match files in directory to the pattern
         * 
         * Filesystem directory iterator is used to iterate over the supplied directory,
         * comparing each filename to the regex version of the pattern. If recursive if true,
         * a recursive directory iterator is used. If a file matches the pattern, it is stored 
         * in the valid_files vector.
         */
        void matchFiles();

        /**
         * @brief Print the valid files to the console.
         * 
         */
        void printFiles();
        
    private: 
        //std::string path; // path to directory
        fs::directory_iterator iterator_; // File iterator for given path
        fs::recursive_directory_iterator recursive_iterator_; // Recursive directory iterator
        bool recursive_; // Iterate over subdirectories

        /**
         * @brief Match files to pattern using a directory iterator.
         * 
         * Called from matchFiles() method. Matches files to the pattern from a directory iterator
         * if recursive is false.
         */
        void matchFilesOneDir();

        /**
         * @brief Match files to pattern using a recursive directory iterator. 
         * If a file has the same name and is in a different subdirectory, the filepath will be 
         * added to the existing tuple in the array stored in the second member of the tuple.
         *
         * Called from matchFiles() method. Matches files to the pattern from a directory iterator
         * if recursive is false.
         */
        void matchFilesMultDir();

};