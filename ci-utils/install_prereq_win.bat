mkdir local_install
mkdir local_install\include

curl -L https://github.com/pybind/pybind11/archive/refs/tags/v2.11.1.zip -o v2.11.1.zip
unzip v2.11.1.zip
pushd pybind11-2.11.1
mkdir build_man
pushd build_man
cmake -DCMAKE_INSTALL_PREFIX=../../local_install/  -DPYBIND11_TEST=OFF ..
cmake --build . --config Release --target install
popd
popd


if errorlevel 1 exit 1
