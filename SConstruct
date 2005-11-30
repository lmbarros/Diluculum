#
# SConstruct
# Diluculum
# By Leandro Motta Barros
#


# ------------------------------------------------------------------------------
#  The "base" building environment
#  This is used as a template to create the more specific building environments
#  that are actually used to build something.
# ------------------------------------------------------------------------------

# Inherit the environment from, well, from the environment.
# TODO: I suppose that's not the best way to create a 'SConstruct'. But for a
#       rootless GoboLinux user (with installed in his own $HOME), this
#       simplifies the things quite a lot.
import os
envBase = Environment (ENV = os.environ,
                       CPPPATH = "#/include",
                       LIBPATH = "#/lib")

# Add some flags manually...
if envBase["CXX"] == "g++":
    envBase["CXXFLAGS"] += " -Wall"

    buildMode = ARGUMENTS.get ("mode", "no-opt")

    if buildMode == "opt":
        envBase["CXXFLAGS"] += " -O3"
    elif buildMode == "debug":
        envBase["CXXFLAGS"] += " -g"
        envBase["LINKFLAGS"] += " -g"
    elif buildMode == "profile":
        envBase["CXXFLAGS"] += " -g -pg"
        envBase["LINKFLAGS"] += " -g -pg"

# A friendly help message...
envBase.Help("""
Diluculum build system

Just type 'scons' to build everything. Or, use the following option to
select the compilation mode:

   mode=<MODE>, where <MODE> is one of the following:
      o 'profile', for compiling with profiling ('gprof') support
      o 'debug', for compiling with debug support
      o 'no-opt' (the default), for compiling without optimizations or
        debug support or anything else
      o 'opt', for compiling with optimizations enabled
""")


# ------------------------------------------------------------------------------
#  The library building environment
#  This is one is used to build the Diluculum library. Currently, it is no
#  different than the base building environment.
# ------------------------------------------------------------------------------
envLib = envBase.Copy()


# ------------------------------------------------------------------------------
#  The tests building environment
#  This is one is used to build the Diluculum unit tests.
# ------------------------------------------------------------------------------
envTests = envBase.Copy (LIBS = ["Diluculum", "lua", "lualib", "dl",
                                 "boost_filesystem-gcc-mt",
                                 "boost_unit_test_framework-gcc-mt"])



# ------------------------------------------------------------------------------
#  The build targets
#  The things that are actually built.
# ------------------------------------------------------------------------------
envLib.Library ("lib/Diluculum", [ "Sources/LuaExceptions.cpp",
                                   "Sources/LuaState.cpp",
                                   "Sources/LuaUtils.cpp",
                                   "Sources/LuaValue.cpp",
                                   "Sources/LuaVariable.cpp",
                                   "Sources/MakeLuaFunction.cpp"])

envTests.Program ("Tests/TestLuaValue", "Tests/TestLuaValue.cpp")
envTests.Program ("Tests/TestLuaVariable", "Tests/TestLuaVariable.cpp")
envTests.Program ("Tests/TestLuaUtils", "Tests/TestLuaUtils.cpp")
envTests.Program ("Tests/TestLuaState", "Tests/TestLuaState.cpp")
envTests.Program ("Tests/TestMakeLuaFunction", "Tests/TestMakeLuaFunction.cpp")
