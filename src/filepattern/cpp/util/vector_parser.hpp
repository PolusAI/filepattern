#pragma once

#include <unordered_map>

#include "util.hpp"

class VectorParser {
    public:
        /**
         * @brief Get the filename from a line of a stitching vector
         * 
         * @param line Line from a stitching vector file
         * @return std::string filename from the line
         */
        static std::string getFileName(const std::string& line);

        /**
         * @brief Parse the variables from a line of a stitching vector
         * 
         * @param tup Tuple to add variables to
         * @param stitching_vector_line Line from a stitching vector
         * @param STITCH_VARIABLES Variables in the stitching vector
         * @param STITCH_REGEX regex to match line to
         * @param variables current variables in file map
         */
        static void parseVectorLine(Tuple& tup,
                                    const std::string& stitching_vector_line, 
                                    const std::vector<std::string>& STITCH_VARIABLES, 
                                    const std::vector<std::regex>& STITCH_REGEX,
                                    std::vector<std::string> variables);

        static bool isStitchingVector(const std::string& line);

};