#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_ROW_LEN 500
#define MAX_LABEL_LEN 50
#define MEM_SIZE (1 << 16)

//defining the nodes of the linked list that will store the labels and their position in the memory
typedef struct label{
	char Label_name [MAX_LABEL_LEN+1];
	int Label_line_number;
	struct label *next;
} Label;


///FUNCTION DECLARATIONS///

//The function get as an input the asm file pointer, and returns a pointer to linked list containing the label's names and the matched line in each node
Label* Save_Labels_list (FILE* fp);

//The function determines if the word is an operation. returns 1 for True, 0 for False
int is_operation (char* op);

//The function loads the memory into the array mem
void load_to_memory (Label* head, FILE* fp_in, FILE* fp_out, int mem[MEM_SIZE]);

//Given strings, written in hexa or binary- these functions returns the correct operator, register and imm number
int get_op_num (char* op);
int get_register_num (char* reg);
int get_imm_num (char* imm, Label* head);

//The function uses the array mem and writes to the output file
void write_to_file (int mem[MEM_SIZE], FILE* fp_out);

//The function releases the allocated memory for the linked list
void free_allocated_memory (Label* head);


int main (int argc, char* argv[]){

	//Check for enough command line arguments
	if (argc != 3){
		printf ("error, invalid number of input arguments\n");
		exit (1);
	}
	//Create the linked label's list head
	Label *head = NULL;

	//Files
	FILE* fp_in = NULL;
	FILE* fp_out = NULL;

	//main memory
	int mem[MEM_SIZE]={0};

	//open input asm file
	fp_in=fopen (argv[1],"r");
	if (fp_in==NULL){
		printf("Input file did not open. Exit..\n");
		return 1;
	}

	//create linked list of labels
	head = Save_Labels_list (fp_in);
	fclose (fp_in);

	//open output text file
	fp_out = fopen (argv[2],"w");
	if (fp_out==NULL){
		printf("output file did not open. Exit..\n");
		return 1;
	}

	//re-open input text file (could also use rewind function)
	fp_in=fopen (argv[1],"r");
	if (fp_in==NULL){
		printf("Input file did not open. Exit..\n");
		return 1;
	}

	//save the main memory in the array mem
	load_to_memory (head, fp_in,fp_out,mem);

	//using mem, write to the output file
	write_to_file (mem,fp_out);

	//close files
	fclose (fp_in);
	fclose (fp_out);

	//free linked list memory
	free_allocated_memory (head);

	//goodbye!
	return 0;
}

Label* Save_Labels_list (FILE* fp){
	Label *head = NULL, *new_label = NULL, *curr_label= NULL;
	int line_number = 0;
	char curr_line[MAX_ROW_LEN+1], *first_word = NULL, *operation = NULL;

	//read new line
	while (fgets(curr_line, sizeof(curr_line), fp)) {
		operation = NULL;
		first_word = strtok(curr_line,"\t ,\n");
		if (first_word != NULL)
			operation = strtok (NULL,"\t ,\n");//note that operation might be a label, added solution to such case
		if (first_word != NULL){
			if (first_word[strlen(first_word)-1] != ':')//This line does not starts with a label. continue
				line_number += 1;
			else {                       // This line starts with a label. it might also have an instruction!
				first_word[strlen(first_word)-1] = '\0';
				new_label = (Label*) malloc (sizeof(Label));
				if (new_label == NULL){
					printf ("fatal error, memory allocation failed, exiting..\n");
					exit (1);
				}
				//add new node
				new_label -> next = NULL;
				new_label -> Label_line_number = line_number;
				strcpy (new_label->Label_name, first_word);
				if (head == NULL){
					head = new_label;
					curr_label = new_label;
				}
				else{
					curr_label -> next = new_label;
					curr_label = new_label;
				}
				if (operation != NULL){
					if (is_operation(operation))                 //check the case that this line contains both label and an instruction
						line_number += 1;
				}
			}
		}
	}
	return (head);
}

int is_operation (char* op){
	int result = 0;
	result =strcmp (op,"add") && strcmp (op, "sub") && strcmp (op, "and") && strcmp (op, "or") && strcmp (op, "sll") && strcmp (op, "sra") && strcmp (op, "limm") && strcmp (op, "beq") && strcmp (op, "bgt") && strcmp (op, "ble") && strcmp (op, "bne") && strcmp (op, "jal") && strcmp (op, "lw") && strcmp (op, "sw") && strcmp (op, "jr") && strcmp (op, "halt");//strcmp returns 0 if they are equal, might be confusing!
	if (0==result)
		result = 1;
	else
		result = 0;
	return result;
}

void load_to_memory (Label* head, FILE* fp_in, FILE* fp_out,int mem[MEM_SIZE]){
	int op_num,rd_num,rs_num,rt_num,imm_num, address_num, data_num, command, line_number = 0;
	char curr_line[MAX_ROW_LEN+1], *first_word = NULL, *op = NULL, *rd = NULL, *rs = NULL, *rt = NULL, *imm = NULL, *address = NULL, *data = NULL;

	//read new line
	while (fgets(curr_line, sizeof(curr_line), fp_in)) {
		first_word = NULL, op = NULL, rd = NULL, rs = NULL, rt = NULL, imm = NULL;//reset the pointers for new line
		first_word = strtok(curr_line,"\t ,\n");
		if (first_word == NULL)                                  // this is a blank line, ignore
			continue;
		else{
			if (strcmp(first_word,".word")==0){     //word type command encountered
				address = strtok (NULL,"\t ,\n");
				data = strtok (NULL,"\t ,\n");
				if ((address[0]=='0')&&(address[1]=='x'))      //address given in hexa
					address_num= strtol(address,NULL,16);
				else
					address_num = strtol(address,NULL,10);     //address given in decimals
				if ((data[0]=='0')&&(data[1]=='x'))            //data given in hexa
					data_num= strtol(data,NULL,16);
				else                                           //data given in decimals
					data_num = strtol(data,NULL,10);
				mem[address_num] = data_num;
				continue;
			}
			if (first_word[strlen(first_word)-1] != ':') {//line contains only an instruction 
				op = first_word;
				rd = strtok (NULL,"\t ,\n");
				rs = strtok (NULL,"\t ,\n");
				rt = strtok (NULL,"\t ,\n");
				imm = strtok (NULL,"\t ,\n");

				//convert strings to numbers 
				op_num = get_op_num (op)<<28;
				rd_num = get_register_num (rd)<<24;
				rs_num = get_register_num (rs)<<20;
				rt_num = get_register_num (rt)<<16;
				imm_num = get_imm_num (imm,head);

				//an awesome trick to save the number representing the instruction value: shift the bits to their right places
				command = ((op_num)+(rd_num)+(rs_num)+(rt_num)+(imm_num));
				//handle the special case of NEGATIVE imm

				if (imm_num<0)
					command += 65536;
				mem[line_number] = command;

				line_number +=1;
			}
			else{          //this line starts with a label. it MIGHT also have an instruction!
				op = strtok (NULL,"\t ,\n");
				if (op == NULL)     ////this line contains only a label
					continue;
				if (is_operation (op)==0)      /// only a label, continue
					continue;
				else      //contains both a label and an instruction
					rd = strtok (NULL,"\t ,\n");
					rs = strtok (NULL,"\t ,\n");
					rt = strtok (NULL,"\t ,\n");
					imm = strtok (NULL,"\t ,\n");
					op_num = get_op_num (op);
					rd_num = get_register_num (rd);
					rs_num = get_register_num (rs);
					rt_num = get_register_num (rt);
					imm_num = get_imm_num (imm,head);
					command = ((op_num<< 28)+(rd_num<<24)+(rs_num<<20)+(rt_num<<16)+imm_num);
					mem[line_number] = command;
					line_number +=1;
			}
		}
	}
}



int get_op_num (char* op){
	char* operations[16] = { "add", "sub", "and", "or", "sll", "sra", "limm", "beq", "bgt", "ble", "bne", "jal", "lw", "sw", "jr", "halt" };
	int i=0;
	//search for operation in the operations array
	while (strcmp(op,operations[i])!=0)
		i+=1;
	return i;
}

int get_register_num (char* reg){
	char* registers[16] = { "$zero", "$at", "$v0", "$a0", "$a1", "$t0", "$t1", "$t2", "$t3", "$s0", "$s1", "$s2", "$gp", "$sp", "$fp", "$ra" };
	int i=0;
	//search for register in the registers array
	while (strcmp(reg,registers[i])!=0)
		i+=1;
	return i;
}

int get_imm_num (char* imm, Label* head){
	int result; 
	Label* current = head;
	//check if imm is a label, if so return its value
	while (current != NULL){
		if (strcmp(imm,current->Label_name)==0)
			return (current ->Label_line_number);
		current = current ->next;
	}
	// if we get here, the imm is NOT a label! therefore it is a constant integer, in decimals OR hexadecimals
	//hexadecimal:
	if ((imm[0]=='0')&&(imm[1]=='x'))
		result= strtol(imm,NULL,16);
	//decimal:
	else
		result = strtol(imm,NULL,10);
	return (result);
}

void write_to_file (int mem[MEM_SIZE], FILE* fp_out){
	int i=0;
	
	//copy (almost) all the values stored in the memory array to the output file, in hexadecimal representation
	while (i<MEM_SIZE-1){
		fprintf (fp_out,"%08X\n" ,mem[i]);
		i++;
	}

	//in order to handle nice file with no \n on its last line
	fprintf (fp_out,"%08X" ,mem[i]);
}

void free_allocated_memory (Label* head){
	Label *tmp = NULL, *curr = head;
	while (curr != NULL){
	tmp = curr;
	curr = curr->next;
	free (tmp);
	}
	head = NULL; //mark as an empty list
}
