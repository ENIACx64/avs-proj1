#!/bin/sh

rsync -avzrP xvodak06@barbora.it4i.cz:~/avs/datalog.csv ~/Desktop/git/avs-proj1/

python3 scripts/plot_evaluate.py datalog.csv --save eval.png
