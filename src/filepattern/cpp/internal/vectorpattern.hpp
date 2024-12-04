#pragma once
#include "internal_pattern.hpp"
#include "../pattern.hpp"
#include "../util/vector_parser.hpp"
#include "../util/util.hpp"


class VectorPattern: public InternalPattern {

    private:
        std::string path_; // path to stitching vector
        std::ifstream infile_; // stream to read in stitching vector

    public:

        const static std::regex STITCH_REGEX_; // regex to match stitching vector line
        const static std::vector<std::regex> STITCH_REGEX_VECTOR_;
        const static std::vector<std::string> STITCH_VARIABLES_; // variable names for stitching vector

        /**
         * @brief Constructor of VectorPattern.
         *
         * @param path Path to stitching vector
         * @param pattern Pattern to match filename to
         */
        VectorPattern(const std::string& path, const std::string& pattern, bool suppress_warnings = false, bool sorted=true);

        /**
         * @brief Match filenames to the pattern.
         *
         * Compares the filename from each line of the stitching vector to the provided pattern.
         * Each variable from the line of the stitching vector is extracted and added to the variable value
         * map. Each variable from the named groups in the pattern are also added to the mapping. The matching
         * files are stored in memory in the validFiles vector.
         */
        void matchFiles();

        /**
         * @brief Makes a guess of the pattern given a stitching vector.
         *
         * Reads in the filenames from the stitching vector and makes a guess of the pattern.
         *
         * @param path Path to the stitching vector.
         * @param variables Name of variables. Optional
         * @return std::string Guess of the pattern
         */
        std::string inferPattern(const std::string& path, std::string& variables, const std::string& block_size);
};
