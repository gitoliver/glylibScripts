#/usr/bin/env bash
source motif1.txt
source motif2.txt

find ./library/ -exec grep -l "LINK.*$motif1.*$motif2" {} \; > search_results.txt

rm -rf 1.search_results 2>/dev/null
mkdir 1.search_results

for f in `more search_results.txt`
do
	cp $f ./1.search_results/
done

find ./1.search_results/ -exec grep -l "LINK.*$motif2" {} \; > search_results.txt

source motif2.txt

rm -rf 2.search_results
mkdir 2.search_results

for f in `more search_results.txt`
do
        cp $f ./2.search_results/
done
rm search_results.txt
echo "finished"
