#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/complex.h>
#include <pybind11/stl/filesystem.h>
#include <iostream>

#include "include/filepattern.h"

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

}
