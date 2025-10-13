add_executable(example_basic basic.cpp)
target_link_libraries(example_basic PRIVATE cx)

add_executable(spsc_ring spsc_ring.cpp)
target_link_libraries(spsc_ring PRIVATE cx)

# add more examples as you go:
# add_executable(example_pad examples/pad.cpp)
# target_link_libraries(example_pad PRIVATE cx)
