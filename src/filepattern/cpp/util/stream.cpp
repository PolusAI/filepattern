#include "stream.hpp"

Stream::Stream(const std::string& block_size, const bool is_infer) {
    this->is_infer = is_infer;
    this->tmpdir = fs::temp_directory_path().string();
    this->tmpdir += "/fs_stream_tmp_" + s::getTimeString() + "/";
    this->block_size_str = block_size;
    this->block_size = Block::parseblockSize(block_size);

    this->empty = false;
    this->valid_files = tmpdir + "validFiles.txt";
    this->counter = 0;

    if (fs::exists(tmpdir)){
        fs::remove_all(tmpdir);
    }

    bool created = fs::create_directory(tmpdir);
    if (!created) {
        throw std::runtime_error("Could not create temporary file.");
    }
    this->out_name = tmpdir + "/temp.txt";
    this->infile.open(valid_files);
}


void Stream::writeBlock(const std::vector<std::string>& vec){
    std::ofstream file(this->out_name, std::ios_base::app);

    for(const auto& element: vec){
        file << '\n' << element;
    }
    file.close();
}

void Stream::writeValidFiles(const Tuple& mapping){
    counter++;
    std::ofstream file(valid_files, ios_base::app);
}

std::vector<Tuple> Stream::getValidFilesBlock(){

    if(this->valid_files_empty){
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
    map = this->temp_map;

    while(size < block_size && this->infile >> str){

        if (map.size() == (this->map_size)) {
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

            infile >> str;
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

    std::streampos ptr = infile.tellg();
    if(!(this->infile >> str)){
        valid_files_empty = true;
    }
    //ptr +=1;
    infile.seekg(ptr, std::ios::beg);
    this->temp_map = map;
    return vec;
}

long double Stream::currentSize(const int stringSize, const long double& previousSize){
    return sizeof(std::string) + stringSize + previousSize;
}

bool Stream::isEmpty() {
    return empty;
}

bool Stream::endOfValidFiles(){
    return valid_files_empty;
}

std::string Stream::getFilePath(){
    return out_name;
}

std::string Stream::getValidFilesPath(){
    return valid_files;
}

std::string Stream::getBlockSizeStr(){
    return this->block_size_str;
}
