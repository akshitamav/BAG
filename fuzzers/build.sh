#!/bin/bash -eu

echo "Building BAG for fuzzing..."

SRC_DIR=$SRC/bag
cd $SRC_DIR

BUILDTYPE=Debug
LIBRARY=-lbaglibd
HDF_END=_debug
XML_END=
EXTRA_CXX_FLAGS=-g

cmake -G Ninja -DCMAKE_BUILD_TYPE=$BUILDTYPE -B build -S . \
  -DCMAKE_INSTALL_PREFIX:PATH=/opt \
  -DCMAKE_PREFIX_PATH='/opt;/opt/local;/opt/local/HDF_Group/HDF5/1.14.3/' \
  -DBAG_BUILD_SHARED_LIBS:BOOL=OFF \
  -DBAG_BUILD_TESTS:BOOL=OFF -DBAG_CODE_COVERAGE:BOOL=OFF \
  -DBAG_BUILD_PYTHON:BOOL=OFF -DBAG_BUILD_EXAMPLES:BOOL=OFF

cmake --build build --config $BUILDTYPE --target install

echo $(pkg-config baglib)
echo $(ldconfig -p | grep "bag")
ls /opt/lib/static
ls /opt/lib

echo "CXXFLAGS:"
echo $CXXFLAGS

echo "Building bag_read_fuzzer..."
$CXX $CXXFLAGS $EXTRA_CXX_FLAGS \
  -I$SRC_DIR/api \
  fuzzers/bag_read_fuzzer.cpp -o $OUT/bag_read_fuzzer \
  $LIB_FUZZING_ENGINE \
  -L/opt/lib/static $LIBRARY \
  -L/opt/lib -lhdf5_cpp$HDF_END \
  -L/opt/lib -lhdf5$HDF_END \
  -L/opt/lib -lxml2$XML_END

echo "Building bag_extended_fuzzer..."
$CXX $CXXFLAGS $EXTRA_CXX_FLAGS \
  -I$SRC_DIR/api \
  fuzzers/bag_extended_fuzzer.cpp -o $OUT/bag_extended_fuzzer \
  $LIB_FUZZING_ENGINE \
  -L/opt/lib/static $LIBRARY \
  -L/opt/lib -lhdf5_cpp$HDF_END \
  -L/opt/lib -lhdf5$HDF_END \
  -L/opt/lib -lxml2$XML_END

$CXX $CXXFLAGS $EXTRA_CXX_FLAGS \
  -I$SRC_DIR/api \
  fuzzers/driver.cpp -o $OUT/driver \
  $LIB_FUZZING_ENGINE \
  -L/opt/lib/static $LIBRARY \
  -L/opt/lib -lhdf5_cpp$HDF_END \
  -L/opt/lib -lhdf5$HDF_END \
  -L/opt/lib -lxml2$XML_END


echo "Building seed corpus..."
zip -j $OUT/bag_extended_fuzzer_seed_corpus.zip $SRC_DIR/examples/sample-data/*.bag
