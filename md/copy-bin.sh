export GCCH="/usr/local/gcc63"
export PATH="$GCCH/bin:$PATH"
export LD_LIBRARY_PATH=$GCCH/lib:$GCCH/lib64:$GCCH/libexec:/usr/local/lib:/usr/lib
export CC=$GCCH/bin/gcc
export CXX=$GCCH/bin/g++
export CFLAGS="-O3 -Wall"
export CXXFLAGS="-O3 -Wall"



target=/app/sean/bin/gom/bin
binary=/usr/local/bin/gmd
build_rls=/home/sean/projects/3p/build/md/gcc/release
build_dbg=/home/sean/projects/3p/build/md/gcc/debug

here=/home/sean/projects/3p/md/md


cd $build_rls
make -j24
make install

cd $build_dbg
make -j24
make install

cp -f $binary/* $target/

cd $here
