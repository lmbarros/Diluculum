#
# SConstruct
# Diluculum
# By Leandro Motta Barros
#

import os
env = Environment (ENV = os.environ,
                   CC = 'g++',
                   CCFLAGS = '-Wall',
                   LIBS=["lua", "lualib", "boost_filesystem-gcc-mt", "dl",
                         "boost_unit_test_framework-gcc-mt"],
                   CPPPATH = "Sources")

env.Program ("Tests/TestLuaValue", [ "Sources/LuaUtils.cpp",
                                     "Sources/LuaValue.cpp",
                                     "Tests/TestLuaValue.cpp" ])

env.Program ("Tests/TestLuaVariable", [ "Sources/LuaState.cpp",
                                        "Sources/LuaUtils.cpp",
                                        "Sources/LuaValue.cpp",
                                        "Sources/LuaVariable.cpp",
                                        "Tests/TestLuaVariable.cpp" ])

env.Program ("Tests/TestLuaState", [ "Sources/LuaState.cpp",
                                     "Sources/LuaUtils.cpp",
                                     "Sources/LuaValue.cpp",
                                     "Sources/LuaVariable.cpp",
                                     "Tests/TestLuaState.cpp" ])
