#include <fstream>
#include <cstring>
#include <stdlib.h>

using namespace std;

class p1 {

	public:

	void parseInput()
	{

		fstream input_file, output_file;
		string line, addr;
		char cbuff[100], hex_loc[4];
    		int loc;

		// Open files
		input_file.open("objcode.txt", ios::in );
		output_file.open("abs_load_output.txt", ios::out | ios::app);

		// Read input file till end
		while ( getline(input_file, line) )
		{
			// Clears cbuff from previous input
			memset(&cbuff, 0, sizeof(cbuff));

			// Copies input line to cbuff(char array)
			strcpy(cbuff, line.c_str());

      			// Checks for header record
      			if(cbuff[0] == 'H')
        			continue;

			// Checks for end record
      			if ( cbuff[0] == 'E')
        			break;

			// Retrieves starting address from text record, converts to int
      			addr = line.substr(3,4);
      			loc = strtol(addr.c_str(), NULL, 16);

			// Iterates through object code bytes of text record
      			for ( int i = 9; cbuff[i] != '\0'; i = i+2 )
      			{
          			sprintf(hex_loc, "%X", loc);
				// Loads hex location and one byte to file
          			output_file << hex_loc <<"\t"<<cbuff[i]<<cbuff[i+1]<<endl;
          			loc = loc + 1;
      			}

    		}
  	}
};

int main ()
{
	p1 obj;
	obj.parseInput();
}
