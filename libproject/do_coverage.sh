#!/usr/bin/sh

cd test
./libproject_test
cd ../src
gcov -o CMakeFiles/project.dir/libproject.cpp.gcno ../../src/libproject.cpp
mkdir ../coverage_report
gcovr --object-directory= . -r ../../src/ --html --html-details -o ../coverage_report/report.html

exit 0
