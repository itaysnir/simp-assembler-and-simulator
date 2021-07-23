# simp-assembler-and-simulator
This project simulates a SIMP processor. 

It contains an assembler written in C, which translates the project's assembly into machine code. 
In order to run the assembler, run:

./asm program_name.asm mem.txt

It also contains a processor simulator, simulates the fetch-decode-execute-writeback loop.  
In order to run the simulator, run:

./sim memin.txt memout.txt regout.txt trace.txt count.txt

Also, few program tests are added.
