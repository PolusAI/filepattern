#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/complex.h>
#include <pybind11/stl/filesystem.h>
#include <iostream>

#include "interface/filepattern.h"

/*
#include "pattern.hpp"
#include "internal/internal_pattern.hpp"
#include "internal/filepattern.hpp"
#include "internal/stringpattern.hpp"
#include "internal/vectorpattern.hpp"
#include "external/external_pattern.hpp"
#include "external/external_filepattern.hpp"
#include "external/external_stringpattern.hpp"
#include "external/external_vectorpattern.hpp"
*/
 
namespace py = pybind11;

//PYBIND11_MAKE_OPAQUE(std::vector<std::vector<Tuple>>);

PYBIND11_MODULE(backend, m){

    py::class_<FilePattern>(m, "FilePattern")
        .def(py::init<const std::string &, const std::string &, const std::string&, bool, bool>())
        .def("getMatching", &FilePattern::getMatching)
        .def("getOccurrences", &FilePattern::getOccurrences)
        .def("getUniqueValues", &FilePattern::getUniqueValues)
        .def("outputName", &FilePattern::outputName)
        .def("getVariables", &FilePattern::getVariables)
        .def("getNewNaming", &FilePattern::getNewNaming)
        .def("groupBy", &FilePattern::groupBy)
        .def("currentBlockLength", &FilePattern::currentBlockLength)
        .def("getSlice", &FilePattern::getSlice)
        .def("getMatchingBlock", &FilePattern::getMatchingBlock)
        .def("getItem", &FilePattern::getItem)
        .def("getItemList", &FilePattern::getItemList)
        //.def("setGroup", &FilePattern::setGroup)
        .def("setGroup", py::overload_cast<const std::vector<std::string>&>(&FilePattern::setGroup))
        .def("setGroupStr",   py::overload_cast<std::string&>(&FilePattern::setGroup))
        .def("length", &FilePattern::length)
        .def_static("inferPattern", py::overload_cast<const std::string&, std::string&, const std::string&>(&FilePattern::inferPattern))
        .def_static("inferPattern", py::overload_cast<std::vector<std::string>&, std::string&>(&FilePattern::inferPattern))
        .def("__iter__", [](FilePattern &v){

            if(v.fp_->external) {

                if(v.fp_->group_.size() == 0 || (v.fp_->group_.size() != 0 && v.fp_->group_[0] == "")) {
                    
                    v.next(); 
                    return py::make_iterator(v.fp_->current_block_.begin(), v.fp_->current_block_.end());

                } else {

                    v.nextGroup();
                    return py::make_iterator(v.fp_->current_group_.begin(), v.fp_->current_group_.end());
                }

            } else {

                if(v.fp_->group_.size() == 0 || (v.fp_->group_.size() != 0 && v.fp_->group_[0] == "")){

                    return py::make_iterator(v.fp_->valid_files_.begin(), v.fp_->valid_files_.end());
                    //return py::make_iterator(v.fp_->valid_grouped_files_.begin(), v.fp_->valid_grouped_files_.end());
                } 
                else{ 
                    
                    return py::make_iterator(v.fp_->valid_grouped_files_.begin(), v.fp_->valid_grouped_files_.end());
                }
            }
            }, 
            py::keep_alive<0, 1>()); // Keep vector alive while iterator is used 

    /*
    py::class_<Pattern>(m, "Pattern")
        .def("filePatternToRegex", &Pattern::filePatternToRegex)
        .def("getPattern", &Pattern::getPattern)
        .def("setPattern", &Pattern::setPattern)
        .def("getRegexPattern", &Pattern::getRegexPattern)
        .def("getVariables", &Pattern::getVariables)
        .def("printVariables", &Pattern::printVariables)
        .def("getOccurrences", &Pattern::getOccurrences)
        .def("getUniqueValues", &Pattern::getUniqueValues)
        .def("getTmpDirs", &Pattern::getTmpDirs)
        .def("setGroup", &Pattern::setGroup)
        .def("getRegex", &Pattern::getRegex)
        .def_readonly("group", &Pattern::group_)
        .def_readonly("validFiles", &Pattern::valid_files_)
        .def("__iter__", [](const Pattern &v){
            if(v.group_.size() == 0 || v.group_[0] != ""){
                return py::make_iterator(v.valid_grouped_files_.begin(), v.valid_grouped_files_.end());
            } 
            else{ 
                return py::make_iterator(v.valid_files_.begin(), v.valid_files_.end());
            }
            }, 
            py::keep_alive<0, 1>()); // Keep vector alive while iterator is used 

    py::class_<InternalPattern, Pattern>(m, "InternalPattern")
        .def("groupBy", &InternalPattern::groupBy)
        .def("getMatching", &InternalPattern::getMatching)
        .def("outputName", &InternalPattern::outputName)
        .def("getItem", &InternalPattern::getItem)
        .def("getSlice", &InternalPattern::getSlice)
        .def("getItemList", &InternalPattern::getItemList)
        .def("inferPattern", py::overload_cast<std::vector<std::string>&, std::string&>(&InternalPattern::inferPattern))
        .def("inferPattern", py::overload_cast<const std::string&, std::string&>(&InternalPattern::inferPattern));

    py::class_<FilePatternObject, InternalPattern>(m, "FilePattern")
        .def(py::init<const std::string &, const std::string &, bool, bool>())
        .def("matchFiles", &FilePatternObject::matchFiles)
        .def("printFiles", &FilePatternObject::printFiles);

    py::class_<StringPattern, InternalPattern>(m, "StringPattern")
        .def(py::init<const std::string &, const std::string &, bool>())
        .def("matchFiles", &StringPattern::matchFiles)
        .def("printFiles", &StringPattern::readFile);

    py::class_<ExternalPattern, Pattern>(m, "ExternalPattern")
        .def("getMatching", &ExternalPattern::getMatching)
        .def("getMatchingBlock", &ExternalPattern::getMatchingBlock)
        .def("next", &ExternalPattern::next)
        .def("getValidFilesBlock", &ExternalPattern::getValidFilesBlock)
        .def("groupBy", &ExternalPattern::groupBy)
        .def("currentBlockLength", &ExternalPattern::currentBlockLength)
        .def("outputName", &ExternalPattern::outputName)
        .def("getGroupLength", &ExternalPattern::getGroupLength)
        .def("inferPattern", &ExternalPattern::inferPattern)
        .def("getItem", &ExternalPattern::getItem)
        .def("getSlice", &ExternalPattern::getSlice)
        .def("getItemList", &ExternalPattern::getItemList)
        .def("__iter__", [](ExternalPattern &v){ 
            if(v.group_[0] == "") {
                v.next();
                return py::make_iterator(v.current_block_.begin(), v.current_block_.end());
            } else {
                v.nextGroup();
                return py::make_iterator(v.current_group_.begin(), v.current_group_.end());
            }}, 
            py::keep_alive<0, 1>()); // Keep vector alive while iterator is used ;
        

    py::class_<ExternalFilePattern, ExternalPattern>(m, "ExternalFilePattern")
        .def(py::init<const std::string&, const std::string&, const std::string&, bool, bool>())
        .def("__iter__", [](ExternalFilePattern &v){ 
            if(v.group_[0] == "") {
                v.next();
                return py::make_iterator(v.current_block_.begin(), v.current_block_.end());
            } else {
                v.nextGroup();
                return py::make_iterator(v.current_group_.begin(), v.current_group_.end());
            }}, 
            py::keep_alive<0, 1>()); // Keep vector alive while iterator is used ;
    
    py::class_<ExternalStringPattern, ExternalPattern>(m, "ExternalStringPattern")
        .def(py::init<const std::string&, const std::string&, const std::string&, bool>());

    py::class_<VectorPattern, InternalPattern>(m, "InternalVectorPattern")
        .def(py::init<const std::string&, const std::string&, bool>())
        .def("inferPattern", &VectorPattern::inferPattern);

    py::class_<ExternalVectorPattern, ExternalPattern>(m, "ExternalVectorPattern")
        .def(py::init<const std::string&, const std::string&, const std::string&, bool>())
        .def("inferPattern", &ExternalVectorPattern::inferPattern);

    */
}
