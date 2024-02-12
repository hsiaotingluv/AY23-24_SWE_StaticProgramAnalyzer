#!/usr/bin/env bash

MODE="all" # One of ["all", "sp", "pkb", "qps", "tests"]

usage() {
    echo "Usage: run_clang_tidy.sh [all|sp|pkb|qps|tests]"
}

if [ "$#" -eq 0 ]; then
    echo "Running clang-tidy on all files in the project"
elif [ "$#" -eq 1 ]; then
    MODE=$1
else
    usage
    exit 1
fi

run_clang_tidy() {
    sleep 2
    find "$1" \( -iname '*.h' -o -iname '*.cpp' -o -iname '*.hpp' \) -exec clang-tidy {} -p build/compile_commands.json\
    -header-filter=.*\
    -checks=-*,bugprone-*,clang-analyzer-*,cppcoreguidelines-*,performance-*,portability-*,readability-*,-readability-identifier-length,-readability-else-after-return \;
}

run_clang_format() {
    find "$1" \( -iname '*.h' -o -iname '*.cpp' -o -iname '*.hpp' \) -exec clang-format -style=file -i {} \;
}

run() {
    run_clang_format $1
    run_clang_tidy $1
}


if test $MODE = "all"; then
    echo "Lint all files in the project"
    run src/
elif test $MODE = "sp"; then
    echo "Lint on all files in SP"
    run src/spa/src/sp
elif test $MODE = "pkb"; then
    echo "Lint on all files in PKB"
    run src/spa/src/PKB
elif test $MODE = "qps"; then
    echo "Lint on all files in QPS"
    run src/spa/src/qps
elif test $MODE = "tests"; then
    echo "Lint all tests"
    run src/unit_testing/src
    run src/integration_testing/src
else
    echo "Invalid mode: ${MODE}"
    usage
fi
