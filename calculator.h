#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include <cctype>

using namespace std;

double applyOp(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0) {
                cerr << "Error: Division by zero" << endl;
                return 0;
            }
            return a / b;
        default:
            return 0;
    }
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

double evaluateExpression(const string &expression) {
    stack<double> values;
    stack<char> ops;
    
    for (size_t i = 0; i < expression.length(); i++) {
        if (isspace(expression[i])) {
            continue;
        }
        
        if (isdigit(expression[i]) || (expression[i] == '-' && (i == 0 || expression[i-1] == '(' || expression[i-1] == '+' || expression[i-1] == '-' || expression[i-1] == '*' || expression[i-1] == '/'))) {
            string valStr;
            if (expression[i] == '-') {
                valStr += expression[i];
                i++;
            }
            while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                valStr += expression[i];
                i++;
            }
            values.push(stod(valStr));
            i--;
        } else if (expression[i] == '(') {
            ops.push(expression[i]);
        } else if (expression[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                double val2 = values.top();
                values.pop();
                double val1 = values.top();
                values.pop();
                char op = ops.top();
                ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            if (!ops.empty()) ops.pop();
        } else {
            if (expression[i] == '.' && (i == 0 || !isdigit(expression[i-1]))) {
                string valStr = "0.";
                i++;
                while (i < expression.length() && isdigit(expression[i])) {
                    valStr += expression[i];
                    i++;
                }
                values.push(stod(valStr));
                i--;
            } else {
                while (!ops.empty() && precedence(ops.top()) >= precedence(expression[i])) {
                    double val2 = values.top();
                    values.pop();
                    double val1 = values.top();
                    values.pop();
                    char op = ops.top();
                    ops.pop();
                    values.push(applyOp(val1, val2, op));
                }
                ops.push(expression[i]);
            }
        }
    }

    while (!ops.empty()) {
        double val2 = values.top();
        values.pop();
        double val1 = values.top();
        values.pop();
        char op = ops.top();
        ops.pop();
        values.push(applyOp(val1, val2, op));
    }

    return values.top();
}
