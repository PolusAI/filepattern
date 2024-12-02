#pragma once
#include <memory>

#include "../internal/arraypattern.hpp"
#include "../internal/filepattern.hpp"
#include "../internal/stringpattern.hpp"
#include "../internal/vectorpattern.hpp"
#include "../external/external_filepattern.hpp"
#include "../external/external_stringpattern.hpp"
#include "../external/external_vectorpattern.hpp"
#include "../util/vector_parser.hpp"

#ifdef WITH_PYTHON_H
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> 
#include <pybind11/numpy.h>
namespace py = pybind11;
#endif
class FilePatternFactory {

    public:

        FilePatternFactory() {}

        std::unique_ptr<PatternObject> getObject(
            const std::string& path, 
            const std::string& file_pattern, 
            const std::string& block_size, 
            bool recursive, 
            bool suppressWarnings,
            const std::vector<std::string>& file_array) {

            // check if array of strings was passed
            if (file_array.size() > 0) {
                return std::make_unique<ArrayPattern>(file_array, file_pattern, suppressWarnings);
            }

            if (block_size == "") {
                if(fs::is_regular_file(path)) {
                    std::ifstream infile(path);
                    std::string str;

                    std::getline(infile, str);

                    if(VectorParser::isStitchingVector(str)) {
                        return std::make_unique<VectorPattern>(path, file_pattern, suppressWarnings); // need to add builder to FPOjbect
                    }

                    return std::make_unique<StringPattern>(path, file_pattern, suppressWarnings); // need to add builder to FPOjbect
                }

                return std::make_unique<FilePatternObject>(path, file_pattern, recursive, suppressWarnings); // need to add builder to FPOjbect
            }

            if(fs::is_regular_file(path)) {
                std::ifstream infile(path);
                    std::string str;

                    std::getline(infile, str);

                if(VectorParser::isStitchingVector(str)) {
                    return std::make_unique<ExternalVectorPattern>(path, file_pattern, block_size, suppressWarnings); // need to add builder to FPOjbect
                }

                return std::make_unique<ExternalStringPattern>(path, file_pattern, block_size, suppressWarnings); // need to add builder to FPOjbect
            }

            return std::make_unique<ExternalFilePattern>(path, file_pattern, block_size, recursive, suppressWarnings); // need to add builder to FPOjbect
        }
};
