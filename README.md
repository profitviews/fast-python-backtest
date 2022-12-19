# fast-python-backtest

 [![Build](https://github.com/profitviews/fast-python-backtest/actions/workflows/cmake.yml/badge.svg)](https://github.com/profitviews/fast-python-backtest/actions)
 [![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/profitviews/fast-python-backtest/master/LICENSE.MIT)
 [![codecov](https://codecov.io/gh/profitviews/fast-python-backtest/branch/main/graph/badge.svg?token=1JmtZA8soP)](https://codecov.io/gh/profitviews/fast-python-backtest)

<img src="/assets/images/fast-python.png" style="width:500px"> 


## Build steps

Linux or MacOS.  See [here](https://github.com/profitviews/fast-python-backtest/blob/main/windows.md) for Windows.

1. Clone project
   ```
   git clone https://github.com/profitviews/fast-python-backtest.git cpp_crypto_algos
   cd fast-python-backtest
   ```

2. Install Conan
   ```
   python3 -m venv .venv         # Create a Python virtual env
   source ./.venv/bin/activate   # Activate the virtual env
   pip install conan             # Install conan
   ```

3. Install Conan Package & Configure CMake 
   ```
   mkdir build
   cd build
   conan install ../ --build missing
   source ./activate.sh
   cmake -DCMAKE_BUILD_TYPE=Debug ..
   ```
   
4. Build
   ```bash
   cmake --build .
   ```

This will create (with debugging symbols):

