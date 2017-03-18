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
  map <string, string> symtab;
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

  void readSymtab() {

    fstream file;
    string input1, input2;

    file.open("symtab.txt", ios::in );
    while (  file >> input1 >> input2 ) {
      symtab.insert ( make_pair ( input1 , input2 ) ) ;
    }
  }

  void parseFile() {
    int hexno = 0, lineno = 0;
    fstream interm_file, input_file;
    string line;
    char cbuff[255], tokens[4][255], addr[5], instruction[7];
    char *str, *token;

    // Open files
    input_file.open("locctr.txt", ios::in );
    interm_file.open("interim.txt", ios::out | ios::app);

    while ( getline(input_file, line) )
    {
      lineno++;

      // Copies input line to cbuff(char array)
      strcpy(cbuff, line.c_str());

      // Checks for comment line
			if(cbuff[0] == '.'){
				interm_file << line << endl;
				continue;
			}

      // Copies to str (char pointer), tokenizes into LABEL, MNEMONIC, OPERAND
      str = strdup (cbuff);
      int i = 0;
      // Splits by "\t" delimiter
      while((token = strsep(&str, "\t")) && i < 4) {
          strcpy(tokens[i],token);
          i++;
      }

      // Check if Mnemonic is START
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

      interm_file << line <<"\t";

      iter = optab.find(tokens[2]);

      if(strcmp( iter->second.c_str(),"*") != 0)
      {
        strcpy(instruction,iter->second.c_str());

        iter = symtab.find(tokens[3]);
        if (iter != symtab.end())
        {
					strcpy(addr, iter->second.c_str() );
        }

				// No need to check for errors like ,X,C,D etc
				else if ( strstr(tokens[3],",X")!=NULL )
				{
					char * ptr = strstr(tokens[3],",X");
					*ptr = '\0';
					iter = symtab.find(tokens[3]);
					if (iter != symtab.end())
					{
						hexno = ( (strtol( iter->second.c_str(),NULL,16) ) ^ 0x8000 );
						sprintf(addr,"%X",hexno);
					}
				}
				else
				{
					strcpy(addr,"0000");
				}
				strcat(instruction, addr);
				interm_file << instruction << endl;

      }
			else
			{
				if(strcmp(tokens[2],"BYTE")==0)
				{
					char byt[3];
					if(tokens[3][0] == 'C')
					{
						strcpy(instruction,"");
						for(int index = 2;tokens[3][index]!='\0';index++)
						{
							if(tokens[3][index]!='\''){
								sprintf(byt,"%02X", tokens[3][index]);
								strcat(instruction,byt);
							}
						}
							interm_file << instruction << endl;
					}

					else if(tokens[3][0] == 'X')
					{
						strcpy(instruction,"");
						for(int index = 2; tokens[3][index]!='\0';index++)
						{
							if(tokens[3][index]!='\''){
								sprintf(byt,"%c", tokens[3][index]);
								strcat(instruction,byt);
							}
						}
						interm_file << instruction << endl;
					}
					else
					{
						cout << "\nERROR in line " << lineno << endl;
					}
				}

				else if (strcmp(tokens[2],"WORD") == 0)
				{
						hexno = strtol(tokens[3], NULL, 16);
						sprintf(instruction,"%06X",hexno);
						interm_file << instruction << endl;
				}
				else
					interm_file << endl;
    	}
  	}
	}
};

int main () {
	p1 obj;
	obj.readOptab();
  obj.readSymtab();
  obj.parseFile();
}
