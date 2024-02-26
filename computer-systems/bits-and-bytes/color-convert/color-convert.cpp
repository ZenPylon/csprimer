#include <iostream>
#include <fstream>
#include <regex.h>

using namespace std;

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "Usage: color-convert css_file.css" << endl;
        exit(1);
    }
    string line;
    ifstream css_file (argv[1]);

    if (!css_file.is_open()) {
        cout << "Unable to open CSS File. Please double-check the path and filename." << endl;
        exit(1);
    }

    while (getline(css_file, line)) {
        cout << line << '\n';
    }

    return 0;
}