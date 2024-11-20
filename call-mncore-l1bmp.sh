#!/bin/bash

child_script="./mncore-l1bmp.sh"

if [ "$#" -ne 7 ]; then
  echo "Usage: $0 <初期値lb1> <初期値lm1> <lbインクリメント値> <方角の値1> <初期値lb2> <初期値lm2> <方角の値2>"
  exit 1
fi

if [ ! -f "$child_script" ]; then
  echo "Error: $child_script が見つかりません。"
  exit 1
fi

# 引数を取得
initial_lb1=$1       # 初期値lb1
initial_lm1=$2       # 初期値lm1
lb_increment=$3     # lbのインクリメント値
direction1=$4        # 方角の値1

# 引数を取得
initial_lb2=$5       # 初期値lb2
initial_lm2=$6       # 初期値lm2
direction2=$7        # 方角の値2

# 定数
max_count=8   # 最大カウント
repeat=128     # 必要な回数に変更可能

# 出力ファイルを初期化
output_file="halo-exchange-ns.txt"

# 処理を交互に繰り返す
for (( i=0; i<repeat; i++ )); do
  if (( i % 2 == 0 )); then
    # セット1の処理
    lb=$((initial_lb1 + (i / 2) * 2))  # 初期値lb1を2ずつ増加
    dir=$((direction1 + (i / 2) * 2))  # 方角の値1を2ずつ増加
    bash "$child_script" $lb $initial_lm1 $max_count $lb_increment $dir >> "$output_file"
  else
    # セット2の処理
    lb=$((initial_lb2 + (i / 2) * 2))  # 初期値lb2を2ずつ増加
    dir=$((direction2 + (i / 2) * 2))  # 方角の値2を2ずつ増加
    bash "$child_script" $lb $initial_lm2 $max_count $lb_increment $dir >> "$output_file"
  fi
done
