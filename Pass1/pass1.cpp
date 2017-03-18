#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <map>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

class p1 {

	map <string, string> optab;
	map <string, string>::iterator iter;

	public:

	// Reading optab.txt and creating hashmap of symbol and opcode
	void readOptab() {

		fstream file;
		string input1, input2;

		file.open("optab.txt", ios::in );
		while (  file >> input1 >> input2 ) {
			optab.insert ( make_pair ( input1 , input2 ) ) ;
		}
	}

	void parseInput() {

		int locctr = 0, lineno = 0;
		fstream input_file, sym_file, output_file;
		string line;
		char cbuff[255], tokens[3][255], hex_loc[4];
		char *str, *token;

		// Open files
		input_file.open("input.txt", ios::in );
		output_file.open("locctr.txt", ios::out | ios::app);
		sym_file.open("symtab.txt", ios::out | ios::app);

		// Read input file till end
		while ( getline(input_file, line) )
		{
			lineno++;

			// Copies input line to cbuff(char array)
			strcpy(cbuff, line.c_str());

			// Checks for comment line
			if(cbuff[0] == '.'){
				output_file << line << endl;
				continue;
			}

			// Copies to str (char pointer), tokenizes into LABEL, MNEMONIC, OPERAND
			str = strdup (cbuff);
			int i = 0;
			// Splits by "\t" delimiter
			while((token = strsep(&str, "\t")) && i < 3) {
					strcpy(tokens[i],token);
					i++;
			}

			//Check if Mnemonic is START
			if (strcmp(tokens[1],"START") == 0)
			{
				// Stores initial location
				locctr = strtol(tokens[2], NULL, 16);
				output_file <<"\t" << line << endl;
				continue;
			}

			else if (strcmp(tokens[1],"END") == 0){
					// Converts location to hex
					sprintf(hex_loc, "%X", locctr);
					sym_file << tokens[1] <<"\t" << hex_loc << endl;
					output_file << hex_loc << "\t\t" << tokens[1]<< endl;
					continue;
			}

			// Checks if MNEMONIC exits in optab
			else if ( optab.find(tokens[1]) == optab.end())
			{
				cout << "\nERROR in line " << lineno << endl;
				break;
			}

			// Writes line to output file with location
			sprintf(hex_loc, "%X", locctr);
			output_file << hex_loc<< "\t"<< tokens[0] <<"\t"<< tokens[1]<< "\t"<< tokens[2]<< endl;

			// If symbol in LABEL field, write to symtab with hex location
			if(strlen(tokens[0])!=0)
			{
				sym_file << tokens[0] <<"\t" << hex_loc << endl;
			}

			// Obtain (key, value) from map for MNEMONIC
			iter = optab.find(tokens[1]);

			// Handle location counter update
			if((strlen(tokens[0])!=0) && (strcmp( iter->second.c_str(),"*") == 0))
			{
				//Handle BYTE
				if(strcmp(tokens[1],"BYTE") == 0)
				{
					if(tokens[2][0] == 'C')
					{
						locctr +=  strlen(tokens[2]) - 3;
						// -3 for C , ', '
					}
					else if(tokens[2][0] == 'X')
					{
						int hexdigits = strlen(tokens[2])- 3;
						locctr += ( hexdigits % 2 == 0 ? hexdigits/2 :(hexdigits+1)/2 );
					}
					else
					{
						cout << "\nERROR in line " << lineno << endl;
						break;
					}
				}
				else if(strcmp(tokens[1],"RESB")==0)
				{
					locctr += strtol(tokens[2], NULL, 10);
				}
				else if(strcmp(tokens[1],"RESW")==0)
				{
					locctr += strtol(tokens[2], NULL, 10)*3;
				}
				else if(strcmp(tokens[1],"WORD")==0)
				{
					locctr += 3;
				}
			}

			// Checks error where no label defined for WORD, BYTE, RESW, RESB
			else if ( (strlen(tokens[0])==0) && (strcmp( iter->second.c_str(),"*") == 0))
			{
				cout << "ERROR in line " << lineno << endl;
				cout << "No variable defined " << endl;
				break;
			}
			// Increments by 3 if normal instruction
			else {
					locctr += 3;
			}
		}
		input_file.close();
		sym_file.close();
		output_file.close();
	}
};

int main () {
	p1 obj;
	obj.readOptab();
	obj.parseInput();
}
