#!/bin/bash
echo "[build]"
make clean
make TEST=no DEBUG=no LOG_LEVEL=LOG_PANIC
echo "[build done]"
./ysh