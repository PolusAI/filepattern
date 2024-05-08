/**
 * @file fs_stream.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2021-12-23
 *
 * @copyright Copyright (c) 2021
 *
 */
#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstddef>
#include <map>
#include "block.hpp"
#include "util.hpp"

#if __has_include(<filesystem>)
  #include <filesystem>
  namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
  #include <experimental/filesystem>
  namespace fs = std::experimental::filesystem;
#else
  error "Missing the <filesystem> header."
#endif

class FilesystemStream {

    public:
        int counter_; // for debugging (to be removed)
        unsigned int map_size_; // size of maps in stream

        /**
         * @brief Construct a new Filesystem Stream object.
         *
         * Creates a data stream of filenames from a filesystem directory iterator.
         *
         * @param path Path to directory to stream filenames from
         * @param recursive True will create a recursive directory iterator that will iterate through subdirectories
         * @param block_size Maximum size of memory the stream will consume at a time
         */
        FilesystemStream(const std::string& path, bool recursive=false, const std::string& block_size="50 MB", const bool is_infer=false);

        FilesystemStream(){}

        ~FilesystemStream();

        /**
         * @brief Get a block of filenames that consumes at most block_size of main memory.
         *
         * @return std::vector<std::string> Vector of filenames
         */
        std::vector<std::string> getBlock();

        /**
         * @brief Get a block of filenames that consumes at most block_size of main memory.
         *
         * @return std::vector<std::string> Vector of filenames
         */
        std::vector<std::string> getBlockIterator();

        /**
         * @brief Get a block of filenames that consumes at most block_size of main memory.
         *
         * @return std::vector<std::string> Vector of filenames
         */
        std::vector<std::string> getBlockTxt();

        /**
         * @brief Write a block of strings to the output file.
         *
         * @param vec Vector to write to output file.
         */
        void writeBlock(const std::vector<std::string>& vec);

        /**
         * @brief Write valid files maps to output file.
         *
         * @param mapping Mapping to write to the output file.
         */
        void writeValidFiles(const Tuple& mapping);

        /**
         * @brief Current size of
         *
         * @param double
         * @return long double
         */
        long double currentSize(const int&, const long double&);

        /**
         * @brief True if no more files, otherwise false
         *
         * @return true No more files remaining
         * @return false More files remain
         */
        bool isEmpty();

        /**
         * @brief Returns path to files
         *
         * @return std::string Path to files
         */
        std::string getFilePath();

        /**
         * @brief Get a block of matched files that uses at most block_size of memory.
         *
         * @return std::vector<Tuple> Block of matched files
         */
        std::vector<Tuple> getValidFilesBlock();

        /**
         * @brief End of vaildFiles.txt has been reached
         *
         * @return true No more matched files remain
         * @return false There are still matched files
         */
        bool endOfValidFiles();

        /**
         * @brief Returns path of validFiles.txt, which stores the matched files
         *
         * @return std::string Path to validFiles.txt
         */
        std::string getValidFilesPath();

        /**
         * @brief Returns the string version of the block_size
         *
         * @return std::string String containing the max amount of memory used
         */
        std::string getBlockSizeStr();

        /**
         * @brief Returns path of the temporary directory used by the FilesystemStream object
         *
         * @return std::string Path to the temporary directory
         */
        std::string getTmpPath();

        Tuple getFileByIndex(int i);

        unsigned int getValidFilesSize();

        std::vector<Tuple> getValidFilesSlice(int i, int j, int step);

    private:
        fs::directory_iterator directory_iterator_; // directory iterator to stream from
        fs::recursive_directory_iterator recursive_directory_iterator_;
        fs::directory_iterator end_; // End of iterator
        fs::recursive_directory_iterator rec_end_; // End of recursive iterator

        bool recursive_; // Iterate over subdirectories
        bool txt_input_; // The path is to a text file

        std::ifstream infile_;
        std::ifstream inputfile_;

        std::string tmpdir_; // path to temporary directory
        std::string out_name_;
        std::string valid_files_; // store matched files block
        std::string block_size_str_; // string version of max amount of memory
        Map temp_map_;
        bool is_infer_; // Called from inferPattern method. Calculates memory used from matrix in inferPattern
        bool empty_; // no more files remaining
        bool valid_files_empty_; // no more matched files
        long double block_size_; // max amount of memory to use
        unsigned int valid_files_size_;

        /**
         * @brief Updates the amount of memory being used
         *
         * @param size Current amount of memory
         * @param current String to add memory use of
         */
        void updateSize(long& size, const std::string& current);
};
