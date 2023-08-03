#include "stringpattern.hpp"
#include <fstream>

using namespace std;

StringPattern::StringPattern(const string& file_name, const string& file_pattern, bool suppress_warnings) {

    if (!fs::exists(file_name)) {
        throw std::invalid_argument("Path \"" + file_name + "\" does not exist.");
    }

    this->setSuppressWarnings(suppress_warnings);
    this->file_name_ = file_name; // store path to target directory
    this->setFilePattern(file_pattern); // cast input string to regex
    this->setRegexFilePattern("");
    this->readFile(); // read file into memory
    this->matchFiles(); // match files to pattern
    this->sortFiles();
}

void StringPattern::readFile(){
    string str;
    ifstream in(this->file_name_);
    if(!in.is_open()) {
        throw runtime_error("File \"" + this->file_name_ + "\" not found.");
    }
    // read filenames into memory
    while(getline(in, str)){
        if(str.size()) this->files_.push_back(str);
    }

}

void StringPattern::matchFiles(){
    filePatternToRegex(); // get regex equivalent of filepattern

    //string file_path;
    regex pattern_regex = regex(this->getRegexFilePattern()); // convert to regex
    smatch sm; // store matching groups
    for (const auto& file_path : this->files_) {
        // Get the current file
        if(regex_match(file_path, sm, pattern_regex)){
            this->valid_files_.push_back(getVariableMap(file_path, sm)); // write to txt file
        }
    }
}

