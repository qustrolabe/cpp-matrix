# Catch2 tests
add_executable(
  test
  # Tests
  test/test_matrix2d.cpp test/test_matrix2darray.cpp test/test_vec.cpp)

target_include_directories(test PUBLIC ${Catch2_INCLUDE_DIRS})
target_include_directories(test PUBLIC src)
target_link_libraries(test Catch2::Catch2WithMain)

add_executable(main-test test/main.cpp) # Non-Catch2 testing playground
target_include_directories(main-test PUBLIC src)