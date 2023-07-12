#pragma once

#include <iostream>
#include <fstream>

#include "../util/util.hpp"

//#include "iterator_tpl.h"

#include "../internal/filepattern.hpp"
#include "../internal/stringpattern.hpp"
#include "../internal/vectorpattern.hpp"
#include "../external/external_filepattern.hpp"
#include "../external/external_stringpattern.hpp"
#include "../external/external_vectorpattern.hpp"
#include "../util/vector_parser.hpp"



class FilePatternFactory {

    public:

        FilePatternFactory() {}

        PatternObject* getObject(const std::string& path, const std::string& file_pattern, const std::string& block_size, bool recursive, bool suppressWarnings) {
            if (block_size == "") {
                if(fs::is_regular_file(path)) {
                    std::ifstream infile(path);
                    std::string str;
    
                    std::getline(infile, str);
    
                    if(VectorParser::isStitchingVector(str)) {
                        return new VectorPattern(path, file_pattern, suppressWarnings); // need to add builder to FPOjbect
                    }
                    
                    return new StringPattern(path, file_pattern, suppressWarnings); // need to add builder to FPOjbect
                }

                return new FilePatternObject(path, file_pattern, recursive, suppressWarnings); // need to add builder to FPOjbect
            }
    
            if(fs::is_regular_file(path)) {
                std::ifstream infile(path);
                    std::string str;
    
                    std::getline(infile, str);
    
                if(VectorParser::isStitchingVector(str)) {
                    return new ExternalVectorPattern(path, file_pattern, block_size, suppressWarnings); // need to add builder to FPOjbect
                }
                
                return new ExternalStringPattern(path, file_pattern, block_size, suppressWarnings); // need to add builder to FPOjbect
            }

            return new ExternalFilePattern(path, file_pattern, block_size, recursive, suppressWarnings); // need to add builder to FPOjbect
        }
};