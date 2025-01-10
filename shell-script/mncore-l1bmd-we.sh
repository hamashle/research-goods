#!/bin/bash

# 引数のチェック
if [ "$#" -ne 5 ]; then
  echo "Usage: $0 <初期値lm> <初期値lb> <最大カウント> <lbインクリメント値> <列番号>"
  exit 1
fi

# 引数から値を取得
lm=$1        # 初期値lm
lb=$2        # 初期値lb
max_count=$3 # 出力の最大カウント
lb_increment=$4 # lbのインクリメント値
i=$5     # 列番号

# lmのインクリメント値は固定値（2）
lm_increment=20

# ループで文字列を生成
for (( j=1; j<=max_count; j++ ))
do
  echo "l1bmd \$lm${lm} \$lb${lb} # (${j},${i})"
  lm=$((lm + lm_increment))
  lb=$((lb + lb_increment))
done
