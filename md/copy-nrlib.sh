export GCCH="/usr/local/gcc64"
export PATH="$GCCH/bin:$PATH"
export LD_LIBRARY_PATH=$GCCH/lib:$GCCH/lib64:$GCCH/libexec:/usr/local/lib:/usr/lib
export CC=$GCCH/bin/gcc
export CXX=$GCCH/bin/g++
export CFLAGS="-O3 -Wall"
export CXXFLAGS="-O3 -Wall"



here=/home/sean/pkg/md/md
nrlib_path=/home/sean/pkg/build/nrlib/gcc/Release/nrlib

cd $nrlib_path
make -j4
make install

cd ../../Debug/nrlib
make -j4
make install

cd $here
