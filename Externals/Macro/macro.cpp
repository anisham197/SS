#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
using namespace std;

map <string, string> namtab;
map<string,string>::iterator iter;

void readMacros()
{
  fstream input_file, macro_file, middle_file;
	string line, label, opcode, operand;
  int count = 1;

	// Open files
	input_file.open("input.txt", ios::in );
  middle_file.open("middle.txt", ios::out);

	// Read input file till end
	while ( getline(input_file, line) )
	{
    stringstream s(line);
    getline(s, label, '\t');
    getline(s, opcode, '\t');
    getline(s, operand, '\n');

    // Checks for Macro
    if(opcode == "MACRO")
    {
      // Store each macro in a different file
      stringstream ss;
      ss << count;
      string filename = ss.str();

      // Store the filename along with macro name
      namtab[label] = filename;

      // Open the file to store macro definition
      macro_file.open( filename.c_str(), ios::out);

      while( getline(input_file, line) )
			{
				stringstream s2 (line);
        getline(s2, label, '\t');
        getline(s2, opcode, '\t');
        getline(s2, operand, '\n');

        if(opcode == "MEND")
        {
          count++; // Increment count to change filename of next macro
          macro_file.close();
          break;
        }
        // Write line to macro file
        macro_file << line << endl;
      }
    }

    else
      // Stores all lines from input_file excluding macro definitions
      middle_file << line << endl;
  }
  input_file.close();
}

void parseInput()
{
  fstream input_file, macro_file, output_file;
  string line, label, opcode, operand;

  // Open files
  input_file.open("middle.txt", ios::in );
  output_file.open("output.txt", ios::out);

  // Read input file till end
	while ( getline(input_file, line) )
	{
    stringstream s(line);
    getline(s, label, '\t');
    getline(s, opcode, '\t');
    getline(s, operand, '\n');

    if(opcode == "END")
    {
      output_file << line << endl;
      break;
    }

    // If macro is called
    else if ( (iter = namtab.find(opcode)) != namtab.end())
    {
        // Write the label (if any) before macro call to file
        output_file << label;

        // Retrieve filename where corresponding macro_definition is stored and open
        string filename = iter -> second;
        macro_file.open( filename.c_str(), ios::in);

        // Write out contents of macro_defintion to output file
        while ( getline (macro_file, line))
          output_file << line << endl;

        macro_file.close();
    }

    else
		{
      // Write out other lines
			output_file << line << endl;
		}

  }
}

int main ()
{
  readMacros();
  parseInput();
  return 0;
}
