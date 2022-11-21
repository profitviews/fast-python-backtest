#include "print.hpp"
#include "program_options.hpp"

#include <boost/program_options.hpp>

#include <thread>
#include <exception>
#include <filesystem>
#include <memory>

namespace profitview
{

struct ProgramArgs
{
    int number_of_threads;

    void addOptions(boost::program_options::options_description& options)
    {
        namespace po = boost::program_options;
        // clang-format off 
        options.add_options()
            ("threads", po::value<int>(&number_of_threads)->required(), "Number of threads to create");
        // clang-format on
    }
};

class ThreadPool
{
public:
    ThreadPool(): thread_count_{0} {}
    ThreadPool(int thread_count)
    : thread_count_{thread_count} 
    {
    }
    int size() const { return thread_count_; }
private:
    std::vector<std::jthread> threads_;
    int thread_count_;
};

}    // namespace profitview

int main(int argc, char* argv[])
{
    using namespace profitview;
    ProgramArgs options;
    auto const result = parseProgramOptions(argc, argv, options);
    if (result)
        return result.value();

    fmt_ns::print("Pool will have {} threads\n", options.number_of_threads);

    ThreadPool pool{options.number_of_threads};

    fmt_ns::print("Currently {} threads in pool\n", pool.size());

}