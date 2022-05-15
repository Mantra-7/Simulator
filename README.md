# Pipelined Processor Simulator

This is a pipelined processor simulator. You can give instructions, data cache state, and register file state in input and it will show the final data cache and statistics of execution.

You can also use assembler given to make binary instructions from assembly. Butif you already have the input files in input folder then you can directly go to instructions to run the simulator. 

There are 2 versions of this simulator. One of them has all code in a single file named PipelineProcessor.cpp and other has code distributed over multiple c++ files.

We have also made Super Scalar processor which has multiple pipelines for various type of instructions and can execute instructions in out of order fashion. Current version does not support branch instructions.

## Instructions to run assembler:

Go to Assembler folder -
cd Assembler/

Put assembly code in input file, and run 
./msasm input output

Now, output contains binary instructions in hexadecimal form.

You can use generate.cpp file to get this instructions in output file in input/ICache.txt and also if dcache state and register file state are in different files then use this to put it in input/DCache.txt and input/RF.txt.

## Instructions to run simulator:

Put all inputs properly in input folder by running above code.

### Single File Version:

g++ PipelineProcessor.cpp -o PipelineProcessor\
./PipelineProcessor

### Super Scalar Version:

g++ SuperScalar.cpp -o SuperScalar\
./SuperScalar

### Distributed Version:

Create executable using makefile -\
make clean\
make

Run the simulator -\
bin/simm

runsim executes these 3 commands in order.

## Outputs

Outputs are given in output folder.

ODCache.txt contains final DCache, Outputs.txt contains the statistics and ORF.txt cantains final Register File.

### Developed by:

1. Mantra Trambadia | CS20B083 | Mantra-7

2. Saptarshi Sen    | CS20B068 | Saptarshi-sen

You can find the github repository of this project at: https://github.com/Mantra-7/Simulator