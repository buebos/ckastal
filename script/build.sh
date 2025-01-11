#!/bin/bash

check_and_create_dir() {
    local dir="$1"
    
    if [ ! -d "$dir" ]; then
        mkdir -p "$dir"
    fi
}

is_windows=false
kernel_release="$(uname -r)"

if [[ "$kernel_release" == *"Microsoft"* || "$kernel_release" == *"microsoft"* ]]; then
    is_windows=true
fi

output_dir="build"

check_and_create_dir "$output_dir"

if [[ $is_windows == true ]]; then
    cmd.exe /C "gcc -o $output_dir/sample sample/main.c"
else
    gcc -o $output_dir/sample sample/main.c
fi
