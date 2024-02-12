# /usr/bin/env python3

import os

PATH = os.path.dirname(os.path.realpath(__file__))
build_folder = "coverage_build"

if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(description="Run code coverage")
    parser.add_argument(
        "--build", help="Path to build folder", default=build_folder, type=str
    )
    args = parser.parse_args()

    build_folder = args.build
    BUILD_PATH = os.path.join(PATH, build_folder)
    if not os.path.exists(BUILD_PATH):
        os.system(f"mkdir {BUILD_PATH}")

    os.chdir(BUILD_PATH)
    os.system("cmake -DCMAKE_BUILD_TYPE=Debug ..")
    os.system("make SPA_coverage")

    # Run http server
    os.chdir(os.path.join(BUILD_PATH, "SPA_coverage"))
    os.system("python3 -m http.server 8080")
