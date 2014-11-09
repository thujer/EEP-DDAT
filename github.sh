#!/bin/bash

git init
git add .
git commit -a -m "First commit"
git push
git add remote $1
git push origin master -f
