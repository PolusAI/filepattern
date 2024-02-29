#include "external_pattern.hpp"

using namespace std;

ExternalPattern::ExternalPattern(const string& path, const string& block_size, bool recursive):
stream_(FilesystemStream(path, recursive, block_size)){
    this->valid_files_path_ = this->stream_.getValidFilesPath(); // Store path to valid files txt file
    this->tmp_directories_.push_back(this->valid_files_path_);
    this->infile_.open(this->valid_files_path_); // open temp file for the valid files
    this->group_stream_.open(this->stream_.getValidFilesPath());
    matching_init_ = false;
}

ExternalPattern::~ExternalPattern() {
    this->infile_.close();
    this->group_stream_.close();
}

void ExternalPattern::getMatchingLoop(ifstream& infile,
                                      ofstream& outfile,
                                      const string& variable,
                                      const vector<Types>& values,
                                      Types& temp,
                                      Tuple& temp_map){
    // while infile still has a map:
    //   read map and add to outfile if value matches
    while(m::getMap(infile, temp_map, this->map_size_)){
        temp = get<0>(temp_map)[variable];
        for(const auto& value: values){

            if(s::to_string(temp) == s::to_string(value)){
                m::writeMap(outfile, temp_map);
            }
        }
    }

}

void ExternalPattern::getMatchingHelper(const tuple<string, vector<Types>>& variable_map, const string& matching){
    string variable = get<0>(variable_map); // get key from argument
    vector<Types> values = get<1>(variable_map); // get value from argument

    // throw error if argument variable is not in the pattern
    if(find(begin(this->variables_), end(this->variables_), variable) == end(this->variables_)) {
        throw invalid_argument("\"" + variable + "\" is not a variable. Use a variable that is contained in the pattern.");
    }

    Types temp;
    vector<Tuple> iter;
    Tuple temp_map;
    // if first or only variable to match, iterate over valid files
    if(!(fs::exists(matching))) {

        ifstream valid_files(this->valid_files_path_);
        ofstream outfile(matching);

        this->getMatchingLoop(valid_files, outfile, variable, values, temp, temp_map);

        valid_files.close();
        outfile.close();

    } else { // iterate files that matched previous call

        // copy contents
        std::ifstream infile(matching);
        std::ofstream outfile(this->matching_copy_);
        outfile << infile.rdbuf();
        infile.close();
        outfile.close();


        // clear contents of matching.txt
        std::ofstream ofs;
        ofs.open(matching, std::ofstream::out | std::ofstream::trunc);
        ofs.close();


        ifstream in(this->matching_copy_);
        ofstream out(matching);
        this->getMatchingLoop(in, out, variable, values, temp, temp_map);
    }
}

void ExternalPattern::getMatchingInit(const vector<tuple<string, vector<Types>>>& variables) {
    // construct temporary directory path
    this->fp_tmpdir_ = fs::temp_directory_path().string();
    if (s::endsWith(this->fp_tmpdir_, "\\")) this->fp_tmpdir_.pop_back();
    this->fp_tmpdir_ += SLASH + "filepattern_" + s::getTimeString() + SLASH;

    this->tmp_directories_.push_back(this->fp_tmpdir_);

    // remove old contents if it already exists
    if(fs::exists(this->fp_tmpdir_)){
        fs::remove_all(this->fp_tmpdir_);
    }

    bool created = fs::create_directory(this->fp_tmpdir_);

    if (!created) {
        std::cerr << "WARNING: temporary directory " << this->fp_tmpdir_ << " could not be created.";
    }

    fs::permissions(this->fp_tmpdir_, fs::perms::all);

    // create a path to store matching files
    this->matching_ = this->fp_tmpdir_ + "matching.txt";
    this->matching_copy_ = this->fp_tmpdir_ + SLASH + "temp.txt";
    if(fs::exists(this->matching_)) {

        fs::remove(this->matching_);
    }

    // Iterate over each variable passed from front end
    for(const auto& variable_map: variables){
        this->getMatchingHelper(variable_map, this->matching_);
    }

    this->matching_stream_.open(this->matching_);

    this->matching_init_ = true;
}

vector<Tuple> ExternalPattern::getMatching(const vector<tuple<string, vector<Types>>>& variables){

    if (!matching_init_) {
        this->getMatchingInit(variables);
        vector<Tuple> vec;
        return vec;
    }

    return this->getMatchingBlock();
}

vector<Tuple> ExternalPattern::getMatchingBlock(){

    long size = sizeof(vector<Tuple>); // store amount of memory begin used by this method
    // throw error if block_size is too small to fit the empty vector
    if(size > this->block_size_) throw runtime_error("The block size is smaller than the size of a vector. The block size must be increased");

    Tuple temp;
    vector<Tuple> vec;
    bool more_files;

    // If there are no more files to return, return an empty vector
    if(!this->matching_stream_.is_open()) return vec;

    // while memory is still available:
    //      get file from matching.txt in tmpdir and add to return vec
    while(size < this->block_size_){
        more_files = m::getMap(this->matching_stream_, temp, this->map_size_);

        m::preserveType(temp);

        if(!more_files) {
            this->matching_stream_.close();
            this->matching_init_ = false;
            break;
        }
        vec.push_back(temp);

        size += m::getSize(temp);
    }

    return vec;
}


void ExternalPattern::groupByHelper(){

    std::vector<std::pair<std::vector<std::pair<std::string, Types>> , std::vector<Tuple>>> temp_group;

    vector<Tuple> temp_vec;
    vector<std::pair<std::string, Types>> grouped_variables;
    string group_by;
    //for(const auto& group_by: this->group){

    for(int j = 1; j < this->group_.size(); ++j){
        group_by = this->group_[j];

        for(auto& vec: this->current_group_){


            if(vec.second.size() == 0) return;

            grouped_variables.clear();
            for(auto& g: vec.first) grouped_variables.push_back(g);
            // Sort the matched files by the group_by parameter
            sort(vec.second.begin(), vec.second.end(), [&group_by = as_const(group_by)](Tuple& p1, Tuple& p2){
                return get<0>(p1)[group_by] < get<0>(p2)[group_by];
            });

            Types current_value = get<0>(vec.second[0])[group_by]; // get the value of variable
            vector<Tuple> empty_vec;
            int i = 0;
            int group_ptr = 0;

            //group files into vectors based on group_by variable
            while(i < vec.second.size()){

                while(std::get<0>(vec.second[i])[group_by] == current_value) {

                    temp_vec.push_back(vec.second[i]);

                    ++i;
                    if (i >= vec.second.size()) break;
                }

                grouped_variables.push_back(make_pair(group_by, current_value));
                temp_group.push_back(make_pair(grouped_variables, temp_vec));
                sort(temp_group[group_ptr].second.begin(), temp_group[group_ptr].second.end(), [](Tuple& m1, Tuple& m2){
                    return get<1>(m1)[0] < get<1>(m2)[0];
                });
                temp_vec.clear();

                if (i < vec.second.size()){
                     current_value = get<0>(vec.second[i])[group_by];
                     grouped_variables.pop_back();
                }
                ++group_ptr;
            }

        }

        this->current_group_ = temp_group;

        temp_group.clear();
    }

}

void ExternalPattern::nextGroup(){

    if(this->first_call_) this->groupBy(this->group_);
    this->current_group_.clear();

    string str;
    vector<Tuple> empty;
    vector<std::pair<std::string, Types>> grouped_variables;

    this->current_group_.push_back(make_pair(grouped_variables, empty));


    if(get<0>(this->temp_).size() != 0) this->current_group_[0].second.push_back(this->temp_);

    // add mapping from previous call to return block
    if(!this->first_call_){
        this->current_group_[0].second.clear();
        if(get<0>(this->temp_).size() != 0) this->current_group_[0].second.push_back(this->temp_);
    }

    // check if end of file
    streampos ptr = this->group_stream_.tellg();

    if(!(this->group_stream_ >> str)){
        // reset variables in case of another call
        this->current_group_[0].second.clear();
        this->group_stream_.close();
        this->group_stream_.open(this->valid_files_path_);
        this->first_call_ = true;
        this->groupByHelper();
        return;
    }
    this->group_stream_.seekg(ptr, ios::beg);
    // iterate over valid files temp file while the group variable is constant

    bool value_added = false;
    while(m::getMap(this->group_stream_, this->temp_, this->map_size_)){

        m::preserveType(this->temp_);

        // if method has not been called, initialize data structures
        if(this->first_call_) {
            this->current_value_ = get<0>(this->temp_)[this->group_[0]];
            this->current_group_.resize(1);
            //grouped_variables.push_back(make_pair(group[0], current_value));
            this->current_group_[0] = make_pair(grouped_variables, empty);
            this->current_group_[0].second.push_back(this->temp_);
            this->first_call_ = false;
        } else {
            // add to block if value matches current value
            if(!value_added){
                grouped_variables.push_back(make_pair(this->group_[0], this->current_value_));
                this->current_group_[0].first = grouped_variables;
                value_added = true;
            }

            if(get<0>(this->temp_)[this->group_[0]] == this->current_value_) {

                    this->current_group_[0].second.push_back(this->temp_);
            } else {

                // update variable value and end loop on variable value change
                        // sort block by basename
                sort(this->current_group_[0].second.begin(), this->current_group_[0].second.end(), [](Tuple& m1, Tuple& m2){
                    return get<1>(m1)[0] < get<1>(m2)[0];
                });
                this->current_value_ = get<0>(this->temp_)[this->group_[0]];
                value_added = false;

                this->groupByHelper();
                return;
            };
        }
    }
    sort(this->current_group_[0].second.begin(), this->current_group_[0].second.end(), [](Tuple& m1, Tuple& m2){
        return get<1>(m1)[0] < get<1>(m2)[0];
    });
    this->groupByHelper();
}

void ExternalPattern::next(){
    this->current_block_ = this->getValidFilesBlock(); // get block of valid files
}

int ExternalPattern::currentBlockLength(){
    if(this->group_[0] == "") return this->current_block_.size();// + this->currentGroup.size();
    else return this->current_group_[0].second.size();
}

std::vector<Tuple> ExternalPattern::getValidFilesBlock(){
    // return an empty vector if no more files
    if(this->stream_.endOfValidFiles()){
        std::vector<Tuple> empty;
        return empty;
    }

    // return a vector with a memory footprint of at most block_size
    return this->stream_.getValidFilesBlock();

}

void ExternalPattern::groupBy(vector<string>& group_by) {
    this->setGroup(group_by);
    // sort valid files externally
    string path = this->stream_.getValidFilesPath();
    this->tmp_directories_.push_back(path);
    ExternalMergeSort sort = ExternalMergeSort(std_map,
                                               path,
                                               path,
                                               this->stream_.getBlockSizeStr(),
                                               group_by[0],
                                               this->stream_.map_size_);
}

string ExternalPattern::externalOutPutName(){
    string output_name = this->getFilePattern(); // store a copy of the filePattern to modify
    regex pattern_regex(this->getRegexFilePattern()); // regex version of filePattern

    std::ifstream infile;

    Tuple temp, min, max;
    int idx = 0;
    string tempStr;
    // iterate over every variable in mapping
    for(const auto& var: this->variables_){
        infile.open(this->valid_files_path_); // open matched files

        m::getMap(infile, temp, this->map_size_);
        min = temp;
        max = temp;

        // find min and max values
        while(m::getMap(infile, temp, this->map_size_)){
            if(get<0>(temp)[var] < get<0>(min)[var]) min = temp;
            if(get<0>(temp)[var] > get<0>(max)[var]) max = temp;
        }

        // update output name
        this->replaceOutputName(min, max, var, output_name, idx, tempStr, pattern_regex);
        ++idx;

        infile.close();
    }

    return output_name;
}

string ExternalPattern::outputName(vector<Tuple>& vec){
    // if vector is passed, process using in memory version
    if(vec.size() != 0) return this->outputNameHelper(vec);
    else return this->externalOutPutName(); // if no vector provided, iterate over all matched files

}

string ExternalPattern::inferPattern(vector<string>& vec, string& variables){
    auto pattern = inferPatternInternal(vec, variables);
    return s::escape_regex_characters(pattern);
}

string ExternalPattern::inferPattern(const string& path, string& variables, const string& block_size){

    if (!fs::exists(path)) {
        throw std::invalid_argument("Path \"" + path + "\" does not exist.");
    }

    FilesystemStream stream = FilesystemStream(path, true, block_size, true); // create a stream from directory

    vector<string> vec = stream.getBlock();
    for(auto& str: vec) str = s::getBaseName(str); // Get basename of each file
    string pattern = inferPatternInternal(vec, variables); // Get pattern from first block

    // while the stream is nonempty:
    //      process each block using the in memory version with the current pattern
    while(!stream.isEmpty()){
        vec = stream.getBlock();
        for(auto& str: vec) {
            replace(str.begin(), str.end(), '\\', '/');
        }

        pattern = inferPatternInternal(vec, variables, pattern);

    }

    fs::remove_all(stream.getTmpPath());
    return s::escape_regex_characters(pattern);
}

int ExternalPattern::getGroupLength(){
    return this->current_group_.size();
}

void ExternalPattern::sortFiles(){

}

Tuple ExternalPattern::getItem(int key){

    if(key < 0) {
        if(this->stream_.getValidFilesSize() + key < 0) throw out_of_range("Index " + std::to_string(key) + " is out of range.");
        return this->stream_.getFileByIndex(this->stream_.getValidFilesSize()+key);
    }

    if(key >= this->stream_.getValidFilesSize()) throw out_of_range("Index " + std::to_string(key) + " is out of range.");

    return this->stream_.getFileByIndex(key);
}

vector<Tuple> ExternalPattern::getItemList(vector<int>& key){

    vector<Tuple> vec;

    int valid_files_size = this->stream_.getValidFilesSize();

    for(const auto& index: key){
        if(index < 0) {
            if(valid_files_size + index < 0) throw out_of_range("Index " + std::to_string(index) + " is out of range.");
            vec.push_back(this->stream_.getFileByIndex(valid_files_size+index));
        } else {
            if(index > valid_files_size) throw out_of_range("Index " + std::to_string(index) + " is out of range.");
            vec.push_back(this->stream_.getFileByIndex(index));
        }
    }

    return vec;
}

vector<Tuple> ExternalPattern::getSlice(vector<Types>& key){

    string key0 = s::to_string(key[0]);
    string key1 = s::to_string(key[1]);
    string key2 = s::to_string(key[2]);

    if(s::is_number(key0) && key1 == "None"  && key2 == "None"){
        int i = stoi(key0);

        if(i >= this->stream_.getValidFilesSize()) throw out_of_range("Index " + std::to_string(i) + " is out of range.");
        int j = this->stream_.getValidFilesSize();
        int step =  1;
        return this->stream_.getValidFilesSlice(i, j, step);
    }

    // A start and stop index is provided with no step size, i.e. valid_files[i:j]
    if(s::is_number(key0) && s::is_number(key1)  && key2 == "None"){
        int i =  stoi(key0);
        int j = stoi(key1);

        if(i > this->stream_.getValidFilesSize()) throw out_of_range("Index " + std::to_string(i) + " is out of range.");
        if(j > this->stream_.getValidFilesSize()) throw out_of_range("Index " + std::to_string(j) + " is out of range.");
        if(j >= 0 && i > j) throw out_of_range("Invalid range.");

        if(j < 0) j += this->stream_.getValidFilesSize() + 1;

        return this->stream_.getValidFilesSlice(i, j, 1);
    }

    // A start, stop, and step is provided
    if(s::is_number(key0) && s::is_number(key1)  && s::is_number(key2)){
        int i = stoi(key0);
        int j = stoi(key1);

        if(i > this->stream_.getValidFilesSize()) throw out_of_range("Index " + std::to_string(i) + " is out of range.");
        if(j > this->stream_.getValidFilesSize()) throw out_of_range("Index " + std::to_string(j) + " is out of range.");

        int step =  stoi(key2);
        return this->stream_.getValidFilesSlice(i, j, step);
    }

    if(s::is_number(key0) && key1 == "None" && s::is_number(key2)){
        int i = stoi(key0);
        if(i > this->stream_.getValidFilesSize()) throw out_of_range("Index " + std::to_string(i) + " is out of range.");

        int j = this->stream_.getValidFilesSize();
        int step =  stoi(key2);
        return this->stream_.getValidFilesSlice(i, j, step);
    }

    vector<Tuple> empty;
    return empty;
}

long ExternalPattern::getBlockSize() {
    return this->block_size_;
}
std::string ExternalPattern::getMatching_(){
    return this->matching_;
}
std::string ExternalPattern::getMatchingCopy_(){
    return this->matching_copy_;
}

std::string ExternalPattern::getValidFilesPath(){
    return this->valid_files_path_;
}

int ExternalPattern::getMapSize(){
    return this->map_size_;
}

Types ExternalPattern::getCurrentValue(){
    return this->current_value_;
}

std::string ExternalPattern::getFpTmpdir(){
    return fp_tmpdir_;
}

bool ExternalPattern::getFirstCall(){
    return this->first_call_;
}

Tuple ExternalPattern::getTemp(){
    return this->temp_;
}

void ExternalPattern::setBlockSize(long block_size) {this->block_size_ = block_size;}

void ExternalPattern::setMatching(std::string matching) {this->matching_ = matching;}

void ExternalPattern::setMatchingCopy(std::string matching_copy){this-> matching_copy_ = matching_copy;}

void ExternalPattern::setValidFilesPath(std::string valid_files_path){this->valid_files_path_ = valid_files_path;}

void ExternalPattern::setMapSize(int map_size){this-> map_size_ = map_size;}

void ExternalPattern::setCurrentValue(Types current_value){this->current_value_ = current_value;}

void ExternalPattern::setFpTmpdir(std::string fp_tmpdir){this-> fp_tmpdir_ = fp_tmpdir;}

void ExternalPattern::setFirstCall(bool first_call){this->first_call_ = first_call;}

void ExternalPattern::setTemp(Tuple temp){this-> temp_ = temp;}
