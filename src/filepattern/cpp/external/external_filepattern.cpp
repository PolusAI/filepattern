#include "external_filepattern.hpp"

using namespace std;

ExternalFilePattern::ExternalFilePattern(const string& path, const string& filePattern, const string& block_size, bool recursive, bool suppressWarnings):
ExternalPattern(path, block_size, recursive) {

    this->setSuppressWarnings(suppressWarnings);
    this->setPath(path); // store path to target directory
   // this->stream = {path, true, block_size};

    this->setBlockSize(Block::parseblockSize(block_size));
    this->setFpTmpdir("");

    this->setFilePattern(filePattern); // cast input string to regex
    this->setRegexFilePattern(""); // Regex equivalent of the pattern
    this->recursive_ = recursive; // Recursive directory iteration
    this->total_files_ = 0; // Number of files matched (to be removed)
    this->setMapSize(0); //To be updated later in program, set for compiling
    this->setValidFilesPath(this->stream_.getValidFilesPath()); // Store path to valid files txt file
    
    this->tmp_directories_.push_back(this->getValidFilesPath());
    this->setFirstCall(true); // first call to next() has not occurred

    this->matchFiles(); // match files to pattern

    ExternalMergeSort sort = ExternalMergeSort(std_map,
                                               this->getValidFilesPath(),
                                               this->getValidFilesPath(),
                                               this->stream_.getBlockSizeStr(),
                                               "",
                                               this->stream_.map_size_);

    this->group_stream_.open(this->stream_.getValidFilesPath());
    this->infile_.open(this->getValidFilesPath()); // open temp file for the valid files
    this->end_of_file_ = false; // end of valid files

}

ExternalFilePattern::~ExternalFilePattern(){
    this->infile_.close();
    for(auto& dir: this->tmp_directories_){
        if(dir != "") d::remove_dir(dir);

    }

}


void ExternalFilePattern::printFiles(){

    vector<Tuple> files;

    while(true){
        files = this->stream_.getValidFilesBlock();
        for(const auto& file: files){
            this->total_files_++;
            if(std::get<0>(file).size() < this->stream_.map_size_) continue;

            for(const auto& element: std::get<0>(file)){
               cout << element.first << ":" << s::to_string(element.second) << endl;
            }
            for(const auto& element: std::get<1>(file)){
                cout << "file: " << element << endl;
            }
            cout << endl;
        }

        if (this->stream_.endOfValidFiles()) break;

    }
}

void ExternalFilePattern::matchFiles() {

    filePatternToRegex(); // Get regex of filepattern

    this->setMapSize(this->variables_.size()); // Store map size for reading from txt file

    if(this->recursive_){
        this->matchFilesMultDir();
    } else {
        this->matchFilesOneDir();
    }

}

void ExternalFilePattern::matchFilesOneDir(){
    vector<string> block;

    regex pattern_regex = regex(this->getRegexFilePattern());
    string file;
    smatch sm;

    // iterate over files
    while(!this->stream_.isEmpty()){
        block = this->stream_.getBlock();

        for (auto& file_path : block) {
            replace(file_path.begin(), file_path.end(), '\\', '/');
            file = s::getBaseName(file_path);

            if(regex_match(file, sm, pattern_regex)){
                this->stream_.writeValidFiles(getVariableMap(file_path, sm)); // write to txt file
            }

        }

    }
}

/* In progress - need to find a way to avoid copying file on every write */
void ExternalFilePattern::matchFilesMultDir(){
    /*
    string pattern;
    Map mapping;
    Tuple member;

    vector<string> parsedRegex;

    int i, j;
    string s;
    string file_path;
    string file;
    bool matched;
    vector<string> block;
    // Iterate over every file in directory
    regex pattern_regex = regex(this->regexFilePattern);
    string str = "";
    this->validFilesPath = stream.getValidFilesPath();
    ifstream infile(validFilesPath);
    Tuple current;
    string temp;

    // iterate over directory and subdirectory in block_size chunks
    while(!this->stream.isEmpty()){
        block = stream.getBlock(); // get block of files from directory iterator
        for (const auto& file_path : block) {
            // Get the current file

            file = s::getBaseName(file_path);

            // Check if filename matches filepattern
            mapping.clear();
            if(regex_match(file, pattern_regex)) {
                matched = false;

                infile.open(stream.getValidFilesPath());  // open another stream to check if filename exists

                while(m::getMap(infile, current, this->map_size)) {
                    temp = s::getBaseName(std::get<1>(current)[0]);

                    // filename has already been found in another subdirectory
                    if(temp == file){

                        streampos ptr = infile.tellg();
                        matched = true; // found match

                        ptr -= 1; // move back one line in file
                        infile.seekg(ptr, ios::beg);

                        str = ' ' + file_path;
                        //infile << str << endl;
                        break;
                    }
                }
                infile.close();

                // If file was not found in another subdirectory
                if(!matched){
                    //mapping["file"] = file;
                    std::get<1>(member).push_back(file_path);

                    //std::get<0>(member)["file"] = file; // Add basename to mapping
                    // loop over the variables in the file pattern, creating a mapping
                    std::get<0>(member) = this->matchFilesLoop(mapping, file, pattern_regex, parsedRegex);
                    this->map_size = std::get<0>(member).size();

                    stream.writeValidFiles(member);
                    std::get<1>(member).clear();
                }
            }
        }
    }
    */
}
