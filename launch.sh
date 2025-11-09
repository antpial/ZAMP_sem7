#!/bin/bash

# Go to server directory and build
cd serwer-z-tlem-qt5-qt6
make &

sleep 5

# Return to base directory and build
cd ..
make
