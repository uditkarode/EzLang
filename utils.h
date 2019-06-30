#include <utility>
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <regex>
#include "ast.h"

using namespace std;

const vector<string> keywords = {"if", "times"};

enum TokenType {
    NUMBER_LITERAL, IDENTIFIER, ASSIGNMENT, PLUS, MINUS, DIVIDE, MULTIPLY, NEWLINE
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
    string assignment_var;
    string tmp_expr;
    bool assignment_chain = false;

    AST ast;

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

    // todo remove
    int prec(char c){
        if(c == '^')
            return 3;
        else if(c == '*' || c == '/')
            return 2;
        else if(c == '+' || c == '-')
            return 1;
        else
            return -1;
    }

    string infixToPostfix(string s){
        stack<char> st;
        st.push('N');
        int l = s.length();
        string ns;
        for(int i = 0; i < l; i++){
            // If the scanned character is an operand, add it to output string.
            if(isdigit(s[i]))
                ns+=s[i];

                // If the scanned character is an ‘(‘, push it to the stack.
            else if(s[i] == '(')

                st.push('(');

                // If the scanned character is an ‘)’, pop and to output string from the stack
                // until an ‘(‘ is encountered.
            else if(s[i] == ')')
            {
                while(st.top() != 'N' && st.top() != '(')
                {
                    char c = st.top();
                    st.pop();
                    ns += c;
                }
                if(st.top() == '(')
                {
                    char c = st.top();
                    st.pop();
                }
            }

                //If an operator is scanned
            else{
                while(st.top() != 'N' && prec(s[i]) <= prec(st.top()))
                {
                    char c = st.top();
                    st.pop();
                    ns += c;
                }
                st.push(s[i]);
            }

        }
        //Pop all the remaining elements from the stack
        while(st.top() != 'N')
        {
            char c = st.top();
            st.pop();
            ns += c;
        }

        return ns;
    }

    //todo remove

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

    bool isOperator(char c)
    {
        return c == '+' || c == '-' ||
               c == '*' || c == '/' ||
               c == '^';
    }

    AstNode* constructTree(string expression){
        stack<AstNode *> st;
        AstNode *t, *t1, *t2;

        // Traverse through every character of
        // input expression
        for (int i=0; i<expression.length(); i++)
        {
            // If operand, simply push into stack
            if (!isOperator(expression[i]))
            {
                t = AST::newNode(expression[i]);
                st.push(t);
            }
            else // operator
            {
                t = AST::newNode(expression[i]);

                // Pop two top nodes
                t1 = st.top(); // Store top
                st.pop();      // Remove top
                t2 = st.top();
                st.pop();

                //  make them children
                t->right = t1;
                t->left = t2;

                // Add this subexpression to stack
                st.push(t);
            }
        }

        //  only element will be root of expression
        // tree
        t = st.top();
        st.pop();

        return t;
    }

    int toInt(string s)
    {
        int num = 0;

        // Check if the integral value is
        // negative or not
        // If it is not negative, generate the number
        // normally
        if(s[0]!='-')
            for (int i=0; i<s.length(); i++)
                num = num*10 + (int(s[i])-48);
            // If it is negative, calculate the +ve number
            // first ignoring the sign and invert the
            // sign at the end
        else
            for (int i=1; i<s.length(); i++)
            {
                num = num*10 + (int(s[i])-48);
                num = num*-1;
            }

        return num;
    }

    int eval(AstNode* root)
    {
        // empty tree
        if (!root)
            return 0;

        // leaf node i.e, an integer
        if (!root->left && !root->right)
            return toInt(root->val);

        // Evaluate left subtree
        int l_val = eval(root->left);

        // Evaluate right subtree
        int r_val = eval(root->right);

        // Check which operator to apply
        if (root->val=="+")
            return l_val+r_val;

        if (root->val=="-")
            return l_val-r_val;

        if (root->val=="*")
            return l_val*r_val;

        return l_val/r_val;
    }

    void parse(){
        for(int i=0; i<tokens.size(); i++){
            auto tok = tokens[i];
            if(tok.type == ASSIGNMENT && !assignment_chain){
                assignment_chain = true;
                assignment_var = tokens[i-1].val;
            } else if(assignment_chain && tok.type == NEWLINE){
                assignment_chain = false;
                string postfix = infixToPostfix(tmp_expr);
                auto root = constructTree(postfix);
                int answer = eval(root);
                //variables
                cout << answer << '\n';//answer << "WASTHEANSWER";
                tmp_expr = assignment_var = "";
            } else if(assignment_chain){
                tmp_expr.append(tok.val);
            }
        }
    }

    void printTokens() {
        for (Token &tok: tokens) {
            cout << tok.val << " [" << getEnum(tok.type) << ']' << '\n';
        }
    }
};