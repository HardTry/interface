export GCCH="/usr/local/gcc730"
export PATH="$GCCH/bin:$PATH"
export LD_LIBRARY_PATH=$GCCH/lib:$GCCH/lib64:$GCCH/libexec:/usr/local/lib:/usr/lib
export CC=$GCCH/bin/gcc
export CXX=$GCCH/bin/g++
export CFLAGS="-O3 -Wall"
export CXXFLAGS="-O3 -Wall"



target=/app/sean/bin/gom/bin
binary=/usr/local/bin/gmd
build_rls=/home/sean/projects/build/interface/gcc730/release
build_dbg=/home/sean/projects/build/interface/gcc730/debug

here=/home/sean/projects/interface/md


cd $build_rls
make -j4
make install

cd $build_dbg
make -j4
make install

cp -f $binary/* $target/

cd $here
