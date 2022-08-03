create() {
    mkdir -p build
    cd build
    cmake ..
    cmake --build .
}


cd tool

create
./generateAST

cd ../..

create
cd ..
