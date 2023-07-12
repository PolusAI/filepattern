#include "sort.hpp"

using namespace std;

ExternalMergeSort::ExternalMergeSort(const Structure& structure, 
                                     const string& input_file_name, 
                                     const string& output_file_name, 
                                     const string& block_size, 
                                     const string& variable="",
                                     const int map_size=0) {

    this->input_file_name_ = input_file_name;
    this->output_file_name_ = output_file_name;
    this->sort_variable_ = variable;
    this->block_size_str_ = block_size;
    this->block_size_ = Block::parseblockSize(block_size);
    this->map_size_ = map_size;
    this->tmpdir_ = fs::temp_directory_path().string(); // Find temporary directory

    this->tmpdir_ += "/extern_sort_tmp_" + s::getTimeString() + "/"; // temp directory
    // remove files from directory if already exits
    if(fs::exists(tmpdir_)){ 
        uintmax_t n = fs::remove_all(tmpdir_); 
    }
    // Create temp directory
    bool created = fs::create_directory(tmpdir_);
    if(!created) {
            throw invalid_argument("Error creating tmp directory");
    }

    // Call function based on data type
    if(structure == std_map){
        this->sortMapFile();
    } else {
        this->sortFile();
    }

    uintmax_t n2 = fs::remove_all(tmpdir_); // Remove temp directory 
}


long ExternalMergeSort::currentSize(const int string_size, 
                                      const long& previous_size){
    return sizeof(string) + string_size + previous_size; // update amount of memory being used
}
    
void ExternalMergeSort::sortMapFile(){

    int vector_capacity = 0;
    int string_size = sizeof(string);

    int block_num = 0;
    string block_name;

    ifstream infile(this->input_file_name_);
    vector<Tuple> vec;

    // Get blocks of files and sort while not end of file
    while (true) {
        getMapBlock(infile, vec); 
        if(vec.size() == 0){
            break;
        }
        // sort block
        if(this->sort_variable_ == ""){
            sort(vec.begin(), vec.end(), [](Tuple& a, Tuple& b) {
                return get<1>(a)[0] < get<1>(b)[0];
            });
        } else {
            sort(vec.begin(), vec.end(), 
                [&sort_variable_ = as_const(sort_variable_)](Tuple& a, Tuple& b) {
                    return get<0>(a)[sort_variable_] < get<0>(b)[sort_variable_];
            });
        }

        //write to file
        block_name = this->tmpdir_ + to_string(block_num) + ".txt";;
        ofstream out(block_name, ios_base::app);
        this->writeMapTmpFile(out, vec);

        this->files_to_merge_.push_back(block_name);
        //clear vec and continue reading file   
        vec.clear();
        block_num++;
    }

    //merge sorted files 
    this->mergeMaps();
}

/*This function could be made cleaner by writing each map to a single line in the text file*/
bool ExternalMergeSort::getMapBlock(ifstream& infile, vector<Tuple>& vec){

    Tuple member;
    
    long size = sizeof(vector<Tuple>);

    Map map;
    string str;
    string key, value;
    int value_length;
    size_t pos;
    Types result;
    map = this->temp_map_;
    // while the memory consumption is under block_size and infile still has a line
    while(size < this->block_size_ && infile >> str){
        
        // if map is full, add to block
        if (map.size() == (this->map_size_)) {
            size += sizeof(map) + sizeof(vector<string>);
            
            for(const auto& item : map){
                size += item.first.length() + s::size(item.second);
            }
            get<0>(member) = map;
            str.pop_back(); // remove trailing comma
            get<1>(member).push_back(str);
            vec.push_back(member);
            map.clear();
            get<1>(member).clear();
            infile >> str;
        }
        // split line on : (line is in the form "key:value")
        pos = str.find(":");
        key = str.substr(0, pos);
        value_length = str.length() - pos;
        value = str.substr(pos+1, value_length);

        // Conserve variable type (string or int)
        if(s::is_number(value)){
            result = stoi(value);
        } else {
            result = value;
        }

        map[key] = result; // add key value pair to map
        size += value_length + pos;
    }

    this->temp_map_ = map;

    // check if end of file
    streampos ptr = infile.tellg();
    if(!(infile >> str)){
        return false;
    } else {
        infile.seekg(ptr, ios::beg);
        return true;
    }
    
}

void ExternalMergeSort::sortFile(){

    string str; 
    int block_num = 0;
    string block_name;
    double previous_size = sizeof(vector<string>);

    ifstream file(this->input_file_name_);
    if(!file.is_open()){ 
        throw invalid_argument("Cannot find file \"" + this->input_file_name_ + "\".");
    }

    // while the text file still has files:
    //      read in a block of files and sort and then write to output file
    while (getline(file, str)) {

        this->vec_.push_back(str); // add filename to block
        previous_size = currentSize(str.length(), previous_size); // update amount of memory used

        // check if the amount of memory is below the specified amount and end of file hasn't been reached
        while(previous_size < this->block_size_ && !file.eof()){
            // add more filenames to block
            getline(file,str);
            this->vec_.push_back(str);
            previous_size = currentSize(str.length(), previous_size);
        } 

        // sort the block
        sort(this->vec_.begin(), this->vec_.end(), 
            [](const string& a, const string& b) {return a < b;});

        //write to file
        block_name = to_string(block_num) + ".txt";;
        this->writeTmpFile(block_name);

        // add txt file name to vector to be merged 
        this->files_to_merge_.push_back(block_name);

        //clear vec and continue reading file   
        this->vec_.clear();
        previous_size = sizeof(vector<string>);
        block_num++;
    }

    //merge sorted files 
    this->merge();
}

void ExternalMergeSort::writeTmpFile(string& output){
    output = this->tmpdir_ + output;
    ofstream out(output);
    for (const auto& element: this->vec_) out << element << "\n";
}

void ExternalMergeSort::writeMapTmpFile(ofstream& file, 
                                        const vector<Tuple>& vec){
    
    //loop over map, writing the key value pair in the form "key:value\n"
    for(const auto& mapping: vec){
        for(const auto& element: get<0>(mapping)){
            file << element.first << ":" << s::to_string(element.second) << '\n';
        }

        for(const auto& element: get<1>(mapping)){
            #ifdef JAVA_BINDING
            file << element << "," << '\n';
            #else
            file << element.string() << "," << '\n';
            #endif
        } 
    }
}

void ExternalMergeSort::writeFile(const string& output){
    // write string to output file
    ofstream out(output);
    for (const auto& element: this->vec_) out << element << "\n";
}

void ExternalMergeSort::merge(){

    string str1, str2;
    string out_file;
    int length;
    int count;
    int iter = 0;

    while(this->files_to_merge_.size() > 1){

        length = files_to_merge_.size();
        for(int i = 0; i < (length - (length%2)); i+=2){

            ifstream file1(files_to_merge_[i]);
            ifstream file2(files_to_merge_[i+1]);

            if(!file1.is_open()) {
                string error = "Cannot open file " + files_to_merge_[i];
                throw runtime_error(error);
            }

            if(!file2.is_open()) {
                string error = "Cannot open file " + files_to_merge_[i+1];
                throw runtime_error(error);
            }

            out_file = this->tmpdir_ + to_string(iter) + "_" + to_string(i) + "_" + to_string(i+1) + ".txt"; 
            this->new_files_.push_back(out_file);
            ofstream outfile(out_file, ios_base::app);

            count = 0;
            while(true){
                if(count == 0) {
                    getline(file1, str1); 
                    getline(file2, str2);
                    count++;
                }

                // if file1 empty
                if(file1.eof()){
                    if(file2.eof()){
                        file2.close();
                        outfile.close();
                        break;
                    }
                    // write the rest of file2
                    outfile << str2 << '\n';
                    while(getline(file2, str2)) {
                        if (str2 != " " && str2 != ""){ 
                            outfile << str2 << '\n';
                        };
                    }

                    file1.close();
                    file2.close();
                    outfile.close();
                    break;

                } else if(file2.eof()){

                    // write the rest of file1
                    outfile << str1 << '\n';
                    while(getline(file1, str1)) {
                        if (str1 != " " && str1 != ""){ 
                            outfile << str1 << '\n';
                        }
                    }

                    file1.close();
                    file2.close();
                    outfile.close();
                    break; 

                } else if(str1 < str2){
                    //write str1 to output
                    if (str1 != " " && str1 != "") {
                        outfile << str1 << '\n';
                    }
                    getline(file1, str1); 
                } else {
                    if (str2 != " " && str2 != ""){
                        outfile << str2 << '\n';
                    }
                    getline(file2, str2);
                    //write str2 to output
                }
            }
            outfile.close();
            
        }
        iter++;
        files_to_merge_ = new_files_;
        new_files_.clear();
    }

    ifstream file(this->files_to_merge_[0]);
    if(!file.is_open()){
        string error = "Cannot open file " + files_to_merge_[0];
        throw runtime_error(error);
    }

    this->vec_.clear();

    while (getline(file, str1)) {
        vec_.push_back(str1);
    }
    writeFile(this->output_file_name_);

}   

void ExternalMergeSort::mergeMaps(){

    string out_file;
    int length;
    int iter = 0;
   
    while(this->files_to_merge_.size() > 1){

        length = files_to_merge_.size();
        for(int i = 0; i < (length - (length%2)); i+=2){
            out_file = this->tmpdir_ + to_string(iter) + "_" + to_string(i) + "_" + to_string(i+1) + ".txt";
            this->twoWayMergeMaps(files_to_merge_[i], files_to_merge_[i+1], out_file);
            this->new_files_.push_back(out_file);
        }

        if(length%2 == 1){
            out_file = out_file = this->tmpdir_ + to_string(iter) + "_" + to_string(length-1) + "_" + to_string(length-1) + ".txt";
            this->twoWayMergeMaps(files_to_merge_[files_to_merge_.size()-1], new_files_[0], out_file);
            new_files_[0] = out_file;
        }

        iter++;
        files_to_merge_ = new_files_;
        new_files_.clear();
    }
    
    ifstream file(files_to_merge_[0]);
    if(!file.is_open()){
        string error = "Cannot open file " + files_to_merge_[0];
        throw runtime_error(error);
    }

    Tuple map1, map2;
    ofstream outfile(this->output_file_name_);
    while(m::getMap(file, map1, this->map_size_)){
        m::writeMap(outfile, map1);
        get<1>(map1).clear();
    }
    file.close();
    outfile.close();
}  

void ExternalMergeSort::twoWayMergeMaps(const string& fileName1, const string& fileName2, const string& out_file){

    ifstream file1(fileName1);
    ifstream file2(fileName2);

    if(!file1.is_open()) {
        string error = "Cannot open file " + fileName1;
        throw runtime_error(error);
    }

    if(!file2.is_open()) {
        string error = "Cannot open file " + fileName2;
        throw runtime_error(error);
    }
 
    ofstream outfile(out_file);

    int count = 0;
    Tuple map1, map2;
    while(true){
        // if first iteration, read in a map from both files
        if(count == 0) {
            m::getMap(file1, map1, this->map_size_); 
            m::getMap(file2, map2, this->map_size_);
            count++;
        }

        // if file1 empty
        if(file1.eof()){
            // if file2 is also empty
            if(file2.eof()){
                //close files and break
                file1.close();
                file2.close();
                outfile.close();
                break;
            }

            // write the rest of file2
            m::writeMap(outfile, map2);
            while(m::getMap(file2, map2, this->map_size_)) {
                m::writeMap(outfile, map2);
            }

            //close files
            file1.close();
            file2.close();
            outfile.close();
            break;

        } else if(file2.eof()){
            // write the rest of file1
            m::writeMap(outfile, map2);
            while(m::getMap(file1, map1, this->map_size_)) {
                m::writeMap(outfile, map1);
            }

            //close files
            file1.close();
            file2.close();
            outfile.close();
            break; 

        } else if(this->sort_variable_ == ""){
            if(get<1>(map1)[0] <= get<1>(map2)[0]) {
                            //write str1 to output
                m::writeMap(outfile, map1);
                m::getMap(file1, map1, this->map_size_); 
            } else {
                //write str2 to output
                m::writeMap(outfile, map2);
                m::getMap(file2, map2, this->map_size_);
            }
        } else if(get<0>(map1)[this->sort_variable_] <= get<0>(map2)[this->sort_variable_]){
            //write str1 to output
            m::writeMap(outfile, map1);
            m::getMap(file1, map1, this->map_size_); 
        } else {
            //write str2 to output
            m::writeMap(outfile, map2);
            m::getMap(file2, map2, this->map_size_);
        }
    }
    outfile.close();
}