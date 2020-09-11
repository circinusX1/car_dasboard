#!/bin/sh

pkill antico-deluxe
pkill Xephyr

export LD_LIBRARY_PATH=$(pwd)/amelib
echo $LD_LIBRARY_PATH

MCOOKIE=$(mcookie)

DISPLAY=:0
export DISPLAY=:0
Xephyr -ac -screen 600x900 -br -reset -terminate :1 &
sleep 4
xauth add $(hostname)/unix:1 . $MCOOKIE
xauth add localhost.localdomain/unix:1 . $MCOOKIE

export DISPLAY=:1
DISPLAY=:1
read yn
xauth remove $(hostname)/unix:1 
xauth remove localhost.localdomain/unix:1
echo $DISPLAY

export DISPLAY=:1


