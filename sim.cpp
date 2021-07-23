#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_ROW_LEN 500
#define MEM_SIZE (1 << 16)

//function declarations

int get_num (int x, int MSB, int LSB);
void write_to_file (int mem[MEM_SIZE], FILE* fp_out);
void write_regs (int registers[16], FILE* fp_out);


int main(int argc, char *argv[]){
	int i=0, count=0,last = MEM_SIZE - 1;
	int inst, op, rd, rs, rt, imm, PC;
	if (argc != 6){
		printf ("error, invalid number of input arguments\n");
		exit (1);
	}

	// zero memory
	int mem[MEM_SIZE] = {0};
	
	// registers value
	int registers[16] = {0};

	// files
	FILE* fp_memin;
	FILE* fp_memout;
	FILE* fp_regout;
	FILE* fp_trace;
	FILE* fp_count;

	// check that we have 5 cmd line parameters
	if (argc != 6) {
		printf ("error, invalid number of input arguments\n");
		exit(1);
	}
	// open input text file
	fp_memin = fopen(argv[1], "r");
	if (fp_memin==NULL){
		printf("Input file did not open. Exit..\n");
		return 1;
	}
	
	//open output text files
	fp_memout = fopen (argv[2],"w");
	fp_regout = fopen (argv[3],"w");
	fp_trace = fopen (argv[4],"w");
	fp_count = fopen (argv[5],"w");
	if (!fp_memout || !fp_regout || !fp_trace || !fp_count){
		printf ("Output file did not open, Exit..\n");
		return 1;
	}

	// read memin.txt into mem

	while (!feof(fp_memin)){
		if (fscanf(fp_memin, "%08X\n", &mem[i]) != 1)
			break;
		i++;
	}

	// find last non-zero memory entry

	while (last >= 0 && mem[last] == 0)
		last--;
	
	// decode instructions
	PC = 0;
	while (PC <= last) {

		// fetch next instruction
		inst = mem[PC];

		// decode instruction
		op = get_num(inst, 31, 28);
		rd = get_num(inst, 27, 24);
		rs = get_num(inst, 23, 20);
		rt = get_num(inst, 19, 16);
		imm = get_num(inst, 15, 0);

		if (imm>40960) //negative imm
			imm = imm-65536;

		//write to trace.txt
		fprintf (fp_trace,"%08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X\n",PC, inst, registers[0],registers[1],registers[2],registers[3],registers[4],registers[5],registers[6],registers[7],registers[8],registers[9],registers[10],registers[11],registers[12],registers[13],registers[14], registers[15]);
	
		// execute instructions
		//add
		if (0==op){
			registers[rd] = registers[rs] + registers[rt];
			PC += 1;
			count +=1;
			continue;
		}
		//sub
		if (1==op){
			registers[rd] = registers[rs] - registers[rt];
			PC += 1;
			count +=1;
			continue;
		}
		//and
		if (2==op){
			registers[rd] = registers[rs] & registers[rt];
			PC += 1;
			count +=1;
			continue;
		}
		//or
		if (3==op){
			registers[rd] = registers[rs]| registers[rt];
			PC += 1;
			count +=1;
			continue;
		}
		//sll
		if (4==op){
			registers[rd] = registers[rs] << registers[rt];
			PC += 1;
			count +=1;
			continue;
		}
		//sra
		if (5==op){
			registers[rd] = registers[rs] >> registers[rt];
			PC += 1;
			count +=1;
			continue;
		}

		//limm
		if (6==op){
			registers[rd] = imm;
			PC += 1;
			count +=1;
			continue;
		}

		//beq
		if (7==op){
			if (registers[rs] == registers[rt])
				PC = imm;
			else
				PC += 1;
			count +=1;
			continue;
		}
		//bgt
		if (8==op){
			if (registers[rs] > registers[rt])
				PC = imm;
			else
				PC += 1;
			count +=1;
			continue;
		}
		//ble
		if (9==op){
			if ((registers[rs] < registers[rt])||(registers[rs] == registers[rt]))
				PC = imm;
			else
				PC += 1;
			count +=1;
			continue;
		}
		//bne
		if (10==op){
			if (registers[rs] != registers[rt])
				PC = imm; 
			else
				PC += 1;
			count +=1;
			continue;
		}
		//jal
		if (11==op){
			registers[15] =PC +1;
			PC = imm;
			count +=1;
			continue;
		}
		//lw
		if (12==op){
			registers[rd] = mem[registers[rs]+imm];
			PC += 1;
			count +=1;
			continue;
		}
		//sw
		if (13==op){
			mem[registers[rs]+imm]=registers[rd];
			PC += 1;
			count +=1;
			continue;
		}
		//jr
		if (14==op){
			PC = registers[rd];
			count +=1;
			continue;
		}
		if (15==op){
			count +=1;
			break;
		}
	}

	//write to memout.txt
	write_to_file (mem, fp_memout);

	//write to regout.txt
	write_regs (registers, fp_regout);

	//write to count.txt
	fprintf (fp_count,"%d" ,count);

	fclose (fp_memin);
	fclose (fp_memout);
	fclose (fp_regout);
	fclose (fp_trace);
	fclose (fp_count);
	
	return 0;
}

int get_num (int x, int MSB, int LSB){

	//shifting to ignore elements prior the LSB
	int tmp = x >> LSB;

	//create a mask to ignore elements above the MSB
	int mask = (1 << (MSB - LSB + 1)) - 1;

	return tmp & mask;
}

void write_to_file (int mem[MEM_SIZE], FILE* fp_out){
	int i=0;
	while (i<MEM_SIZE-1){
		fprintf (fp_out,"%08X\n" ,mem[i]);
		i++;
	}
	fprintf (fp_out,"%08X" ,mem[i]);
}

void write_regs (int registers[16], FILE* fp_out){
	int i;
	for (i=0;i<15;i++){
		fprintf (fp_out,"%08X\n" ,registers[i]);
	}
	fprintf (fp_out,"%08X" ,registers[i]);
}
