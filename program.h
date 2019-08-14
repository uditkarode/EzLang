#include "utils.h"
#include "lexer.h"
#include "parser.h"

class EzProgram {
    string source;

    vector<Token> tokens;
    map<string, string> variables;

    Lexer lexer;
    Parser parser;


public:
    explicit EzProgram(string source) { this->source = move(source); }

    void tokenize(){ tokens = lexer.tokenize(source); }//for(Token s: tokens){ cout << s.val << " (" << Utils::getTokenTypeString(s.type) << ") "; } }
    void parse(){ parser.parse(tokens, variables); }
};
