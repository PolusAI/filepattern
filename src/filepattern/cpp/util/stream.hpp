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

class Stream {

    public:
        int counter; // for debugging (to be removed)
        int map_size; // size of maps in stream

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
        long double currentSize(const int, const long double&);

        bool isEmpty();

        std::string getFilePath();

        std::vector<Tuple> getValidFilesBlock();

        bool endOfValidFiles();

        std::string getValidFilesPath();

        std::string getBlockSizeStr();

    protected:

        std::string tmpdir;
        long double block_size;
        std::string out_name;
        bool empty;
        bool valid_files_empty;
        std::string valid_files;
        std::ifstream infile;
        std::string block_size_str;
        Map temp_map;
        bool is_infer;

        Stream(const std::string&, const bool is_infer=false);
        Stream(){}
};
