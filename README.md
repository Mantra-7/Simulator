# Simulator

This is a pipelined processor simulator. You can give instructions in input and it will show the running in output file.

You can also use assembler given to make binary instructions from assembly.

## Instructions to run assembler:

Go to Assembler folder -
cd Assembler/

Put assembly code in input file, and run 
./msasm input output

Now, output contains binary instructions in hexadecimal form.

## Instructions to run simulator:

Put instructions in ICacheData.txt and DCache data into DCacaheData.txt.
Create executable using makefile -
make clean
make

Run the simulator -
bin/simm
