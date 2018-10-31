cmake_minimum_required(VERSION 3.5)
project(SDL2_ffmpeg C)
include(GNUInstallDirs)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(SDL2_FFMPEG_VERSION_MAJOR "0")
set(SDL2_FFMPEG_VERSION_MINOR "9")
set(SDL2_FFMPEG_VERSION_PATCH "0")
set(SDL2_FFMPEG_VERSION ${SDL2_FFMPEG_VERSION_MAJOR}.${SDL2_FFMPEG_VERSION_MINOR}.${SDL2_FFMPEG_VERSION_PATCH})
add_definitions(
    -DSDL2_FFMPEG_VERSION_MAJOR=${SDL2_FFMPEG_VERSION_MAJOR}
    -DSDL2_FFMPEG_VERSION_MINOR=${SDL2_FFMPEG_VERSION_MINOR}
    -DSDL2_FFMPEG_VERSION_PATCH=${SDL2_FFMPEG_VERSION_PATCH}
)

# Use C++11
PROJECT(simple_OpenGL)
if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -D__STDC_CONSTANT_MACROS")
endif (CMAKE_COMPILER_IS_GNUCXX)

# Packages

find_package(OpenCV REQUIRED core highgui)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb -Werror -fno-omit-frame-pointer -Wno-deprecated-declarations")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} -ggdb -O2 -fno-omit-frame-pointer -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -DNDEBUG")
set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} -Os -DNDEBUG")

option(BUILD_EXAMPLES "Build examples" ON)
option(USE_DYNAMIC_LIBASS "Use dynamically loaded libass" OFF)
option(USE_ASAN "Use AddressSanitizer" OFF)

find_package(SDL2 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(ffmpeg COMPONENTS avcodec avformat avutil swscale swresample)

if(CMAKE_SYSTEM_NAME MATCHES ".*Linux")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLinux -std=c++11")

FIND_PATH(
    PATH /usr/local/include /usr/include
)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DLinux -std=gnu11")

set(LIBRARIES
    -lGL
    -lGLEW
    ${SDL2_LIBRARIES}
    ${FFMPEG_LIBRARIES}
    ${OpenCV_LIBRARIES}
)
else()
set(LIBRARIES
    ${SDL2_LIBRARIES}
    ${FFMPEG_LIBRARIES}
)
endif()

set(INCLUDES
    include/
    ${SDL2_INCLUDE_DIRS}
    ${FFMPEG_INCLUDE_DIRS}
)

if(USE_DYNAMIC_LIBASS)
    if(WIN32 OR MINGW OR MSYS)
        set(DYNAMIC_LIBASS_NAME "\"libass-9.dll\"")
    else()
        set(DYNAMIC_LIBASS_NAME "\"libass.so\"")
    endif()
    add_definitions(-DUSE_DYNAMIC_LIBASS)
    add_definitions(-DDYNAMIC_LIBASS_NAME=${DYNAMIC_LIBASS_NAME})
else()
    find_package(ass)
    set(LIBRARIES ${LIBRARIES} ${ASS_LIBRARIES})
    set(INCLUDES ${INCLUDES} ${ASS_INCLUDE_DIRS})
endif()

FILE(GLOB_RECURSE SOURCES "src/*.c")
FILE(GLOB INSTALL_HEADERS "include/sdl2_ffmpeg/*.h")

add_library(SDL2_ffmpeg SHARED ${SOURCES})
add_library(SDL2_ffmpeg_static STATIC ${SOURCES})

set_target_properties(SDL2_ffmpeg PROPERTIES VERSION ${SDL2_FFMPEG_VERSION})
set_target_properties(SDL2_ffmpeg PROPERTIES SOVERSION ${SDL2_FFMPEG_VERSION_MAJOR})

set_target_properties(SDL2_ffmpeg PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(SDL2_ffmpeg_static PROPERTIES DEBUG_POSTFIX "d")

target_compile_definitions(SDL2_ffmpeg PRIVATE "KIT_DLL;KIT_DLL_EXPORTS")
target_compile_options(SDL2_ffmpeg PRIVATE "-fvisibility=hidden")

set_property(TARGET SDL2_ffmpeg PROPERTY C_STANDARD 99)

set_property(TARGET SDL2_ffmpeg_static PROPERTY C_STANDARD 99)

if(USE_ASAN)
    set(LIBRARIES asan ${LIBRARIES})
    target_compile_options(SDL2_ffmpeg PRIVATE "-fsanitize=address")
    message(STATUS "DEVELOPMENT: AddressSanitizer enabled!")
endif()

include_directories(${INCLUDES})
target_link_libraries(SDL2_ffmpeg ${LIBRARIES})

set(PKG_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/SDL2_ffmpeg.pc")

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/pkg-config.pc.in"
    ${PKG_CONFIG_FILE}
    @ONLY
)

if(BUILD_EXAMPLES)

### compile a C++ file into an object file
##set(CMAKE_CXX_COMPILE_OBJECT "<CMAKE_CXX_COMPILER>  <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT> -c <SOURCE>")
##set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_CXX_COMPILER>  <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES>")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_SOURCE_FILES_PROPERTIES( examples/example_simple_OpenGL.c PROPERTIES LANGUAGE CXX )
set_SOURCE_FILES_PROPERTIES( examples/example_simple_OpenCV.c PROPERTIES LANGUAGE CXX )
set_SOURCE_FILES_PROPERTIES( examples/example_simple_OpenCV_YUV.c PROPERTIES LANGUAGE CXX )
#set_SOURCE_FILES_PROPERTIES( examples/example_YUV_shaders_to_cvmat.c PROPERTIES LANGUAGE CXX )
# for testing cv::Mat locally defined 
#set_SOURCE_FILES_PROPERTIES( src/internal/video/kitvideo.c PROPERTIES LANGUAGE CXX )

    add_executable(audio examples/example_audio.c)
    add_executable(complex examples/example_complex.c)
    add_executable(simple examples/example_simple.c)
    add_executable(simple_OpenGL examples/example_simple_OpenGL.c)
    add_executable(simple_OpenCV examples/example_simple_OpenCV.c)
    add_executable(simple_YUV2OpenCV examples/example_simple_OpenCV_YUV.c)
#    add_executable(simple_YUV2OpenGL_Shaders examples/example_YUV_shaders_to_cvmat.c)
    add_executable(custom examples/example_custom.c)
    add_executable(rwops examples/example_rwops.c)

    if(MINGW)
        target_link_libraries(audio mingw32)
        target_link_libraries(complex mingw32)
        target_link_libraries(simple mingw32)
        target_link_libraries(custom mingw32)
        target_link_libraries(rwops mingw32)
    endif()

    set_property(TARGET audio PROPERTY C_STANDARD 99)
    set_property(TARGET complex PROPERTY C_STANDARD 99)
    set_property(TARGET simple PROPERTY C_STANDARD 99)
    set_property(TARGET custom PROPERTY C_STANDARD 99)
    set_property(TARGET rwops PROPERTY C_STANDARD 99)
    set_property(TARGET simple_OpenGL PROPERTY C_STANDARD 99)

    set_property(TARGET simple_OpenGL PROPERTY CXX_STANDARD 11)
    set_property(TARGET simple_OpenCV PROPERTY CXX_STANDARD 11)
    set_property(TARGET simple_YUV2OpenCV PROPERTY CXX_STANDARD 11)
#    set_property(TARGET simple_YUV2OpenGL_Shaders PROPERTY CXX_STANDARD 11)

    target_link_libraries(audio SDL2_ffmpeg_static ${LIBRARIES})
    target_link_libraries(complex SDL2_ffmpeg_static ${LIBRARIES})
    target_link_libraries(simple SDL2_ffmpeg_static ${LIBRARIES})
    target_link_libraries(simple_OpenGL SDL2_ffmpeg_static ${LIBRARIES})
    target_link_libraries(simple_OpenCV SDL2_ffmpeg_static ${LIBRARIES})
    target_link_libraries(simple_YUV2OpenCV SDL2_ffmpeg_static ${LIBRARIES})
#    target_link_libraries(simple_YUV2OpenGL_Shaders SDL2_ffmpeg_static ${LIBRARIES})
    target_link_libraries(custom SDL2_ffmpeg_static ${LIBRARIES})
    target_link_libraries(rwops SDL2_ffmpeg_static ${LIBRARIES})

endif()

# documentation target
add_custom_target(docs COMMAND doxygen WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

# Installation
install(FILES ${PKG_CONFIG_FILE} DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
install(FILES ${INSTALL_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sdl2_ffmpeg)
INSTALL(TARGETS SDL2_ffmpeg SDL2_ffmpeg_static
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
