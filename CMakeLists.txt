cmake_minimum_required(VERSION 3.20)

project(cx_concurrency
  VERSION 0.1.0
  DESCRIPTION "Learning/experimental C++ concurrency primitives (spin mutex, lock guard, padding, etc.)"
  LANGUAGES CXX)

# ===== Options =====
option(CX_BUILD_EXAMPLES "Build examples" ON)
option(CX_BUILD_BENCH    "Build benchmarks" OFF)   # flip ON when you add bench/
option(CX_BUILD_TESTS    "Build tests" OFF)
option(CX_WARN_AS_ERR    "Treat compiler warnings as errors" OFF)
option(CX_ENABLE_LTO     "Enable interprocedural optimization/LTO" OFF)
option(BUILD_SHARED_LIBS "Build as shared libs" OFF)  # default: static library

# ===== C++ standard / compiler flags =====
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Warnings
# if (MSVC)
#   add_compile_options(/W4)
#   if (CX_WARN_AS_ERR) add_compile_options(/WX) endif()
# else()
#   add_compile_options(-Wall -Wextra -Wpedantic)
#   if (CX_WARN_AS_ERR) add_compile_options(-Werror) endif()
# endif()

# # LTO
# if (CX_ENABLE_LTO)
#   include(CheckIPOSupported)
#   check_ipo_supported(RESULT _ipo_ok OUTPUT _ipo_msg)
#   if (_ipo_ok)
#     set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
#   else()
#     message(WARNING "IPO/LTO not supported: ${_ipo_msg}")
#   endif()
# endif()

# ===== Library (src + public headers in include/) =====
add_subdirectory(src)

# ===== Optional subdirs =====
if (CX_BUILD_EXAMPLES AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples/CMakeLists.txt")
  add_subdirectory(examples)
endif()

if (CX_BUILD_BENCH AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/bench/CMakeLists.txt")
  add_subdirectory(bench)
endif()

if (CX_BUILD_TESTS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
  enable_testing()
  add_subdirectory(tests)
endif()

# ===== Install (minimal) =====
include(GNUInstallDirs)

install(TARGETS cx
  EXPORT cxTargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT cxTargets
  NAMESPACE cx::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cx_concurrency)
