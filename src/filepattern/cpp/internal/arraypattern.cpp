#include "arraypattern.hpp"

ArrayPattern::ArrayPattern(const std::vector<std::string> file_array, const std::string& pattern, bool suppress_warnings=false){
    this->setSuppressWarnings(suppress_warnings);

    this->setRegexFilePattern(""); // Regex version of pattern

    this->matchFiles(file_array);

    this->sortFiles();
};

void ArrayPattern::matchFiles(const std::vector<std::string>& file_array) {
    Map mapping;
    std::vector<std::string> parsed_regex;

    std::string s;
    std::string file, file_path;
    Tuple member;
    // Iterate over every file in directory

    // check if bracket expression was not properly parsed
    if (this->getRegexFilePattern().find('{') != std::string::npos || this->getRegexFilePattern().find('}') != std::string::npos) {
        auto start = this->getRegexFilePattern().find('{');
        auto end = this->getRegexFilePattern().find('}');
        auto length = end - start;
        throw std::invalid_argument("Invalid pattern found in bracket expressions in filepattern: \"" + this->getRegexFilePattern().substr(start, length+1) + "\"");
    }

    std::regex pattern_regex = std::regex(this->getRegexFilePattern());
    std::smatch sm;

    // iterate over file array and add valid files
    for (const auto& file_path : file_array) {
        // Get the current file
        replace(file_path.begin(), file_path.end(), '\\', '/');
        file = s::getBaseName(file_path);
        if(regex_match(file, sm, pattern_regex)){
            this->valid_files_.push_back(getVariableMap(file_path, sm)); // write to txt file
        }
    }
};