#include "print.hpp"

#include <arrow/api.h>
#include <arrow/io/api.h>

#include <parquet/arrow/reader.h>

#include <pybind11/stl.h>
#include <pybind11/pybind11.h>

#include <boost/range/irange.hpp>

#include <filesystem>
#include <variant>
#include <string_view>
#include <functional>
#include <string>
#include <vector>
#include <stdexcept>

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

    std::vector<ParquetColumnTypes> column(int column_number) {
        if(0 > column_number || column_number >= schema_->num_fields()) throw std::range_error("Column number out of range");
        // https://github.com/apache/arrow/blob/910a749f8b7d15795c153a3ee41013f29b75f605/cpp/src/arrow/type_fwd.h#L75
        // using ArrayVector = std::vector<std::shared_ptr<Array>>;
        auto chunks {table_->column(column_number)->chunks()};
        std::vector<ParquetColumnTypes> result{};
        if(auto number_of_chunks{chunks.size()}; number_of_chunks > 0) {
            ColumnVisitor v([&result](ParquetColumnTypes d){ result.emplace_back(d); });
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
        ColumnVisitor(std::function<void(ParquetColumnTypes)> v) : v_{v} {}

        arrow::Status Visit(const arrow::DoubleArray& array) {
            for(const auto& a: array) v_(*a);
            return arrow::Status::OK();
        }

        arrow::Status Visit(const arrow::Int64Array& array) {
            for(const auto& a: array) v_(*a);
             return arrow::Status::OK();
        }

        arrow::Status Visit(const arrow::StringArray& array) {
            for(const auto& a: array) v_(*a);
            return arrow::Status::OK();
        }

        std::function<void(ParquetColumnTypes)> v_;
    };

    const std::string file_name_;
    std::shared_ptr<arrow::Schema> schema_;
    std::shared_ptr<arrow::Table> table_;
};

namespace py = pybind11;

PYBIND11_MODULE(pybind11_parquet_class_test, m) {
    m.doc() = "pybind11 parquet class plugin"; // optional module docstring

    py::class_<ParquetTable>(m, "ParquetTable")
        .def(py::init<std::string const&>())
        .def("print_stats", &ParquetTable::print_stats)
        .def("column", &ParquetTable::column)
    ;
}