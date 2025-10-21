#include <stdio.h>
#include <string.h>

typedef struct Student
{
    int rollNumber;
    char name[50];
    int marks[3];
    float totalMarks;
    float averageMarks;
    char grade;
} Student;

char calculateGrade(const float averageMarks)
{
    char grade;
    if (averageMarks >= 85)
    {
        grade = 'A';
    }
    else if (averageMarks >= 70)
    {
        grade = 'B';
    }
    else if (averageMarks >= 50)
    {
        grade = 'C';
    }
    else if (averageMarks >= 35)
    {
        grade = 'D';
    }
    else
    {
        grade = 'F';
    }
    return grade;
}

void inputStudentData(Student students[],const int numberOfStudents)
{
    for (int index = 0; index < numberOfStudents; ++index)
    {
        scanf("%d %s %d %d %d",
              &students[index].rollNumber,
              students[index].name,
              &students[index].marks[0],
              &students[index].marks[1],
              &students[index].marks[2]);
    }
}

void calculateStudentData(Student students[],const int numberOfStudents){
    for(int index = 0;index < numberOfStudents; ++index){    
        students[index].totalMarks =
                students[index].marks[0] +
                students[index].marks[1] +
                students[index].marks[2];
    
        students[index].averageMarks = students[index].totalMarks / 3.0f;
        students[index].grade = calculateGrade(students[index].averageMarks);
    }
}

void printPerformance(const char grade)
{
    int starCount = 0;

    switch (grade)
    {
    case 'A':
        starCount = 5;
        break;
    case 'B':
        starCount = 4;
        break;
    case 'C':
        starCount = 3;
        break;
    case 'D':
        starCount = 2;
        break;
    default:
        break;
    }

    for (int i = 0; i < starCount; ++i)
    {
        printf("*");
    }
}

void printStudentResult(const Student student[],const int numberOfStudents)
{
    for (int index = 0; index < numberOfStudents; index++)
    {
        printf("\n");

        printf("Roll: %d\n", student[index].rollNumber);
        printf("Name: %s\n", student[index].name);
        printf("Total: %.0f\n", student[index].totalMarks);
        printf("Average: %.2f\n", student[index].averageMarks);
        printf("Grade: %c\n", student[index].grade);

        if (student[index].grade == 'F')
        {
            printf("\n");
            continue;
        }
        printf("Performance: ");
        printPerformance(student[index].grade);
        printf("\n");
    }
}

void printRollNumbersRecursive(const Student students[], int index,const int totalCount)
{
    if (index == totalCount)
    {
        return;
    }

    printf("%d ", students[index].rollNumber);
    printRollNumbersRecursive(students, index + 1, totalCount);
}

int main()
{
    int numberOfStudents;
    scanf("%d", &numberOfStudents);

    Student students[numberOfStudents];

    inputStudentData(students, numberOfStudents);
    calculateStudentData(students, numberOfStudents);
    printStudentResult(students, numberOfStudents);

    printf("\nList of Roll Numbers (via recursion): ");
    printRollNumbersRecursive(students, 0, numberOfStudents);

    return 0;
}
