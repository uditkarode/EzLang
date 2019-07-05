#include "utils.h"

class Lexer {
    vector<Token> tokens;
    bool identifier_chain = false;
    bool number_chain = false;
    string chain_tmp;
    int lines = 0;

    void clearNumberChain(){
        if(number_chain){
            tokens.emplace_back(Token(chain_tmp, NUMBER_LITERAL));
            number_chain = false;
            chain_tmp = "";
        }
    }

    void clearIdentifierChain(){
        if(identifier_chain){
            tokens.emplace_back(Token(chain_tmp, IDENTIFIER));
            identifier_chain = false;
            chain_tmp = "";
        }
    }

    public:
    vector<Token> tokenize(string source) {
        bool noConsider = false;

        for (int i=0; i<source.length(); i++) {
            if(noConsider){
                noConsider = false;
                continue;
            }

            char token = source[i];

            if(token == ' ' || token == '\n'){
                clearIdentifierChain();
                clearNumberChain();
            }

            if (token == ' ') continue;
            else if (token == '\n') {
                lines++;
                tokens.emplace_back(Token("\\n", NEWLINE));
            } else if (isdigit(token)) {
                if(number_chain) chain_tmp.append(string(1, token));
                else if(isdigit(source[i+1])){
                    number_chain = true;
                    chain_tmp.append(string(1, token));
                } else tokens.emplace_back(Token(token, NUMBER_LITERAL));
            } else if (token == '+') {
                clearNumberChain();
                tokens.emplace_back(Token(token, PLUS));
            } else if (token == '-') {
                //todo work this out
                clearNumberChain();
                if(isdigit(source[i+1])){
                    tokens.emplace_back(Token(source[i+1], NEGATIVE_NUMBER_LITERAL));
                    noConsider = true;
                } else {
                    tokens.emplace_back(Token(token, MINUS));
                }
            } else if (token == '*') {
                clearNumberChain();
                tokens.emplace_back(Token(token, MULTIPLY));
            } else if (token == '/') {
                clearNumberChain();
                tokens.emplace_back(Token(token, DIVIDE));
            } else if (token == '=') {
                tokens.emplace_back(Token(token, ASSIGNMENT));
            } else if(token == '('){
                tokens.emplace_back(Token(token, OPENING_BRACKET));
            } else if(token == ')'){
                tokens.emplace_back(Token(token, CLOSING_BRACKET));
            } else if(regex_match(string(1, token), regex("[a-zA-Z]"))) {
                if(identifier_chain) chain_tmp.append(string(1, token));
                else if(regex_match(string(1, source[i+1]), regex("[a-zA-Z]"))){
                    identifier_chain = true;
                    chain_tmp.append(string(1, token));
                } else tokens.emplace_back(Token(token, IDENTIFIER));
            }
        }
        return tokens;
    }
};