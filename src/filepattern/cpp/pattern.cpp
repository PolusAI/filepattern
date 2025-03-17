#include "pattern.hpp"

void Pattern::getPathFromPattern(const std::string& path){
    this->path_ = path;
    this->file_pattern_ = path;

    auto firstBracketGroup = path.find("{"); // find location of first named group
    auto firstCustomGroup = path.find("(?P<");

    bool customGroupFirst = (firstCustomGroup < firstBracketGroup);

    auto firstGroup = customGroupFirst ? firstCustomGroup : firstBracketGroup; 

    if(firstGroup == std::string::npos) return; // return if none found
    
    // find first slash before named group
    while(path[firstGroup] != '/'){
        --firstGroup;
        if(firstGroup == 0) {
            throw std::invalid_argument("Invalid path. Atleast one directory without a named group must be provided.");
        }
    }
    ++firstGroup;

    this->path_ = path.substr(0, firstGroup); // piece of path without named groups becomes the path
    this->file_pattern_ = path.substr(firstGroup, path.length()-1); // the rest of the path is the pattern

}

void Pattern::setGroup(const std::vector<std::string>& groups){

    if (groups.size() == 1 && groups[0] == "*__all__*") {
        this->group_ = groups;
        return;
    }

    for (const auto& group: groups) {

        if(find(this->variables_.begin(), this->variables_.end(), group) != this->variables_.end()) {
            continue;
            //this->group = group;
        } else if(group != "") {
            throw std::invalid_argument("Group by variable must be contained in the pattern.");
        }
    }

    this->group_ = groups;
}

void Pattern::setIsSorted(bool sorted) {this->sorted_ = sorted;}

std::vector<std::string> Pattern::getVariables(){
    return this->variables_;
}

void Pattern::filePatternToRegex(){

    replace(path_.begin(), path_.end(), '\\', '/');

    std::tuple vars = getRegex(this->file_pattern_, this->suppress_warnings_);

    this->regex_file_pattern_ = std::get<0>(vars);
    this->variables_ = std::get<1>(vars);
    this->named_groups_ = std::get<2>(vars);
}

std::tuple<std::string, std::vector<std::string>, std::vector<std::string>> Pattern::getRegex(std::string& pattern, bool suppressWarning){

    getNewNaming(pattern, suppressWarning);

    // regex to match variables
    std::string e_str = "(\\{(\\w+):([0\\.dcf+]+)\\})|(\\(\\?P<(\\w+)>(.+)\\))";
    std::regex e(e_str, std::regex_constants::ECMAScript); // check for bracket expressions or named groups

    std::string group_str = "\\?P<(\\w+)>(.+)";

    std::regex group(group_str, std::regex_constants::ECMAScript); // check for regex named groups

    std::string var_str = "\\{(\\w+):([0\\.dcf+]+)\\}";
    std::regex var(var_str, std::regex_constants::ECMAScript); // pattern style of groups (e.g {r:ddd})


    std::map<wchar_t, std::string> patternMap; // map of variable types to regex equivalent
    patternMap['d'] = "[0-9]"; // integer matchingß
    patternMap['c'] = "[a-zA-Z]"; // character matching
    patternMap['f'] = "[0-9\\.]"; // match any floating point
    patternMap['.'] = "\\."; // decimal for floating point patterns
    patternMap['0'] = "0"; // 0 for decimal patterns
    patternMap['+'] = "+"; // arbitrary length matching

    std::string str, rgx; // temp string and regex
    std::vector<std::pair<std::string,std::string>> matches; // map between bracket expression and regex
    std::vector<std::string> variables; // store variable names
    std::string patternCopy = pattern; // get a copy of pattern since regex_search is inplace
    std::smatch sm, m; // regex matches

    std::string temp;
    // extract bracket expressions from pattern and store regex
    while (regex_search(patternCopy, m, e)){
        temp = m[0];

        // find any named groups with regex style naming
        if(temp.rfind("(?P<", 0) == 0) {

           while (regex_search(temp, sm, group)){

                rgx = sm[2]; // store regex value in named group
                rgx.pop_back(); // remove trailing )

                variables.push_back(sm[1]); // store variable name
                str = sm[0]; // store entire group
                str = "(" + str;

                matches.push_back(make_pair(str, rgx));
                temp = sm.suffix().str();
           }
        } else {
            // get named groups with the filepattern style groups (e.g. {r:ddd})
            while (regex_search(temp, sm, var)){
                str = sm[2]; // store variable values (e.g. ddd)
                rgx = ""; // construct regex
                //last = '';
                for(const auto c: str){
                    rgx += patternMap[c];
                }
                variables.push_back(sm[1]); // store variable name
                matches.push_back(make_pair(sm[0], rgx));
                temp = sm.suffix().str();
            }
        }

        patternCopy = m.suffix().str();
    }


    std::string regexFilePattern = pattern;
    std::vector<std::string> namedGroups;

    // Replace bracket groups with regex capture groups
    for(const auto& match: matches){

        namedGroups.push_back(match.first);

        // Create capture group
        str = "(" + match.second + ")";
        s::replace(regexFilePattern, match.first, str);
    }

    return make_tuple(regexFilePattern, variables, namedGroups);
}


Tuple Pattern::getVariableMapMultDir(const std::string& filePath, const std::smatch& sm){
    Tuple tup;

    bool matched = false;
    std::string basename;
    std::string file = s::getBaseName(filePath);
    // iterate over matched files, checking if filename already exists
    for (auto& valid_file: this->valid_files_) {
        #ifdef JAVA_BINDING
        basename = s::getBaseName(s::to_string(get<1>(valid_file)[0])); // store the basename
        #else
        basename = s::getBaseName(std::get<1>(valid_file)[0].string()); // store the basename
        #endif
        // if the filename is found, add the filepath to the vector in the second member of the tuple
        if(basename == file){
            matched = true;
            std::get<1>(valid_file).push_back(filePath); // Add path to existing mapping
            break;
        }
    }

    // Create new mapping if not already present
    if(!matched){
        tup = getVariableMap(filePath, sm);
    }

    return tup;
}

Tuple Pattern::getVariableMap(const std::string& filePath, const std::smatch& sm){
    Tuple tup;
    // filename matches the pattern

    std::get<1>(tup).push_back(filePath);

    std::string str;
    // Extract capture groups from filename and store in mapping
    for(unsigned int i = 1; i < sm.size(); ++i){

        str = sm[i];

        // conserve variable type
        if (s::is_number(str)) {
            if (s::is_integer(str)) {
                std::get<0>(tup)[variables_[i-1]] = std::stoi(str);
            } else {
                std::get<0>(tup)[variables_[i-1]] = std::stod(str);
            }
        } else {
            std::get<0>(tup)[variables_[i-1]] = str;
        }

        this->variable_occurrences_[this->variables_[i-1]][std::get<0>(tup)[this->variables_[i-1]]] += 1; // update count of the variable occurrence
        this->unique_values_[this->variables_[i-1]].insert(std::get<0>(tup)[this->variables_[i-1]]); // update the unique values for the variable
    }

    return tup;
}

std::map<std::string, std::map<Types, int>> Pattern::getOccurrences(const std::vector<std::tuple<std::string, std::vector<Types>>>& mapping){
    // if no variables request, return all variables
    if(mapping.size() == 0){
        return this->variable_occurrences_;
    }

    std::map<Types, int> temp;
    std::map<std::string, std::map<Types, int>> occurrences;
    std::string variable;
    // loop over vector passed in that contains the variable mapped to value(s)
    for(const auto& tup: mapping){
        if(std::get<1>(tup).size() == 0){
            occurrences[std::get<0>(tup)] = this->variable_occurrences_[std::get<0>(tup)];
        } else {
            for(const auto& value: std::get<1>(tup)){
                variable = std::get<0>(tup);
                temp[value] = this->variable_occurrences_[std::get<0>(tup)][value];
            }
            occurrences[variable] = temp;
        }
    }

    return occurrences;
}

std::string Pattern::getPattern(){
    return this->file_pattern_;
}

void Pattern::setPattern(const std::string& pattern){
    this->file_pattern_ = pattern;
}

std::string Pattern::getRegexPattern(){
    return this->regex_file_pattern_;
}

void Pattern::printVariables(){
    int i = 0;
    std::cout << "The variables are: ";
    int size = this->getVariables().size();
    for(const auto& var: this->getVariables()){
        std::cout << var;
        if(i<size-1){
            std::cout << ", ";
        }
        i++;
    }
    std::cout << std::endl;
}

std::map<std::string, std::set<Types>> Pattern::getUniqueValues(const std::vector<std::string>& vec){
    if(vec.size() == 0) return this->unique_values_; // if no variables are passed, return all variables

    std::map<std::string, std::set<Types>> temp;
    // return variables that were requested
    for(const auto& str: vec){
        temp[str] = unique_values_[str];
    }
    return temp;
}


void Pattern::getNewNaming(std::string& pattern, bool suppressWarnings){

    if(pattern == "") return;

    std::string vars = "\\{([rtczyxp+]+)\\}"; // check for old naming style or named grouped
    std::regex e(vars);

    std::string str; // temp string

    std::vector<std::pair<std::string, std::string>> matches; // map between bracket expression and regex

    std::string patternCopy = pattern; // get a copy of pattern since regex_search is inplace

    std::smatch m; // regex matches

    bool replaced = false;
    // extract bracket expressions from pattern and store regex
    while (regex_search(patternCopy, m, e)){
        str = m[1];

        matches.push_back(make_pair(m[0], m[1]));

        //regexMatches.push_back(m);
        patternCopy = m.suffix().str();

    }
    // Replace bracket groups with regex capture groups
    for(const auto& match: matches){
        // Create capture group

        if(match.first.find(':') == std::string::npos) {

            replaced = true;
            str = "{";
            str += match.second[0];
            str += ":";
            for(const auto& c: match.second){
                if(c != '+') str += "d";
                else str += "+";
            }
            str += "}";

            s::replace(pattern, match.first, str);
        }
    }
    if(replaced && !suppressWarnings){
        std::cout << "WARNING: The old style of pattern was used. This style may become deprecated in future releases." << std::endl;
        std::cout << "The recommended pattern to use is: " << pattern <<
                ". See the documentation for details about the new style." << std::endl;
    }
}

void Pattern::replaceOutputName(Tuple& min, Tuple& max, const std::string& var, std::string& outputName, const int idx, std::string& temp, const std::regex& patternRegex){
        std::string file;
        std::smatch sm;
        if(std::get<0>(min)[var] == std::get<0>(max)[var]){

            #ifdef JAVA_BINDING
            file = s::getBaseName(get<1>(min)[0]); // get basename of filepath
            #else
            file = s::getBaseName((std::get<1>(min)[0]).string()); // get basename of filepath
            #endif
            regex_match(file, sm, patternRegex);

            s::replace(outputName, this->named_groups_[idx], sm[idx+1]);

        } else { // if min is different than max, put range in outputname

            temp = "(";

            #ifdef JAVA_BINDING
            file = s::getBaseName(get<1>(min)[0]); // get basename of filepath
            #else
            file = s::getBaseName((std::get<1>(min)[0]).string()); // get basename of filepath
            #endif
            regex_match(file, sm, patternRegex); // find variables

            temp += sm[idx+1];
            temp += "-";

            #ifdef JAVA_BINDING
            file = s::getBaseName(get<1>(max)[0]); // get basename of filepath
            #else
            file = s::getBaseName((std::get<1>(max)[0]).string()); // get basename of filepath
            #endif
            regex_match(file, sm, patternRegex);

            temp += sm[idx+1];
            temp += ")";

            s::replace(outputName, this->named_groups_[idx], temp);
        }
}

std::string Pattern::outputNameHelper(std::vector<Tuple>& vec){
    if(vec.size() == 0){
        vec = this->valid_files_;
    }

    std::string outputName = this->file_pattern_;

    int idx = 0;
    int min, max;
    std::smatch sm;
    std::string temp, file;

    std::regex patternRegex(this->regex_file_pattern_);

    for(auto& var: this->variables_){

        min = m::getMinIdx(vec, var);
        max = m::getMaxIdx(vec, var);

        // if min is the same as max, put variable value in output
        this->replaceOutputName(vec[min], vec[max], var, outputName, idx, temp, patternRegex);

        ++idx;
    }

    return outputName;

}

std::string Pattern::inferPatternInternal(std::vector<std::string>& files, std::string& variables, const std::string& startingPattern){
    
    variables += "rtczyxp"; // extra variables incase not enough were given

    std::string pattern;

    if(startingPattern == "") {
        pattern = files[0];
    } else {
        pattern = startingPattern;
    }

    std::string regexStr;
    std::string patternCpy = pattern;
    std::regex rgx = std::regex(std::get<0>(getRegex(patternCpy)));

    for(auto& file : files){

        if(!regex_match(file, rgx)) {

            pattern = swSearch(pattern, file, variables);

            patternCpy = pattern;

            regexStr = std::get<0>(getRegex(patternCpy));
            rgx = std::regex(regexStr);
        }
    }

    if (pattern == "") {
        throw std::runtime_error("Error: could not not infer pattern from input.");
    }

    return pattern;
}

std::string Pattern::swSearch(std::string& pattern, std::string& filename, const std::string& variables){
    std::string numbers = "0123456789<>"; // numeric values, '<' -> d and '>' -> d+
    std::string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ@$"; // alphabetic values, '@' -> c+ and '$' -> c
    std::smatch sm; // store regex group matches
    std::string rgxStr; // string version of rgx
    std::regex rgx;
    std::string temp, smStr;
    bool match;

    //vlist = [(0,0,"", "")]
    for (const auto& v: variables){

        // construct named group
        rgxStr = "\\{(";
        rgxStr += v;
        rgxStr += "):([dc+]+)\\}";

        rgx = std::regex(rgxStr); // regex to match
        match = false; // match if found
        while(regex_search(pattern, sm, rgx)){

            match = true;
            // check if + is in group
            smStr = sm[0];
            if(s::contains(smStr, '+')) {
                if(sm[2] == "d+") s::replace(pattern, sm[0], ">");
                else  s::replace(pattern, sm[0], "@");

            } else {
                temp = "";
                smStr = sm[2].str();

                if(s::contains(smStr, 'd')){
                    temp.insert(0, s::getCount(sm[0].str(), 'd'), '<');
                    s::replace(pattern, sm[0], temp);
                } else {
                    temp.insert(0, s::getCount(sm[0].str(), 'c'), '$');
                    s::replace(pattern, sm[0], temp);
                }
            }
        }
        if(!match) {
            break;
        }
    }

    // scoring function
    std::map<std::string, std::map<std::string, int>> sab = {
        {"numeric", {
            {"match", 2},
            {"penalty", 1}
        }},
        {"alpha", {
            {"match", 2},
            {"penalty", 1}
        }},
        {"char", { // larger penalty for non-numeric, non-alphabetic characters
            {"match", 5},
            {"penalty", 3}
        }}
    };

    // scoring matrix creation
    const int m = pattern.length();
    const int n = filename.length();
    std::vector<std::vector<int>> matrix(m+1, std::vector<int>(n+1, 0));//[m+1][n+1]; // switch this to linear indexing in future
    //fill(matrix[0], matrix[0] + (m+1) * (n+1), 0);


    int p_idx = 0;
    int f_idx = 0;
    bool pIsNumeric, fIsNumeric;
    bool pIsCharacter, fIsCharacter;
    std::vector<int> scores;
    int s, wi, wj;

    // populate scoring matrix
    for(const auto& p: pattern){
        pIsNumeric = count(numbers.begin(), numbers.end(), p);
        pIsCharacter = count(alphabet.begin(), alphabet.end(), p);

        f_idx = 0;
        for(const auto& f: filename){

            fIsNumeric = count(numbers.begin(), numbers.end(), f);
            fIsCharacter = count(alphabet.begin(), alphabet.end(), f);

            scores.clear();
            scores.push_back(0);

            // generate similarity score
            if(fIsNumeric){
                if(pIsNumeric){
                    s = matrix[p_idx][f_idx] + sab["numeric"]["match"];
                } else {
                    s = matrix[p_idx][f_idx] - sab["numeric"]["match"];
                }
            } else if(fIsCharacter) {
                if(pIsCharacter){
                    s = matrix[p_idx][f_idx] + sab["alpha"]["match"];
                } else {
                    s = matrix[p_idx][f_idx] - sab["alpha"]["match"];
                }
            } else {
                if (p == f){
                    s = matrix[p_idx][f_idx] + sab["char"]["match"];
                } else {
                    s = matrix[p_idx][f_idx] - sab["char"]["match"];
                }
            }

            scores.push_back(s);

            // calculate gap scores
            if (fIsNumeric){
                if (pIsNumeric) {
                    wi = matrix[p_idx+1][f_idx] - sab["numeric"]["penalty"];
                    wj = matrix[p_idx][f_idx+1] - sab["numeric"]["penalty"];
                } else {
                    wi = matrix[p_idx+1][f_idx] - sab["alpha"]["penalty"];
                    wj = matrix[p_idx][f_idx+1] - sab["alpha"]["penalty"];
                }
            } else if(fIsCharacter) {
                if(pIsCharacter){
                    wi = matrix[p_idx+1][f_idx] - sab["alpha"]["penalty"];
                    wj = matrix[p_idx][f_idx+1] - sab["alpha"]["penalty"];
                } else {
                    wi = matrix[p_idx+1][f_idx] - sab["numeric"]["penalty"];
                    wj = matrix[p_idx][f_idx+1] - sab["numeric"]["penalty"];
                }
            } else {
                wi = matrix[p_idx+1][f_idx] - sab["char"]["penalty"];
                wj = matrix[p_idx][f_idx+1] - sab["char"]["penalty"];
            }

            scores.push_back(wi);
            scores.push_back(wj);

            // assign matrix score
            matrix[p_idx+1][f_idx+1] = *max_element(scores.begin(), scores.end());

            ++f_idx;

        }
        ++p_idx;
    }

    // find best score
    int bestScore = matrix[m][n];
    int row       = m;
    int col       = n;

    for(int r = 1; r < m+1; ++r){
        for(int c = 1; c < n+1; ++c){
            if(matrix[r][c] > bestScore){
                bestScore = matrix[r][c];
                row = r;
                col = c;
            }
        }
    }

    // traceback, building a new pattern template
    std::string patternTemplate = ""; //(1, filename[col-1]);
    patternTemplate.push_back(filename[col-1]);
    int lastRow = row;
    int lastCol = col;
    int r, c;

    while(true){ // loop until best_score == 0 is reached

        // Default to next set of characters
        r = row - 1;
        c = col - 1;
        bestScore = matrix[r][c];

        if(matrix[row][col-1] > bestScore){
            bestScore = matrix[row][col-1];
            r = row;
            c = col - 1;
        }

        if(matrix[row-1][col] > bestScore){
            bestScore = matrix[row-1][col];
            r = row - 1;
            c = col;
        }

        if (bestScore == 0) break;

        row = r;
        col = c;

        // Default to the matching value if available
        if(filename[col-1] == pattern[row-1] && lastCol != col && lastRow != row){
            patternTemplate = filename[col-1] + patternTemplate;
        } else {

            // If the values don't match, throw error if non-numeric and non-alphabetic
            if(!count(numbers.begin(), numbers.end(), filename[col-1]) ||
            !count(numbers.begin(), numbers.end(), pattern[row-1])) {
                if(count(alphabet.begin(), alphabet.end(), filename[col-1]) &&
                   count(alphabet.begin(), alphabet.end(), pattern[row-1])){
                    if(lastCol != col && lastRow != row){
                        if (pattern[row-1] == '@'){
                            patternTemplate = '@' + patternTemplate;
                        } else {
                            patternTemplate = '$' + patternTemplate;
                        }
                    } else {
                        patternTemplate = "@" + patternTemplate;
                    }
                } else {
                    throw std::runtime_error("Non-numeric, non-alphabetic characters found that do not match");
                }

            } else if(lastCol != col && lastRow != row){ // progrsssion was made so add a placeholder
                if(pattern[row-1] == '>'){
                    patternTemplate = '>' + patternTemplate;
                } else {
                    patternTemplate = '<' + patternTemplate;
                }

            } else { // Lengths don't match, so add a place holder
                patternTemplate = ">" + patternTemplate;
            }
        }

        lastRow = row;
        lastCol = col;
    }

    pattern = patternTemplate;
    int vi = 0;
    rgx = "[<>\\$@]+";
    std::string vdef;

    // Construct a new filepattern
    for (auto i = std::sregex_iterator(patternTemplate.begin(), patternTemplate.end(), rgx); i != std::sregex_iterator(); ++i) {
        temp = (*i).str();

        if(temp.find('>') != std::string::npos){
            vdef = "{";
            vdef += variables[vi];
            vdef += ":d+}";
        } else if (temp.find('<') != std::string::npos){
            vdef = "{";
            vdef += variables[vi];
            vdef += ":";
            vdef.insert(vdef.length(), temp.length(), 'd');
            vdef += "}";
        } else if (temp.find('@') != std::string::npos) {
            vdef = "{";
            vdef += variables[vi];
            vdef += ":c+}";
        } else if (temp.find('$') != std::string::npos){
            vdef = "{";
            vdef += variables[vi];
            vdef += ":";
            vdef.insert(vdef.length(), temp.length(), 'c');
            vdef += "}";
        }

        s::replace(pattern, temp, vdef);

        ++vi;
    }

    return pattern;
}

std::vector<std::string> Pattern::getTmpDirs(){
    return this->tmp_directories_;
}


void Pattern::setRegexExpression(std::regex regex_expression){
    this->regex_expression_ = regex_expression;
}

void Pattern::setFilePattern(std::string file_pattern){
    this->file_pattern_ = file_pattern;
}

void Pattern::setPathPattern(std::string path_pattern){
    this->path_pattern_ = path_pattern;
}

void Pattern::setRegexFilePattern(std::string regex_file_pattern){
    this->regex_file_pattern_ = regex_file_pattern;
}

void Pattern::setPath(std::string path){
    this->path_ = path;
}

void Pattern::setJustPath(bool just_path){
    this->just_path_ = just_path;
}

void Pattern::setSuppressWarnings(bool suppress_warnings){
    this->suppress_warnings_ = suppress_warnings;
}

std::regex Pattern::getRegexExpression(){
    return this-> regex_expression_;
}

std::string Pattern::getFilePattern(){
    return this->file_pattern_;
}

std::string Pattern::getPathPattern(){
    return this->path_pattern_;
}

std::string Pattern::getRegexFilePattern(){
    return this->regex_file_pattern_;
}

std::string Pattern::getPath(){
    return this->path_;
}

bool Pattern::getJustPath(){
    return this->just_path_;
}

bool Pattern::captureDirectoryNames() {
    return this->capture_directory_names_;
}

void Pattern::setCaptureDirectoryNames(bool capture) {
    this->capture_directory_names_ = capture;
}

bool Pattern::getSuppressWarnings(){
    return this->suppress_warnings_;
}

bool Pattern::isSorted() {return this->sorted_;}