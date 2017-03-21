#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <map>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

class p1 {

	fstream input_file, output_file;
  	map <string, string> symtab;
	map <string, string>::iterator iter;

	public:

	// Reading symtab.txt and creating hashmap of label and location
	void readSymtab() 
	{

    		fstream file;
    		string input1, input2;

    		file.open("symtab.txt", ios::in );
    		while (  file >> input1 >> input2 ) 
		{
      			symtab.insert ( make_pair ( input1 , input2 ) ) ;
    		}
  	}

	// Initialising new Text Record
	void initTextRecord( char* trec, unsigned int loc )
	{
		char temp[7];
		trec[0] = 'T';
		trec[1] = '\0';			// Clears previous text record
		sprintf(temp,"%06XLL",loc); 	// Adds location, LL for length
		strcat(trec,temp);
	}

	// Writes text record to output file
	void writeTextRecord( char* trec)
	{
		char hexlen[3];
		int rec_len = 0; 
		// Calculates length of record in half bytes
		for(int i = 9; trec[i]!='\0'; i++ )
			rec_len++;

		// If length of record = 0, doesnt write to file ( incase of RESW, RESB )
		if(rec_len == 0) 
			return;

		// Calculates rec_len in bytes
		rec_len = (rec_len % 2 == 0) ? rec_len/2 : (rec_len + 1)/2;

		// Converts length to hex and stores
		sprintf(hexlen,"%02X",rec_len);
		trec[7] = hexlen[0];
		trec[8] = hexlen[1];

		// Writes to file
		output_file << trec << endl;
	}

  	void parseFile() 
	{
    		unsigned int locctr = 0,hexno = 0;
    		string line;
    		char cbuff[100], tokens[5][20];
    		char hrec[20], trec[70], erec[8],tempstr[7];
    		char *str, *token;

    		// Open files
    		input_file.open("interim.txt", ios::in );
    		output_file.open("objcode.txt", ios::out | ios::app);

		// Read input file till end
    		while ( getline(input_file, line) )
    		{
      			// Copies input line to cbuff(char array)
      			strcpy(cbuff, line.c_str());

      			// Checks for comment line
			if(cbuff[0] == '.')
				continue;			

			// Clears all tokens
			for ( int i = 0; i < 5; i++)
				memset(&tokens[i], 0, sizeof(tokens[i]));

      			// Copies to str (char pointer), tokenizes into LABEL, MNEMONIC, OPERAND
      			str = strdup (cbuff);
      			int i = 0;
      
			// Splits by "\t" delimiter
      			while((token = strsep(&str, "\t")) && i < 5) 
			{
          			strcpy(tokens[i],token);
          			i++;
      			}

      			// Check if Mnemonic is START
      			if (strcmp(tokens[2],"START") == 0)
      			{
				// Gets inital location        			
				locctr = strtol(tokens[3], NULL, 16);

				// Gets final location
        			iter = symtab.find("END");
        			hexno = strtol(iter->second.c_str(), NULL, 16);

				// Initialises Header Record
        			hrec[0] = 'H';

				// Copies filename ( 6 places ), fills remaining slots with space
        			strncpy(tokens[1], tokens[1], 6);
        			for ( int i = 1; i < 7; i++)
              				hrec[i] = (tokens[1][i-1]!='\0'?tokens[1][i-1]:' ');

				// Stores inital location and length of file
        			sprintf(tempstr,"%06X",locctr);
        			strcat(hrec,tempstr);
        			sprintf(tempstr,"%06X",hexno-locctr);
        			strcat(hrec,tempstr);

				// Outputs header record
        			output_file << hrec << endl;

				// Initialises text record
				locctr = strtol(tokens[3], NULL, 16);
        			initTextRecord(trec, locctr);

				// Initialises end record with address of first executable instruction
  				sprintf(erec,"E%06x",locctr);
        			continue;
      			}

			// If END, writes last text record and end record to output file
      			else if (strcmp(tokens[2],"END") == 0)
      			{
				writeTextRecord(trec);
        			output_file << erec << endl;
        			continue;
      			}

			
			// If instruction has Object Code
			if ( strlen(tokens[4]) != 0 )
			{
				// Checks if text record exceeds length, writes text record and initialises new text record
				if( (strlen(trec)+strlen(tokens[4]) ) > 69)
				{
					writeTextRecord(trec);
					locctr = strtol(tokens[0], NULL, 16);
					initTextRecord(trec, locctr);
				}
				// Adds object code to text record
				strcat(trec, tokens[4]);
			}

			// If RESB, writes previous T record, initialises new T record with location of next instruction
			else if(strcmp(tokens[2],"RESB") == 0)
			{
				locctr = strtol(tokens[0], NULL, 16);
				// Number of bytes reserved given in decimal
				unsigned int tloc = locctr + strtol(tokens[3], NULL, 10);
				writeTextRecord(trec);
				initTextRecord(trec,tloc);
			}

			// If RESW, writes previous T record, initialises new T record with location of next instruction
			else if(strcmp(tokens[2],"RESW") == 0)
			{
				locctr = strtol(tokens[0], NULL, 16);
				// Number of words reserved given in decimal
				unsigned int tloc = locctr + strtol(tokens[3], NULL, 10)*3;
				writeTextRecord(trec);
				initTextRecord(trec,tloc);
			}
    		}
  	}
};

int main () 
{
	p1 obj;
	obj.readSymtab();
 	obj.parseFile();
}
