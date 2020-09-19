#!/bin/bash
export DISPLAY=:0

w=$(which Xephyr)
[[ -z ${w} ]] && echo "please install Xephyr to be able to run on curent desktop" && exit 1

./start-xephir.sh
export DISPLAY=:1

./carutza -display ":1"

pkill Xephyr
export DISPLAY=:0
