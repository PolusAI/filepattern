#include "vector_parser.hpp"
#include "../internal/VectorPattern.hpp"

using namespace std; 

string VectorParser::getFileName(string& stitching_vector_line){
    string temp = stitching_vector_line;
    s::removeSpaces(temp); 
    return s::split(s::split(temp, ";")[0], ":")[1]; // return only the filename from the line
}

bool VectorParser::isStitchingVector(std::string& line) {

    try {

        Tuple temp;
        std::vector<std::string> vars;

        // attempt to parse lin
        parseVectorLine(temp, 
                        line, 
                        VectorPattern::STITCH_VARIABLES_, 
                        VectorPattern::STITCH_REGEX_VECTOR_,
                        vars);

        // no exception thrown -> valid stitching vector
        return true;               
    } catch (std::exception& e){

        // line did not have format of stitching vector 
        return false;
    }
};

void VectorParser::parseVectorLine(Tuple& tup,
                                    std::string& stitching_vector_line, 
                                    const std::vector<std::string>& stitch_variables, 
                                    const std::vector<regex>& stitch_regex,
                                    std::vector<std::string> variables) {

    smatch sm;

    std::unordered_map<std::string, std::string> vars;

    for (const auto& rgx: stitch_regex) {
        // if the line from the stitching vector matching the stitching vector regex pattern
        if (regex_search(stitching_vector_line, sm, rgx)){  

            if (sm[1] == "grid") {

                if (sm.size() < 4) {
                    std::string error  = "The \"grid\" variable in the stitching vector must include x and y coordinates.\n\t Thrown by the line: " 
                                        + stitching_vector_line;
                    throw std::runtime_error(error);
                }

                vars["gridX"] = sm[2];
                vars["gridY"] = sm[3];

            } else if (sm[1] == "position") {

                if (sm.size() < 4) {
                    std::string error  = "The \"position\" variable in the stitching vector must include x and y coordinates.\n\t Thrown by the line: " 
                                        + stitching_vector_line;
                    throw std::runtime_error(error);
                }

                vars["posX"] = sm[2];
                vars["posY"] = sm[3];
            
            } else if (sm[1] == "corr") {

                vars["correlation"] = sm[2];
                std::cout << sm[2] << std::endl;
            
            } else {

                std::string var = sm[1]; // variable name
                
                // check if variable is valid
                bool found = false;
                for (const auto& stitch_var: stitch_variables) {
                    if (stitch_var == var) found = true;
                }

                if (!found) {
                    throw std::runtime_error("Invalid variable \"" + var + "\" found in stitching vector");
                }

                vars[var] = sm[2];
            }

        }

    }

    if (vars.size() != stitch_variables.size()) {
        throw std::runtime_error("Incorrect number of variables in stitching vector.");
    }

    for(auto& variable: vars){

        variables.push_back(variable.first); // add stitching vector variables to variables list

        // maintain datatype of variable
        if(s::is_number(variable.second)){
            if (s::is_integer(variable.second)) {
                std::cout << "integer" << std::endl;
                get<0>(tup)[variable.first] = stoi(variable.second);
            } else {
                std::cout << "double: " << stod(variable.second) << std::endl;
                get<0>(tup)[variable.first] = stod(variable.second);
            }
        } else {
            get<0>(tup)[variable.first] = variable.second;
        }
    }
    

}