/**
 * @file sort.hpp
 * @author Jesse McKinzie (Jesse.McKinzie@axleinfo.com)
 * @brief External memory merge sort
 * @version 0.1
 * @date 2021-12-23
 * 
 * 
 */

#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <chrono>
#include <filesystem>
#include <map>
//#include "file_stream.hpp"
#include "block.hpp"
#include "util.hpp"

namespace fs = std::filesystem;

enum Structure {std_map, std_vector};

typedef std::vector<std::string> vector_string;
class ExternalMergeSort {

    public:

        /**
         * @brief Construct a new External Merge Sort object to sort a .txt file 
         * using at most block_size of main memory.
         * 
         * Sorts a .txt file by line if Structure=std_vector and by supplied variable
         * if Structure=std_map is used. The file is first sorted into n chunks where 
         * n=total_file_size/block_size. The n sorted blocks are then merged using an 
         * iterative 2-way merge. Once the n files are merged into one file, the file
         * copied to a file with the supplied output name.
         *
         * @param structure Data structure to sort (std_vector or std_map)
         * @param input_file_name Path to input file 
         * @param output_file_name Path to output file (will be created if doesn't exist)
         * @param block_size Maximum amount of memory, format is "<size> <unit>" where 
         * unit is either B, KB, MB, or GB. Format 
         * @param variable Variable to sort by if std_map is passed
         * @param map_size Size of map if std_map is used
         */
        ExternalMergeSort(const Structure& structure, 
                          const std::string& input_file_name, 
                          const std::string& output_file_name, 
                          const std::string& block_size, 
                          const std::string& variable,
                          const int map_size);

    private:
        vector_string vec_; // Vector to store blocks of files
        std::string input_file_name_; // Store the input path 
        std::string output_file_name_; // Store the output path
        vector_string files_to_merge_; // Vector to keep track of files to merge
        vector_string new_files_; // Vector to store files that were merged
        long double block_size_; // Max size of memory to use at once
        std::string tmpdir_; // Create a directory to store temporary files
        std::string sort_variable_; // Variable to sort by if std_map
        std::string block_size_str_; // String input of blocksize
        int map_size_; // Number of variables in the map
        Map temp_map_; 

        /**
         * @brief Calculate the current size of a vector that contains strings
         * 
         * @param string_size Size of the string to be added to the vector 
         * @param previous_size Size of vector before adding another string
         * @return double The number of bytes the vector with the new string added takes up
         */
        long currentSize(const int string_size, const long& previous_size);

        /**
         * @brief Write a string to an output file in the temporary directory.
         * 
         * @param output String to write to output 
         */
        void writeTmpFile(std::string& output);
        
        /**
         * @brief Sort a file of strings.
         * 
         */
        void sortFile();

        /**
         * @brief Sort a file of maps.
         * 
         */
        void sortMapFile();

        /**
         * @brief Two-way iterative merge of maps containing maps
         * 
         */
        void mergeMaps();

        /**
         * @brief Write a string to an output file.
         * 
         */
        void writeFile(const std::string&);

        /**
         * @brief Merge two files that contain strings.
         * 
         */
        void merge();

        /**
         * @brief Write a vector of maps to an output file in the temporary directory.
         * 
         * @param file ofstream to write map to
         * @param vec Vector of maps to write output to
         */
        void writeMapTmpFile(std::ofstream& file, 
                             const std::vector<Tuple>& vec);

        /**
         * @brief Get a block of maps from an input file that takes up less than or equal 
         block_size bytes of memory. Returns true if not end of file and false otherwise.
         * 
         * @return true If not end of file.
         * @return false If end of file.
         */
        bool getMapBlock(std::ifstream&, std::vector<Tuple>&);
        
        /**
         * @brief Merge two files of maps together.
         * 
         * @param file_name1 First file to merge
         * @param file_name2 Second file to merge
         * @param out_file Output file with merged files
         */
        void twoWayMergeMaps(const std::string& file_name1, const std::string& file_name2, const std::string& out_file);

};
