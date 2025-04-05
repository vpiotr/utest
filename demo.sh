# run this script to build and run the demo
# make sure you have cmake installed

mkdir -p _build && cd _build
cmake ..
cmake --build .
./bin/utest_demo
