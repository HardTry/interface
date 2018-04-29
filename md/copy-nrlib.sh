export GCCH="/usr/local/gcc730"
export PATH="$GCCH/bin:$PATH"
export LD_LIBRARY_PATH=$GCCH/lib:$GCCH/lib64:$GCCH/libexec:/usr/local/lib:/usr/lib
export CC=$GCCH/bin/gcc
export CXX=$GCCH/bin/g++
export CFLAGS="-O3 -Wall"
export CXXFLAGS="-O3 -Wall"



here=/home/sean/projects/interface/md
nrlib_path=/home/sean/projects/build/nrlib/gcc730/release/nrlib

cd $nrlib_path
make -j4
make install

cd ../../debug/nrlib
make -j4
make install

cd $here
