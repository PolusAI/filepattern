#pragma once
#include "external_pattern.hpp"
#include "../util/vector_parser.hpp"
#include "../util/block.hpp"
#include "../util/util.hpp"


class ExternalVectorPattern: public ExternalPattern {

    private:
        std::ifstream infile_; // stream for validFiles.txt
        std::string path_; // path to stitching vector
        std::ifstream vector_reader_; //// stream to read stitching vector

    public:

        const static std::regex STITCH_REGEX_; // regex to match stitching vector line
        const static std::vector<std::regex> STITCH_REGEX_VECTOR_;
        const static std::vector<std::string> STITCH_VARIABLES_; // variable names for stitching vector

        /**
         * @brief Constructor of ExternalVectorPattern
         *
         * @param path Path to stitching vector
         * @param pattern Pattern to match filenames to
         * @param block_size Maximum amount of memory to use
         */
        ExternalVectorPattern(const std::string& path, const std::string& pattern, const std::string& block_size, bool suppress_warnings=false, bool sorted=true);

        /**
         * @brief Deconstructor of ExternalVectorPattern. Removes temporary directories and files.
         *
         */
        ~ExternalVectorPattern();

        /**
         * @brief Match files from the stitching vector to the pattern
         *
         */
        void matchFiles();

        /**
         * @brief Guesses the pattern that the filename follows in a stitching vector.
         *
         * @param path Path to stitching vector
         * @param variables Variable names. Optional
         * @param block_size Maximum amount of memory to use
         * @return std::string Guess of the pattern
         */
        std::string inferPattern(const std::string& path, std::string& variables, const std::string& block_size);
};
