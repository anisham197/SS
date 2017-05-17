#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

void parseInput()
{
    fstream input_file, output_file;
    string line,temp;

    int newstart = 0, oldstart = 0, opcode, inc, bmask;

    //get new start address
    cout << "Enter the actual starting address" << endl;
    cin >> hex >> newstart;

    //i/o files
    input_file.open("input.txt", ios::in );
    output_file.open("output.txt", ios::out);

    while(getline(input_file,line)) //read each line
    {
        stringstream s; //stringstream for tokenisation
        s << line; //add line to string stream

        if(line[0] == 'H'){
            s >> temp >> temp; //read H and PROG name to temp
            s >> hex >> oldstart;  //Read original start address
            inc = newstart - oldstart; //compute increment
            continue;
        }

        if(line[0]== 'E')
        {
            break;
        }

        else if(line[0] == 'T')
        {
            int addr = 0;
            s >> temp;//skip T character
            //read start address and bit mask
            s >> hex >> addr;
            s >> hex >> bmask;
            //update start address
            addr += inc;
            while(s >> hex >> opcode)
            {
                //for each opcode check if modification is needed
                if(bmask & 0x800)
                {
                    //if modification is needed, add increment
                    opcode += inc;
                }
                output_file << uppercase << hex << addr << "\t"<< hex << opcode << endl;
                //update location counter by calculating length of opcode digits
                stringstream s2;
                s2 << hex << opcode;
		            int len = s2.str().size();

                addr += (len % 2 == 0 ) ? len/2 : (len+1)/2;
                bmask = bmask << 1;//shift bitmask by 1 for next opcode
            }
        }
    }
    input_file.close();
    output_file.close();
}

int main()
{
    parseInput();
}
