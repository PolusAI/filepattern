#pragma once
#include "internal_pattern.hpp"
#include <string>
#include <vector>

class ArrayPattern : public InternalPattern {
    public:
        ArrayPattern(const std::vector<std::string> file_array, const std::string& pattern, bool suppress_warnings=false);

        void matchFiles(const std::vector<std::string>& file_array);

};

