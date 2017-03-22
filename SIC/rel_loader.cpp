#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <bitset>

using namespace std;

/* File does not have length of text record before bit mask,
File has only 3 byte object codes, no space separated object program */

void parseInput()
{

	fstream input_file, output_file;
	string line, old_addr, new_addr, trec_addr, bitmask, binary;
	char hex_loc[4], hex_opcode[7];
	int old_loc, bm, inc, new_loc, trec_loc, new_trec_loc;

	// Open files
	input_file.open("load_input.txt", ios::in );
	output_file.open("reloc_output.txt", ios::out);

	cout << "Enter the actual starting address" << endl;
	cin >> new_addr;

	// Convert new starting address to int
	new_loc = strtol(new_addr.c_str(), NULL, 16);

	// Read input file till end
	while ( getline(input_file, line) )
	{
		// Checks for header record
		if(line[0] == 'H'){

			// Retrieves old starting address
			old_addr = line.substr(9,4);
			old_loc = strtol(old_addr.c_str(), NULL, 16);

			// Calculates value to incr/decr each location by
			inc = new_loc - old_loc;
			continue;
		}

		// Checks for end record
		if ( line[0] == 'E')
			break;

		// Retrieves starting address from text record, converts to int
		trec_addr = line.substr(3,4);
		trec_loc = strtol(trec_addr.c_str(), NULL, 16);

		//  Calculate new locaton properly
		new_trec_loc = trec_loc + inc ;

		// Identifies bitmask, converts to binary
    		bitmask = line.substr(7, 3);
    		bm = strtol(bitmask.c_str(), NULL, 16);
    		binary = bitset<12>(bm).to_string();

		// Iterates through object code bytes of text record
    		int count = 0;
    		int i;
		for ( i = 10; line[i] != '\0'; i = i+6 )
		{
			// Converts location to hex
        		sprintf(hex_loc, "%X", new_trec_loc);

			// If bitmask 0 for objectcode, print as it is with new location
        		if ( binary[count] == '0' )
        		{
            			output_file << hex_loc <<"\t" << line.substr(i,6)<<endl;
        		}

        		else
        		{
				// Obtain object code to modify
               			string temp = line.substr(i,6);

          			int temp_opcode = strtol(temp.c_str(), NULL, 16);
          			temp_opcode += inc;

				// Convert modified object code to hex
          			sprintf(hex_opcode, "%X", temp_opcode);
          			output_file << hex_loc <<"\t" << hex_opcode << endl;

        		}

			// Increment location and bit mask index
  			new_trec_loc = new_trec_loc + 3;
        		count++;
		}

	}
}

int main ()
{
	parseInput();
}
