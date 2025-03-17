#include "fs_stream.hpp"

FilesystemStream::FilesystemStream(const std::string& path, bool recursive, const std::string& block_size, const bool is_infer) {
    this->valid_files_size_ = 0;
    this->is_infer_ = is_infer; // Is a call from inferPattern (handles memory footprint calculation different if true)
    this->tmpdir_ = fs::temp_directory_path().string(); // temp directory to store txt files
    if (s::endsWith(this->tmpdir_, "\\")) this->tmpdir_.pop_back();
    this->tmpdir_ += SLASH + "fs_stream_tmp_" + s::getTimeString() + SLASH; // path to temp directory
    this->block_size_str_ = block_size; // string version of block_size
    this->block_size_ = Block::parseblockSize(block_size); // parse string to long

    this->empty_ = false; // no more files
    this->valid_files_ = this->tmpdir_ + "valid_files.txt"; // path to txt file to store matched files
    this->counter_ = 0;

    bool created = fs::create_directory(this->tmpdir_); // create temp directory
    //throw error if temp directory cannot be created
    if (!created) {
        throw std::runtime_error("Could not create temporary file.");
    }
    fs::permissions(this->tmpdir_, fs::perms::all);

    this->out_name_ = this->tmpdir_ + SLASH + "temp.txt";
    this->infile_.open(this->valid_files_);
    this->infile_.close();

    this->recursive_ = false;

    try {
        // handle text file or directory based on path name
        if (s::endsWith(path, ".txt")) {
            this->txt_input_ = true;
            this->inputfile_.open(path);
            if (!this->inputfile_.is_open()) {
                throw std::invalid_argument("Invalid path \"" + path + "\".");
            }
        }
        else {
            txt_input_ = false;
            this->recursive_directory_iterator_ = fs::recursive_directory_iterator(path);
            this->rec_end_ = fs::end(this->recursive_directory_iterator_);
            this->directory_iterator_ = fs::directory_iterator(path); // store iterator for target directory
            this->end_ = fs::end(this->directory_iterator_);
        }
    }
    catch (const std::runtime_error& e) {
        std::string error = "No directory found. Invalid path \"" + path + "\".";
        throw std::runtime_error(error);
    }
}

FilesystemStream::~FilesystemStream() {
    this->infile_.close();
    d::remove_dir(this->tmpdir_);
}

std::vector<std::string> FilesystemStream::getBlock(){
    if(this->txt_input_){
        return this->getBlockTxt();
    } else {
        return this->getBlockIterator();
    }
}

void FilesystemStream::updateSize(long& size, const std::string& current){
    if(this->is_infer_){
        size +=  sizeof(std::vector<std::vector<int>>) + current.length()*sizeof(std::vector<int>) + 2*current.length()*sizeof(int);
    }
    if(size > this->block_size_) throw std::runtime_error("The block size is too small. Block size must be increased.");
}


std::vector<std::string> FilesystemStream::getBlockIterator(){

    std::vector<std::string> vec; // files to return
    long previous_size = sizeof(std::vector<std::string>); //memory used

    std::string current;

    if(this->recursive_){

        try {
            current = (*this->recursive_directory_iterator_).path().string();
        } catch (std::exception& e){
            std::cout << e.what() << std::endl;
        }

        this->updateSize(previous_size, current);

        while(this->currentSize(current.length(), previous_size) < this->block_size_){
            vec.push_back(current);
            ++this->valid_files_size_;
            ++this->recursive_directory_iterator_;

            if(fs::begin(this->recursive_directory_iterator_) == this->rec_end_){
                this->empty_ = true;
                break;
            } else {
                previous_size = this->currentSize(current.length(), previous_size);

                try{
                    current = (*this->recursive_directory_iterator_).path().string();
                } catch (std::exception& e){
                    std::cout << e.what() << std::endl;
                }
            }
        }
    } else{
        try {

            current = (*this->directory_iterator_).path().string();
        } catch (std::exception& e){
            std::cout << e.what() << std::endl;
        }

        this->updateSize(previous_size, current);

        while(this->currentSize(current.length(), previous_size) < this->block_size_){
            vec.push_back(current);
            ++this->valid_files_size_;
            ++this->directory_iterator_;

            if(fs::begin(this->directory_iterator_) == this->end_){
                this->empty_ = true;
                break;
            } else {
                previous_size = this->currentSize(current.length(), previous_size);

                try{
                    current = (*this->directory_iterator_).path().string();
                } catch (std::exception& e){
                    std::cout << e.what() << std::endl;
                }
            }
        }

    }

    return vec;
}

std::vector<std::string> FilesystemStream::getBlockTxt(){

    std::vector<std::string> vec;
    long size = sizeof(std::vector<std::string>);
    std::string str;
    // get string while less than block size
    this->inputfile_ >> str;
    this->updateSize(size, str);
    vec.push_back(str);
    ++this->valid_files_size_;
    size = this->currentSize(str.length(), size);

    while(size < this->block_size_ && this->inputfile_ >> str){
        size = this->currentSize(str.length(), size);
        ++this->valid_files_size_;
        vec.push_back(str);
    }

    //check if end of file
    std::streampos ptr = this->inputfile_.tellg();
    if(!(this->inputfile_ >> str)){
        this->empty_ = true;
    }
    this->inputfile_.seekg(ptr, std::ios::beg);

    return vec;
}

std::string FilesystemStream::getTmpPath(){
    return this->tmpdir_;
}


void FilesystemStream::writeBlock(const std::vector<std::string>& vec){
    std::ofstream file(this->out_name_, std::ios_base::app);

    for(const auto& element: vec){
        file << '\n' << element;
    }
    file.close();
}

void FilesystemStream::writeValidFiles(const Tuple& mapping){
    this->counter_++;
    std::ofstream file(this->valid_files_, std::ios_base::app);

    for(const auto& element: std::get<0>(mapping)){
        file << element.first << ":" << s::to_string(element.second) << '\n';
    }

    for(const auto& element: std::get<1>(mapping)){
        #ifdef JAVA_BINDING
        file << element << "," << '\n';
        #else
        file << element.string() << "," << '\n';
        #endif
    }

    file << '\n';
    file.close();
    this->valid_files_empty_ = false;
    if(this->counter_ == 1){
        this->map_size_ = std::get<0>(mapping).size();
        this->infile_.open(this->valid_files_);
    }

}

std::vector<Tuple> FilesystemStream::getValidFilesBlock(){

    if(this->valid_files_empty_){
        std::vector<Tuple> empty;
        return empty;
    }

    std::vector<Tuple> vec;
    Tuple member;

    long size = sizeof(std::vector<Tuple>);

    Map map;
    std::string str;
    std::string key, value;
    int value_length;
    size_t pos;
    Types result;
    map = this->temp_map_;

    while(size < this->block_size_ && this->infile_ >> str){

        if (map.size() == (this->map_size_)) {
            size += sizeof(map) + sizeof(std::vector<std::string>);

            //sizeof(Tuple) +
            for(const auto& item : map){
                size += item.first.length() + s::size(item.second);
            }

            std::get<0>(member) = map;
            str.pop_back(); // remove trailing comma
            std::get<1>(member).push_back(str);

            vec.push_back(member);

            map.clear();
            std::get<1>(member).clear();

            this->infile_ >> str;
        }

        pos = str.find(":");
        key = str.substr(0, pos);
        value_length = str.length() - pos;
        value = str.substr(pos+1, value_length);

        if(s::is_number(value)){
            result = stoi(value);
        } else {
            result = value;
        }

        map[key] = result;
        size += value_length + pos;
    }

    std::streampos ptr = this->infile_.tellg();
    if(!(this->infile_ >> str)){
        this->valid_files_empty_ = true;
        this->infile_.close();
    }
    //ptr +=1;
    this->infile_.seekg(ptr, std::ios::beg);
    this->temp_map_ = map;
    return vec;
}

long double FilesystemStream::currentSize(const int& stringSize, const long double& previous_size){
    return sizeof(std::string) + stringSize + previous_size;
}

bool FilesystemStream::isEmpty() {
    return this->empty_;
}

bool FilesystemStream::endOfValidFiles(){
    return this->valid_files_empty_;
}

std::string FilesystemStream::getFilePath(){
    return this->out_name_;
}

std::string FilesystemStream::getValidFilesPath(){
    return this->valid_files_;
}

std::string FilesystemStream::getBlockSizeStr(){
    return this->block_size_str_;
}


Tuple FilesystemStream::getFileByIndex(int i) {
    int file_index = (this->map_size_ + 1)*i + 1;
    //cout << "file: " << file_index << endl;
    std::ifstream in = std::ifstream(this->valid_files_);

    std::string str;

    Map map;
    std::string key, value;
    int value_length;
    size_t pos;
    Types result;

    f::goToLine(in, file_index);
    for(int j = 0; j < this->map_size_; ++j){

        getline(in, str);

        pos = str.find(":");
        key = str.substr(0, pos);
        value_length = str.length() - pos;
        value = str.substr(pos+1, value_length);

        if(s::is_number(value)){
            result = stoi(value);
        } else {
            result = value;
        }

        map[key] = result;
    }

    in >> str;
    Tuple temp;

    std::get<0>(temp) = map;
    str.pop_back(); // remove trailing comma
    std::get<1>(temp).push_back(str);

    return temp;
}

unsigned int FilesystemStream::getValidFilesSize(){
    return this->valid_files_size_;
}

std::vector<Tuple> FilesystemStream::getValidFilesSlice(int i, int j, int step){
    std::vector<Tuple> vec;

    for(int index = i; index < j; index += step){
        vec.push_back(getFileByIndex(index));
    }

    return vec;
}
