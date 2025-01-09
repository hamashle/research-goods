#!/bin/bash

# 出力ファイル
output_file="output_lm_set_20-56.txt"
> "$output_file"

# lm 番号のリスト (繰り返し使用)
lm_numbers=(20 32 44 56)

# ループでコードを生成
for (( m=0; m<16; m++ )); do
  for (( p=0; p<4; p++ )); do
    for lm in "${lm_numbers[@]}"; do
      # 値を文字列として計算
      value="0000000000${lm}$(printf "%02X" $m)$(printf "%02X" $p)"

      # d set コマンドを生成
      echo "d set \$lm${lm}m${m}p${p} 1 $value" >> "$output_file"
    done
  done
done

# 完了メッセージ
echo "Configuration has been written to $output_file"
