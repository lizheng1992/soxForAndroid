

cmake_minimum_required(VERSION 3.4.1)

set(lib_src_DIR ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI})
AUX_SOURCE_DIRECTORY(src/main/cpp/ DIR_SRCS)
add_library(sox STATIC IMPORTED)
set_target_properties(sox PROPERTIES IMPORTED_LOCATION
                             ${lib_src_DIR}/libsox.a)

add_library( # Sets the name of the library.
             xessox-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/native-lib.cpp
               ${DIR_SRCS})


find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )



target_link_libraries( # Specifies the target library.
                       xessox-lib
                       sox
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )