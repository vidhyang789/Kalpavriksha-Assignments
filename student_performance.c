#include <stdio.h>
#include <string.h>

typedef struct Student
{
    int roll;
    char name[50];
    int marks[3];
    float total;
    float average;
    char grade;
} Student;

char calculateGrade(float average)
{
    char student_grade;
    if (average >= 85)
    {
        student_grade = 'A';
    }
    else if (average >= 70)
    {
        student_grade = 'B';
    }
    else if (average >= 50)
    {
        student_grade = 'C';
    }
    else if (average >= 35)
    {
        student_grade = 'D';
    }
    else
    {
        student_grade = 'F';
    }
    return student_grade;
}

void inputStudentData(Student students[], int no_of_students)
{
    for (int index = 0; index < no_of_students; ++index)
    {
        scanf("%d %s %d %d %d", &students[index].roll, students[index].name, &students[index].marks[0], &students[index].marks[1], &students[index].marks[2]);
        students[index].total = students[index].marks[0] + students[index].marks[1] + students[index].marks[2];
        students[index].average = students[index].total / 3.0f;
        students[index].grade = calculateGrade(students[index].average);
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

void printStudentResult(const Student student[], int no_of_student)
{
    for (int index = 0; index < no_of_student; index++)
    {

        printf("\n");

        printf("Roll: %d\n", student[index].roll);
        printf("Name: %s\n", student[index].name);
        printf("Total: %.0f\n", student[index].total);
        printf("Average: %.2f\n", student[index].average);
        printf("Grade: %c\n", student[index].grade);

        if (student[index].grade != 'F')
        {
            printf("Performance: ");
            printPerformance(student[index].grade);
            printf("\n");
        }
    }
}

void printRollNumbersRecursive(const Student students[], int index, int count)
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
    int no_of_students;
    scanf("%d", &no_of_students);

    Student students[no_of_students];

    inputStudentData(students, no_of_students);
    printStudentResult(students, no_of_students);

    printf("\nList of Roll Numbers (via recursion): ");
    printRollNumbersRecursive(students, 0, no_of_students);

    return 0;
}
