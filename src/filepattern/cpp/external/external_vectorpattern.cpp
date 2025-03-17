#include "external_vectorpattern.hpp"

const std::regex ExternalVectorPattern::STITCH_REGEX_ = std::regex("(corr): (.*); (position): \\((.*), (.*)\\); (grid): \\((.*), (.*)\\);"); // regex of a stitching vector line
const std::vector<std::regex> ExternalVectorPattern::STITCH_REGEX_VECTOR_ = {std::regex("(corr):\\s*(.*?);"), std::regex("(position):\\s*\\((.*?),\\s*(.*?)\\);"), std::regex("(grid):\\s*\\((.*),\\s*(.*)\\);")};
const std::vector<std::string> ExternalVectorPattern::STITCH_VARIABLES_ = {"correlation","posX","posY","gridX","gridY"}; // stitching vector variables

ExternalVectorPattern::ExternalVectorPattern(const std::string& path, const std::string& file_pattern, const std::string& block_size, bool suppress_warnings, bool sorted):
ExternalPattern(path, block_size, false){
    this->setSuppressWarnings(suppress_warnings);
    this->path_ = path; // store path to target directory
    this->setFpTmpdir("");

    this->setBlockSize(Block::parseblockSize(block_size));

    this->vector_reader_.open(path);
    if(!this->vector_reader_.is_open()) throw std::invalid_argument("Invalid path \"" + path + "\".");

    this->setFilePattern(file_pattern); // cast input string to regex
    this->setRegexFilePattern(""); // Regex version of pattern

    this->setMapSize(0); //To be updated later in program, set for compiling
    this->setValidFilesPath(this->stream_.getValidFilesPath()); // Store path to valid files txt file
    this->tmp_directories_.push_back(this->getValidFilesPath());
    this->setFirstCall(true); // first call to next() has not occurred

    this->matchFiles();
    this->setIsSorted(sorted);

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

    this->setRegexExpression(std::regex(this->getRegexFilePattern()));

    std::string line, file;
    Tuple temp;
    std::smatch sm;

    while(std::getline(this->vector_reader_, line)){
        file = VectorParser::getFileName(line);
        if(regex_match(file, sm, this->getRegexExpression())){
            temp = getVariableMap(file, sm);
            VectorParser::parseVectorLine(temp, line, this->STITCH_VARIABLES_, this->STITCH_REGEX_VECTOR_, this->variables_);
            this->stream_.writeValidFiles(temp);
        }
    }
    this->vector_reader_.close();
}

std::string ExternalVectorPattern::inferPattern(const std::string& path, std::string& variables, const std::string& block_size){

    long block = Block::parseblockSize(block_size); // parse string

    std::vector<std::string> files;
    std::string file;
    std::ifstream infile(path);

    long size = sizeof(std::vector<std::string>) + sizeof(std::vector<std::vector<int>>); // memory footprint

    std::string pattern = "";
    // while the stitching vector contains another line:
    //      get block of file from stitching vector and call internal memory version of infer pattern on the block
    while(getline(infile, file)){

        file = VectorParser::getFileName(file);

        files.push_back(s::escape_regex_characters(file));

        size += sizeof(std::string) + file.length() + sizeof(int)*2*file.length(); // account for size of filenames

         if(size >= block) {
            pattern = inferPatternInternal(files, variables, pattern);
            size = sizeof(std::vector<std::string>) + sizeof(std::vector<std::vector<int>>);
            files.clear();
        }
    }
    // when no more lines are left, process the last block if its nonempty
    if(files.size() != 0){
        pattern = inferPatternInternal(files, variables, pattern);
    }

    return pattern;
}
