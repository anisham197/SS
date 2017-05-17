#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<map>
#include<iomanip>
using namespace std;

map <string, string> optab, symtab;
map<string,string>::iterator iter;

// Reading optab.txt and creating hashmap of symbol and opcode
void readOptab()
{
	fstream file;
	string opcode, value;

	file.open("optab.txt", ios::in);
	while (  file >> opcode >> value )
	{
		optab[ opcode ] = value;
	}
  file.close();
}

void parseInput()
{

	int locctr = 0, lineno = 0;
	fstream input_file, output_file;
	string line, label, opcode, operand;

	// Open files
	input_file.open("input.txt", ios::in );
	output_file.open("output.txt", ios::out);

	// Read input file till end
	while ( getline(input_file, line) )
	{
    lineno++;

    // Checks for comment line
    if(line[0] == '.')
    {
        output_file << line << endl;
        continue;
    }

		stringstream s(line);
		getline(s, label, '\t');
		getline(s, opcode, '\t');
		getline(s, operand, '\n');

    if(opcode == "START")
    {
        stringstream s(operand);
        s >> hex >> locctr;
        output_file << "\t"<< line << endl;
        continue;
    }

    else if(opcode == "END")
    {
      output_file <<setfill('0')<<setw(4) << uppercase << hex << locctr << "\t"<< line << endl;
      continue;
    }

    // Checks if MNEMONIC exits in optab
		else if ( optab.find(opcode) == optab.end())
		{
			cout << "\nERROR: Invalid mnemonic at line " << lineno << endl;
			break;
		}

    // Writes line to output file with location
    output_file <<setfill('0')<<setw(4)<< uppercase << hex << locctr <<"\t"<< line << endl;

    //check whether label is present
    if(label.size() != 0)
    {
        if(symtab.find(label) == symtab.end())
        {
            //not found, so insert
            stringstream s;
            s <<setfill('0')<<setw(4) << uppercase << hex << locctr;
            symtab[label] = s.str();
        }
        else
        {
            cout<<"\nERROR: Duplicate symbol at line "<< lineno << endl;
            break;
        }
    }

    iter = optab.find(opcode);

    //check whether variable is defined for BYTE,WORD,RESW,RESB opcodes
    if(iter->second == "*" )
    {
        if ( label.size() == 0 || operand.size() == 0){
          cout<<"\nERROR: No label/operand specified at line "<< lineno << endl;
          break;
        }

        else if ( opcode == "WORD")
            locctr += 3;

        else if(opcode == "RESW")
        {
            stringstream s(operand);
            int no;
            s>>no;
            locctr += 3*no;
        }

        else if(opcode == "RESB")
        {
            stringstream s(operand);
            int n;
            s>>n;
            locctr += n;
        }

        else if(opcode == "BYTE")
        {
            if(operand[0] == 'C')
            {
                //Subtract 3 for {C,','}
                locctr += (operand.size()-3);
            }
            else if(operand[0] == 'X')
            {
                int digits = operand.size()-3;
                locctr += (digits%2 == 0 ? digits/2 : (digits+1)/2);
            }
            else
            {
                cout<<"\nERROR: Invalid symbol for operand at line "<< lineno << endl;
                break;
            }
        }
    }
    else
    {
      // Increments by 3 if normal instruction
      locctr += 3;
    }
  }
  input_file.close();
  output_file.close();
}

void writeSymtab()
{
    fstream file;
    file.open("symtab.txt", ios::out);
    for( iter = symtab.begin(); iter != symtab.end(); iter++)
        file << iter->first << "\t" << iter->second << endl;
    file.close();
}

int main()
{
   readOptab();
   parseInput();
   writeSymtab();
   return 0;
}
