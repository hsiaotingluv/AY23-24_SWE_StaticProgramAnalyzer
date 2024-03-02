#!/bin/bash

accumulated_result=""

for source_file in $(find "Team11/Tests11" -type f -name "*_source.txt"); do
  input_file="${source_file/_source.txt/_queries.txt}"

  # Skip if "Milestone1" is in the source file name
  if [[ $source_file == *"Milestone1"* ]]; then
    continue
  fi

  output=$(python3 Team11/run_autotester.py "$source_file" "$input_file" -i)

  # Extract the last line of the output
  result=$(echo "$output" | tail -n 1)

  # Accumulate the result
  accumulated_result="${accumulated_result}\n${result}"
done

echo -e $accumulated_result

# Check if accumulated_result contains "Failed"
if [[ $accumulated_result == *"Failed"* ]]; then
    echo "Some tests are failing. Exiting with status code 1."
    exit 1
else
    echo "All tests passed. Exiting with status code 0."
    exit 0
fi

