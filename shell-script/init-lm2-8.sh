#!/bin/bash

# 出力ファイル
output_file="output_lm.txt"

# 初期化
> "$output_file"

# 固定値
fixed_value="4014000000000000"

# m: 0 ~ 15
for m in $(seq 0 15); do
  # p: 0 ~ 3
  for p in $(seq 0 3); do
    # 出力 lm2, lm4, lm6, lm8
    printf "d set \$lm2m%dp%d 1 %s\n" "$m" "$p" "$fixed_value" >> "$output_file"
    printf "d set \$lm4m%dp%d 1 %s\n" "$m" "$p" "$fixed_value" >> "$output_file"
    printf "d set \$lm6m%dp%d 1 %s\n" "$m" "$p" "$fixed_value" >> "$output_file"
    printf "d set \$lm8m%dp%d 1 %s\n" "$m" "$p" "$fixed_value" >> "$output_file"
  done
done

# 完了メッセージ
echo "Configuration has been written to $output_file"
