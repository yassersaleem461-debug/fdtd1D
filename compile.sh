#!/bin/bash
CC="/opt/homebrew/opt/llvm/bin/clang++"
SDK="$(xcrun --show-sdk-path)"

EIGENINCLUDE="-I/opt/homebrew/opt/eigen/include/eigen3" 
CXXFLAGS="-g -O3 -fopenmp -std=c++20 -isysroot $SDK -nostdinc++ -isystem $SDK/usr/include/c++/v1"
GSL_CFLAGS="$(pkg-config --cflags gsl)"
GSL_LIBS="$(pkg-config --libs gsl)"

$CC $CXXFLAGS $EIGENINCLUDE $GSL_CFLAGS \
  -o template template.cpp \
  $GSL_LIBS -lm