#!/bin/bash
echo "[build]"
make clean
make TEST=yes DEBUG=yes LOG_LEVEL=LOG_DEBUG
echo "[build done]"
./ysh