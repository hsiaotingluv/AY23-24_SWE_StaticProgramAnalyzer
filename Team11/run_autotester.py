# /usr/bin/env python3

import os
import xml.etree.ElementTree as ET


def is_file(filename):
    return os.path.isfile(filename) and os.path.exists(filename)


def should_fail_early(filename):
    return "invalid" in filename and "simple" in filename


FILE_PATH = os.path.dirname(os.path.abspath(__file__))
if __name__ == "__main__":
    import argparse
    import subprocess

    parser = argparse.ArgumentParser(description="Run the autotester.")
    parser.add_argument("source", help="The source file to test.")
    parser.add_argument("query", nargs="?", help="The query file.", default="")
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

    if not is_file(args.query):
        deduced_name = args.source.replace("_source.txt", "_queries.txt")
        print(f"Query file {args.query} does not exist. Trying {deduced_name}")
        args.query = deduced_name

    assert is_file(args.query)

    output_name = os.path.basename(args.output)
    output_path = os.path.join(FILE_PATH, output_name)
    run_server = bool(args.run_server)

    if is_file(output_path) and not args.ignore_exists:
        raise ValueError(
            f"Output file {output_path} already exists! Either delete the file or specify something else."
        )

    exec_args = [args.autotester, args.source, args.query, output_path]
    result = subprocess.run(exec_args, check=False, stderr=subprocess.PIPE, text=True)

    total_tc = 0
    fix_folder = "Team11/Tests11/"
    prefix = args.source.split('.')[0].replace(fix_folder, "")
    try:
        tree = ET.parse(f"{output_path}")
        root = tree.getroot()

        errors = []


        def traverse(node):
            global total_tc
            # do something with node
            if node.tag == "query":
                is_passing = False
                for child in node:
                    # Using Python vars quirks here for fast, hackish style
                    if child.tag == "id":
                        test_case_number = child.text
                        total_tc += 1
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
    except Exception:
        if should_fail_early(args.source):
            print(
                f"[{prefix}] Pass all system testing (1/1)"
            )

            exit(0)
        else:
            sanitized_stderr = result.stderr.replace("\n", " ")
            print(
                f"[{prefix}] Failed to parse the SIMPLE program due to {sanitized_stderr}"
            )
            exit(1)

    if should_fail_early(args.source):
        print(
            f"[{prefix}] Failed, SPA successfully parsed invalid SIMPLE program"
        )

        exit(1)

    if run_server:
        path = os.path.join(FILE_PATH, "Code11", "tests")
        subprocess.run(["mv", f"{output_path}", "Code11/tests"], check=True)
        subprocess.run(
            ["python3", "-m", "http.server", "8080", "--directory", path],
            check=False,
        )

    if errors:
        print(
            f"[{prefix}] Failed test cases: {','.join(map(str, errors))} ({total_tc - len(errors)}/{total_tc})"
        )
        exit(1)
    else:
        print(f"[{prefix}] Pass all system testing ({total_tc}/{total_tc})")
        exit(0)
