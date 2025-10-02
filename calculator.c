#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

int values[MAX];
char ops[MAX];
int valTop = -1, opTop = -1;

//so actually i am using here some stack logic in which opTop is the pointer to stack of 
//operations stack of operations generally contains all the operations and valTop is the 
//pointer to the top of values and it contains values in which to perform so as the operation
//comes we will check if any operations is there whose priority is more than first we perform 
//them and save the result again in the stack of values as this we go further

void pushVal(int x) {   //to push the value in values array
    values[++valTop] = x; 
}

int popVal() { //to pop the value in values array
    return values[valTop--]; 
}

void pushOp(char x) { //to push the operations in ops array
    ops[++opTop] = x; 
}

char popOp() { //to pop the operation in ops array
    return ops[opTop--]; 
}

int precedence(char op) {  //selecting priority
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

int applyOp(int a, int b, char op, int *errorFlag) {  //to apply operation btwn two numbers
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

        if (isdigit(expr[i])) {  //if digit store complete digit in stack
            int val = 0;
            while (i < strlen(expr) && isdigit(expr[i])) {
                val = (val * 10) + (expr[i] - '0');
                i++;
            }
            pushVal(val);
            i--;
        }
        else if (expr[i] == '+' || expr[i] == '-' ||
                 expr[i] == '*' || expr[i] == '/') {  //if more priority exist then first complete opr then push
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

    while (opTop != -1) {   //do with remaining
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
