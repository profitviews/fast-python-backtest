#include "print.hpp"
#include "program_options.hpp"

#include <arrow/api.h>
#include <arrow/io/api.h>
#include <parquet/arrow/reader.h>
#include <parquet/arrow/writer.h>
#include <parquet/exception.h>

#include <boost/program_options.hpp>
#include <boost/range/irange.hpp>

#include <exception>
#include <filesystem>
#include <memory>

namespace profitview
{

struct ProgramArgs
{
    std::vector<std::string> parquetFile;

    void addOptions(boost::program_options::options_description& options)
    {
        namespace po = boost::program_options;
        // clang-format off 
        options.add_options()
            ("parquet", po::value(&parquetFile)->multitoken()->required(), "Parquet files to load in.");
        // clang-format on
    }
};

}    // namespace profitview

int main(int argc, char* argv[])
{
    using namespace profitview;
    ProgramArgs options;
    auto const result = parseProgramOptions(argc, argv, HelpDocumentation{}, options);
    if (result)
        return result.value();

    for (auto const& file : options.parquetFile)
    {
        print_ns::print("Reading {} at one\n", file);

        if (!std::filesystem::exists(file))
            throw std::runtime_error(fmt_ns::format("Enable to find file {}", file));

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

}