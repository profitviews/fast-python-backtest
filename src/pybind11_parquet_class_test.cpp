#include "print.hpp"

#include <arrow/api.h>
#include <arrow/io/api.h>

#include <arrow/status.h>
#include <parquet/arrow/reader.h>

#include <pybind11/stl.h>
#include <pybind11/pybind11.h>

#include <boost/range/irange.hpp>

#include <cstdint>
#include <filesystem>
#include <functional>
#include <stdexcept>
#include <string_view>
#include <string>
#include <variant>
#include <vector>

using namespace profitview;

class ParquetTable {
public:
    ParquetTable(std::string const& file_name) : schema_{}, table_{} {
        if (!std::filesystem::exists(file_name))
            throw std::runtime_error("Enable to find file");

        std::shared_ptr<arrow::io::ReadableFile> infile;
        PARQUET_ASSIGN_OR_THROW(infile, arrow::io::ReadableFile::Open(file_name));
        std::unique_ptr<parquet::arrow::FileReader> reader;
        PARQUET_THROW_NOT_OK(parquet::arrow::OpenFile(
            infile, arrow::default_memory_pool(), &reader));
        PARQUET_THROW_NOT_OK(reader->ReadTable(&table_));
        schema_ = table_->schema();
    }

    void print_stats() {
        print_ns::print("Loaded {} rows in {} columns.\n", 
            table_->num_rows(), table_->num_columns());

        print_ns::print("Field names: \n");
        for (auto i: boost::irange(schema_->num_fields())) {
            const auto& field{schema_->field(i)};
            print_ns::print("Field {}: '{}' has type {}\n", 
                i + 1, field->name(), field->type()->name());
        }
    }

    using ParquetColumnTypes = std::variant<double, std::int64_t, std::string_view>;

    std::vector<ParquetColumnTypes> column(int column_number) {
        if(0 > column_number || column_number >= schema_->num_fields())
            throw std::range_error("Column number out of range");
        auto chunks {table_->column(column_number)->chunks()};
        std::vector<ParquetColumnTypes> result{};
        if(auto number_of_chunks{chunks.size()}; number_of_chunks > 0) {
            ColumnVisitor visitor([&result](ParquetColumnTypes d)
                { result.emplace_back(d); });
            result.reserve(chunks[0]->length()*number_of_chunks);
            for(const auto& chunk: chunks) 
                if(chunk->Accept(&visitor) != arrow::Status::OK()) 
                    throw std::runtime_error("Accept fail");
        }
        return result;
    }

private:
    struct ColumnVisitor : public arrow::ArrayVisitor
    {
        ColumnVisitor(std::function<void(ParquetColumnTypes)> operation) 
        : operation_{operation} {}

        arrow::Status Visit(const arrow::DoubleArray& array) {
            for(const auto& element: array) operation_(*element);
            return arrow::Status::OK();
        }

        arrow::Status Visit(const arrow::Int64Array& array) {
            for(const auto& element: array) operation_(*element);
            return arrow::Status::OK();
        }

        arrow::Status Visit(const arrow::StringArray& array) {
            for(const auto& element: array) operation_(*element);
            return arrow::Status::OK();
        }

        std::function<void(ParquetColumnTypes)> operation_;
    };

    std::shared_ptr<arrow::Schema> schema_;
    std::shared_ptr<arrow::Table> table_;
};

namespace py = pybind11;

PYBIND11_MODULE(pybind11_parquet_class_test, parquet_module) {
    parquet_module.doc() = "pybind11 parquet class plugin";

    py::class_<ParquetTable>(parquet_module, "ParquetTable")
        .def(py::init<std::string const&>())
        .def("print_stats", &ParquetTable::print_stats)
        .def("column", &ParquetTable::column)
    ;
}