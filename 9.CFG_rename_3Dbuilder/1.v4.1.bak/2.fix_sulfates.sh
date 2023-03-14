#!/bin/bash
cat output3.txt | sed -r 's/S\]\[/S,/g;' > output4.txt
echo "done"
