#include "print.hpp"

#include <arrow/api.h>
#include <arrow/array/array_binary.h>
#include <arrow/io/api.h>
#include <arrow/status.h>
#include <arrow/type.h>
#include <arrow/type_fwd.h>
#include <arrow/visitor.h>
#include <cstdint>
#include <memory>
#include <parquet/arrow/reader.h>
#include <parquet/arrow/writer.h>
#include <parquet/exception.h>

#include <pybind11/stl.h>
#include <pybind11/pybind11.h>

#include <boost/range/irange.hpp>

#include <boost/core/demangle.hpp>

#include <filesystem>
#include <variant>
#include <string_view>
#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <type_traits>
#include <stdexcept>
#include <typeinfo>

void read_parquet(std::string const& file) {
    using namespace profitview;

    print_ns::print("Reading {} at one\n", file);

    if (!std::filesystem::exists(file))
        throw std::runtime_error("Enable to find file");

    std::shared_ptr<arrow::io::ReadableFile> infile;
    PARQUET_ASSIGN_OR_THROW(infile, arrow::io::ReadableFile::Open(file, arrow::default_memory_pool()));
    std::unique_ptr<parquet::arrow::FileReader> reader;
    PARQUET_THROW_NOT_OK(
        parquet::arrow::OpenFile(infile, arrow::default_memory_pool(), &reader));
    std::shared_ptr<arrow::Table> table;
    PARQUET_THROW_NOT_OK(reader->ReadTable(&table));

    print_ns::print("Loaded {} rows in {} columns.\n", table->num_rows(), table->num_columns());

    const auto& schema{table->schema()};
    print_ns::print("Field names: \n");
    for (auto i: boost::irange(schema->num_fields())) {
        const auto& field{schema->field(i)};
        print_ns::print("Field {}: '{}' has type {}\n", i + 1, field->name(), field->type()->name());
    }
}

using namespace profitview;
class ParquetTable {
public:
    using ParquetColumnTypes = std::variant<double, long, std::string_view>;
    ParquetTable(std::string const& file_name) : file_name_{file_name}, schema_{}, table_{} {
        if (!std::filesystem::exists(file_name_))
            throw std::runtime_error("Enable to find file");

        std::shared_ptr<arrow::io::ReadableFile> infile;
        PARQUET_ASSIGN_OR_THROW(infile, arrow::io::ReadableFile::Open(file_name_, arrow::default_memory_pool()));
        std::unique_ptr<parquet::arrow::FileReader> reader;
        PARQUET_THROW_NOT_OK(
            parquet::arrow::OpenFile(infile, arrow::default_memory_pool(), &reader));
        PARQUET_THROW_NOT_OK(reader->ReadTable(&table_));
        schema_ = table_->schema();
    }

    std::vector<std::variant<double, long, std::string_view>> column(int column_number) {
        if(0 > column_number || column_number >= schema_->num_fields()) throw std::range_error("Column number out of range");
        // https://github.com/apache/arrow/blob/910a749f8b7d15795c153a3ee41013f29b75f605/cpp/src/arrow/type_fwd.h#L75
        // using ArrayVector = std::vector<std::shared_ptr<Array>>;
        auto chunks {table_->column(column_number)->chunks()};
        std::vector<std::variant<double, long, std::string_view>> result{};
        if(auto number_of_chunks{chunks.size()}; number_of_chunks > 0) {
            ColumnVisitor v([&result](std::variant<double, long, std::string_view> d){ result.emplace_back(d); });
            // ColumnVisitor v([&result](ColumnType d){ result.emplace_back(d); });
            print_ns::print("Number of Chunks: {}. Items per chunk: {}\n", number_of_chunks, chunks[0]->length());
            result.reserve(chunks[0]->length()*number_of_chunks);
            for(const auto& chunk: chunks) chunk->Accept(&v);
        }
        return result;
    }

    void print_stats() {
        print_ns::print("Loaded {} rows in {} columns.\n", table_->num_rows(), table_->num_columns());

        print_ns::print("Field names: \n");
        for (auto i: boost::irange(schema_->num_fields())) {
            const auto& field{schema_->field(i)};
            print_ns::print("Field {}: '{}' has type {}\n", i + 1, field->name(), field->type()->name());
        }
    }
private:
    struct ColumnVisitor : public arrow::ArrayVisitor
    {
        ColumnVisitor(std::function<void(std::variant<double, long, std::string_view>)> v) : v_{v} {}
        // ColumnVisitor(std::function<void(double)> v) : v_d_{v} {}
        // ColumnVisitor(std::function<void(long)> v) : v_i_{v} {}
        // ColumnVisitor(std::function<void(std::string_view)> v) : v_s_{v} {}

        // arrow::Status Visit(const arrow::DoubleArray& array) { return GenericVisit(array); }
        arrow::Status Visit(const arrow::DoubleArray& array) {
            // print_ns::print("Type of F: {}\n", demangle(typeid(F).name()));
            // print_ns::print("Calling Visit for DoubleArray\n");
            // print_ns::print("Array type: {}\n", demangle(typeid(array).name()));
            // print_ns::print("F type: {}, array element type: {}\n", demangle(typeid(F).name()), demangle(typeid(*array[0]).name()));
            // print_ns::print("F non-demangled type: {}, array element type: {}\n", typeid(F).name(), typeid(*array[0]).name());
            for(const auto& a: array) v_(*a);
            // for(const auto& a: array) if constexpr (std::is_same_v<std::remove_cvref_t<decltype(array)>::TypeClass::c_type, std::remove_cvref_t<decltype(*a)>>) {
            //     // print_ns::print("True branch: Surely the same: std::is_same_v<std::remove_cvref_t<F>, std::remove_cvref_t<decltype(*a)>>: {}\n",
            //     // (std::is_same_v<std::remove_cvref_t<F>, std::remove_cvref_t<decltype(*a)>> ? "Yes!" : "Nah..."));
            // }
            // else {
            //     // print_ns::print("False branch: Surely the same: std::is_same_v<std::remove_cvref_t<F>, std::remove_cvref_t<decltype(*a)>>: {}\n",
            //     // (std::is_same_v<std::remove_cv_t<F>, std::remove_cv_t<decltype(*a)>> ? "Yes!" : "Nah..."));
            //     // print_ns::print("F type: {}; array element type (*a): {}\n", demangle(typeid(F).name()), demangle(typeid(*a).name()));

            //     // print_ns::print("False branch: {}\n",
            //     // (std::is_same_v<std::remove_cv_t<F>, std::remove_cv_t<decltype(double{})>> ? "F is double" : "F not double"));

            //     // print_ns::print("Types: F type: {}, array element type: {}, types are {}\n", 
            //     // typeid(F).name(), 
            //     // typeid(decltype(*a)).name(),
            //     // (std::is_same_v<F, decltype(*a)> ? "same" : "different"));
            //     ;
            // }
            return arrow::Status::OK();
        }

        // arrow::Status Visit(const arrow::Int64Array& array) { return GenericVisit(array); }
        arrow::Status Visit(const arrow::Int64Array& array) {
            // using namespace boost::core;
            // print_ns::print("Type of F: {}\n", demangle(typeid(F).name()));
            // print_ns::print("Calling Visit for Int64Array\n");
            // print_ns::print("Array type: {}\n", demangle(typeid(array).name()));
            // print_ns::print("F type: {}, array element type: {}\n", demangle(typeid(F).name()), demangle(typeid(*array[0]).name()));
            // print_ns::print("F non-demangled type: {}, array element type: {}\n", typeid(F).name(), typeid(*array[0]).name());
            for(const auto& a: array) v_(*a);
            // for(const auto& a: array) if constexpr (std::is_same_v<std::remove_cvref_t<decltype(array)>::TypeClass::c_type, std::remove_cvref_t<decltype(*a)>>) {
            //     // print_ns::print("Surely the same: std::is_same_v<std::remove_cvref_t<F>, std::remove_cvref_t<decltype(*a)>>: {}\n",
            //     // (std::is_same_v<std::remove_cvref_t<F>, std::remove_cvref_t<decltype(*a)>> ? "Yes!" : "Nah..."));
            //     v_i_(*a); 
            // }
            // else {
            //     // print_ns::print("Surely the same: std::is_same_v<std::remove_cvref_t<F>, std::remove_cvref_t<decltype(*a)>>: {}\n",
            //     // (std::is_same_v<std::remove_cvref_t<F>, std::remove_cvref_t<decltype(*a)>> ? "Yes!" : "Nah..."));

            //     // print_ns::print("Types: F type: {}, array element type: {}, types are {}\n", 
            //     // typeid(F).name(), 
            //     // typeid(decltype(*a)).name(),
            //     // (std::is_same_v<F, decltype(*a)> ? "same" : "different"));
            //     ;
            // }
             return arrow::Status::OK();
        }

        // arrow::Status Visit(const arrow::StringArray& array) { return GenericVisit(array); }
        arrow::Status Visit(const arrow::StringArray& array) {
            // using namespace boost::core;
            // print_ns::print("Type of F: {}\n", demangle(typeid(F).name()));
            // print_ns::print("Calling Visit for StringArray\n");
            // print_ns::print("Array type: {}\n", demangle(typeid(array).name()));
            // print_ns::print("F type: {}, array element type: {}\n", demangle(typeid(F).name()), demangle(typeid(*array[0]).name()));
            // print_ns::print("F non-demangled type: {}, array element type: {}\n", typeid(F).name(), typeid(*array[0]).name());
            for(const auto& a: array) v_(*a);
            // for(const auto& a: array) if constexpr (std::is_same_v<std::remove_cvref_t<decltype(array)>::TypeClass::c_type, std::remove_cvref_t<decltype(*a)>>) {
            //     // print_ns::print("Surely the same: std::is_same_v<std::remove_cvref_t<F>, std::remove_cvref_t<decltype(*a)>>: {}\n",
            //     // (std::is_same_v<std::remove_cvref_t<F>, std::remove_cvref_t<decltype(*a)>> ? "Yes!" : "Nah..."));
            //     v_s_(*a); 
            // }
            // else {
            //     // print_ns::print("Surely the same: std::is_same_v<std::remove_cvref_t<F>, std::remove_cvref_t<decltype(*a)>>: {}\n",
            //     // (std::is_same_v<std::remove_cvref_t<F>, std::remove_cvref_t<decltype(*a)>> ? "Yes!" : "Nah..."));

            //     // print_ns::print("Types: F type: {}, array element type: {}, types are {}\n", 
            //     // typeid(F).name(), 
            //     // typeid(decltype(*a)).name(),
            //     // (std::is_same_v<F, decltype(*a)> ? "same" : "different"));
            //     ;
            // }
            return arrow::Status::OK();
        }

        // template<typename T>
        // arrow::Status GenericVisit(const T& array) {
        //     bool first{true};
        //     for(const auto a: array) {
        //         if(first) {
        //             first = false;
        //             print_ns::print("Array type: {}\n", typeid(array).name());
        //             print_ns::print("Array element type: {}\n", typeid(*a).name());
        //         }
        //         // print_ns::print("{}\n", *a);
        //         invoke<decltype(*a)>(*a);
        //     }
        //     return arrow::Status::OK();
        // }
        // std::function<void(double)> v_d_;
        // std::function<void(long)> v_i_;
        // std::function<void(std::basic_string_view<char, std::char_traits<char> >)> v_s_;
        std::function<void(std::variant<double, long, std::string_view>)> v_;

        // template<typename V> void invoke(V v) {
        //     if constexpr(std::is_same<V, double>::value) { v_d_(v); return; }
        //     if constexpr(std::is_same<V, int64_t>::value) { v_i_(v); return; }
        //     if constexpr(std::is_same<V, std::string_view>::value) { v_s_(v); return; }
        // }
    };

    const std::string file_name_;
    std::shared_ptr<arrow::Schema> schema_;
    std::shared_ptr<arrow::Table> table_;
};

namespace py = pybind11;

PYBIND11_MODULE(simple_pybind11_parquet_test, m) {
    m.doc() = "pybind11 parquet plugin"; // optional module docstring

    m.def("read_parquet", &read_parquet, "A function that reads parquet files");
} 

PYBIND11_MODULE(pybind11_parquet_class_test, m) {
    m.doc() = "pybind11 parquet class plugin"; // optional module docstring

    py::class_<ParquetTable>(m, "ParquetTable")
        .def(py::init<std::string const&>())
        .def("print_stats", &ParquetTable::print_stats)
        .def("column", &ParquetTable::column)
    ;
}