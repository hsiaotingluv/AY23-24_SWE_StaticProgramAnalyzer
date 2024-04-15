import os
import subprocess
from typing import List, Optional

QUERIES_NAME = "generated_queries.txt"
SOURCE_NAME = "generated_source.txt"
OUTPUT_NAME = "raw_final_queries.txt"


def get_queries() -> List[str]:
    with open("queries.txt", "r", encoding="utf-8") as f:
        return [line.strip() for line in f.readlines() if line.strip() != ""]


def generate_nice_files(queries: List[str], answers: Optional[List[str]]) -> None:
    timeout = 5000
    with open(QUERIES_NAME, "w+", encoding="utf-8") as f:
        if answers is None:
            for i, query in enumerate(queries, start=1):
                f.write(f"{i} - generated TC {i}\n")
                f.write(f"{query}\n\n\n")
                f.write(f"{timeout}\n")
        else:
            for i, (query, answer) in enumerate(zip(queries, answers), start=1):
                total_items = 1
                if "<" in query and ">" in query:
                    start = query.index("<")
                    end = query.index(">")
                    q = query[start + 1 : end]
                    total_items = len(q.split(","))

                tokens = answer.split()
                answer = ", ".join(
                    " ".join(tokens[i : i + total_items])
                    for i in range(0, len(tokens), total_items)
                )

                f.write(f"{i} - generated TC {i}\n")
                f.write(f"{query}\n\n")
                f.write(f"{answer}\n")
                f.write(f"{timeout}\n")


def run_autotester():
    current_script_path = os.getcwd()
    parent_dir = os.path.dirname(current_script_path)
    os.chdir(parent_dir)

    command = f"python3 run_autotester.py Tests11/generated_source.txt -i > Tests11/{OUTPUT_NAME}"
    subprocess.run(command, shell=True, check=False)

    os.chdir(current_script_path)


def main():
    q = get_queries()
    generate_nice_files(q, None)
    run_autotester()

    answers = []
    with open(OUTPUT_NAME, "r", encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if "Your answer" in line:
                ans = line.split(":")[1:][0].strip()
                answers.append(ans)

    generate_nice_files(q, answers)


if __name__ == "__main__":
    main()
