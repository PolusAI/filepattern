#include "external_vectorpattern.hpp"

using namespace std;

const std::regex ExternalVectorPattern::STITCH_REGEX_ = std::regex("(corr): (.*); (position): \\((.*), (.*)\\); (grid): \\((.*), (.*)\\);"); // regex of a stitching vector line
const std::vector<std::regex> ExternalVectorPattern::STITCH_REGEX_VECTOR_ = {std::regex("(corr):\\s*(.*?);"), std::regex("(position):\\s*\\((.*?),\\s*(.*?)\\);"), std::regex("(grid):\\s*\\((.*),\\s*(.*)\\);")};
const std::vector<std::string> ExternalVectorPattern::STITCH_VARIABLES_ = {"correlation","posX","posY","gridX","gridY"}; // stitching vector variables

ExternalVectorPattern::ExternalVectorPattern(const string& path, const string& file_pattern, const string& block_size, bool suppress_warnings):
ExternalPattern(path, block_size, false){
    this->setSuppressWarnings(suppress_warnings);
    this->path_ = path; // store path to target directory
    this->setFpTmpdir("");

    this->setBlockSize(Block::parseblockSize(block_size));

    this->vector_reader_.open(path);
    if(!this->vector_reader_.is_open()) throw invalid_argument("Invalid path \"" + path + "\".");

    this->setFilePattern(file_pattern); // cast input string to regex
    this->setRegexFilePattern(""); // Regex version of pattern

    this->setMapSize(0); //To be updated later in program, set for compiling
    this->setValidFilesPath(this->stream_.getValidFilesPath()); // Store path to valid files txt file
    this->tmp_directories_.push_back(this->getValidFilesPath());
    this->setFirstCall(true); // first call to next() has not occurred

    this->matchFiles();

    this->group_stream_.open(this->stream_.getValidFilesPath());
    this->infile_.open(this->getValidFilesPath()); // open temp file for the valid files
}

ExternalVectorPattern::~ExternalVectorPattern(){
    // Loop over stored paths and delete contents and directory
    for(auto& dir: this->tmp_directories_){
        if(dir != "") d::remove_dir(dir);

    }
}

void ExternalVectorPattern::matchFiles(){   
    this->filePatternToRegex();

    this->setMapSize(this->variables_.size() + this->STITCH_VARIABLES_.size()); // Change the size of the map to include stitching variables

    this->setRegexExpression(regex(this->getRegexFilePattern()));

    string line, file;
    Tuple temp;
    smatch sm;

    while(getline(this->vector_reader_, line)){
        file = VectorParser::getFileName(line);
        if(regex_match(file, sm, this->getRegexExpression())){
            temp = getVariableMap(file, sm);
            VectorParser::parseVectorLine(temp, line, this->STITCH_VARIABLES_, this->STITCH_REGEX_VECTOR_, this->variables_);
            this->stream_.writeValidFiles(temp);
        }
    }
    this->vector_reader_.close();
}

string ExternalVectorPattern::inferPattern(const string& path, string& variables, const string& block_size){

    long block = Block::parseblockSize(block_size); // parse string
    
    vector<string> files;
    string file;
    ifstream infile(path);

    long size = sizeof(vector<string>) + sizeof(vector<vector<int>>); // memory footprint
    
    string pattern = "";
    // while the stitching vector contains another line:
    //      get block of file from stitching vector and call internal memory version of infer pattern on the block
    while(getline(infile, file)){

        file = VectorParser::getFileName(file);

        files.push_back(file);
        
        size += sizeof(string) + file.length() + sizeof(int)*2*file.length(); // account for size of filenames
            
         if(size >= block) {
            pattern = inferPatternInternal(files, variables, pattern);
            size = sizeof(vector<string>) + sizeof(vector<vector<int>>);
            files.clear();
        }
    }
    // when no more lines are left, process the last block if its nonempty
    if(files.size() != 0){
        pattern = inferPatternInternal(files, variables, pattern);
    }

    return pattern;
}