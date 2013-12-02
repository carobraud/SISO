#!/bin/sh

fi=DO_RT.cpp
fi=$1
fout=$(basename $1 .cpp)

if [ $2 = 0 ]
then 
    g++ $fi -o $fout  -lm -lcomedi -lrt -Wall -Wextra -ansi -pedantic -O0 -g -fno-tree-pre -Dcimg_use_vt100 -I/usr/X11R6/include -Dcimg_use_xshm -Dcimg_use_xrandr -L/usr/X11R6/lib -lpthread -lX11 -lXext -lXrandr -I../../NetCDF/include -lnetcdf_c++ -lnetcdf -L../../NetCDF/lib/ 2> cerr.txt; cat cerr.txt | grep error; rm cerr.txt
else
    g++ $fi -o $fout  -lm -lcomedi -lrt -Wall -Wextra -ansi -pedantic -O0 -g -fno-tree-pre -Dcimg_use_vt100 -I/usr/X11R6/include -Dcimg_use_xshm -Dcimg_use_xrandr -L/usr/X11R6/lib -lpthread -lX11 -lXext -lXrandr -I../../NetCDF/include -lnetcdf_c++ -lnetcdf -L../../NetCDF/lib/ 
fi