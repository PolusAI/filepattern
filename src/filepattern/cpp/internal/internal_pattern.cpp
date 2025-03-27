#include "internal_pattern.hpp"
#include "../util/alphanum.hpp"

void InternalPattern::next() {}

void InternalPattern::nextGroup() {}

void InternalPattern::groupByHelper(const std::vector<std::string>& groups){

    std::vector<std::pair<std::vector<std::pair<std::string, Types>> , std::vector<Tuple>>> temp;
    std::vector<Tuple> temp_vec;
    std::vector<std::pair<std::string, Types>> grouped_variables;

    for(const auto& group_by: groups){

        for(auto& vec: this->valid_grouped_files_){
            
            grouped_variables.clear();

            for(auto& g: vec.first) grouped_variables.push_back(g);

            // Sort the matched files by the group_by parameter
            if (isSorted()) {
                sort(vec.second.begin(), vec.second.end(), [&group_by = std::as_const(group_by)](Tuple& p1, Tuple& p2){
                    return std::get<0>(p1)[group_by] < std::get<0>(p2)[group_by];
                });
            }

            Types current_value = std::get<0>(vec.second[0])[group_by]; // get the value of variable
            std::vector<Tuple> empty_vec;
            unsigned int i = 0;
            int group_ptr = 0;

            //group files into vectors based on group_by variable
            while(i < vec.second.size()){


                while(std::get<0>(vec.second[i])[group_by] == current_value) {

                    temp_vec.push_back(vec.second[i]);

                    ++i;
                    if (i >= vec.second.size()) break;
                }

                grouped_variables.push_back(make_pair(group_by, current_value));
                temp.push_back(make_pair(grouped_variables, temp_vec));
                sort(temp[group_ptr].second.begin(), temp[group_ptr].second.end(), [](Tuple& m1, Tuple& m2){
                    return std::get<1>(m1)[0] < std::get<1>(m2)[0];
                });
                temp_vec.clear();

                if (i < vec.second.size()){
                     current_value = std::get<0>(vec.second[i])[group_by];
                     grouped_variables.pop_back();
                }
                ++group_ptr;
            }

        }
        this->valid_grouped_files_ = temp;

        temp.clear();
    }
}


void InternalPattern::groupBy(std::vector<std::string>& groups) {

    // Cannot group empty files so return
    if (valid_files_.size() == 0) return;


    std::vector<std::pair<std::string, Types>> grouped_variables;
    this->setGroup(groups);
    this->valid_grouped_files_.clear();
    Tuple member;

    if(groups.size() == 1 && groups[0] == "*__all__*") {

        std::vector<std::pair<std::string, Types>> empty;
        this->valid_grouped_files_.push_back(make_pair(empty, this->valid_files_));
        return;
    }

    std::string group_by = groups[0];
    // Sort the matched files by the group_by parameter
    if (isSorted()) {
        sort(this->valid_files_.begin(), this->valid_files_.end(), [&group_by = as_const(group_by)](Tuple& p1, Tuple& p2){
            return std::get<0>(p1)[group_by] < std::get<0>(p2)[group_by];
        });
    }

    Types current_value = std::get<0>(this->valid_files_[0])[group_by]; // get the value of variable

    std::vector<Tuple> empty_vec;
    unsigned int i = 0;
    int group_ptr = 0;

    //group files into vectors based on group_by variable
    while(i < this->valid_files_.size()){
        //this->validGroupedFiles.push_back(empty_vec);
        grouped_variables.clear();
        grouped_variables.push_back(make_pair(group_by, current_value));
        this->valid_grouped_files_.push_back(make_pair(grouped_variables, empty_vec));
        while(std::get<0>(this->valid_files_[i])[group_by] == current_value) {
            this->valid_grouped_files_[group_ptr].second.push_back(this->valid_files_[i]);

            // sort group of variables
            if (isSorted()) {
                sort(this->valid_grouped_files_[group_ptr].second.begin(), this->valid_grouped_files_[group_ptr].second.end(), [](Tuple& m1, Tuple& m2){
                    return std::get<1>(m1)[0] < std::get<1>(m2)[0];
                });
            }

            ++i;
            if (i >= this->valid_files_.size()) break;
        }

        if (i < this->valid_files_.size()) current_value = std::get<0>(this->valid_files_[i])[group_by];
        ++group_ptr;
    }

    groups.erase(groups.begin());
    this->groupByHelper(groups);

}

std::vector<Tuple> InternalPattern::getMatchingBlock() {
    std::vector<Tuple> vec;
    return vec;
}

void InternalPattern::getMatchingLoop(std::vector<Tuple>& iter,
                                      const std::string& variable,
                                      const std::vector<Types>& values,
                                      Types& temp){
    for(auto& file: iter){
        temp = std::get<0>(file)[variable];
        for(const auto& value: values){
            if(temp == value){
                this->matching_.push_back(file);
            }
        }
    }
}

void InternalPattern::getMatchingHelper(const std::tuple<std::string, std::vector<Types>>& variableMap){
    std::string variable = std::get<0>(variableMap); // get key from argument
    std::vector<Types> values = std::get<1>(variableMap); // get value from argument

    // throw error if argument variable is not in the pattern
    if(find(begin(this->variables_), end(this->variables_), variable) == end(this->variables_)) {
        throw std::invalid_argument("\"" + variable + "\" is not a variable. Use a variable that is contained in the pattern.");
    }

    Types temp;
    std::vector<Tuple> iter;
    // if first or only variable to match, iterate over valid files
    if(this->matching_.size() == 0) {
        this->getMatchingLoop(this->valid_files_, variable, values, temp);
    } else { // iterate files that matched previous call
        iter = this->matching_;
        this->matching_.erase(this->matching_.begin(), this->matching_.end());
        this->getMatchingLoop(iter, variable, values, temp);
    }
}

std::vector<Tuple> InternalPattern::getMatching(const std::vector<std::tuple<std::string, std::vector<Types>>>& variables){

    // clear the vector that stores matching files
    this->matching_.erase(this->matching_.begin(), this->matching_.end());

    // match files for each argument
    for(const auto& variableMap: variables){
        this->getMatchingHelper(variableMap);
    }

    return this->matching_;
}

std::string InternalPattern::outputName(std::vector<Tuple>& vec){
    return this->outputNameHelper(vec);
}

std::string InternalPattern::inferPattern(const std::string& path, std::string& variables, const std::string& block_size){
    std::vector<std::string> vec;

    if (!fs::exists(path)) {
        throw std::invalid_argument("Path \"" + path + "\" does not exist.");
    }

    if(s::endsWith(path, ".txt")){

        std::ifstream infile(path);

        std::string str;
        while(std::getline(infile, str)) {
            vec.push_back(s::escape_regex_characters(str));
        }

    } else {

        fs::directory_iterator iterator = fs::directory_iterator(path);

        for(auto& file: iterator){
            vec.push_back(s::escape_regex_characters(s::getBaseName(file.path().string())));
        }
    }

    return inferPatternInternal(vec, variables);
}

std::string InternalPattern::inferPattern(std::vector<std::string>& vec, std::string& variables){
    for (auto& file: vec) {
        file = s::escape_regex_characters(file);
    }

    return inferPatternInternal(vec, variables);
}

void InternalPattern::sortFiles(){

    if (this->valid_files_.size() == 0) return;

    doj::alphanum_less<std::string> comparator; // alphanum comparison algorithm for strings with numeric and alphabetic chars
    sort(this->valid_files_.begin(), this->valid_files_.end(), [comparator](Tuple& m1, Tuple& m2){

        #ifdef JAVA_BINDING
            return comparator(std::get<1>(m1)[0], std::get<1>(m2)[0]);
        #else
            return comparator(std::get<1>(m1)[0].u8string(), std::get<1>(m2)[0].u8string());
        #endif
    });
}

Tuple InternalPattern::getItem(unsigned int key){
    if(key < 0) {
        if(this->valid_files_.size() + key < 0) throw std::out_of_range("Index " + std::to_string(key) + " is out of range.");
        return this->valid_files_[this->valid_files_.size()+key];
    }

    if(key >= this->valid_files_.size()) throw std::out_of_range("Index " + std::to_string(key) + " is out of range.");

    return this->valid_files_[key];
}

std::vector<Tuple> InternalPattern::getItemList(std::vector<int>& key){

    std::vector<Tuple> vec;

    int valid_files_size = this->valid_files_.size();

    for(const auto& index: key){
        if(index < 0) {
            if(valid_files_size + index < 0) throw std::out_of_range("Index " + std::to_string(index) + " is out of range.");
            vec.push_back(this->valid_files_[valid_files_size+index]);
        } else {
            if(index > valid_files_size) throw std::invalid_argument("Index " + std::to_string(index) + " is out of range.");
            vec.push_back(this->valid_files_[index]);
        }
    }

    return vec;
}

std::vector<Tuple> InternalPattern::getSlice(std::vector<Types>& key){

    std::string key0 = s::to_string(key[0]);
    std::string key1 = s::to_string(key[1]);
    std::string key2 = s::to_string(key[2]);

    int valid_files_size = this->valid_files_.size();

    if(s::is_number(key0) && key1 == "None"  && key2 == "None"){
        int i = stoi(key0);
        if(i >= valid_files_size) throw std::out_of_range("Index " + std::to_string(i) + " is out of range.");

        int j = valid_files_size;
        int step =  1;

        return v::sliceVector(this->valid_files_, i, j, step);
    }

    // A start and stop index is provided with no step size, i.e. validFiles[i:j]
    if(s::is_number(key0) && s::is_number(key1)  && key2 == "None"){
        int i =  stoi(key0);
        int j = stoi(key1);

        if(i > valid_files_size) throw std::out_of_range("Index " + std::to_string(i) + " is out of range.");
        if(j > valid_files_size) throw std::out_of_range("Index " + std::to_string(j) + " is out of range.");
        if(j >= 0 && i > j) throw std::out_of_range("Invalid range.");

        if(j < 0) j += valid_files_size + 1;

        return v::sliceVector(this->valid_files_, i, j, 1);
    }

    // A start, stop, and step is provided
    if(s::is_number(key0) && s::is_number(key1)  && s::is_number(key2)){
        int i = stoi(key0);
        int j = stoi(key1);

        if(i > valid_files_size) throw std::out_of_range("Index " + std::to_string(i) + " is out of range.");
        if(j > valid_files_size) throw std::out_of_range("Index " + std::to_string(j) + " is out of range.");

        int step =  stoi(key2);
        return v::sliceVector(this->valid_files_, i, j, step);
    }

    if(s::is_number(key0) && key1 == "None" && s::is_number(key2)){
        int i = stoi(key0);
        if(i > valid_files_size) throw std::out_of_range("Index " + std::to_string(i) + " is out of range.");

        int j = valid_files_size;
        int step =  stoi(key2);

        return v::sliceVector(this->valid_files_, i, j, step);
    }

    std::vector<Tuple> empty;
    return empty;

}

int InternalPattern::currentBlockLength() {return -1;}
