#include "external_stringpattern.hpp"

using namespace std;

ExternalStringPattern::ExternalStringPattern(const string& path, const string& file_pattern, const string& block_size, bool suppress_warnings):
ExternalPattern(path, block_size, false) {
    this->setSuppressWarnings(suppress_warnings);
    this->setPath(path); // store path to target directory
    //this->stream = {path, true, block_size};
    this->setBlockSize(Block::parseblockSize(block_size));
    this->setFpTmpdir("");

    this->setFilePattern(file_pattern); // cast input string to regex
    this->setRegexFilePattern(""); // Regex equivalent of the pattern

    this->total_files_ = 0; // Number of files matched (to be removed)

    this->setMapSize(0); //To be updated later in program, set for compiling

    this->setValidFilesPath(this->stream_.getValidFilesPath()); // Store path to valid files txt file

    this->tmp_directories_.push_back(this->getValidFilesPath());
    
    this->setFirstCall(true); // first call to next() has not occurred

    this->matchFiles(); // match files to pattern
    this->group_stream_.open(this->stream_.getValidFilesPath());
    this->infile_.open(this->getValidFilesPath()); // open temp file for the valid files
    this->end_of_file_ = false; // end of valid files

}

ExternalStringPattern::~ExternalStringPattern(){
    for(auto& dir: this->tmp_directories_){
        if(dir != "") d::remove_dir(dir);
    }
}

void ExternalStringPattern::matchFiles(){
    this->filePatternToRegex();

    this->setMapSize(this->variables_.size());

    vector<string> block;

    regex pattern_regex = regex(this->getRegexFilePattern());
    string file;
    smatch sm;

    // iterate over files
    while(!this->stream_.isEmpty()){

        block = this->stream_.getBlock();

        for (const auto& file : block) {

            if(regex_match(file, sm, pattern_regex)){
                this->stream_.writeValidFiles(getVariableMap(file, sm)); // write to txt file
            }
        }
    }

}
