#include "print.hpp"

#include <arrow/api.h>
#include <arrow/io/api.h>
#include <parquet/arrow/reader.h>
#include <parquet/arrow/writer.h>
#include <parquet/exception.h>

#include <pybind11/pybind11.h>

#include <boost/range/irange.hpp>

#include <filesystem>

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

PYBIND11_MODULE(simple_pybind11_parquet_test, m) {
    m.doc() = "pybind11 parquet plugin"; // optional module docstring

    m.def("read_parquet", &read_parquet, "A function that reads parquet files");
}