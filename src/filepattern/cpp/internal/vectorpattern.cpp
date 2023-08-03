#include "vectorpattern.hpp"

using namespace std;

const std::regex VectorPattern::STITCH_REGEX_ = std::regex("(corr): (.*); (position): \\((.*), (.*)\\); (grid): \\((.*), (.*)\\);"); // regex of a stitching vector line
const std::vector<std::regex> VectorPattern::STITCH_REGEX_VECTOR_ = {std::regex("(corr):\\s*(.*?);"), std::regex("(position):\\s*\\((.*?),\\s*(.*?)\\);"), std::regex("(grid):\\s*\\((.*),\\s*(.*)\\);")};
const std::vector<std::string> VectorPattern::STITCH_VARIABLES_ = {"correlation","posX","posY","gridX","gridY"}; // stitching vector variables

VectorPattern::VectorPattern(const std::string& path, const std::string& filePattern, bool suppressWarnings){

    if (!fs::exists(path)) {
        throw std::invalid_argument("Path \"" + path + "\" does not exist.");
    }

    this->setSuppressWarnings(suppressWarnings);
    this->path_ = path; // store path to target directory

    this->infile_.open(path);
    if(!this->infile_.is_open()) throw invalid_argument("Invalid path \"" + this->path_ + "\".");

    this->setFilePattern(filePattern); // cast input string to regex
    this->setRegexFilePattern(""); // Regex version of pattern

    this->matchFiles(); // match lines of stitching vector to the pattern
    this->sortFiles();
}

void VectorPattern::matchFiles(){   
    this->filePatternToRegex(); // get regex version of the pattern

    this->setRegexExpression(regex(this->getRegexFilePattern())); // cast pattern to regex

    std::string line, file;
    Tuple temp;
    smatch sm;
    while(getline(this->infile_, line)){
        file = VectorParser::getFileName(line); // get filename from line of stitching vector
        if(regex_match(file, sm, this->getRegexExpression())){ // match to regex. groups are in sm
            temp = getVariableMap(file, sm); // get the variable map for the filename
            //VectorParser::parseVectorLine(temp, line, this->STITCH_VARIABLES_, this->STITCH_REGEX_, this->variables_); // parse the vector line
            VectorParser::parseVectorLine(temp, line, this->STITCH_VARIABLES_, this->STITCH_REGEX_VECTOR_, this->variables_); // parse the vector line
            this->valid_files_.push_back(temp); // add file mapping to matched files
        }
    }
    this->infile_.close();
}

std::string VectorPattern::inferPattern(const std::string& path, std::string& variables, const std::string& block_size){

    if (!fs::exists(path)) {
        throw std::invalid_argument("Path \"" + path + "\" does not exist.");
    }

    std::string file;
    ifstream infile(path);
    std::vector<std::string> files;

    // get filenames from stitching vector 
    while(getline(infile, file)){
        file = VectorParser::getFileName(file);
        files.push_back(file);
    }

    // infer pattern from vector of filenames
    return inferPatternInternal(files, variables);
}