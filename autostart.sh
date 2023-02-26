#!/bin/bash

# author: Mr.Jor
# date: 2023-02-25
# description: dwm 自启动执行脚本

pwd_dir=$(cd $(dirname $0);pwd)

# 初始化设置
setting() {
  [ $1 ] &&  sleep $1

  # 关闭蜂鸣器
  xset -b
  # 设置使用键盘时触控板短暂失效
  syndaemon -i 1 -t -K -R -d
  # 设置壁纸
  feh --randomize --bg-fill ~/Pictures/Wallpaper/2023/*
}

# 程序配置
program() {
  [ $1 ] &&  sleep $1

  # 开启输入法
  fcitx5 &
  # 开启 flameshot 截图
  flameshot &
  picom >>/dev/null 2>&1 &
}

# 定时任务
cron() {
  [ $1 ] &&  sleep $1

  let num=10
  while true; do
    # 1 分钟切换一次壁纸
    [ $(( num % 60 )) -eq 0 ] && feh --randomize --bg-fill ~/Pictures/Wallpaper/2023/*
    sleep 10; let num+=10
  done
}

setting 1 &

program 3 &

cron 5 &

