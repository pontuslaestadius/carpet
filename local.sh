if [ ! -d "build" ]; then
  # Control will enter here if $DIRECTORY doesn't exist.
	mkdir build
fi

cd build && cmake -D CMAKE_BUILD_TYPE=Release .. && \
    make
