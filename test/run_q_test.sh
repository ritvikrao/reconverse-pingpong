#!/bin/bash

g++ queue.C ../queue.C -o qtest -std=c++11 -pthread -I ../
./qtest

