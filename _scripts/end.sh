#!/usr/bin/env bash

git push
code "$(git rev-parse --show-toplevel)"
master
git rebase --committer-date-is-author-date "$(git rev-parse --symbolic-full-name --abbrev-ref=loose @{-1})"
