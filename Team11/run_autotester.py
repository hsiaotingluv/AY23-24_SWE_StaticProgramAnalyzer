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

    verbose = True
    single_file_mode = False
    if is_file(args.source) and is_file(args.query):
        files = [(args.source, args.query)]
        single_file_mode = True
    elif not is_file(args.source):
        assert os.path.exists(args.source) and os.path.isdir(
            args.source
        ), f"{args.source} is not a directory!"
        assert not args.run_server, "Cannot run server with multiple source files!"

        # Deduce source files
        source_files = [
            os.path.join(args.source, f)
            for f in os.listdir(args.source)
            if f.endswith("_source.txt")
        ]

        if len(source_files) == 0:
            raise ValueError(f"No source file in directory {args.source}!")

        # Deduce query files
        query_files = [f.replace("_source.txt", "_queries.txt") for f in source_files]
        files = []
        for source, query in zip(source_files, query_files):
            if not is_file(query):
                print(f"Query file {query} does not exist. Skipping {source}")
            else:
                files.append((source, query))
        verbose = False
    elif not is_file(args.query):
        deduced_name = args.source.replace("_source.txt", "_queries.txt")
        print(f"Query file {args.query} does not exist. Trying {deduced_name}")
        files = [(args.source, deduced_name)]
        single_file_mode = True
    else:
        raise ValueError(f"Neither {args.source} nor {args.query} exist!")

    for source, query in files:
        total_tc_file = 0
        output_name = os.path.basename(args.output)
        output_path = os.path.join(FILE_PATH, output_name)
        run_server = bool(args.run_server)

        if is_file(output_path) and not args.ignore_exists:
            raise ValueError(
                f"Output file {output_path} already exists! Either delete the file or specify something else."
            )

        exec_args = [args.autotester, source, query, output_path]
        result = subprocess.run(
            exec_args,
            check=False,
            stderr=subprocess.PIPE,
            stdout=subprocess.DEVNULL if not verbose else None,
            text=True,
        )

        prefix = source.split(".")[0]
        found_exception = None
        try:
            tree = ET.parse(f"{output_path}")
            root = tree.getroot()

            errors = []

            def traverse(node):
                global total_tc_file
                # do something with node
                if node.tag == "query":
                    is_passing = False
                    for child in node:
                        # Using Python vars quirks here for fast, hackish style
                        if child.tag == "id":
                            test_case_number = child.text
                            total_tc_file += 1
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
            found_exception = result.stderr.replace("\n", " ")

        if found_exception and should_fail_early(source):
            print(f"[{prefix}] Pass all system testing (1/1)")
        elif found_exception and not should_fail_early(source):
            print(
                f"[{prefix}] Failed to parse the SIMPLE program due to {found_exception}"
            )
            exit(1)
        elif not found_exception and should_fail_early(source):
            print(f"[{prefix}] Failed, SPA successfully parsed invalid SIMPLE program")
            exit(1)
        else:
            if errors:
                print(
                    f"[{prefix}] Failed test cases: {','.join(map(str, errors))} ({total_tc_file - len(errors)}/{total_tc_file})"
                )
                exit(1)
            else:
                print(
                    f"[{prefix}] Pass all system testing ({total_tc_file}/{total_tc_file})"
                )

    if not single_file_mode:
        print("All tests passed!")

    if run_server:
        path = os.path.join(FILE_PATH, "Code11", "tests")
        subprocess.run(["mv", f"{output_path}", "Code11/tests"], check=True)
        subprocess.run(
            ["python3", "-m", "http.server", "8080", "--directory", path],
            check=False,
        )
