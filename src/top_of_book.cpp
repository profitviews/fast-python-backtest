#include <arrow/api.h>
#include <arrow/io/api.h>
#include <parquet/arrow/reader.h>
#include <parquet/arrow/writer.h>
#include <parquet/exception.h>

#include <iostream>

#include <memory>

int main(int argc, char* argv[])
{
  std::cout << "Reading ../data/quote/bitmex/XBTUSD/20221109.parquet at once" << std::endl;
  std::shared_ptr<arrow::io::ReadableFile> infile;
  PARQUET_ASSIGN_OR_THROW(infile,
                          arrow::io::ReadableFile::Open("../data/quote/bitmex/XBTUSD/20221109.parquet",
                                                        arrow::default_memory_pool()));

//   std::unique_ptr<parquet::arrow::FileReader> reader;
//   PARQUET_THROW_NOT_OK(
//       parquet::arrow::OpenFile(infile, arrow::default_memory_pool(), &reader));
//   std::shared_ptr<arrow::Table> table;
//   PARQUET_THROW_NOT_OK(reader->ReadTable(&table));
//   std::cout << "Loaded " << table->num_rows() << " rows in " << table->num_columns()
//             << " columns." << std::endl;
}