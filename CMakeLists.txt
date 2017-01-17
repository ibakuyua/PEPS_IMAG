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
set(RATE_SOURCE_FILES src/Pricer/RateModels/RateModelGen.cpp src/Pricer/RateModels/ConstantRateModel.cpp src/Pricer/RateModels/GamTauxRateModel.cpp)
set(PRICING_SOURCE_FILES src/Pricer/Pricing/PricerGen.cpp src/Pricer/Pricing/MonteCarloPricer.cpp ${SIMULATION_SOURCE_FILE})
set(SIMULATION_SOURCE_FILES src/Pricer/SimulationModels/ModelGen.cpp src/Pricer/SimulationModels/BlackScholesModel.cpp ${RATE_SOURCE_FILES})
set(PRODUIT_SOURCE_FILES src/Pricer/FinancialProducts/ProductGen.cpp
        src/Pricer/FinancialProducts/Asset.cpp src/Pricer/PayOffs/PayOffFunctions.cpp)
set(BASE_SOURCE_FILES src/Pricer/Marche.cpp src/Pricer/Parser/ParseCSV.cpp src/Pricer/Parser/ParseCSV.h src/Pricer/Tests/Parser/testParseCSV.cpp)
set(STATS_SOURCE_FILES src/Pricer/Stats/StatsFactory.cpp ${STATS_SOURCE_FILES})
set(STATS_SOURCE_FILES src/Pricer/Parser/ParseCSV.cpp ${STATS_SOURCE_FILES})
# Add executables
#Test for rate model
add_executable(test_gamTauxRateModel.te src/Pricer/Tests/RateModelsTest/testGamTauxRateModel.cpp ${RATE_SOURCE_FILES})
target_link_libraries(test_gamTauxRateModel.te ${LIBS})
add_executable(test_constantRateModel.te src/Pricer/Tests/RateModelsTest/testConstantRateModel.cpp ${RATE_SOURCE_FILES})
target_link_libraries(test_constantRateModel.te ${LIBS})
#Test for pricer
add_executable(test_pricerMC.te src/Pricer/Tests/PricerTest/testMonteCarlo.cpp src/Pricer/FinancialProducts/Call.cpp
        ${PRODUIT_SOURCE_FILES} ${PRICING_SOURCE_FILES} ${SIMULATION_SOURCE_FILES})
target_link_libraries(test_pricerMC.te ${LIBS})
#Test for stats
add_executable(test_stats.te src/Pricer/Tests/StatsFactoryTests/testStats.cpp ${STATS_SOURCE_FILES})
target_link_libraries(test_stats.te ${LIBS})
#Test for parsingCSV
add_executable(test_parseCSV.te src/Pricer/Tests/Parser/testParseCSV.cpp ${STATS_SOURCE_FILES})
target_link_libraries(test_parseCSV.te ${LIBS})

# Global executable
add_executable(test_multimonde.te src/Pricer/Tests/testMultimonde.cpp src/Pricer/FinancialProducts/Multimonde.cpp
        ${PRICING_SOURCE_FILES} ${SIMULATION_SOURCE_FILES} ${BASE_SOURCE_FILES}
        ${STATS_SOURCE_FILES} ${PRODUIT_SOURCE_FILES} ${BASE_SOURCE_FILES})
target_link_libraries(test_multimonde.te ${LIBS})
