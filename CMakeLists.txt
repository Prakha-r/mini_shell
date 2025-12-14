cmake_minimum_required(VERSION 3.13)

project(codecrafters-shell C)

file(GLOB_RECURSE SOURCE_FILES src/*.c src/*.h)

set(CMAKE_C_STANDARD 23) # Enable the C23 standard

add_executable(shell ${SOURCE_FILES})
if(WIN32)
  set(READLINE_INCLUDE_DIR "C:/msys64/mingw64/include")
  set(READLINE_LIBRARY "C:/msys64/mingw64/lib/libreadline.a")

  include_directories(${READLINE_INCLUDE_DIR})
  target_link_libraries(shell PRIVATE ${READLINE_LIBRARY})
else()
  target_link_libraries(shell PRIVATE readline)
endif()
