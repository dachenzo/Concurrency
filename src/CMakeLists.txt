# Collect sources (explicit is best; GLOB is ok for a small lab project)
set(CX_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/spin_mutex.cpp
  # add more .cpp files here as you create them
)

add_library(cx ${CX_SRC})

# Public headers live in include/, e.g. include/cx/lock_guard.hpp, padded_lockable.hpp, spin_mutex.hpp, ...
target_include_directories(cx
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

# If you have header-only parts (like lock_guard.hpp), just include them from include/; no extra CMake needed.

# Example feature/arch flags (optional)
if (NOT MSVC)
  target_compile_options(cx PRIVATE -fno-exceptions -fno-rtti) # only if you truly want this
endif()
