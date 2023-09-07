#!/bin/bash

# Overwrite the log files if they exist
echo "" > mcxx_outputlog
echo "" > mcxx_errorlog

# Set up spack dependencies 
# spack load gcc
# spack load cmake
# spack load gperf
# spack load boost
# spack load hdf5
# spack load openmpi@3.1

# Set up ccache for the build
# export CC="ccache gcc"
# export CXX="ccache g++"

# Start compiler build timing
start_time=$SECONDS

# Config and install nanox runtime library
export MERCURIUM=$PWD/mercurium
cd nanox-0.14.1
echo "" > mcxx_outputlog
echo "" > mcxx_errorlog
autoreconf -fiv >> mcxx_outputlog 2>> mcxx_errorlog
./configure --prefix=$MERCURIUM >> mcxx_outputlog 2>> mcxx_errorlog
make -j64 >> mcxx_outputlog 2>> mcxx_errorlog
make install -j64 >> mcxx_outputlog 2>> mcxx_errorlog

# Config and install mcxx compiler
cd ..
echo "" > mcxx_outputlog
echo "" > mcxx_errorlog
autoreconf -fiv >> mcxx_outputlog 2>> mcxx_errorlog
./configure --prefix=$MERCURIUM -enable-ompss --with-nanox=$MERCURIUM >> mcxx_outputlog 2>> mcxx_errorlog
make -j64 >> mcxx_outputlog 2>> mcxx_errorlog
make install -j64 >> mcxx_outputlog 2>> mcxx_errorlog
export PATH=$MERCURIUM/bin:$PATH

# End timing and calculate elapsed time
end_time=$SECONDS
elapsed_time=$((end_time - start_time))
minutes=$((elapsed_time / 60))
seconds=$((elapsed_time % 60))

# Print the elapsed time
echo "--------------------" 
echo -e "BUILD TIME: \t $minutes min $seconds sec" 
echo "--------------------"

# Compile and run test OpenMP program with nondeter and parallelnew pragmas
mcc -O0 -o test --openmp mytest.c >> mcxx_outputlog 2>> mcxx_errorlog
./test
