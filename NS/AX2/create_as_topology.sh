#!/bin/bash

input_file="$1"
output_file="$2"

while IFS='' read -r line || [[ -n "$line" ]]; do
	./aslookup rib.20160101.0000.bz2 line >> "$2"
done < "$input_file"
