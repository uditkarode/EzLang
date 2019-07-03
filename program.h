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

    void tokenize(){ tokens = lexer.tokenize(source); }//for(const Token& token: tokens) { cout << token.val << " [" << Utils::getTokenTypeString(token.type) << "]\n"; } }
    void parse(){ parser.parse(tokens, variables); }
};
