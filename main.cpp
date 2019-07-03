#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <cstring>

#include "program.h"

using namespace std;

int main(int argc, char** argv){
    if(!argv[1]){
        ifstream inputFile;
        inputFile.open("test.ez");//argv[1]);

        stringstream strStream;
        strStream << inputFile.rdbuf();
        string source = strStream.str();

        if(!source.empty()){
            EzProgram program(source);
            program.tokenize();
            program.parse();
        } else {
            cout << "FATAL: no such file in the current directory\n";
        }
    } else {
        cout << "USAGE: " + string(argv[0]) + " [filename]\n";
        cout << "EzLang by uditkarode.github.io\n";
    }
    return 0;
}