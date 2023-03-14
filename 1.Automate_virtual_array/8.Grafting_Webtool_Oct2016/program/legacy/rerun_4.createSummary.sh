#!/bin/bash
cp EXP_BINDERS.txt 0.grafting_NOwgl/
cp EXP_BINDERS.txt 1.grafting_5wgl/
cp EXP_BINDERS.txt 2.grafting_10wgl/
cp EXP_BINDERS.txt 3.grafting_15wgl/
cp EXP_BINDERS.txt 4.grafting_20wgl/

cd 0.grafting_NOwgl/
nohup 3.createSummary.sh &
cd ..

cd 1.grafting_5wgl/
nohup 3.createSummary.sh &
cd ..

cd 2.grafting_10wgl/
nohup 3.createSummary.sh &
cd ..

cd 3.grafting_15wgl/
nohup 3.createSummary.sh &
cd ..

cd 4.grafting_20wgl/
nohup 3.createSummary.sh &
cd ..
