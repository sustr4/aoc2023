#!/bin/bash

make
./day24 > day24.smt2

z3 day24.smt2
