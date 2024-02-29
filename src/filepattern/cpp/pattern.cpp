#include "pattern.hpp"

using namespace std;

void Pattern::getPathFromPattern(const string& path){
    this->path_ = path;
    this->file_pattern_ = path;

    auto firstBracketGroup = path.find("{"); // find location of first named group
    auto firstCustomGroup = path.find("(?P<");

    bool customGroupFirst = (firstCustomGroup < firstBracketGroup);

    auto firstGroup = customGroupFirst ? firstCustomGroup : firstBracketGroup; 

    if(firstGroup == string::npos) return; // return if none found
    
    // find first slash before named group
    while(path[firstGroup] != '/'){
        --firstGroup;
        if(firstGroup == 0) {
            throw invalid_argument("Invalid path. Atleast one directory without a named group must be provided.");
        }
    }
    ++firstGroup;

    this->path_ = path.substr(0, firstGroup); // piece of path without named groups becomes the path
    this->file_pattern_ = path.substr(firstGroup, path.length()-1); // the rest of the path is the pattern

}

void Pattern::setGroup(const vector<string>& groups){

    if (groups.size() == 1 && groups[0] == "*__all__*") {
        this->group_ = groups;
        return;
    }

    for (const auto& group: groups) {

        if(find(this->variables_.begin(), this->variables_.end(), group) != this->variables_.end()) {
            continue;
            //this->group = group;
        } else if(group != "") {
            throw invalid_argument("Group by variable must be contained in the pattern.");
        }
    }

    this->group_ = groups;
}

vector<string> Pattern::getVariables(){
    return this->variables_;
}

void Pattern::filePatternToRegex(){

    replace(path_.begin(), path_.end(), '\\', '/');

    tuple vars = getRegex(this->file_pattern_, this->suppress_warnings_);

    this->regex_file_pattern_ = get<0>(vars);
    this->variables_ = get<1>(vars);
    this->named_groups_ = get<2>(vars);
}

tuple<string, vector<string>, vector<string>> Pattern::getRegex(string& pattern, bool suppressWarning){

    getNewNaming(pattern, suppressWarning);

    // regex to match variables
    std::string e_str = "(\\{(\\w+):([0\\.dcf+]+)\\})|(\\(\\?P<(\\w+)>(.+)\\))";
    std::regex e(e_str, regex_constants::ECMAScript); // check for bracket expressions or named groups

    std::string group_str = "\\?P<(\\w+)>(.+)";

    std::regex group(group_str, regex_constants::ECMAScript); // check for regex named groups

    std::string var_str = "\\{(\\w+):([0\\.dcf+]+)\\}";
    std::regex var(var_str, regex_constants::ECMAScript); // pattern style of groups (e.g {r:ddd})


    map<wchar_t, string> patternMap; // map of variable types to regex equivalent
    patternMap['d'] = "[0-9]"; // integer matchingß
    patternMap['c'] = "[a-zA-Z]"; // character matching
    patternMap['f'] = "[0-9\\.]"; // match any floating point
    patternMap['.'] = "\\."; // decimal for floating point patterns
    patternMap['0'] = "0"; // 0 for decimal patterns
    patternMap['+'] = "+"; // arbitrary length matching

    string str, rgx; // temp string and regex
    vector<pair<string,string>> matches; // map between bracket expression and regex
    vector<string> variables; // store variable names
    string patternCopy = pattern; // get a copy of pattern since regex_search is inplace
    std::smatch sm, m; // regex matches

    string temp;
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


    string regexFilePattern = pattern;
    vector<string> namedGroups;

    // Replace bracket groups with regex capture groups
    for(const auto& match: matches){

        namedGroups.push_back(match.first);

        // Create capture group
        str = "(" + match.second + ")";
        s::replace(regexFilePattern, match.first, str);
    }

    return make_tuple(regexFilePattern, variables, namedGroups);
}


Tuple Pattern::getVariableMapMultDir(const string& filePath, const smatch& sm){
    Tuple tup;

    bool matched = false;
    string basename;
    string file = s::getBaseName(filePath);
    // iterate over matched files, checking if filename already exists
    for(int i = 0; i < this->valid_files_.size(); i++){
        #ifdef JAVA_BINDING
        basename = s::getBaseName(s::to_string(get<1>(this->valid_files_[i])[0])); // store the basename
        #else
        basename = s::getBaseName(get<1>(this->valid_files_[i])[0].string()); // store the basename
        #endif
        // if the filename is found, add the filepath to the vector in the second member of the tuple
        if(basename == file){
            matched = true;
            get<1>(this->valid_files_[i]).push_back(filePath); // Add path to existing mapping
            break;
        }
    }

    // Create new mapping if not already present
    if(!matched){
        tup = getVariableMap(filePath, sm);
    }

    return tup;
}

Tuple Pattern::getVariableMap(const string& filePath, const smatch& sm){
    Tuple tup;
    // filename matches the pattern

    std::get<1>(tup).push_back(filePath);

    string str;
    // Extract capture groups from filename and store in mapping
    for(int i = 1; i < sm.size(); ++i){

        str = sm[i];

        // conserve variable type
        if (s::is_number(str)) {
            if (s::is_integer(str)) {
                get<0>(tup)[variables_[i-1]] = std::stoi(str);
            } else {
                get<0>(tup)[variables_[i-1]] = std::stod(str);
            }
        } else {
            get<0>(tup)[variables_[i-1]] = str;
        }

        this->variable_occurrences_[this->variables_[i-1]][get<0>(tup)[this->variables_[i-1]]] += 1; // update count of the variable occurrence
        this->unique_values_[this->variables_[i-1]].insert(get<0>(tup)[this->variables_[i-1]]); // update the unique values for the variable
    }

    return tup;
}

std::map<string, std::map<Types, int>> Pattern::getOccurrences(const vector<tuple<string, vector<Types>>>& mapping){
    // if no variables request, return all variables
    if(mapping.size() == 0){
        return this->variable_occurrences_;
    }

    std::map<Types, int> temp;
    std::map<string, std::map<Types, int>> occurrences;
    string variable;
    // loop over vector passed in that contains the variable mapped to value(s)
    for(const auto& tup: mapping){
        if(get<1>(tup).size() == 0){
            occurrences[get<0>(tup)] = this->variable_occurrences_[get<0>(tup)];
        } else {
            for(const auto& value: get<1>(tup)){
                variable = get<0>(tup);
                temp[value] = this->variable_occurrences_[get<0>(tup)][value];
            }
            occurrences[variable] = temp;
        }
    }

    return occurrences;
}

string Pattern::getPattern(){
    return this->file_pattern_;
}

void Pattern::setPattern(const string& pattern){
    this->file_pattern_ = pattern;
}

string Pattern::getRegexPattern(){
    return this->regex_file_pattern_;
}

void Pattern::printVariables(){
    int i = 0;
    cout << "The variables are: ";
    int size = this->getVariables().size();
    for(const auto& var: this->getVariables()){
        cout << var;
        if(i<size-1){
            cout << ", ";
        }
        i++;
    }
    cout << endl;
}

map<string, set<Types>> Pattern::getUniqueValues(const vector<string>& vec){
    if(vec.size() == 0) return this->unique_values_; // if no variables are passed, return all variables

    map<string, set<Types>> temp;
    // return variables that were requested
    for(const auto& str: vec){
        temp[str] = unique_values_[str];
    }
    return temp;
}


void Pattern::getNewNaming(string& pattern, bool suppressWarnings){

    if(pattern == "") return;

    string vars = "\\{([rtczyxp+]+)\\}"; // check for old naming style or named grouped
    std::regex e(vars);

    string str; // temp string

    vector<pair<string,string>> matches; // map between bracket expression and regex

    string patternCopy = pattern; // get a copy of pattern since regex_search is inplace

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
        cout << "WARNING: The old style of pattern was used. This style may become deprecated in future releases." << endl;
        cout << "The recommended pattern to use is: " << pattern <<
                ". See the documentation for details about the new style." << endl;
    }
}

void Pattern::replaceOutputName(Tuple& min, Tuple& max, const string& var, string& outputName, const int idx, string& temp, const regex& patternRegex){
        string file;
        smatch sm;
        if(get<0>(min)[var] == get<0>(max)[var]){

            #ifdef JAVA_BINDING
            file = s::getBaseName(get<1>(min)[0]); // get basename of filepath
            #else
            file = s::getBaseName((get<1>(min)[0]).string()); // get basename of filepath
            #endif
            regex_match(file, sm, patternRegex);

            s::replace(outputName, this->named_groups_[idx], sm[idx+1]);

        } else { // if min is different than max, put range in outputname

            temp = "(";

            #ifdef JAVA_BINDING
            file = s::getBaseName(get<1>(min)[0]); // get basename of filepath
            #else
            file = s::getBaseName((get<1>(min)[0]).string()); // get basename of filepath
            #endif
            regex_match(file, sm, patternRegex); // find variables

            temp += sm[idx+1];
            temp += "-";

            #ifdef JAVA_BINDING
            file = s::getBaseName(get<1>(max)[0]); // get basename of filepath
            #else
            file = s::getBaseName((get<1>(max)[0]).string()); // get basename of filepath
            #endif
            regex_match(file, sm, patternRegex);

            temp += sm[idx+1];
            temp += ")";

            s::replace(outputName, this->named_groups_[idx], temp);
        }
}

string Pattern::outputNameHelper(vector<Tuple>& vec){
    if(vec.size() == 0){
        vec = this->valid_files_;
    }

    string outputName = this->file_pattern_;

    int idx = 0;
    int min, max;
    smatch sm;
    string temp, file;

    regex patternRegex(this->regex_file_pattern_);

    for(auto& var: this->variables_){

        min = m::getMinIdx(vec, var);
        max = m::getMaxIdx(vec, var);

        // if min is the same as max, put variable value in output
        this->replaceOutputName(vec[min], vec[max], var, outputName, idx, temp, patternRegex);

        ++idx;
    }

    return outputName;

}

string Pattern::inferPatternInternal(vector<string>& files, string& variables, const string& startingPattern){
    
    variables += "rtczyxp"; // extra variables incase not enough were given

    string pattern;

    if(startingPattern == "") {
        pattern = files[0];
    } else {
        pattern = startingPattern;
    }

    string regexStr;
    string patternCpy = pattern;
    regex rgx = regex(get<0>(getRegex(patternCpy)));

    for(auto& file : files){

        if(!regex_match(file, rgx)) {

            pattern = swSearch(pattern, file, variables);

            patternCpy = pattern;

            regexStr = get<0>(getRegex(patternCpy));
            rgx = regex(regexStr);
        }
    }

    if (pattern == "") {
        throw std::runtime_error("Error: could not not infer pattern from input.");
    }

    return pattern;
}

string Pattern::swSearch(string& pattern, string& filename, const string& variables){
    string numbers = "0123456789<>"; // numeric values, '<' -> d and '>' -> d+
    string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ@$"; // alphabetic values, '@' -> c+ and '$' -> c
    smatch sm; // store regex group matches
    string rgxStr; // string version of rgx
    regex rgx;
    string temp, smStr;
    bool match;

    //vlist = [(0,0,"", "")]
    for (const auto& v: variables){

        // construct named group
        rgxStr = "\\{(";
        rgxStr += v;
        rgxStr += "):([dc+]+)\\}";

        rgx = regex(rgxStr); // regex to match
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
    map<string, map<string, int>> sab = {
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
    vector<vector<int>> matrix(m+1, vector<int>(n+1, 0));//[m+1][n+1]; // switch this to linear indexing in future
    //fill(matrix[0], matrix[0] + (m+1) * (n+1), 0);


    int p_idx = 0;
    int f_idx = 0;
    bool pIsNumeric, fIsNumeric;
    bool pIsCharacter, fIsCharacter;
    vector<int> scores;
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
    string patternTemplate = ""; //(1, filename[col-1]);
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
                    throw runtime_error("Non-numeric, non-alphabetic characters found that do not match");
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
    string vdef;

    // Construct a new filepattern
    for (auto i = sregex_iterator(patternTemplate.begin(), patternTemplate.end(), rgx); i != sregex_iterator(); ++i) {
        temp = (*i).str();

        if(temp.find('>') != string::npos){
            vdef = "{";
            vdef += variables[vi];
            vdef += ":d+}";
        } else if (temp.find('<') != string::npos){
            vdef = "{";
            vdef += variables[vi];
            vdef += ":";
            vdef.insert(vdef.length(), temp.length(), 'd');
            vdef += "}";
        } else if (temp.find('@') != string::npos) {
            vdef = "{";
            vdef += variables[vi];
            vdef += ":c+}";
        } else if (temp.find('$') != string::npos){
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

vector<string> Pattern::getTmpDirs(){
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
