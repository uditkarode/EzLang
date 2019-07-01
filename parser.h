#include <utility>

#include <iostream>
#include "utils.h"

struct AstNode {
    string val;
    AstNode* left{};
    AstNode* right{};

    static AstNode* newNode(string val){
        auto node = new AstNode();
        node->val = move(val);
        node->left = nullptr;
        node->right = nullptr;
        return node;
    }
};

class Parser {
    string assignment_var;
    string tmp_expr;
    bool assignment_chain = false;

    static AstNode* constructTree(const vector<string> &expression){
        stack<AstNode *> st;
        AstNode *t, *t1, *t2;

        // Traverse through every character of
        // input expression
        for (const string& i : expression){
            // If operand, simply push into stack
            if (!Utils::isOperator(i)){
                t = AstNode::newNode(i);
                st.push(t);
            }
            else {
                t = AstNode::newNode(i);

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

    static int evalAST(AstNode* root){
        // empty tree
        if (!root)
            return 0;

        // leaf node i.e, an integer
        if (!root->left && !root->right)
            return Utils::toInt(root->val);

        // Evaluate left subtree
        int l_val = evalAST(root->left);

        // Evaluate right subtree
        int r_val = evalAST(root->right);

        // Check which operator to apply
        if (root->val=="+")
            return l_val+r_val;

        if (root->val=="-")
            return l_val-r_val;

        if (root->val=="*")
            return l_val*r_val;

        return l_val/r_val;
    }

    public:
    void parse(vector<Token> tokens, map<string, string> &variables) {
        for (int i = 0; i < tokens.size(); i++) {
            auto tok = tokens[i];
            // print handling
            if (tok.type == IDENTIFIER && tokens[i + 1].type == NEWLINE) {
                cout << variables[tok.val];
            }

            // assignment handling
            if (tok.type == ASSIGNMENT && !assignment_chain) {
                assignment_chain = true;
                assignment_var = tokens[i - 1].val;
            } else if (assignment_chain && tok.type == NEWLINE) {
                assignment_chain = false;
                vector<string> postfix = Utils::infixToPostfix(tmp_expr);
                auto root = constructTree(postfix);
                int answer = evalAST(root);
                variables[assignment_var] = to_string(answer);
                tmp_expr = assignment_var = "";
            } else if (assignment_chain) {
                tmp_expr.append(tok.val);
            }
        }
    }
};