cmake_minimum_required(VERSION 3.3)
project(Pricing)

# Detect Boost
set(Boost_NO_BOOST_CMAKE ON)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
message(STATUS "Boost include directory ${Boost_INCLUDE_DIR}")

message(${CMAKE_CURRENT_SOURCE_DIR})
# Add math lib
set(LIBS ${LIBS} m)

# Add PNL lib
include_directories(../../lib/pnlLib/build/include/)
set(CMAKE_PREFIX_PATH ../../lib/pnlLib/build)
find_package(Pnl REQUIRED)
set(LIBS ${LIBS} ${PNL_LIBRARIES})
include_directories(${PNL_INCLUDE_DIRS})
# Flags for gcc
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Add source files
set(SOURCE_FILES Tests/Test.cpp
        RateModels/ConstantRateModel.cpp RateModels/GamTauxRateModel.cpp RateModels/GamTauxRateModel.hpp
        SimulationModels/ModelGen.cpp
        SimulationModels/SimuChange/BlackScholesChangeModel.cpp
        SimulationModels/SimuIndex/BlackScholesIndexModel.cpp
        Pricing/PricerGen.cpp Pricing/PricerGen.hpp Pricing/MonteCarloPricer.cpp Pricing/MonteCarloPricer.hpp
        Multimonde.cpp Multimonde.hpp Marche.cpp Marche.hpp)

set(RATE_SOURCE_FILES RateModels/ConstantRateModel.cpp RateModels/GamTauxRateModel.cpp)
# Add executables
#For RateModelTest
add_executable(test_gamTauxRateModel.te Tests/RateModelsTest/testGamTauxRateModel.cpp ${RATE_SOURCE_FILES})
add_executable(test_constantRateModel.te Tests/RateModelsTest/testConstantRateModel.cpp ${RATE_SOURCE_FILES})
target_link_libraries(test_constantRateModel.te ${LIBS})
add_executable(CompileAll.te ${SOURCE_FILES})
target_link_libraries(CompileAll.te ${LIBS})
add_executable(test.te Tests/Test.cpp)
target_link_libraries(test.te ${LIBS})

