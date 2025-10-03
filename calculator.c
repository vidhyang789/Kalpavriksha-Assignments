#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

int values[MAX];
char ops[MAX];
int valTop = -1, opTop = -1;

void pushVal(int x) { 
    values[++valTop] = x; 
}

int popVal() {
    return values[valTop--]; 
}

void pushOp(char x) { 
    ops[++opTop] = x; 
}

char popOp() {
    return ops[opTop--]; 
}

int precedence(char op) { 
    int value = 0;
    if (op == '+' || op == '-') value = 1;
    else if (op == '*' || op == '/') value = 2;
    return value;
}

int applyOp(int a, int b, char op, int *errorFlag) {  
    int result = 0;
    switch (op) {
        case '+': result = a + b; break;
        case '-': result = a - b; break;
        case '*': result = a * b; break;
        case '/':
            if (b == 0) {
                *errorFlag = 1;
                return 0;
            }
            result =  a / b;
            break;
    }
    return result;
}

int evaluateExpression(const char *expr, int *errorFlag) {
    int index;
    int expressionLength = strlen(expr);
    for (index = 0; index < expressionLength; index++) {
        if (expr[index] == ' ') continue;

        if (isdigit(expr[index])) { 
            int val = 0;
            while (index < expressionLength && isdigit(expr[index])) {
                val = (val * 10) + (expr[index] - '0');
                index++;
            }
            pushVal(val);
            index--;
        }
        else if (expr[index] == '+' || expr[index] == '-' ||
                 expr[index] == '*' || expr[index] == '/') {  
            while (opTop != -1 && precedence(ops[opTop]) >= precedence(expr[index])) {
                int val2 = popVal();
                int val1 = popVal();
                char op = popOp();
                int result = applyOp(val1, val2, op, errorFlag);
                if (*errorFlag) return 0;
                pushVal(result);
            }
            pushOp(expr[index]);
        }
        else {
            *errorFlag = 2; 
            return 0;
        }
    }

    while (opTop != -1) {   
        int val2 = popVal();
        int val1 = popVal();
        char op = popOp();
        int result = applyOp(val1, val2, op, errorFlag);
        if (*errorFlag) return 0;
        pushVal(result);
    }

    return popVal();
}

int main() {
    char expr[200];
    printf("Enter expression: ");
    fgets(expr, sizeof(expr), stdin);
    expr[strcspn(expr, "\n")] = '\0';

    int errorFlag = 0;
    int result = evaluateExpression(expr, &errorFlag);

    if (errorFlag == 1) {
        printf("Error: Division by zero.\n");
    } else if (errorFlag == 2) {
        printf("Error: Invalid expression.\n");
    } else {
        printf("%d\n", result);
    }

    return 0;
}
