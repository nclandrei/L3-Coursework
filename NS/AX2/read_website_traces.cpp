/**
  * File that reads a txt file and outputs IP addresses accordingly to
  * dot program rules 
 */

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[]) {
    ifstream in_file;
    ofstream out_file;

    char output_path[] = "traced-";
    strcat(output_path, argv[1]);

    string ip_addresses[30];
    int count = 0;

    in_file.open(argv[1]);
    out_file.open(output_path);

    string first_line;
    getline(in_file, first_line);

    for (string line; getline(in_file,line);) {
        size_t found;
        if ((found = line.find("*")) != string::npos) {
            break;
        }
        int end_pos = line.find(" ", 5);
        ip_addresses[count] = line.substr(4, end_pos-4);
        ++count;
    }

    for (int i = 0; i < count - 1; ++i) {
        out_file << "\"" << ip_addresses[i] << "\" -- \"" << ip_addresses[i+1] << "\"" << endl;
    } 

    in_file.close();
    out_file.close();

    return 0;
}
