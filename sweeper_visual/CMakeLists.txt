cmake_minimum_required(VERSION 3.28)
project(sweeper_visual)

set(CMAKE_CXX_STANDARD 20)
include_directories(hdr)

set(sdl_dir ${PROJECT_SOURCE_DIR}/environment/SDL2-2.30.2/x86_64-w64-mingw32)

include_directories(${PROJECT_SOURCE_DIR} ${sdl_dir}/include)
link_directories(${sdl_dir}/lib)

file(GLOB sdl_dll ${PROJECT_SOURCE_DIR}/cmake-build-debug/*.dll)

file(GLOB source_all ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME}
                ${source_all})

target_link_libraries(${PROJECT_NAME}
                      mingw32
                      SDL2main
                      ${sdl_dll}
                      )
if(WIN32)
    target_link_libraries(${PROJECT_NAME} ws2_32)
endif()