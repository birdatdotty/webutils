#!/usr/bin/sh
g++ main.cc createHTML.cc `pkg-config --libs --cflags libsaveFile libhelperJSON` -o test
