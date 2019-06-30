#include <utility>

#include <utility>

#include <utility>
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <regex>

using namespace std;

const vector<string> keywords = {"if", "times"};

enum TokenType {
    KEYWORD, NUMBER_LITERAL, IDENTIFIER, ASSIGNMENT, PLUS, MINUS, DIVIDE, MULTIPLY, NEWLINE
};

class Token {
public:
    explicit Token(char val, TokenType type) {
        this->type = type;
        this->val = string(1, val);
    }

    explicit Token(string val, TokenType type){
        this->type = type;
        this->val = std::move(val);
    }

    TokenType type;
    string val;
};

class EzProgram {
    string source;
    vector<Token> tokens;
    map<string, string> variables;
    int lines = 0;
    bool identifier_chain = false;
    bool number_chain = false;
    string chain_tmp;

    static bool exists_in(const vector<string> &check, const string &target) {
        for (auto &str: check) if (str == target) return true;
        return false;
    }

public:
    explicit EzProgram(string source) {
        this->source = move(source);
    }

    static string getEnum(int a) {
        switch (a) {
            case 0:
                return "KEYWORD";

            case 1:
                return "NUMBER_LITERAL";

            case 2:
                return "IDENTIFIER";

            case 3:
                return "ASSIGNMENT";

            case 4:
                return "PLUS";

            case 5:
                return "MINUS";

            case 6:
                return "DIVIDE";

            case 7:
                return "MULTIPLY";

            case 8:
                return "NEWLINE";

            default:
                return "UNKNOWN";
        }
    }

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

    void tokenize() {
        for (int i=0; i<source.length(); i++) {
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
                clearNumberChain();
                tokens.emplace_back(Token(token, MINUS));
            } else if (token == '*') {
                clearNumberChain();
                tokens.emplace_back(Token(token, MULTIPLY));
            } else if (token == '/') {
                clearNumberChain();
                tokens.emplace_back(Token(token, DIVIDE));
            } else if (token == '=') {
                tokens.emplace_back(Token(token, ASSIGNMENT));
            } else if(regex_match(string(1, token), regex("[a-zA-Z]"))) {
                if(identifier_chain) chain_tmp.append(string(1, token));
                else if(regex_match(string(1, source[i+1]), regex("[a-zA-Z]"))){
                    identifier_chain = true;
                    chain_tmp.append(string(1, token));
                } else tokens.emplace_back(Token(token, IDENTIFIER));
            }
        }
    }

    void printTokens() {
        for (Token &tok: tokens) {
            cout << tok.val << '[' << getEnum(tok.type) << ']' << '\n';
        }
    }
};