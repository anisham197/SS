#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
using namespace std;

fstream output_file, input_file;
string hrec, trec, erec;

// Initialising new Text Record
void initTextRecord( string loc )
{
  trec = "";
  trec = "T00" + loc + "LL";
}

// Writes text record to output file
void writeTextRecord( )
{
  int rec_len = 0;
  // Calculates length of record in half bytes
  for(int i = 9; i < trec.size(); i++ )
    rec_len++;

  // Calculates rec_len in bytes
  rec_len = (rec_len % 2 == 0) ? rec_len/2 : (rec_len + 1)/2;

  // Converts length to hex and stores
  stringstream s;
  s << uppercase << hex << rec_len;
  string hexlen = s.str();
  if ( hexlen.size() == 2){
    trec[7] = hexlen[0];
    trec[8] = hexlen[1];
  }
  else
    trec[7] = '0';
    trec[8] = hexlen[0];

  // Writes to file
  output_file << trec << endl;
}

void parseFile()
{
    string line, loc, label, opcode, operand, objcode;
    int flag = 0;
    // Open files
    input_file.open("objcode.txt", ios::in );
    output_file.open("output.txt", ios::out);

    // Read input file till end
    while ( getline(input_file, line) )
    {
      loc = "";
      label = "";
      opcode = "";
      operand = "";
      objcode = "";
      stringstream s(line);
      getline(s, loc, '\t');
      getline(s, label, '\t');
      getline(s, opcode, '\t');
      getline(s, operand, '\t');
      getline(s, objcode, '\t');

      //Add the comment line to intermediate file and go to read label
      if(line[0] == '.')
      {
          continue;
          cout << "Comment";
      }

      if(opcode == "START")
      {
          hrec = "H";
          hrec = hrec + label;
          int space = 6 - label.size();
          for ( int i = 0; i < space; i++)
              hrec = hrec + " ";
          hrec = hrec + "00" + operand + "00107A";
          output_file << hrec << endl;

          initTextRecord(operand);
          erec = "E00" + operand;
          continue;
      }

      if(opcode == "END")
      {
          writeTextRecord();
          output_file << erec << endl;
          break;
      }

      if ( objcode.size() != 0 )
      {
        if ( flag == 1 )
        {
          writeTextRecord();
          initTextRecord(loc);
          flag = 0;
        }

        if ( trec.size() + objcode.size() > 69)
        {
          cout << "Writing\n";
          writeTextRecord();
          initTextRecord(loc);
        }
        trec = trec + objcode;
      }

      // If RESB, writes previous T record, initialises new T record with location of next instruction
      else if(opcode == "RESB" || opcode == "RESW")
      {
        flag = 1;
      }
   }
   input_file.close();
   output_file.close();
}

int main ()
{
  parseFile();
}
