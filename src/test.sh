#!/bin/sh

./cc $1 > tmp.s
gcc tmp.s -o app
./app
echo $?