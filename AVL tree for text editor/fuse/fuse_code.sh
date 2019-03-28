#!/bin/sh

cat ../gtest/gtest.h ../gmock/gmock.h ../gmock-gtest-all.cc ../test_cases.cc | 
grep -v '#include "gtest/.*$' | 
grep -v '#include "gmock/.*$' > proj1_tester.cc

cp ../API.h ./

g++ proj1_tester.cc ../naive_implementation.cc -lpthread -o proj1_tester
./proj1_tester
rm proj1_tester
