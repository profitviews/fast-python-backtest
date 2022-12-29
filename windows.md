# Fast Python Backtest - Windows

## Build steps

1. From the Windows main menu, start a PowerShell terminal
2. Downoload MSVC 2022 build tools: 
```powershell
Invoke-WebRequest -OutFile vs_BuildTools.exe -Uri https://aka.ms/vs/17/release/vs_BuildTools.exe
.\vs_BuildTools
```
3. Download Git 
```powershell
Invoke-WebRequest -OutFile Git-2.35.1.2-64-bit.exe -Uri https://github.com/git-for-windows/git/releases/download/v2.35.1.windows.2/Git-2.35.1.2-64-bit.exe
.\Git-2.35.1.2-64-bit
```
4. `git clone https://github.com/profitviews/fast-python-backtest.git`
5. `cd fast-python-backtest`
6. Download Python 
```powershell
Invoke-WebRequest -OutFile python-3.10.2-amd64.exe -Uri https://www.python.org/ftp/python/3.10.2/python-3.10.2-amd64.exe
.\python-3.10.2-amd64
```
7. `python -m venv .venv`
8. `.venv\Scripts\activate.ps1`
9. `pip install conan`
10. `mkdir build`
11. `cd build`
12. `conan profile new windows-msvc --detect`
13. `conan profile update settings.build_type=Debug windows-msvc`
14. Download Cmake
```powershell
Invoke-WebRequest -OutFile cmake-3.23.0-rc2-windows-x86_64.msi -Uri https://github.com/Kitware/CMake/releases/download/v3.23.0-rc2/cmake-3.23.0-rc2-windows-x86_64.msi
.\cmake-3.23.0-rc2-windows-x86_64.msi
$env:Path += ';C:\Program Files\CMake\bin'
```
15. `conan install ../ --build missing --profile windows-msvc`
16. `cmake -DCMAKE_BUILD_TYPE=Debug ..`
17. `cmake --build .`
18. This will create a DLL (with debugging symbols) in `fast-python-backtest\build\lib\Debug\` that can be used with `parquet_table.ipynb` in a Jupyter notebook.
