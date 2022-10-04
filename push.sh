#!/bin/bash
set -x

find . -name "*.out"  | xargs rm -f
find . -name "core.*"  | xargs rm -f

git config user.name xfmeng17
git config user.email xfmeng17@foxmail.com

MESSAGE=`date +"%Y-%m-%d %T"`

git add --all .
git commit -m "${MESSAGE}"
git push origin master
