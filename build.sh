#!/bin/sh

# Step 1: Compile the manifest resource
windres manifest.rc -O coff -o manifest.res

# Step 2: Compile and link everything, including the manifest
g++ -static -mwindows \
    -IC:/wxWidgets/include \
    -IC:/wxWidgets/build-static/lib/gcc_x64_lib/mswu \
    -LC:/wxWidgets/build-static/lib/gcc_x64_lib \
    -o test.exe test.cpp pet.cpp cat.cpp Product.cpp manifest.res \
    -Wl,--start-group \
        -lwxmsw32ud \
        -lwxpngd \
        -lwxzlibd \
        -lole32 -loleaut32 -lcomctl32 -lcomdlg32 -luuid \
        -lgdi32 -lshell32 -ladvapi32 -lversion \
        -lshlwapi -luxtheme -loleacc -lwinspool \
    -Wl,--end-group