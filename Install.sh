#!/bin/sh

if [ $# != 1 ]; then
    echo "Usage: Install <version>"
    exit 0
fi

scons mode=opt
sudo mkdir "/Programs/Diluculum"
sudo mkdir "/Programs/Diluculum/$1"
sudo mkdir "/Programs/Diluculum/$1/docs"

sudo cp -r lib include README.txt HISTORY.txt "/Programs/Diluculum/$1"
sudo rm "/Programs/Diluculum/$1/lib/.sconsign"

doxygen

sudo cp -r Documentation/html/* "/Programs/Diluculum/$1/docs"

SymlinkProgram Diluculum "$1"
