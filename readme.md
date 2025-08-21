#To build from scratch
cd ~/huevadas/projects/physics && rm -rf build && mkdir build && cd build && cmake .. && make ; cd ..

#To build
make

#To test
make test