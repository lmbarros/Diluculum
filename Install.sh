#!/bin/sh

# If this script has any chance of working, it is under GoboLinux.
if [ ! $goboPrograms ]; then
    echo "Sorry, this ad hoc installation script will work only under GoboLinux"
    echo "(or, at least, Rootless GoboLinux). And, frankly, even under"
    echo "GoboLinux things are not guaranteed..."
    exit 1
fi

# Check command-line parameters
if [ $# != 1 ]; then
    echo "Usage: Install <version>"
    exit 1
fi

# If we are running in a "real" (not rootless) GoboLinux, we'll have to use
# 'sudo' to perform some instalation steps.
if [ ! $ROOTLESS_GOBOLINUX ]; then
    SUDO=sudo
fi

# If necessary, recompile the library (the optimized version)
scons mode=opt

# Remake the documentation
doxygen

# Perform the actual instalation
$SUDO mkdir -p "${goboPrograms}/Diluculum/$1/docs"
$SUDO cp -r lib include README.txt HISTORY.txt "${goboPrograms}/Diluculum/$1"
$SUDO rm "${goboPrograms}/Diluculum/$1/lib/.sconsign"
$SUDO cp -r Documentation/html/* "${goboPrograms}/Diluculum/$1/docs"
SymlinkProgram Diluculum "$1"

# Done!
exit 0
