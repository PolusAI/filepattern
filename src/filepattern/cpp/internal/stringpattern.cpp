#include "stringpattern.hpp"
#include <fstream>

StringPattern::StringPattern(const std::string& file_name, const std::string& file_pattern, bool suppress_warnings, bool sorted) {

    if (!fs::exists(file_name)) {
        throw std::invalid_argument("Path \"" + file_name + "\" does not exist.");
    }

    this->setSuppressWarnings(suppress_warnings);
    this->file_name_ = file_name; // store path to target directory
    this->setFilePattern(file_pattern); // cast input string to regex
    this->setRegexFilePattern("");
    this->setIsSorted(sorted);

    this->readFile(); // read file into memory
    this->matchFiles(); // match files to pattern

    if (isSorted()) {
        this->sortFiles();
    }
}

void StringPattern::readFile(){
    std::string str;
    std::ifstream in(this->file_name_);
    if(!in.is_open()) {
        throw std::runtime_error("File \"" + this->file_name_ + "\" not found.");
    }
    // read filenames into memory
    while(std::getline(in, str)){
        if(str.size()) this->files_.push_back(str);
    }

}

void StringPattern::matchFiles(){
    filePatternToRegex(); // get regex equivalent of filepattern

    //string file_path;
    std::regex pattern_regex = std::regex(this->getRegexFilePattern()); // convert to regex
    std::smatch sm; // store matching groups
    for (const auto& file_path : this->files_) {
        // Get the current file
        if(std::regex_match(file_path, sm, pattern_regex)){
            this->valid_files_.push_back(getVariableMap(file_path, sm)); // write to txt file
        }
    }
}
