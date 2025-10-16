#include <stdio.h>
#include <string.h>

typedef struct Student
{
    int rollNumber;
    char studentName[50];
    int marksArray[3];
    float totalMarks;
    float averageMarks;
    char studentGrade;
} Student;

char calculateGrade(float averageMarks)
{
    char studentGrade;
    if (averageMarks >= 85)
    {
        studentGrade = 'A';
    }
    else if (averageMarks >= 70)
    {
        studentGrade = 'B';
    }
    else if (averageMarks >= 50)
    {
        studentGrade = 'C';
    }
    else if (averageMarks >= 35)
    {
        studentGrade = 'D';
    }
    else
    {
        studentGrade = 'F';
    }
    return studentGrade;
}

void inputStudentData(Student students[], int noOfStudents)
{
    for (int index = 0; index < noOfStudents; ++index)
    {
        scanf("%d %s %d %d %d",
              &students[index].rollNumber,
              students[index].studentName,
              &students[index].marksArray[0],
              &students[index].marksArray[1],
              &students[index].marksArray[2]);

        students[index].totalMarks =
            students[index].marksArray[0] +
            students[index].marksArray[1] +
            students[index].marksArray[2];

        students[index].averageMarks = students[index].totalMarks / 3.0f;
        students[index].studentGrade = calculateGrade(students[index].averageMarks);
    }
}

void printPerformance(char grade)
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

void printStudentResult(const Student student[], int noOfStudents)
{
    for (int index = 0; index < noOfStudents; index++)
    {
        printf("\n");

        printf("Roll: %d\n", student[index].rollNumber);
        printf("Name: %s\n", student[index].studentName);
        printf("Total: %.0f\n", student[index].totalMarks);
        printf("Average: %.2f\n", student[index].averageMarks);
        printf("Grade: %c\n", student[index].studentGrade);

        if (student[index].studentGrade != 'F')
        {
            printf("Performance: ");
            printPerformance(student[index].studentGrade);
            printf("\n");
        }
    }
}

void printRollNumbersRecursive(const Student students[], int index, int totalCount)
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
    int noOfStudents;
    scanf("%d", &noOfStudents);

    Student students[noOfStudents];

    inputStudentData(students, noOfStudents);
    printStudentResult(students, noOfStudents);

    printf("\nList of Roll Numbers (via recursion): ");
    printRollNumbersRecursive(students, 0, noOfStudents);

    return 0;
}
