#!/bin/bash

input_file="replace_input.txt"
middle_file1="middle1.txt"
middle_file2="middle2.txt"
output_file="replace_output.txt"

sed -E 's/l1bmp \$lb[0-9] $t/ \nnop\/2/g' "$input_file" > "$middle_file1"
sed -E 's/#nointr/ \nnop\/2/g' "$middle_file1" > "$middle_file2"
sed -E 's/lxor \$ti (\$ln[0-9]+v )\$t \$omr2; nointr/lxor \$t \1 \$t \$omr2; \nnop\/2/g' "$middle_file2" > "$output_file"
