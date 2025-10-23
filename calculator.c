#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STACK_SIZE 100

int operandStack[MAX_STACK_SIZE];
char operatorStack[MAX_STACK_SIZE];
int operandTop = -1, operatorTop = -1;

void pushOperand(int value) { 
    operandStack[++operandTop] = value; 
}

int popOperand() { 
    return operandStack[operandTop--]; 
}

void pushOperator(char op) { 
    operatorStack[++operatorTop] = op; 
}

char popOperator() { 
    return operatorStack[operatorTop--]; 
}

int getPrecedence(char operatorSymbol) { 
    int value = 0;
    if (operatorSymbol == '+' || operatorSymbol == '-'){ 
        value = 1;
    }
    else if (operatorSymbol == '*' || operatorSymbol == '/'){ 
        value = 2;
    }
    return value;
}

int performOperation(int leftOperand, int rightOperand, char operatorSymbol, int *errorFlag) {  
    int result = 0;
    switch (operatorSymbol) {
        case '+': {
            result = leftOperand + rightOperand; 
            break;
        }
        case '-': {
            result = leftOperand - rightOperand; 
            break;
        }
        case '*': {
            result = leftOperand * rightOperand; 
            break;
        }
        case '/': {
            if (rightOperand == 0) {
                *errorFlag = 1;
                return 0;
            }
            result = leftOperand / rightOperand;
            break;
        }
    }
    return result;
}

int evaluateExpression(const char *expression, int *errorFlag) {
    int index;
    int expressionLength = strlen(expression);
    int finalResult = 0;

    for (index = 0; index < expressionLength; index++) {
        if (expression[index] == ' ') continue;

        if (isdigit(expression[index])) { 
            int value = 0;
            while (index < expressionLength && isdigit(expression[index])) {
                value = (value * 10) + (expression[index] - '0');
                index++;
            }
            pushOperand(value);
            index--;
        }
        else if (expression[index] == '+' || expression[index] == '-' ||
                 expression[index] == '*' || expression[index] == '/') {  
            while (operatorTop != -1 && 
                   getPrecedence(operatorStack[operatorTop]) >= getPrecedence(expression[index])) {
                int rightOperand = popOperand();
                int leftOperand = popOperand();
                char operatorSymbol = popOperator();
                int result = performOperation(leftOperand, rightOperand, operatorSymbol, errorFlag);
                if (*errorFlag) break;
                pushOperand(result);
            }
            if (*errorFlag) break;
            pushOperator(expression[index]);
        }
        else {
            *errorFlag = 2;
            break;
        }
    }

    if (*errorFlag == 0) {
        while (operatorTop != -1) {   
            int rightOperand = popOperand();
            int leftOperand = popOperand();
            char operatorSymbol = popOperator();
            int result = performOperation(leftOperand, rightOperand, operatorSymbol, errorFlag);
            if (*errorFlag) break;
            pushOperand(result);
        }
    }

    if (*errorFlag == 0) {
        finalResult = popOperand();
    }

    return finalResult;
}


int main() {
    char expression[200];
    printf("Enter expression: ");
    fgets(expression, sizeof(expression), stdin);
    expression[strcspn(expression, "\n")] = '\0';

    int errorFlag = 0;
    int result = evaluateExpression(expression, &errorFlag);

    if (errorFlag == 1) {
        printf("Error: Division by zero.\n");
    } else if (errorFlag == 2) {
        printf("Error: Invalid expression.\n");
    } else {
        printf("Result: %d\n", result);
    }

    return 0;
}
