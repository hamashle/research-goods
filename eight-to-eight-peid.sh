#!/bin/bash

# 出力ファイル
output_file="output_016.txt"

# 初期化
> "$output_file"

# 固定値
minus_one="BFF0000000000000"

# PE番号（#0 ~ #63）
for pe_number in $(seq 0 63); do
  # m と p を計算
  m=$((pe_number / 4))
  p=$((pe_number % 4))

  # UP（lr32）
  if [ "$pe_number" -lt 8 ]; then
    up="$minus_one"
  else
    up=$(printf "%016X" $((pe_number - 8)))
  fi

  # DOWN（lr34）
  if [ "$pe_number" -ge 56 ]; then
    down="$minus_one"
  else
    down=$(printf "%016X" $((pe_number + 8)))
  fi

  # RIGHT（lr36）
  if [ $(( (pe_number + 1) % 8 )) -eq 0 ]; then
    right="$minus_one"
  else
    right=$(printf "%016X" $((pe_number + 1)))
  fi

  # LEFT（lr38）
  if [ $((pe_number % 8)) -eq 0 ]; then
    left="$minus_one"
  else
    left=$(printf "%016X" $((pe_number - 1)))
  fi

  # 出力
  printf "d set \$lr32m%dp%d 1 %s #PEID=%016X (UP=%s)\n" "$m" "$p" "$up" "$pe_number" "$up" >> "$output_file"
  printf "d set \$lr34m%dp%d 1 %s #PEID=%016X (DOWN=%s)\n" "$m" "$p" "$down" "$pe_number" "$down" >> "$output_file"
  printf "d set \$lr36m%dp%d 1 %s #PEID=%016X (RIGHT=%s)\n" "$m" "$p" "$right" "$pe_number" "$right" >> "$output_file"
  printf "d set \$lr38m%dp%d 1 %s #PEID=%016X (LEFT=%s)\n" "$m" "$p" "$left" "$pe_number" "$left" >> "$output_file"

  # 改行を追加
  echo "" >> "$output_file"
done

# 完了メッセージ
echo "Configuration has been written to $output_file"
