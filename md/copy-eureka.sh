export GCCH="/usr/local/gcc64"
export PATH="$GCCH/bin:$PATH"
export LD_LIBRARY_PATH=$GCCH/lib:$GCCH/lib64:$GCCH/libexec:/usr/local/lib:/usr/lib
export CC=$GCCH/bin/gcc
export CXX=$GCCH/bin/g++
export CFLAGS="-O3 -Wall"
export CXXFLAGS="-O3 -Wall"



target=/app/sean/bin/gom/bin
binary=/usr/local/bin/gmd
here=/home/sean/pkg/md/md

eureka_rls=/home/sean/pkg/build/eureka/gcc/Release
eureka_dbg=/home/sean/pkg/build/eureka/gcc/Debug

cd $eureka_rls
make -j4
make install

cd $eureka_dbg
make -j4
make install

cp -f $binary/* $target/

cd $here
