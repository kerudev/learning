#!/usr/bin/env bash

name="$1"

if [ -z "$name" ]; then
    read -p "Enter new branch name: " name
fi

git checkout -b "$name"
mkdir -p "$name"
code "$name"
