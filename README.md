# PAA2021

Use the command 'make' to compile the program

To run the program specify these arguments:

- path to the instance file
- identifier of the algorithm to be used
- seed to shuffle the data

The output of the program is in bin used for each item the total number of bins used and the time spent, in nanoseconds

To save the results in a file use the following command at the end of arguments:

\> path_of_file_to_save

Example: ./bin/driver ./data/Schoenfield/Hard28_BPP13.txt NF 0 > nf_0_results.txt

A script named 'run.sh' was created for convenience, enabling to easily execute all the algorithms in group of instances a determined number of times with a choosen seed
The output of 'run.sh' is like a .csv file, using the '>' is possible to generate a .csv file with the compiled results

The files 'schoenfield_t10_seed42_results.csv' and 'waescher_t10_seed42_results.csv' are tghe results of the usage of the 'run.sh' on the selected instances, the .ods files were manualy created using the .csv, were the results were grouped for better visualization.

In the folder results, the files created by the 'run.sh' are stored ,but not uploaded