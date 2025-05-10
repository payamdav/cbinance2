add_executable(tradereader_test tradereader_test.cpp)
target_link_libraries(tradereader_test utils trade logscale pubsub market zigzag)
