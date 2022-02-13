cmake_minimum_required(VERSION 3.20)
project(ASTMatcher)

set(CMAKE_CXX_STANDARD 14)

# ${CMAKE_CURRENT_SOURCE_DIR}: /home/prophe/projects/c/ClangCheckers/StreamChecker
# ${PROJECT_BINARY_DIR}: /home/prophe/projects/c/ClangCheckers/StreamChecker/cmake-build-debug
# ${LLVM_PACKAGE_VERSION}: 12.0.0
set(LLVM_DIR /home/prophe/programming/llvm-12.0.0.obj/)
message("yes ${LLVM_DIR}")
message("yes ${PROJECT_BINARY_DIR}")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(
        "$<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-fno-rtti;-Wall>"
        "$<$<AND:$<CXX_COMPILER_ID:GNU>,$<CONFIG:DEBUG>>:-fno-omit-frame-pointer>"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
find_package(LLVM REQUIRED CONFIG HINTS ${LLVM_DIR})
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
# ${LLVM_CMAKE_DIR}: /home/prophe/programming/llvm-12.0.0.obj/lib/cmake/llvm
include(AddLLVM)

# LLVM_DIR must be set to the prefix of /share/llvm/cmake via commandline
# find_package(Clang REQUIRED)
# ${LLVM_DEFINITIONS}: -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS
# ${LLVM_INCLUDE_DIRS}: /home/prophe/programming/llvm-12.0.0.obj/include
# ${LLVM_LIBRARY_DIRS}: /home/prophe/programming/llvm-12.0.0.obj/lib

message("LLVM STATUS:
  Definitions ${LLVM_DEFINITIONS}
  Includes    ${LLVM_INCLUDE_DIRS}
              ${CLANG_INCLUDE_DIRS}
  Libraries   ${LLVM_LIBRARY_DIRS}"
        )

# Now set the LLVM header and library paths:
include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})


include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(ASTMatcher main.cpp)

############## FINAL PROJECT CONFIG #################
if (LLVM_ENABLE_ASSERTIONS)
    add_definitions(-DLLVM_ENABLE_ASSERTIONS=${LLVM_ENABLE_ASSERTIONS})
endif()

message("LLVM_TARGETS_TO_BUILD ${LLVM_TARGETS_TO_BUILD}")
llvm_map_components_to_libnames(REQ_LLVM_LIBRARIES ${LLVM_TARGETS_TO_BUILD}
        core support option
        )

target_link_libraries(ASTMatcher
        PRIVATE
        -Wl,--start-group
        clangAST
        clangBasic
        clangDriver
        clangFrontend
        clangIndex
        clangLex
        clangSema
        clangSerialization
        clangTooling
        clangFormat
        clangToolingInclusions
        clangToolingCore
        clangRewriteFrontend
        clangStaticAnalyzerFrontend
        clangCrossTU
        clangStaticAnalyzerCheckers
        clangStaticAnalyzerCore
        clangParse
        clangEdit
        clangRewrite
        clangAnalysis
        clangASTMatchers
        clangTransformer
        ${REQ_LLVM_LIBRARIES}
        -Wl,--end-group
)

# ${CMAKE_CURRENT_BINARY_DIR}: /home/prophe/projects/c/ClangCheckers/StreamChecker/cmake-build-debug


#target_include_directories(StreamChecker
#        PRIVATE
#        $<TARGET_PROPERTY:streamchecker,INTERFACE_INCLUDE_DIRECTORIES>
#        )
find_package(Threads REQUIRED)
find_package(Curses REQUIRED)



set_target_properties(ASTMatcher
        PROPERTIES
        LINKER_LANGUAGE CXX
        PREFIX ""
        )

install(TARGETS ASTMatcher
        RUNTIME DESTINATION bin
        )

# ${CMAKE_SOURCE_DIR}: /home/prophe/projects/c/ClangCheckers/ASTMatcher
include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${LIBRARY_OUTPUT_PATH})

# add_subdirectory(lib)