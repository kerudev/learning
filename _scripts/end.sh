#!/usr/bin/env bash

git push
code "$(git rev-parse --show-toplevel)"
master
git rebase "$(git rev-parse --symbolic-full-name --abbrev-ref=loose @{-1})"
