#include "filepattern.hpp"
#include <chrono>

using namespace std;

FilePatternObject::FilePatternObject(const string& path, const string& file_pattern, bool recursive, bool suppress_warnings) {

    this->setSuppressWarnings(suppress_warnings);

    if(file_pattern == ""){

        this->getPathFromPattern(path); // set path and file_pattern
        
        try {
            this->recursive_iterator_ = fs::recursive_directory_iterator(this->getPath());
            this->recursive_ = true;
            this->setJustPath(true);
        } catch (const std::runtime_error& e) {
            string error = "No directory found. Invalid path \"" + path + "\"."; 
            throw std::runtime_error(error);
        }

    } else {

        this->setJustPath(false);
        this->setPath(path); // store path to target directory
        this->setFilePattern(file_pattern); // cast input string to regex
        this->recursive_ = recursive; // Iterate over subdirectories

        try {
            if(recursive){
                this->recursive_iterator_ = fs::recursive_directory_iterator(this->getPath());
            } else{ 
                this->iterator_ = fs::directory_iterator(this->getPath()); // store iterator for target directory
            }
        } catch (const std::runtime_error& e) {
            string error = "No directory found. Invalid path \"" + path + "\"."; 
            throw std::runtime_error(error);
        }
    }

    this->setRegexFilePattern(""); // Regex version of pattern

    this->matchFiles();

    this->sortFiles();
}

void FilePatternObject::printFiles(){
    for(const auto& file: this->iterator_){
        //cout << file << endl;
    }
}

void FilePatternObject::matchFilesOneDir(){

    Map mapping;
    vector<string> parsed_regex;

    int i, j;
    string s;
    string file, file_path;
    Tuple member;
    // Iterate over every file in directory

    // check if bracket expression was not properly parsed
    if (this->getRegexFilePattern().find('{') != std::string::npos || this->getRegexFilePattern().find('}') != std::string::npos) {
        auto start = this->getRegexFilePattern().find('{');
        auto end = this->getRegexFilePattern().find('}');
        auto length = end - start;
        throw invalid_argument("Invalid pattern found in bracket expressions in filepattern: \"" + this->getRegexFilePattern().substr(start, length+1) + "\"");
    }
    regex pattern_regex = regex(this->getRegexFilePattern());
    smatch sm;

    for (const auto& entry : this->iterator_) {
        // Get the current file
        file_path = entry.path().string();
        replace(file_path.begin(), file_path.end(), '\\', '/');
        file = s::getBaseName(file_path);
        if(regex_match(file, sm, pattern_regex)){
            this->valid_files_.push_back(getVariableMap(file_path, sm)); // write to txt file
        }
    }

}

void FilePatternObject::matchFilesMultDir(){
    // Iterate over every file in directory
    regex pattern_regex = regex(this->getRegexFilePattern());
    Tuple tup;
    smatch sm;
    string file, file_path;
    // Iterate over directories and subdirectories
    for (const auto& entry : this->recursive_iterator_) {
        file_path = entry.path().string();
        replace(file_path.begin(), file_path.end(), '\\', '/');
        if(this->getJustPath()){
            file = s::eraseSubStr(file_path, this->getPath());
        } else {
            file = s::getBaseName(file_path);
        }
        
        if(regex_match(file, sm, pattern_regex)){
            if(this->getJustPath()) tup = getVariableMap(file_path, sm);
            else tup = getVariableMapMultDir(file_path, sm);
            if(get<0>(tup).size() > 0){
            this->valid_files_.push_back(tup); // write to txt file
            }
        }
    }
}

void FilePatternObject::matchFiles() {

    filePatternToRegex(); // Get regex of filepattern

    if(this->recursive_){
       this->matchFilesMultDir();
    } else {
        this->matchFilesOneDir();
    }
}
