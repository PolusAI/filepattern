#include "vectorpattern.hpp"

using namespace std;

VectorPattern::VectorPattern(const string& path, const string& filePattern, bool suppressWarnings){
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

    this->STITCH_REGEX_ = "corr: (.*); position: \\((.*), (.*)\\); grid: \\((.*), (.*)\\);"; // regex of a stitching vector line
    this->STITCH_VARIABLES_ = {"correlation","posX","posY","gridX","gridY"}; // stitching vector variables

    this->setRegexExpression(regex(this->getRegexFilePattern())); // cast pattern to regex

    string line, file;
    Tuple temp;
    smatch sm;
    while(getline(this->infile_, line)){
        file = VectorParser::getFileName(line); // get filename from line of stitching vector
        if(regex_match(file, sm, this->getRegexExpression())){ // match to regex. groups are in sm
            temp = getVariableMap(file, sm); // get the variable map for the filename
            VectorParser::parseVectorLine(temp, line, this->STITCH_VARIABLES_, this->STITCH_REGEX_, this->variables_); // parse the vector line
            this->valid_files_.push_back(temp); // add file mapping to matched files
        }
    }
    this->infile_.close();
}

string VectorPattern::inferPattern(const string& path, string& variables){
    string file;
    ifstream infile(path);
    vector<string> files;

    // get filenames from stitching vector 
    while(getline(infile, file)){
        file = VectorParser::getFileName(file);
        files.push_back(file);
    }

    // infer pattern from vector of filenames
    return inferPatternInternal(files, variables);
}