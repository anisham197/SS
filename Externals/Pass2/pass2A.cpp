#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<map>
#include<iomanip>
using namespace std;

map<string,string> optab;
map<string,string> symtab;
map<string,string>::iterator iter;

//Read from optab and dave into map
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

//Read from symtab and dave into map
void readSymtab()
{
  fstream file;
  string label, location;

  file.open("symtab.txt", ios::in);
  while (  file >> label >> location )
  {
    symtab[label] = location;
  }
  file.close();
}

void parseInput()
{
  int lineno = 0;
	fstream output_file, input_file;
  string line, loc, label, opcode, operand;
  string object_code;

	// Open files
	input_file.open("input.txt", ios::in );
	output_file.open("objcode.txt", ios::out);

  while(getline(input_file,line))
  {
      object_code = "";
      loc = "";
      label = "";
      opcode = "";
      operand = "";
      stringstream s(line);
      getline(s, loc, '\t');
      getline(s, label, '\t');
      getline(s, opcode, '\t');
      getline(s, operand, '\n');
      lineno++;

      //Add the comment line to intermediate file and go to read label
      if(line[0] == '.')
      {
          output_file << line << endl;
          continue;
      }

      //Write to intermediate file and go to read label
      if(opcode == "START" || opcode == "END")
      {
          output_file << line << endl;
          continue;
      }

      //check whether opcode is prsent in optab
      if (optab.find(opcode) == optab.end())
      {
          cout<<"ERROR: Opcode not valid at line "<< lineno;
          break;
      }
      //Write the line to file
      output_file << line <<"\t";

      //get the iter pointing at opcode
      iter = optab.find(opcode);
      if(iter->second != "*")
      {
          //copy opcode to object code
          object_code =  iter->second;

          //check whether operand is there
          if(operand.size() != 0)
          {
              // if symbol is present in symbol table, concatenate symbol value to obj code
              if((iter = symtab.find(operand)) != symtab.end())
              {
                  object_code = object_code + iter->second;
              }

              //Check whether index reg is used,  npos signifies end of string
              else if(operand.find(",X") != string::npos)
              {
                  //remove ',X' from operand and get actual operand
                  int len = operand.size()-2;
                  string temp = operand.substr(0,len);

                 //check whether it's present in symtab
                  if((iter = symtab.find(temp)) != symtab.end())
                  {
                      int addr;
                      stringstream ss(iter->second);
                      //stores value in addr, hex converts hex to decimal and vice versa
                      ss >> hex >> addr;
                      //OR the int to set index reg bit to '1'
                      addr = addr ^ 0x8000;  //32768;
                      //Convert x to hex string and concatenate to obj code
                      stringstream s2;
                      s2 << hex << addr;
                      object_code += s2.str();
                  }
                  else
                  {
                      cout<<"\nERROR: Invalid operand at line "<<lineno;
                      break;
                  }
              }
              else
              {
                  cout<<"\nERROR: Invalid operand at line "<<lineno;
                  break;
              }
          }
          else
          { // If operand size is 0
            object_code = object_code + "0000";
          }

      }
      else
      {
          if (opcode == "BYTE")
          {
              stringstream ss;
              if(operand[0] == 'C')
              {
                  //remove {c,','} from operand
                  string sub = operand.substr(2);
                  //size-1 to remove the last '
                  //convert ascii value of each char to hex and store it
                  for(int i = 0; i < sub.size()-1; i++)
                      ss << uppercase << hex << (int)sub[i];
                  object_code += ss.str();
              }
              else if(operand[0] == 'X')
              {
                  string sub = operand.substr(2);
                  //append the hex values directly to obj code
                  for(int i = 0; i < sub.size()-1; i++)
                      object_code += sub[i];
              }
              else
              {
                  cout<<"\nerror ";
                  break;
              }
          }
          else if(opcode == "WORD")
          {
              //convert the string to decimal num and the hex string
              stringstream ss(operand);
              int value;
              ss>>value;
              stringstream s2;
              //object code length should be 6
              s2<<setfill('0')<<setw(6)<<uppercase << hex<<value;
              object_code = s2.str();
          }
      }
      output_file<< object_code << endl;
  }
  input_file.close();
  output_file.close();
}

int main()
{
    readOptab();
    readSymtab();
    parseInput();
    return 0;
}
