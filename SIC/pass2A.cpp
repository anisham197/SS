#include <iostream>
#include <fstream>
#include <cstring>
#include <map>
#include <stdlib.h>

using namespace std;

map <string, string> optab, symtab;
map <string, string>::iterator iter;

// Reading optab.txt and creating hashmap of symbol and opcode
void readOptabSymtab() 
{
	fstream file;
	string input1, input2;

	file.open("optab3.txt", ios::in );
	while (  file >> input1 >> input2 ) 
		optab.insert ( make_pair ( input1 , input2 ) ) ;
	file.close();
	
	file.open("symtab.txt", ios::in );
	while (  file >> input1 >> input2 )
		symtab.insert ( make_pair ( input1 , input2 ) ) ;
	file.close();
}

// Generates object codes for each instruction
void parseFile() 
{
	int hexno = 0, lineno = 0;
	fstream interm_file, input_file;
	string line;
	char addr[5], instruction[7];
	char *str, *token;

	// Open files
	input_file.open("locctr.txt", ios::in );
	interm_file.open("interim.txt", ios::out);

	// Read input file till end
  	while ( getline(input_file, line) )
	{
		lineno++;

		char tokens[4][20]={0,0,0,0};

		// Checks for comment line
		if(line[0] == '.')
		{
			interm_file << line << endl;
			continue;
		}


		// Copies to str (char pointer), tokenizes into LABEL, MNEMONIC, OPERAND
		str = strdup (line.c_str());
		int i = 0;

		// Splits by "\t" delimiter
		while((token = strsep(&str, "\t")) && i < 4) 
		{
  			strcpy(tokens[i],token);
  			i++;
		}

		// Check if Mnemonic is START or END
		if (strcmp(tokens[2],"START") == 0 || strcmp(tokens[2],"END") == 0)
		{
			interm_file << line << endl;
			continue;
		}

		// Checks if MNEMONIC exists in optab
		else if ( optab.find(tokens[2]) == optab.end())
		{
			cout << "\nERROR in line " << lineno << endl;
			break;
		}

		// Write instruction to intermediate file
		interm_file << line <<"\t";

		// Gets OPCODE of instruction from optab
		iter = optab.find(tokens[2]);

		// Finds object code for each instruction, except WORD, BYTE, RESW, RESB
		if( iter->second != "*" )
		{
			// Copies OPCODE to instruction
			strcpy(instruction,iter->second.c_str());

			// Checks if Operand field is a symbol, stores location in addr
			iter = symtab.find(tokens[3]);
			if (iter != symtab.end())
			{
				strcpy(addr, iter->second.c_str() );
			}

			// Check if Operand field has a LABEL,X type instruction
			else if ( strstr(tokens[3],",X")!=NULL )
			{
				// Locates part of token that contains ,X
				char * ptr = strstr(tokens[3],",X"); 
				// ptr has location of comma, change it to \0 to get only LABEL
				*ptr = '\0';

				// Checks if Operand field is a valid symbol and gets location
				iter = symtab.find(tokens[3]);
				if (iter != symtab.end())
				{
					// Converts addr to long and ORs with 8000 to make index bit 1
					hexno = ( (strtol( iter->second.c_str(),NULL,16) ) ^ 0x8000 );
					sprintf(addr,"%X",hexno);
				}
			}
			// Instruction does not have an operand, set address to 0
			else
			{
				strcpy(addr,"0000");
			}
			// Attaches address to opcode, outputs to file
			strcat(instruction, addr);
			interm_file << instruction << endl;

		}
		// Handles object code generation for WORD, BYTE etc
		else
		{
			// Checks for BYTE
			if(strcmp(tokens[2],"BYTE")==0)
			{
				char byt[3];
				if(tokens[3][0] == 'C')
				{
					// Clears instruction
					strcpy(instruction,"");
					// Retrieves each character between ' ', converts to hex and outputs to file
					for(int index = 2;tokens[3][index]!='\'';index++)
					{
						sprintf(byt,"%02X", tokens[3][index]);
						strcat(instruction,byt);
					}
					interm_file << instruction << endl;
				}

				else if(tokens[3][0] == 'X')
				{
					strcpy(instruction,"");
					// Retrieves each character between ' ', outputs to file
					for(int index = 2; tokens[3][index]!='\'';index++)
					{
						sprintf(byt,"%c", tokens[3][index]);
						strcat(instruction,byt);
					}
					interm_file << instruction << endl;
				}
				// If not C or X
				else
				{
					cout << "\nERROR in line " << lineno << endl;
				}
			}

			// Checks for WORD and stores operand as hex
			else if (strcmp(tokens[2],"WORD") == 0)
			{
				hexno = strtol(tokens[3], NULL, 10);
				sprintf(instruction,"%06X",hexno);
				interm_file << instruction << endl;
			}
			// Handles RESB, RESW
			else
				interm_file << endl;
		}
	}
}

int main () 
{
	readOptabSymtab();
 	parseFile();
}
