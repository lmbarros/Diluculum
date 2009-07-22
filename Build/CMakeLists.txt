#
# Diluculum build system
# Leandro Motta Barros
#

# General settings
project(Diluculum)
cmake_minimum_required(VERSION 2.6)


# Add CTest support
enable_testing()


# Packages
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost 1.39 COMPONENTS unit_test_framework REQUIRED)
add_definitions(-DBOOST_ALL_DYN_LINK)


# Include directories
include_directories(${Boost_INCLUDE_DIRS}
                    ${CMAKE_SOURCE_DIR}/include)

# Link directories
link_directories(${Boost_LIBRARY_DIRS})


# Build the library
set(DiluculumSources
    Sources/InternalUtils.cpp
    Sources/LuaExceptions.cpp
    Sources/LuaFunction.cpp
    Sources/LuaState.cpp
    Sources/LuaUserData.cpp
    Sources/LuaUtils.cpp
    Sources/LuaValue.cpp
    Sources/LuaVariable.cpp
    Sources/LuaWrappers.cpp)

add_library(Diluculum STATIC ${DiluculumSources})

target_link_libraries(Diluculum lua dl)

# Now, the unit tests
function(AddUnitTest name)
    add_executable(${name} Tests/${name}.cpp)
    target_link_libraries(${name}
                          ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                          Diluculum)
    add_test(${name} ${name})
endfunction(AddUnitTest)

AddUnitTest(TestLuaFunction)
AddUnitTest(TestLuaState)
AddUnitTest(TestLuaUserData)
AddUnitTest(TestLuaUtils)
AddUnitTest(TestLuaValue)
AddUnitTest(TestLuaVariable)
AddUnitTest(TestLuaWrappers)
