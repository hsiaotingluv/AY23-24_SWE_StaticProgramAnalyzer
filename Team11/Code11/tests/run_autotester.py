# /usr/bin/env python3

import os


def is_file(filename):
    return os.path.isfile(filename) and os.path.exists(filename)


FILE_PATH = os.path.dirname(os.path.abspath(__file__))
if __name__ == "__main__":
    import argparse
    import subprocess

    parser = argparse.ArgumentParser(description="Run the autotester.")
    parser.add_argument("source", help="The source file to test.")
    parser.add_argument("query", help="The query file.")
    parser.add_argument("-i", "--ignore_exists", action="store_true")
    parser.add_argument("--output", help="Output xml", default="out.xml")
    parser.add_argument(
        "--autotester",
        help="Path to autotester",
        default=os.path.join(
            FILE_PATH, "..", "build", "src", "autotester", "autotester"
        ),
    )
    args = parser.parse_args()

    assert is_file(args.source)
    assert is_file(args.query)

    output_name = os.path.basename(args.output)
    output_path = os.path.join(FILE_PATH, output_name)

    if is_file(output_path) and not args.ignore_exists:
        raise ValueError(
            f"Output file {output_path} already exists! Either delete the file or specify something else."
        )

    exec_args = [args.autotester, args.source, args.query, output_path]
    subprocess.run(exec_args, check=False)
    subprocess.run(
        ["python3", "-m", "http.server", "8080", "--directory", FILE_PATH], check=False
    )
