#!/bin/bash

# 出力ファイル
output_file="output.txt"

# 初期化
> "$output_file"

# IEEE754倍精度の値を16進数に変換する関数
to_hex_ieee754() {
  python3 -c "import struct; print('{:016X}'.format(struct.unpack('>Q', struct.pack('>d', $1))[0]))"
}

# 固定値 (IEEE754形式に変換)
minus_one=$(to_hex_ieee754 -1.0)
zero=$(to_hex_ieee754 0.0)

# PE番号（#0 ~ #63）
pe_number=0

# m: 0 ~ 15 (10進数)
for m in $(seq 0 15); do
  # p: 0 ~ 3 (10進数)
  for p in $(seq 0 3); do
    # PEID を計算
    pe_id=$((m * 4 + p))

    # 上 (lr32)
    if [ "$pe_id" -le 7 ]; then
      up="$minus_one"
      up_desc="-1"
    else
      up=$(to_hex_ieee754 $((pe_id - 8))) # 上のPEID
      up_desc=$((pe_id - 8))
    fi

    # 下 (lr34)
    if [ "$pe_id" -ge 56 ]; then
      down="$minus_one"
      down_desc="-1"
    else
      down=$(to_hex_ieee754 $((pe_id + 8))) # 下のPEID
      down_desc=$((pe_id + 8))
    fi

    # 右 (lr36)
    if [ $(( (pe_id + 1) % 8 )) -eq 0 ]; then
      right="$minus_one"
      right_desc="-1"
    else
      right=$(to_hex_ieee754 $((pe_id + 1))) # 右のPEID
      right_desc=$((pe_id + 1))
    fi

    # 左 (lr38)
    if [ $((pe_id % 8)) -eq 0 ]; then
      left="$minus_one"
      left_desc="-1"
    else
      left=$(to_hex_ieee754 $((pe_id - 1))) # 左のPEID
      left_desc=$((pe_id - 1))
    fi

    # 出力
    printf "d set \$lr32m%dp%d 1 %s #PEID=%d (UP=%s)\n" "$m" "$p" "$up" "$pe_number" "$up_desc" >> "$output_file"
    printf "d set \$lr34m%dp%d 1 %s #PEID=%d (DOWN=%s)\n" "$m" "$p" "$down" "$pe_number" "$down_desc" >> "$output_file"
    printf "d set \$lr36m%dp%d 1 %s #PEID=%d (RIGHT=%s)\n" "$m" "$p" "$right" "$pe_number" "$right_desc" >> "$output_file"
    printf "d set \$lr38m%dp%d 1 %s #PEID=%d (LEFT=%s)\n" "$m" "$p" "$left" "$pe_number" "$left_desc" >> "$output_file"

    # 次のPE番号
    pe_number=$((pe_number + 1))

    # 1行改行を追加
    echo "" >> "$output_file"
  done
done

# 完了メッセージ
echo "Configuration has been written to $output_file"
