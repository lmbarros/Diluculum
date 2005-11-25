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

env.Program ("Tests/TestLuaValue", [ "Sources/LuaExceptions.cpp",
                                     "Sources/LuaUtils.cpp",
                                     "Sources/LuaValue.cpp",
                                     "Tests/TestLuaValue.cpp" ])

env.Program ("Tests/TestLuaVariable", [ "Sources/LuaExceptions.cpp",
                                        "Sources/LuaState.cpp",
                                        "Sources/LuaUtils.cpp",
                                        "Sources/LuaValue.cpp",
                                        "Sources/LuaVariable.cpp",
                                        "Tests/TestLuaVariable.cpp" ])

env.Program ("Tests/TestLuaUtils", [ "Sources/LuaExceptions.cpp",
                                     "Sources/LuaUtils.cpp",
                                     "Sources/LuaValue.cpp",
                                     "Tests/TestLuaUtils.cpp" ])

env.Program ("Tests/TestLuaState", [ "Sources/LuaExceptions.cpp",
                                     "Sources/LuaState.cpp",
                                     "Sources/LuaUtils.cpp",
                                     "Sources/LuaValue.cpp",
                                     "Sources/LuaVariable.cpp",
                                     "Tests/TestLuaState.cpp" ])

env.Program ("Tests/TestMakeLuaFunction", [ "Sources/LuaExceptions.cpp",
                                            "Sources/LuaState.cpp",
                                            "Sources/LuaUtils.cpp",
                                            "Sources/LuaValue.cpp",
                                            "Sources/LuaVariable.cpp",
                                            "Sources/MakeLuaFunction.cpp",
                                            "Tests/TestMakeLuaFunction.cpp" ])
