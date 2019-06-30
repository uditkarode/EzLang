#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <cstring>

#include "utils.h"

using namespace std;

int main(int argc, char** argv){
    if(argv[1]){
        ifstream inputFile;
        inputFile.open(argv[1]);

        stringstream strStream;
        strStream << inputFile.rdbuf();
        string source = strStream.str();

        if(!source.empty()){
            EzProgram program(source);
            program.tokenize();
            program.printTokens();
        } else {
            cout << "FATAL: no such file in the current directory\n";
        }
    } else {
        cout << "USAGE: ez [filename]\n";
        cout << "EzLang by uditkarode.github.io\n";
    }
    return 0;
}