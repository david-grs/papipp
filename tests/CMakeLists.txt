include_directories(..)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(unit_tests papipp.cc)
target_link_libraries(unit_tests ${GTEST_BOTH_LIBRARIES} papi pthread)
add_test(unit_tests unit_tests)

