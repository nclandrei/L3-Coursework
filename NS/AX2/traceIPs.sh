for var in "$@"
do
	traceroute -n -q 1 "$var" > "$var".txt
done
