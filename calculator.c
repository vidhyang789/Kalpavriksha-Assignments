#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

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
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

int applyOp(int a, int b, char op, int *errorFlag) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) {
                *errorFlag = 1; // Division by zero
                return 0;
            }
            return a / b; // Integer division
    }
    return 0;
}

int evaluateExpression(const char *expr, int *errorFlag) {
    int i;
    for (i = 0; i < strlen(expr); i++) {
        if (expr[i] == ' ') continue;

        if (isdigit(expr[i])) {
            int val = 0;
            while (i < strlen(expr) && isdigit(expr[i])) {
                val = (val * 10) + (expr[i] - '0');
                i++;
            }
            pushVal(val);
            i--;
        }
        else if (expr[i] == '+' || expr[i] == '-' ||
                 expr[i] == '*' || expr[i] == '/') {
            while (opTop != -1 && precedence(ops[opTop]) >= precedence(expr[i])) {
                int val2 = popVal();
                int val1 = popVal();
                char op = popOp();
                int result = applyOp(val1, val2, op, errorFlag);
                if (*errorFlag) return 0;
                pushVal(result);
            }
            pushOp(expr[i]);
        }
        else {
            *errorFlag = 2; // Invalid character
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
    expr[strcspn(expr, "\n")] = '\0'; // Remove newline

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
