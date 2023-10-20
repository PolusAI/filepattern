#include "../include/filepattern.h"
#include "filepattern_factory.h"
#include "../pattern_object.hpp"

FilePattern::FilePattern(const std::string& path, const std::string& filePattern, const std::string& block_size, bool recursive, bool suppressWarnings) {

    FilePatternFactory fpf = FilePatternFactory();

    this->fp_ = std::unique_ptr<PatternObject>(fpf.getObject(path, filePattern, block_size, recursive, suppressWarnings));

    if (block_size != "") {
        this->fp_->external = true;
    } else {
        this->fp_->external = false;
    }

}
FilePattern::~FilePattern() {
    this->fp_.reset();
}

std::vector<Tuple> FilePattern::getFiles() {
    return this->fp_->valid_files_;
}

std::vector<std::pair<std::vector<std::pair<std::string, Types>> , std::vector<Tuple>>> FilePattern::groupBy(std::vector<std::string>& groups) {
    this->fp_->groupBy(groups);
    return this->fp_->valid_grouped_files_;
}

void FilePattern::group(std::vector<std::string>& groups) {
    this->fp_->groupBy(groups);
}

std::vector<Tuple> FilePattern::getMatchingByMap(Map& variables) {
    std::vector<std::tuple<std::string, std::vector<Types>>> variables_vector;

    for(auto const& variable: variables){
        std::vector<Types> vec;
        vec.push_back(variable.second);
        variables_vector.push_back(std::make_tuple(variable.first, vec));
    }

    return this->fp_->getMatching(variables_vector);
}

std::vector<Tuple> FilePattern::getMatching(const std::vector<std::tuple<std::string, std::vector<Types>>>& variables) {
    return this->fp_->getMatching(variables);
}

std::vector<Tuple> FilePattern::getMatchingBlock() {
    return this->fp_->getMatchingBlock();
}

void FilePattern::setGroup(std::string& groups){
    std::vector<std::string> group_vec = {groups};
    this->fp_->group_ = group_vec;
}

void FilePattern::setGroup(const std::vector<std::string>& groups) {
    this->fp_->group_ = groups;
}

std::map<std::string, std::map<Types, int>> FilePattern::getOccurrences(const std::vector<std::tuple<std::string, std::vector<Types>>>& mapping) {
    return this->fp_->getOccurrences(mapping);
}

std::map<std::string, std::map<Types, int>> FilePattern::getOccurrencesByMap(Map& mapping) {

    std::vector<std::tuple<std::string, std::vector<Types>>> variables_vector;
    for(auto const& variable: mapping){
        std::vector<Types> vec;
        vec.push_back(variable.second);
        variables_vector.push_back(std::make_tuple(variable.first, vec));
    }

    return this->fp_->getOccurrences(variables_vector);
}

std::map<std::string, std::set<Types>> FilePattern::getUniqueValues(const std::vector<std::string>& mapping) {
    return this->fp_->getUniqueValues(mapping);
}

std::string FilePattern::outputName(std::vector<Tuple>& vec) {
    return this->fp_->outputName(vec);
}

std::vector<std::string> FilePattern::getVariables() {
    return this->fp_->getVariables();
}

void FilePattern::next() {
    this->fp_->next();
}

void FilePattern::nextGroup() {
    this->fp_->nextGroup();
}

int FilePattern::currentBlockLength() {
    return this->fp_->currentBlockLength();
}

void FilePattern::getNewNaming(std::string& pattern, bool suppressWarnings) {
    Pattern::getNewNaming(pattern, suppressWarnings);
}

std::vector<Tuple> FilePattern::getSlice(std::vector<Types>& key) {
    return this->fp_->getSlice(key);
}

std::vector<Tuple> FilePattern::getSliceByIdx(int key) {
    std::vector<Types> vars {key, "None", "None"};
    return this->fp_->getSlice(vars);
}


std::string FilePattern::inferPattern(const std::string& path, std::string& variables, const std::string& block_size){

    FilePatternFactory fpf = FilePatternFactory();

    // create dummy object to avoid the need for static methods in virtual class
    std::unique_ptr<PatternObject> fp;
    if (block_size == "") {
        fp = std::unique_ptr<PatternObject>(fpf.getObject(path, "", block_size, false, true));
    } else {

        fp = std::unique_ptr<PatternObject>(fpf.getObject(path, "", block_size, false, true));
    }


    return fp->inferPattern(path, variables, block_size);
}

std::string FilePattern::inferPattern(std::vector<std::string>& vec, std::string& variables) {

    FilePatternFactory fpf = FilePatternFactory();

    std::unique_ptr<PatternObject> fp = std::unique_ptr<PatternObject>(fpf.getObject(".", "dummy_pattern", "", false, true));

    return fp->inferPattern(vec, variables);
}

Tuple FilePattern::getItem(int key) {
    return this->fp_->getItem(key);
}

std::vector<Tuple> FilePattern::getItemList(std::vector<int>& key) {
    return this->fp_->getItemList(key);
}

int FilePattern::getSize() {
    return this->fp_->valid_files_.size();
}

int FilePattern::getGroupedSize() {
    return this->fp_->valid_grouped_files_.size();
}

std::string FilePattern::getPattern() {
    return this->fp_->file_pattern_;
}

void FilePattern::setPattern(std::string& pattern) {
    this->fp_->file_pattern_ = pattern;
}

std::string FilePattern::getPath() {
    return this->fp_->path_;
}

std::vector<Tuple>::iterator FilePattern::begin() {
    return  fp_->valid_files_.begin();
}

std::vector<Tuple>::iterator FilePattern::end() {
    return  fp_->valid_files_.end();
}

size_t FilePattern::length() {return fp_->valid_files_.size();}

const std::unique_ptr<PatternObject>& FilePattern::getPatternObject() const{
    return fp_;
}

std::pair<std::vector<std::pair<std::string, Types>> , std::vector<Tuple>> FilePattern::getGroupedSliceByIdx(int idx) {
    return fp_->get_grouped_file_by_idx(idx);
}
