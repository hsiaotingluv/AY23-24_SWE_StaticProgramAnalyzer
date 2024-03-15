#!/usr/bin/env bash

MODE="all" # One of ["all", "sp", "pkb", "qps", "tests"]
RUN_CLANG_TIDY="true"
VERBOSE="false"

usage() {
    echo "Usage: run_lint.sh [all|sp|pkb|qps|tests] (run_clang_tidy: [true|false])"
    echo "Example ./run_lint.sh all false"
}

if [ "$#" -eq 0 ]; then
    :
elif [ "$#" -eq 1 ]; then
    MODE=$1
elif [ "$#" -eq 2 ]; then
    MODE=$1
    RUN_CLANG_TIDY=$2
else
    usage
    exit 1
fi

check_compile_commands_json() {
    PATH_TO_COMPILE_COMMANDS=$(find -iname 'compile_commands.json' -exec echo {} \;)
    if [ -f "$PATH_TO_COMPILE_COMMANDS" ]; then
        echo "compile_commands.json found at ${PATH_TO_COMPILE_COMMANDS}"
        echo ""
    else
        echo "Looking for compile_commands.json..."
        echo "Found: ${PATH_TO_COMPILE_COMMANDS}"
        echo ""
        echo "If compile_commands.json is not found, please run 'cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .'"
        echo "in the build directory to generate it. If there are multiple compile_commands.json files, please remove the extras."
        PATH_TO_COMPILE_COMMANDS=1
    fi
}

run_clang_tidy() {
    echo "Running clang-tidy on all files in $1"

    if [ $VERBOSE = "true" ]; then
        find "$1" \( -iname '*.h' -o -iname '*.cpp' -o -iname '*.hpp' -o -iname '*.tpp' \) -exec echo {} \;
    fi
    sleep 2
    find "$1" \( -iname '*.h' -o -iname '*.cpp' -o -iname '*.hpp' -o -iname '*.tpp' \) -exec clang-tidy {} -p $2\
    -header-filter=.*\
    -checks=-*,bugprone-*,clang-analyzer-*,cppcoreguidelines-*,performance-*,portability-*,readability-*,-readability-identifier-length,-readability-else-after-return,-bugprone-easily-swappable-parameters,-readability-named-parameter,-cppcoreguidelines-special-member-functions \;
}

run_clang_format() {
    find "$1" \( -iname '*.h' -o -iname '*.cpp' -o -iname '*.hpp' -o -iname '*.tpp' \) -exec clang-format -style=file -i {} \;
}

run() {    
    run_clang_format $1
    if test $RUN_CLANG_TIDY = "true"; then
      run_clang_tidy $1 $2
    fi
}

check_compile_commands_json
if [ ${PATH_TO_COMPILE_COMMANDS} = 1 ]; then
    exit 1
fi

SP_PATHS=("src/spa/src/sp/" "src/spa/include/sp/")
PKB_PATHS=("src/spa/src/pkb/" "src/spa/include/pkb/")
QPS_PATHS=("src/spa/src/qps/" "src/spa/include/qps/")
TEST_PATHS=("src/unit_testing/" "src/integration_testing/")
ALL_PATHS=("src/spa" "src/unit_testing" "src/integration_testing/" "src/autotester")

if test $MODE = "all"; then
    echo "Lint all files in the project"

    for i in "${ALL_PATHS[@]}"
    do
        run $i $PATH_TO_COMPILE_COMMANDS
    done
elif test $MODE = "sp"; then
    echo "Lint on all files in SP"

    for i in "${SP_PATHS[@]}"
    do
        run $i $PATH_TO_COMPILE_COMMANDS
    done
elif test $MODE = "pkb"; then
    echo "Lint on all files in PKB"

    for i in "${PKB_PATHS[@]}"
    do
        run $i $PATH_TO_COMPILE_COMMANDS
    done
elif test $MODE = "qps"; then
    echo "Lint on all files in QPS"

    for i in "${QPS_PATHS[@]}"
    do
        run $i $PATH_TO_COMPILE_COMMANDS
    done
elif test $MODE = "tests"; then
    echo "Lint all tests"

    for i in "${TEST_PATHS[@]}"
    do
        run $i $PATH_TO_COMPILE_COMMANDS
    done
else
    echo "Invalid mode: ${MODE}"
    usage
fi
