set(CMAKE_SYSTEM_NAME Windows)

# Using the LLVM MinGW toolchain from here:
# https://github.com/mstorsjo/llvm-mingw/releases/tag/20240320

set(CMAKE_C_COMPILER /opt/llvm-mingw-20240320-msvcrt-x86_64/bin/x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER /opt/llvm-mingw-20240320-msvcrt-x86_64/bin/x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER /opt/llvm-mingw-20240320-msvcrt-x86_64/bin/x86_64-w64-mingw32-windres)
set(CMAKE_SYSROOT /opt/llvm-mingw-20240320-msvcrt-x86_64)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64" CACHE STRING "" FORCE)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
