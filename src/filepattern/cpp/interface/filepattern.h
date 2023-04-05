#pragma once

#include <iostream>
#include <fstream>

#include "../pattern_object.hpp"
#include "../util/util.hpp"

//#include "iterator_tpl.h"

#include "../internal/filepattern.hpp"
#include "../internal/stringpattern.hpp"
#include "../internal/vectorpattern.hpp"
#include "../external/external_filepattern.hpp"
#include "../external/external_stringpattern.hpp"
#include "../external/external_vectorpattern.hpp"


typedef std::variant<int, std::string> Types2;
typedef std::map<std::string, Types> Map2;
typedef std::tuple<Map, std::vector<std::string>> Tuple2;

typedef std::vector<std::tuple<Map, std::vector<std::string>>> FileVector;

class FilePattern {

    public:
    
        std::unique_ptr<PatternObject> fp_;

        FilePattern(const std::string& path, const std::string& filePattern="", const std::string& block_size="", bool recursive=false, bool suppressWarnings=false);

        ~FilePattern() {
            this->fp_.reset();
        }
        //std::vector<Tuple> getMatching(std::map<std::string, std::variant<int, std::string>> variable_value_map);

        std::vector<Tuple> getMatchingByMap (Map& variables);

        std::vector<Tuple> getFiles();

        std::vector<Tuple> getMatching(const std::vector<std::tuple<std::string, std::vector<Types>>>& variables);

        std::map<std::string, std::map<Types, int>> getOccurrences(const std::vector<std::tuple<std::string, std::vector<Types>>>& mapping);

        std::map<std::string, std::map<Types, int>> getOccurrencesByMap(Map& mapping);


        std::map<std::string, std::set<Types>> getUniqueValues(const std::vector<std::string>& mapping);

        std::string outputName(std::vector<Tuple>& vec);
       
        std::vector<std::string> getVariables();

        std::vector<std::pair<std::vector<std::pair<std::string, Types>> , std::vector<Tuple>>> groupBy(std::vector<std::string>& groups);

        void setGroup(std::string& groups);

        void setGroup(const std::vector<std::string>& groups);

        void next();

        void nextGroup();

        int currentBlockLength();
       
        void getNewNaming(std::string& pattern, bool suppressWarnings);

        std::vector<Tuple> getSlice(std::vector<Types>& key);

        std::vector<Tuple> getSliceByIdx(int key);

        //std::string swSearch(std::string& pattern, std::string& filename, const std::string& variables);

        static std::string inferPattern(const std::string& path, std::string& variables, const std::string& block_size="");

        static std::string inferPattern(std::vector<std::string>& vec, std::string& variables);

        std::vector<Tuple> getMatchingBlock();

        Tuple getItem(int key);

        std::vector<Tuple> getItemList(std::vector<int>& key);

        std::vector<Tuple>::iterator begin() {
            return  fp_->valid_files_.begin();
        }
        
        std::vector<Tuple>::iterator end() {
            return  fp_->valid_files_.end();
        }

        int getSize();

        size_t length() {return fp_->valid_files_.size();}

        std::string getPattern();
        void setPattern(std::string& pattern);
        std::string getPath();

        /*
        struct Iterator {
            typedef std::pair<std::vector<std::pair<std::string, Types>> , std::vector<Tuple>> TupleVector;

            using iterator_category = std::forward_iterator_tag;
            using difference_type   = std::ptrdiff_t;

            using value_type1        = Tuple;
            using pointer1           = Tuple*;  // or also value_type*
            using reference1         = Tuple&;  // or also value_type&

            using value_type2        = TupleVector;
            using pointer2           = TupleVector*;  // or also value_type*
            using reference2         = TupleVector&;;  // or also value_type&

            using value_type3        = std::variant<Tuple, TupleVector>;
            using pointer3           = std::variant<Tuple*, TupleVector*>;  // or also value_type*
            using reference3         = std::variant<Tuple, TupleVector>&;  // or also value_type&
            
            Iterator(pointer1 ptr) : m_ptr1(ptr), m_ptr2(nullptr) {ptr1 = true;}

            /*
            reference3 operator*() const {
                if (ptr1) {
                    return *m_ptr1;
                } else {
                    return *m_ptr2;
                }
            }
            
            pointer3 operator->() {
                if (ptr1) {
                    return m_ptr1;
                } else {
                    return m_ptr2;
                }}

            Iterator(pointer2 ptr) : m_ptr1(nullptr), m_ptr2(ptr) {ptr1 = false;}

            //reference2 operator*() const {return *m_ptr2;}
            //pointer2 operator->() {return m_ptr2;}

            Iterator& operator++() {
                if (ptr1) {
                    m_ptr1++;
                } else {
                    m_ptr2++; 
                }
                return *this;
            }

            Iterator operator++(int) {Iterator tmp = *this; ++(*this); return tmp;}

            friend bool operator== (const Iterator& a, const Iterator& b) {
                if (ptr1) {
                    return a.m_ptr1 == b.m_ptr1;
                } else {
                    return a.m_ptr2 == b.m_ptr2;
                }
            };
            friend bool operator!= (const Iterator& a, const Iterator& b) { 
                if (ptr1) {
                    return a.m_ptr1 != b.m_ptr1;
                } else {
                    return a.m_ptr2 != b.m_ptr2;
                }
            };

            
            private:
                static bool ptr1;
                pointer1 m_ptr1;
                pointer2 m_ptr2;
                
        
        };

        Iterator begin() {
            if(fp_->group_.size() == 0) {
                return Iterator( &fp_->valid_files_[0]);
            } else {
                return Iterator( &fp_->valid_grouped_files_[0]);
            }
        }
        
        Iterator end() {
            if(fp_->group_.size() == 0) {
                return Iterator( &fp_->valid_files_[fp_->valid_files_.size()]); 
            } else {
                return Iterator( &fp_->valid_grouped_files_[fp_->valid_grouped_files_.size()]); 
            }
        }
        */
        /*
        // FilePattern Iterator
        STL_TYPEDEFS(std::variant<Tuple, std::pair<std::vector<std::pair<std::string, Types>> , std::vector<Tuple>>>);
        
        struct it_state {
            int pos;
            inline void begin(const FilePattern* ref) { pos = 0; }
            inline void next(const FilePattern* ref) { ++pos; }
            inline void end(const FilePattern* ref) { 
                if (fp_.getGroups().size() == 0) {
                    pos = ref->fp_.valid_files_.size();
                } else {
                    pos = ref->fp_.valid_grouped_files_.size();
                }
            }
            inline std::variant<Tuple, std::pair<std::vector<std::pair<std::string, Types>> , std::vector<Tuple>>>& get(FilePattern* ref) { 
                if(fp_.getGroups().size() == 0) { 
                    return ref->fp_.valid_files_[pos]; 
                } else {
                    return ref->fp_.valid_grouped_files_[pos];
                }
            }
            inline bool cmp(const it_state& s) const { return pos != s.pos; }

            // Optional to allow operator--() and reverse iterators:
            inline void prev(const FilePattern* ref) { --pos; }
            // Optional to allow `const_iterator`:
            inline const std::variant<Tuple, std::pair<std::vector<std::pair<std::string, Types>> , std::vector<Tuple>>>& get(const FilePattern* ref) const { 
                if(fp_.getGroups().size() == 0) { 
                    return ref->fp_.valid_files_[pos]; 
                } else {
                    return ref->fp_.valid_grouped_files_[pos];
                }
            }
        };

        SETUP_ITERATORS(FilePattern, std::variant<Tuple, std::pair<std::vector<std::pair<std::string, Types>> , std::vector<Tuple>>>&, it_state);

        SETUP_REVERSE_ITERATORS(FilePattern, std::variant<Tuple, std::pair<std::vector<std::pair<std::string, Types>> , std::vector<Tuple>>>&&, it_state);
        // End iterator
        */
};

struct PatternInitializer {
    std::string block_size;
    std::string path;
    std::string file_pattern;
    bool recursive;
    bool suppress_warnings;
};


class FilePatternFactory {

    public:

        FilePatternFactory() {}

        PatternObject* getObject(const std::string& path, const std::string& file_pattern, const std::string& block_size, bool recursive, bool suppressWarnings) {
            if (block_size == "") {
                if(s::endsWith(path, ".txt")) {
                    std::ifstream infile(path);
                    std::string str;
    
                    std::getline(infile, str);
    
                    if(std::regex_match(str, std::regex("file\\: .+?; corr\\: .+?; position\\: .+?; grid\\: .+?;"))) {
                        return new VectorPattern(path, file_pattern, suppressWarnings); // need to add builder to FPOjbect
                    }
                    
                    return new StringPattern(path, file_pattern, suppressWarnings); // need to add builder to FPOjbect
                }

                return new FilePatternObject(path, file_pattern, recursive, suppressWarnings); // need to add builder to FPOjbect
            }
    
            if(s::endsWith(path, ".txt")) {
                std::ifstream infile(path);
                    std::string str;
    
                    std::getline(infile, str);
    
                    if(std::regex_match(str, std::regex("file\\: .+?; corr\\: .+?; position\\: .+?; grid\\: .+?;"))) {
                    return new ExternalVectorPattern(path, file_pattern, block_size, suppressWarnings); // need to add builder to FPOjbect
                }
                
                return new ExternalStringPattern(path, file_pattern, block_size, suppressWarnings); // need to add builder to FPOjbect
            }

            return new ExternalFilePattern(path, file_pattern, block_size, recursive, suppressWarnings); // need to add builder to FPOjbect
        }
};