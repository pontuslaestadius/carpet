# Go to source tree
cd ../

echo $PWD

# To go output folder
cd build/

# Make the output from the source tree in the output directory
cmake ..

# Runs the written make test cases
make test

# Make the executable
make
