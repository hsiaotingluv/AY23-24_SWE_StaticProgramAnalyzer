# /usr/bin/env python3

import os
import xml.etree.ElementTree as ET


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
        "--run_server", help="Run Autotester Server", default=False, action="store_true"
    )
    parser.add_argument(
        "--autotester",
        help="Path to autotester",
        default=os.path.join(
            FILE_PATH, "Code11", "build", "src", "autotester", "autotester"
        ),
    )
    args = parser.parse_args()

    assert is_file(args.source)
    assert is_file(args.query)

    output_name = os.path.basename(args.output)
    output_path = os.path.join(FILE_PATH, output_name)
    run_server = bool(args.run_server)

    if is_file(output_path) and not args.ignore_exists:
        raise ValueError(
            f"Output file {output_path} already exists! Either delete the file or specify something else."
        )

    exec_args = [args.autotester, args.source, args.query, output_path]
    subprocess.run(exec_args, check=False)
    tree = ET.parse(f"{output_path}")
    root = tree.getroot()

    errors = []

    def traverse(node):
        # do something with node
        if node.tag == "query":
            is_passing = False
            for child in node:
                # Using Python vars quirks here for fast, hackish style
                if child.tag == "id":
                    test_case_number = child.text
                elif child.tag == "failed":
                    is_passing = False
                elif child.tag == "passed":
                    is_passing = True

            if not is_passing:
                errors.append(test_case_number)
        else:
            for child in node:
                traverse(child)

    traverse(root)

    if run_server:
        path = os.path.join(FILE_PATH, "Code11", "tests")
        subprocess.run(["mv", f"{output_path}", "Code11/tests"], check=True)
        subprocess.run(
            ["python3", "-m", "http.server", "8080", "--directory", path],
            check=False,
        )

    if errors:
        print(f"[{args.source} - {args.query}] Failed test cases: {','.join(map(str, errors))}")
        exit(1)
    else:
        print(f"[{args.source} - {args.query}] Pass all system testing")
