#!/bin/bash

javac dependencyDiscoverer.java

for ((i=0; i<500; i++))
{
	java dependencyDiscoverer -Itest test/*.c test/*.y test/*.l
}

echo "Am terminat"
