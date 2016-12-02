cmake_minimum_required(VERSION 3.3)
project(Pricing)

# Detect Boost
set(Boost_NO_BOOST_CMAKE ON)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
message(STATUS "Boost include directory ${Boost_INCLUDE_DIR}")

# Detect PNL
find_package(Pnl REQUIRED)
set(LIBS ${LIBS} ${PNL_LIBRARIES})
include_directories(${PNL_INCLUDE_DIRS})

# Add math lib
set(LIBS ${LIBS} m)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES test.cpp
        RateModels/ConstantRateModel.cpp RateModels/GamTauxRateModel.cpp RateModels/GamTauxRateModel.hpp
        SimulationModels/ModelGen.cpp
        SimulationModels/SimuChange/BlackScholesChangeModel.cpp
        SimulationModels/SimuIndex/BlackScholesIndexModel.cpp
        Pricing/PricerGen.cpp Pricing/PricerGen.hpp Pricing/MonteCarloPricer.cpp Pricing/MonteCarloPricer.hpp
        Multimonde.cpp Multimonde.hpp Marche.cpp Marche.hpp)
add_executable(Pricer ${SOURCE_FILES})
target_link_libraries(Pricer ${LIBS})

