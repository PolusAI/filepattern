#include "vector_parser.hpp"

using namespace std; 

string VectorParser::getFileName(string& stitching_vector_line){
    string temp = stitching_vector_line;
    s::removeSpaces(temp); 
    return s::split(s::split(temp, ";")[0], ":")[1]; // return only the filename from the line
}

void VectorParser::parseVectorLine(Tuple& tup,
                                    string& stitching_vector_line, 
                                    const vector<string>& STITCH_VARIABLES, 
                                    const regex& STITCH_REGEX,
                                    vector<string> variables) {

    smatch sm;
    // if the line from the stitching vector matching the stitching vector regex pattern
    if (regex_search(stitching_vector_line, sm, STITCH_REGEX)){   
        for(int i = 0; i < STITCH_VARIABLES.size(); ++i){
            variables.push_back(STITCH_VARIABLES[i]); // add stitching vector variables to variables list
            // maintain datatype of variable
            if(s::is_number(s::to_string(sm[i+1]))){
                get<0>(tup)[STITCH_VARIABLES[i]] = stoi(sm[i+1]);
            } else {
                get<0>(tup)[STITCH_VARIABLES[i]] = sm[i+1];
            }
        }
    } 

}