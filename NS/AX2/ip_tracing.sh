#!/bin/bash

filename="$1"

while IFS='' read -r line || [[ -n "$line" ]]; do
    traceroute -n -q 1 "$line" > ${line:4}.txt
done < "$filename"
