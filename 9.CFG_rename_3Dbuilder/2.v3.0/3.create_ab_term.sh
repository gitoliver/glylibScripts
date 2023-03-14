#!/bin/bash
cat output4.txt | grep "a.-OH" > output5.txt
cat output4.txt | grep "b.-OH" >> output5.txt
cat output4.txt | grep -v "b.-OH" | grep -v "a.-OH" > output4a.txt
cat output4.txt | grep -v "b.-OH" | grep -v "a.-OH" > output4b.txt
cat output4a.txt | sed 's/\t/a\t/g' | sed 's/1-OH/a1-OH/g' | sed 's/2-OH/a2-OH/g' >> output5.txt
cat output4b.txt | sed 's/\t/b\t/g' | sed 's/1-OH/b1-OH/g' | sed 's/2-OH/b2-OH/g' >> output5.txt
cat output5.txt | sed 's/b-/b1-/g' | sed 's/a-/a1-/g' > output6.txt
