#!/bin/bash

# 引数のチェック
if [ "$#" -ne 4 ]; then
  echo "Usage: $0 <初期値lb> <初期値lm> <最大カウント> <lbインクリメント値>"
  exit 1
fi

# 引数から値を取得
lb=$1        # 初期値lb
lm=$2        # 初期値lm
max_count=$3 # 出力の最大カウント
lb_increment=$4 # lbのインクリメント値

# lmのインクリメント値は固定値（2）
lm_increment=2

# ループで文字列を生成
for (( j=1; j<=max_count; j++ ))
do
  echo "l1bmp \$lb${lb} \$lm${lm}"
  lm=$((lm + lm_increment))
  lb=$((lb + lb_increment))
done
