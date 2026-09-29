#!/bin/sh
count=$#
if [ $count -ne 0 ]
then
echo "There are $count parameters given"
mean=0
for n in $@
do
mean=$((mean + $n))
done
mean=$((mean / $count))
echo "Mean = $mean"
else
echo "No arguments given"
fi
