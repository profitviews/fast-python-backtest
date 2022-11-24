#include "print.hpp"
#include "program_options.hpp"

#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/program_options.hpp>
#include <boost/range/irange.hpp>

#include <thread>
#include <chrono>

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

}    // namespace profitview

int main(int argc, char* argv[])
{
    using namespace profitview;
    ProgramArgs options;
    auto const result = parseProgramOptions(argc, argv, options);
    if (result)
        return result.value();

    print_ns::print("Pool will have {} threads\n", options.number_of_threads);

    boost::asio::thread_pool pool{static_cast<size_t>(options.number_of_threads)};

    print_ns::print("Currently {} threads in pool\n", options.number_of_threads);

    for(auto i: boost::irange(options.number_of_threads))
    {
        print_ns::print("Adding thread {}\n", i + 1);
        boost::asio::post(pool, [i]()
        { 
            print_ns::print("Sleeping for {} seconds\n", i + 1);
            std::this_thread::sleep_for(std::chrono::milliseconds((i+1)*1000));
            print_ns::print("Finishing thread {}\n", i + 1);
        });
    }
    pool.wait();
}