#include <fstream>
#include <sstream>
#include <string>
using namespace std;

void parseInput()
{

	fstream input_file, output_file;
	string line, addr;

	// Open files
	input_file.open("objcode.txt", ios::in );
	output_file.open("output.txt", ios::out);

	// Read input file till end
	while ( getline(input_file, line) )
	{
		// Checks for header record
		if(line[0] == 'H')
			continue;

		// Checks for end record
		if ( line[0] == 'E')
			break;

		// Retrieves starting address from text record, converts to int
		addr = line.substr(3,4);

    stringstream ss(addr);
    int loc_hex;
    ss >> hex >> loc_hex ;

		// Iterates through object code bytes of text record
		for ( int i = 9; i < line.size(); i = i+2 )
		{
			  // Loads hex location and one byte to file
  			output_file << uppercase << hex << loc_hex <<"\t"<<line[i]<<line[i+1]<<endl;
        loc_hex++;
		}
	}
}

int main ()
{
	parseInput();
}
