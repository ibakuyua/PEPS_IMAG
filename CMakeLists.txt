cmake_minimum_required(VERSION 3.3)
project(PEPS_IMAG)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Detect Boost
set(Boost_NO_BOOST_CMAKE ON)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
message(STATUS "Boost include directory ${Boost_INCLUDE_DIR}")

# Add math lib
set(LIBS ${LIBS} m)

# Add PNL lib
set(CMAKE_PREFIX_PATH lib/pnlLib/build)
find_package(Pnl REQUIRED)
include_directories(lib/pnlLib/build/include/)
set(LIBS ${LIBS} ${PNL_LIBRARIES})

#Source files
set(RATE_SOURCE_FILES src/Pricer/RateModels/ConstantRateModel.cpp src/Pricer/RateModels/GamTauxRateModel.cpp)
set(PRICING_SOURCE_FILES src/Pricer/Pricing/PricerGen.cpp src/Pricer/Pricing/MonteCarloPricer.cpp ${SIMULATION_SOURCE_FILE})
set(SIMULATION_SOURCE_FILES src/Pricer/SimulationModels/ModelGen.cpp
        src/Pricer/SimulationModels/SimuChange/BlackScholesChangeModel.cpp
        src/Pricer/SimulationModels/SimuIndex/BlackScholesIndexModel.cpp ${RATE_SOURCE_FILES})
set(BASE_SOURCE_FILES src/Pricer/Marche.cpp src/Pricer/Multimonde.cpp)

# Add executables
add_executable(test_gamTauxRateModel.te src/Pricer/Tests/RateModelsTest/testGamTauxRateModel.cpp ${RATE_SOURCE_FILES})
target_link_libraries(test_gamTauxRateModel.te ${LIBS})
add_executable(test_constantRateModel.te src/Pricer/Tests/RateModelsTest/testConstantRateModel.cpp ${RATE_SOURCE_FILES})
target_link_libraries(test_constantRateModel.te ${LIBS})

# Pour dégriser les fichiers.cpp
add_executable(test.te src/Pricer/Tests/Test.cpp ${PRICING_SOURCE_FILES} ${SIMULATION_SOURCE_FILES} ${BASE_SOURCE_FILES})
target_link_libraries(test.te ${LIBS})
