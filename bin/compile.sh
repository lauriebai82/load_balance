#!/bin/sh

gcc ../src/server.c ../src/lb.c -o server
gcc ../src/client.c ../src/lb.c -o client
gcc ../src/test.c ../src/lb.c -o test
gcc ../src/proxy.c ../src/lb.c ../src/link.c -o proxy
