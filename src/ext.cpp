#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <Python.h>

#include <arrow/python/pyarrow.h>
// we might get duplicate decl error if we also include this
// #include <arrow/array/array_primitive.h>
// #include <arrow/type_fwd.h>
#include <arrow/array/builder_primitive.h>
#include <arrow/array/builder_binary.h>
#include <arrow/array/array_binary.h>



#include<iostream>
#include<set>
#include<string>

#include "caster.hpp"

namespace py = pybind11;
using DoubleArray = arrow::DoubleArray;
using Array = arrow::Array;

double sum(std::shared_ptr<arrow::DoubleArray> a) {
    double sum = 0;
    for(int i = 0; i < a->length(); i++) {
        sum += a->Value(i);
    }
    return sum;
}

std::shared_ptr<arrow::DoubleArray> madd(std::shared_ptr<arrow::DoubleArray>& a,
                                        std::shared_ptr<arrow::DoubleArray>& b,
                                        std::shared_ptr<arrow::DoubleArray>& c) {
    if ((a->length() != b->length()) || (a->length() != c->length())) {
        throw std::length_error("Arrays are not of equal length");
    }
    arrow::DoubleBuilder builder;
    arrow::Status status = builder.Resize(a->length());
    if (!status.ok()) {
        throw std::bad_alloc();
    }
    for(int i = 0; i < a->length(); i++) {
        builder.UnsafeAppend(a->Value(i) + b->Value(i) * c->Value(i));
    }
    std::shared_ptr<arrow::DoubleArray> array;
    arrow::Status st = builder.Finish(&array);
    return array;
}



std::shared_ptr<arrow::Int32Array> getUniqueRowIndex(std::shared_ptr<arrow::StringArray>& a) {
    std::unordered_set<std::string> UniqueStrings;
    // std::unordered_set<std::string>::iterator it;
    long cnt;
    cnt=0;
    long lens = a->length();
    std::string tmp;
    arrow::Int32Builder builder;
    arrow::Status status = builder.Resize(lens);
    if (!status.ok()) {
        throw std::bad_alloc();
    }
    for(long i = 0; i < lens; i++) {
        tmp = a->GetString(i);
        // it = UniqueStrings.find(a->GetString(i));
        if(UniqueStrings.contains(tmp)){

        }
        else{
            UniqueStrings.insert(tmp);
            builder.UnsafeAppend(i);
            cnt+=1;
        }

        
    }
    status = builder.Resize(cnt);
    std::shared_ptr<arrow::Int32Array> array;
    arrow::Status st = builder.Finish(&array);
    return array;
}

std::shared_ptr<arrow::BooleanArray> duplicatesFilter(std::shared_ptr<arrow::StringArray>& a) {
    std::unordered_set<std::string> UniqueStrings;
    std::unordered_set<std::string>::iterator it;
    // long cnt=0;
    arrow::BooleanBuilder builder;
    long lens = a->length();
    std::string tmp;
    arrow::Status status = builder.Resize(lens);
    if (!status.ok()) {
        throw std::bad_alloc();
    }
    for(long i = 0; i < lens; i++) {
        
        tmp = a->GetString(i);
        if(UniqueStrings.contains(tmp)){
            builder.UnsafeAppend(false);

        }
        else{
            UniqueStrings.insert(tmp);
            builder.UnsafeAppend(true);
            // cnt+=1;
        }

        
    }
    // status = builder.Resize(cnt);
    std::shared_ptr<arrow::BooleanArray> array;
    arrow::Status st = builder.Finish(&array);
    return array;
}

std::shared_ptr<arrow::BooleanArray> duplicatesFilterInt(std::shared_ptr<arrow::Int32Array>& a) {
    std::unordered_set<std::uint32_t> UniqueStrings;
    arrow::BooleanBuilder builder;
    long lens = a->length();
    std::uint32_t tmp;
    // arrow::Scalar scalar;
    arrow::Status status = builder.Resize(lens);
    if (!status.ok()) {
        throw std::bad_alloc();
    }
    for(long i = 0; i < lens; i++) {
        
        tmp = a->Value(i);
        if(UniqueStrings.contains(tmp)){
            builder.UnsafeAppend(false);

        }
        else{
            UniqueStrings.insert(tmp);
            builder.UnsafeAppend(true);
            // cnt+=1;get
        }

        
    }
    std::shared_ptr<arrow::BooleanArray> array;
    arrow::Status st = builder.Finish(&array);
    return array;
}



PYBIND11_MODULE(ext, m) {
    arrow::py::import_pyarrow();
    m.doc() = "arrow extension";
    m.def("madd", &madd, py::call_guard<py::gil_scoped_release>());
    m.def("getUniqueRowIndex", &getUniqueRowIndex, py::call_guard<py::gil_scoped_release>());
    m.def("duplicatesFilter", &duplicatesFilter, py::call_guard<py::gil_scoped_release>());
    m.def("duplicatesFilterInt", &duplicatesFilterInt, py::call_guard<py::gil_scoped_release>());
    m.def("sum", &sum, py::call_guard<py::gil_scoped_release>());
}
