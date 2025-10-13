#include <stdio.h>
#include <string.h>

typedef struct Student
{
    int roll;
    char name[50];
    int marks1;
    int marks2;
    int marks3;
    float total;
    float average;
    char grade;
} Student;

void inputStudentData(Student *student)
{   
    scanf("%d %s %d %d %d", &student->roll, student->name, &student->marks1, &student->marks2, &student->marks3);
}

float calculateTotal(const Student *student)
{
    return student->marks1 + student->marks2 + student->marks3;
}

float calculateAverage(const Student *student)
{
    return student->total / 3.0f;
}

char calculateGrade(float average)
{
    if (average >= 85)
    {
        return 'A';
    }
    else if (average >= 70)
    {
        return 'B';
    }
    else if (average >= 50)
    {    
        return 'C';
    }
    else if (average >= 35)
    {    
        return 'D';
    }
    else
    {    
        return 'F';
    }
}

void printPerformance(char grade)
{
    int stars = 0;

    switch (grade)
    {
        case 'A':
            stars = 5;
            break;
        case 'B':
            stars = 4;
            break;
        case 'C':
            stars = 3;
            break;
        case 'D':
            stars = 2;
            break;
        default:
            return;
    }

    for (int i = 0; i < stars; ++i)
    {
        printf("*");
    }
}

void printStudentResult(const Student *student)
{
    printf("\n");

    printf("Roll: %d\n", student->roll);
    printf("Name: %s\n", student->name);
    printf("Total: %.0f\n", student->total);
    printf("Average: %.2f\n", student->average);
    printf("Grade: %c\n", student->grade);

    if (student->grade != 'F')
    {
        printf("Performance: ");
        printPerformance(student->grade);
        printf("\n");
    }

}


void printRollNumbersRecursive(const Student *students, int index, int count)
{
    if (index == count)
    {    
        return;
    }
    
    printf("%d ", students[index].roll);
    
    printRollNumbersRecursive(students, index + 1, count);
    
}

int main()
{
    int n;
    scanf("%d", &n);

    Student students[n];
    for (int i = 0; i < n; ++i)
    {
        inputStudentData(&students[i]);
        students[i].total = calculateTotal(&students[i]);
        students[i].average = calculateAverage(&students[i]);
        students[i].grade = calculateGrade(students[i].average);
    }

    for (int i = 0; i < n; ++i)
    {
        printStudentResult(&students[i]);
    }

    printf("\nList of Roll Numbers (via recursion): ");
    printRollNumbersRecursive(students, 0, n);

    return 0;
}
