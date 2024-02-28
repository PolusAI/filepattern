#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/complex.h>
#include <pybind11/stl/filesystem.h>

#include "pattern_object.hpp"

#ifdef WITH_LIBFILEPATTERN
#include "filepattern/filepattern.h"
#else
#include "include/filepattern.h"
#endif

namespace py = pybind11;

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
        .def("setGroup", py::overload_cast<const std::vector<std::string>&>(&FilePattern::setGroup))
        .def("setGroupStr",   py::overload_cast<std::string&>(&FilePattern::setGroup))
        .def("length", &FilePattern::length)
        .def_static("getRegex", &FilePattern::getRegex)
        .def_static("inferPattern", py::overload_cast<const std::string&, std::string&, const std::string&>(&FilePattern::inferPattern))
        .def_static("inferPattern", py::overload_cast<std::vector<std::string>&, std::string&>(&FilePattern::inferPattern))
        .def("iterator", [](FilePattern &v){
            auto& pattern_obj = v.getPatternObject();
            if(pattern_obj != nullptr) {
                if(pattern_obj->group_.size() == 0 || (pattern_obj->group_.size() != 0 && pattern_obj->group_[0] == "")){

                        return py::make_iterator(pattern_obj->valid_files_.begin(), pattern_obj->valid_files_.end());
                        
                } else {

                    return py::make_iterator(pattern_obj->valid_grouped_files_.begin(), pattern_obj->valid_grouped_files_.end());
                }
        }}, py::keep_alive<0, 1>())
        .def("iteratorExternal", [](FilePattern &v){
            auto& pattern_obj = v.getPatternObject();
            if(pattern_obj != nullptr) {
                

                if(pattern_obj->group_.size() == 0 || (pattern_obj->group_.size() != 0 && pattern_obj->group_[0] == "")) {

                    v.next();
                    return py::make_iterator(pattern_obj->current_block_.begin(), pattern_obj->current_block_.end());

                } else {

                    v.nextGroup();
                    return py::make_iterator(pattern_obj->current_group_.begin(), pattern_obj->current_group_.end());
                }
                
        }}, py::keep_alive<0, 1>());


}
