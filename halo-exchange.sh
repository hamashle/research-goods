#!/bin/bash

# 引数チェック
if [ $# -ne 7 ]; then
  echo "Usage: $0 <lm_put> <lb_dir> <lb_increment> <lm_get> <lm_get_increment> <loop_count> <file_name>"
  exit 1
fi

# 引数を取得
lm_put=$1        # l1bmd の lm{任意の値}v の値
lb_dir=$2        # lb の方向
lb_increment=$3    # lb の増加量
lm_get=$4        # lm の開始値
lm_get_increment=$5
loop_count=$6    # ループ回数
file_name=$7

# 初期化
output_file="${file_name}.txt"
> "$output_file"
###一時的に、v12にしてる
{
  echo "l1bmd \$lm${lm_put}v12 \$lb256 # 64 * 4個分書き込まれる \$lm${lm_put}v \$address の部分"
  echo "nop"
  echo "nop"
  echo ""
} >> "$output_file"

for (( i=0; i<loop_count; i++ )); do
  
  lb=$((256 + i * lb_increment))

  {
    echo "l1bmp \$lb$lb_dir \$t"
    echo "lxor \$lbf \$ln0v \$t \$omr2;"
    echo "maskm 2"
    echo "l1bmp \$lb$((lb)) \$lm$lm_get"
    echo "l1bmp \$lb$((lb + 64)) \$lm$((lm_get + lm_get_increment))"
    echo "l1bmp \$lb$((lb + 128)) \$lm$((lm_get + lm_get_increment * 2))"
    echo "l1bmp \$lb$((lb + 192)) \$lm$((lm_get + lm_get_increment * 3))"
    echo "mask 0"
    echo ""
  } >> "$output_file"
  lb_dir=$((lb_dir + 4))
done

# 完了メッセージ
echo "Code generated and written to $output_file"
