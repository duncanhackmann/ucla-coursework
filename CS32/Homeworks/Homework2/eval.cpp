//
//  eval.cpp
//  Homework 2
//
//  Created by Duncan Hackmann on 5/1/23.
//

#include <stdio.h>
#include <iostream>
#include <string>
#include <stack>
#include <cassert>
using namespace std;


bool valid (const string& infix);
void convert(const string& infix, string& postfix);
int evaluate(string infix, string& postfix, bool& result) {
    if (valid(infix)) {
        convert(infix, postfix);
        
        stack<char> operands;
        char ch;
        char operand1;
        char operand2;
        
        for (int i = 0; i < postfix.length(); i++) {
            ch = postfix[i];
            if (ch == 'T' || ch == 'F') {
                operands.push(ch);
            } else if (ch == '!') {
                operand1 = operands.top();
                operands.pop();
                if(operand1 == 'T') {
                    operands.push('F');
                } else {
                    operands.push('T');
                }
            } else {
                operand2 = operands.top();
                operands.pop();
                operand1 = operands.top();
                operands.pop();
                
                if (ch == '&') {
                    if (operand1 == 'T' && operand2 == 'T') {
                        operands.push('T');
                    } else {
                        operands.push('F');
                    }
                }
                
                if (ch == '^') {
                    if ((operand1 == 'T' && operand2 == 'F') || (operand1 == 'F' && operand2 == 'T')) {
                        operands.push('T');
                    } else {
                        operands.push('F');
                    }
                }
            }
        }
        
        if (operands.top() == 'F') {
            result = false;
        } else {
            result = true;
        }
        
        return 0;
    } else {
        return 1;
    }
}


void convert(const string& infix, string& postfix) {
    postfix = "";
    stack<char> operators;
    char ch;
    
    for (int i = 0; i < infix.length(); i++) {
        ch = infix[i];
        switch (ch) {
            case 'T':
                postfix += ch;
                break;
            
            case 'F':
                postfix += ch;
                break;
                
            case '(':
                operators.push(ch);
                break;
                
            case ')':
                while (operators.top() != '(') {
                    postfix += operators.top();
                    operators.pop();
                }
                operators.pop();
                break;
                
            case '!':
                while (!operators.empty() && operators.top() != '(' && operators.top() != '&' && operators.top() != '^') {
                    postfix += operators.top();
                    operators.pop();
                }
                operators.push(ch);
                break;
                
            case '&':
                while (!operators.empty() && operators.top() != '(' && operators.top() != '^') {
                    postfix += operators.top();
                    operators.pop();
                }
                operators.push(ch);
                break;
                
            case '^':
                while (!operators.empty() && operators.top() != '(') {
                    postfix += operators.top();
                    operators.pop();
                }
                operators.push(ch);
                break;
                
            default:
                break;
        }
    }
    
    
    while (!operators.empty()) {
        postfix += operators.top();
        operators.pop();
    }
}

bool valid(const string& infix)
{
    char current;
    char next;
    // tring with no spaces to make easier
    string s = "";
    
    for(int i = 0; i < infix.length(); i++) {
        current = infix[i];
        if(current!=' ') {
            s+=current;
        }
    }
    
    char last = s[s.length()-1];
    
    // check last char of string to see if valid
    if(last != 'T' && last != 'F' && last != ')') {
        return false;
    }
    
    // check if empty
    if(infix.length() == 0) {
        
        return false;
    }
    
    // check for string of length 1
    if(infix.length()==1) {
        if(infix[0]!='F' && infix[0]!='T') {
            
            return false;
        } else {
            return true;
        }
    }
    
    // check if parentheses are balanced
    int open = 0;
    int closed = 0;
    for(int i = 0; i<s.length(); i++) {
        current=s[i];
        if (current == '(') {
            open++;
        }
        if(current==')') {
            closed++;
        }
    }
    
    if(closed != open) {
        
        return false;
    }
    
    // check each char in the string and its following char
    for(int i=0; i<s.length()-1; i++) {
        current = s[i];
        next = s[i+1];
        
        if (current=='T')
        {
            if (next != ')' && next != '^' && next != '&')
            {
                
                return false;
            }
        }
        
        else if (current == 'F') {
            if(next != ')' && next != '^' && next != '&')
            {
                
                return false;
            }
        }
        
        else if (current == '!') {
            if(next!='T' && next!='F' && next!='(' && next!='!')
            {
                
                return false;
            }
        } else if (current == '&') {
            if (next!='T' && next!='F' && next!='(' && next!='!') {
                
                return false;
            }
        }
        
        else if (current=='^')
        {
            if (next!='T' && next!='F' && next!='(' && next!='!') {
                
                return false;
            }
        }
        
        else if (current=='(') {
            if (next!='T' && next!='F' && next!='!' && next != '(') {
                
                return false;
            }
        }
        
        else if (current==')') {
            if (next!='^' && next!='&' && next != ')' && next != '(') {
                return false;
            }
            
        } else {
            return false;
        }
    }
    
    return true;
}

int main()
        {
    string postfix;
    bool result;
    
    cout << evaluate("T&T", postfix, result) << endl;
    cout << postfix << endl;
    cout << result << endl;
        }
