#! /bin/bash

CFLAGS="-fsanitize=address -fno-omit-frame-pointer -static-libasan" make -j10
