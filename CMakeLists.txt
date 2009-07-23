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
find_package(Lua51 REQUIRED)
add_definitions(-DBOOST_ALL_DYN_LINK)


# Include directories
include_directories(${Boost_INCLUDE_DIRS}
                    ${LUA_INCLUDE_DIR}
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

target_link_libraries(Diluculum ${LUA_LIBRARIES})

if(${CMAKE_SYSTEM_NAME} MATCHES Linux)
    target_link_libraries(Diluculum dl)
endif(${CMAKE_SYSTEM_NAME} MATCHES Linux)

# Now, the unit tests
function(AddUnitTest name)
    add_executable(${name} Tests/${name}.cpp)
    target_link_libraries(${name}
                          ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                          ${LUA_LIBRARIES}
                          Diluculum)
    if(${CMAKE_SYSTEM_NAME} MATCHES Linux)
        target_link_libraries(Diluculum dl)
    endif(${CMAKE_SYSTEM_NAME} MATCHES Linux)
    add_test(${name} ${name})
endfunction(AddUnitTest)

add_library(ATestModule SHARED Tests/ATestModule.cpp)
target_link_libraries(ATestModule
    ${LUA_LIBRARIES}
    Diluculum)
set_target_properties(ATestModule
    PROPERTIES PREFIX "")

AddUnitTest(TestLuaFunction)
AddUnitTest(TestLuaState)
AddUnitTest(TestLuaUserData)
AddUnitTest(TestLuaUtils)
AddUnitTest(TestLuaValue)
AddUnitTest(TestLuaVariable)
AddUnitTest(TestLuaWrappers)

# Copy the files needed by the unit tests
configure_file(${CMAKE_SOURCE_DIR}/Tests/ReturnThread.lua
    ${CMAKE_BINARY_DIR}/ReturnThread.lua
    COPYONLY)

configure_file(${CMAKE_SOURCE_DIR}/Tests/SyntaxError.lua
    ${CMAKE_BINARY_DIR}/SyntaxError.lua
    COPYONLY)

configure_file(${CMAKE_SOURCE_DIR}/Tests/TestLuaStateDoFile.lua
    ${CMAKE_BINARY_DIR}/TestLuaStateDoFile.lua
    COPYONLY)

configure_file(${CMAKE_SOURCE_DIR}/Tests/TestLuaStateDoFileNoReturn.lua
    ${CMAKE_BINARY_DIR}/TestLuaStateDoFileNoReturn.lua
    COPYONLY)
