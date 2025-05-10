cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(LANGUAGE CXX)
set(CMAKE_BUILD_PARALLEL_LEVEL 4)
set(CMAKE_GENERATOR "Ninja")
set(CMAKE_LINKER_TYPE LLD)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")


project(cbinance)

add_subdirectory(src/libs/utils)
add_subdirectory(src/libs/core/pubsub)
# add_subdirectory(src/libs/ta/timeseries)
add_subdirectory(src/libs/ta/logscale)
# add_subdirectory(src/libs/ta/vols)
# add_subdirectory(src/libs/binance_candles)
# add_subdirectory(src/libs/statistics)
add_subdirectory(src/libs/trade)
add_subdirectory(src/libs/ta/zigzag)
# add_subdirectory(src/libs/ta/candle_anal)
add_subdirectory(src/libs/market)
# add_subdirectory(src/libs/simulator)
# add_subdirectory(src/scripts/trades)
# add_subdirectory(src/scripts/chart_data)
add_subdirectory(src/scripts/tests)



