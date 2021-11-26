#!/bin/bash
tentativas=$1
path_intancias=$2
seed=$3

echo Algoritmo,Instancia,Tempo_Nanosegundos,N_bins

for alg in NF WF FF BF NFD WFD FFD BFD
do
    files=$(ls $path_intancias)
    for filename in $files
    do
        soma_tempos=0
        tempo=0
        bins=0
        for tentativa in $(seq $tentativas)
        do
            ./bin/driver $path_intancias$filename $alg $seed > ./results/$alg$filename$tentativa.txt
            tempo=$(tail -1 ./results/$alg$filename$tentativa.txt)
            tempo="${tempo##* }"
            soma_tempos=$((soma_tempos + tempo))
            bins=$(tail -2 ./results/$alg$filename$tentativa.txt | head -1)
            bins="${bins##* }"
        done
        echo $alg,$filename,$((soma_tempos / tentativas)),$bins
    done
done
