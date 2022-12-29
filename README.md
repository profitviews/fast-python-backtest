# Fast Python Backtest

 [![Build](https://github.com/profitviews/fast-python-backtest/actions/workflows/cmake.yml/badge.svg)](https://github.com/profitviews/fast-python-backtest/actions)
 [![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/profitviews/fast-python-backtest/master/LICENSE.MIT)
 [![codecov](https://codecov.io/gh/profitviews/fast-python-backtest/branch/main/graph/badge.svg?token=1JmtZA8soP)](https://codecov.io/gh/profitviews/fast-python-backtest)

<img src="/assets/images/fast-python.png" style="width:500px"> 

## Overview

This project has the aspiration to provide examples for backtesting trading algorithms conceived of and partly written in Python but with aspects that need better performance using C++.

It uses the [Pybind11](https://github.com/pybind/pybind11) system to interface between the two languages.

The initial delivery is example code for a possible use-case, namely the extraction of data from time-series storage

## Time-series Data Extraction

In order to backtest, data is typically extracted from storage and "replayed".  That is, data recorded from past trading sessions is read and then fed to the trading algorithms to be tested.  It's performance is then analysed in various ways.

The amount of data required for a valid backtest is normally a number of years worth and can include full book data and so in an active market may be hundreds of gigabytes or more.

Further, in order to reasonably simulate a real-world trading environment, the data has to be read from storage quickly.  Its storage and retrieval must take into account memory limitations in the systems used.

For this reason the [Parquet](https://parquet.apache.org/) file format is a popular one for backtesting purposes.  Parquet was created as part of [Hadoop](https://hadoop.apache.org/) to deal with very large data sets and uses columnular storage.  This means it is optimised for very fast retrieval (and storage) of series of data.

Apache provides the [Arrow](https://arrow.apache.org/) columnular memory format which uses Parquet as one of its file formats.  It has a [C++ library](https://arrow.apache.org/docs/cpp/) which we use.

### Our Example

The code provided simply loads a Parquet table from a file and makes available some information about it and its columns.  It is tested on only a particular file, however there's no reason it shouldn't work on other files with columns of the same types.

In our example, we retrieve a [Table](https://arrow.apache.org/docs/cpp/api/table.html#_CPPv4N5arrow5TableE) from a Parquet file and use PyBind11 to expose methods for Python usage.

Due to the architecture of Arrow, it is necessary to implement [Visitors](https://refactoring.guru/design-patterns/visitor).  In our case this is the [ArrayVisitor](https://arrow.apache.org/docs/cpp/api/array.html#_CPPv4N5arrow12ArrayVisitorE).  Others who wish to solve more complex problems can expand on this code or learn from it. 

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

5. This will create (with debugging symbols):
   ```
   parquet_table.cpython-310-x86_64-linux-gnu.so
   ```
   
6. Open `parquet_table.ipynb` in a Jupyter notebook

7. The cells in `parquet_table.ipynb` should now function
