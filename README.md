# Chord Implementation

## Overview
To begin, view the `main.cpp` file for instructions on compiling
or just invoke the command `make` and run `bin/release/Chord`

## Dependencies
1. C++11

## Known issues
1. Finger table may sometimes(?) be incorrect
2. Slow adding and removing of nodes due to steps needed to
   fix finger tables for every insertion and removal

## TODO
1. Implement graph data structure instead of using std::map
