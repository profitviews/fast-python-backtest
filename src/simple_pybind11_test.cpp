// #include "print.hpp"
// #include "program_options.hpp"

#include <pybind11/pybind11.h>

// #include <boost/program_options.hpp>
// #include <boost/range/irange.hpp>

// #include <exception>
// #include <filesystem>
// #include <vector>
// #include <memory>

// namespace profitview
// {

// struct ProgramArgs
// {
//     std::vector<int> integers_to_add;

//     void addOptions(boost::program_options::options_description& options)
//     {
//         namespace po = boost::program_options;
//         // clang-format off 
//         options.add_options()
//             ("integer_to_add", po::value(&integers_to_add)->multitoken()->required(), "Integers to add together.");
//         // clang-format on
//     }
// };

// }    // namespace profitview

// int add(auto const& integers_to_add) {
//     int result = 0;
//     for (auto const& i : integers_to_add)
//     {
//         result+=i;
//     }
//     return result;
// }

int add(int i, int j) {
    return i + j;
}

PYBIND11_MODULE(simple_pybind11_test, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    m.def("add", &add, "A function that adds two numbers");
}