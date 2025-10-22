#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void displayMatrix(int *matrixPtr, int matrixSize)
{
    for (int index = 0; index < matrixSize; ++index)
    {
        for (int index2 = 0; index2 < matrixSize; ++index2)
        {
            printf("%3d", *(matrixPtr + index * matrixSize + index2));
        }
        printf("\n");
    }
}

static void rotateMatrix(int *matrixPtr, int matrixSize)
{
    for (int layer = 0; layer < matrixSize / 2; ++layer)
    {
        int first = layer;
        int last = matrixSize - 1 - layer;
        for (int index = first; index < last; ++index)
        {
            int offset = index - first;
            int *top = matrixPtr + first * matrixSize + index;
            int *left = matrixPtr + (last - offset) * matrixSize + first;
            int *bottom = matrixPtr + last * matrixSize + (last - offset);
            int *right = matrixPtr + (index)*matrixSize + last;
            int temp = *top;
            *top = *left;
            *left = *bottom;
            *bottom = *right;
            *right = temp;
        }
    }
}

static void applySmoothingFilter(int *matrixPtr, int matrixSize)
{
    for (int index = 0; index < matrixSize; ++index)
    {
        for (int index2 = 0; index2 < matrixSize; ++index2)
        {
            int sum = 0;
            int count = 0;
            for (int row = -1; row <= 1; ++row)
            {
                for (int col = -1; col <= 1; ++col)
                {
                    int newRowIndex = index + row;
                    int newColIndex = index2 + col;
                    if (newRowIndex >= 0 && newRowIndex < matrixSize && newColIndex >= 0 && newColIndex < matrixSize)
                    {
                        sum += *(matrixPtr + newRowIndex * matrixSize + newColIndex);
                        ++count;
                    }
                }
            }
            *(matrixPtr + index * matrixSize + index2) = sum / count;
        }
    }
}

int main(void)
{
    int size;

    printf("Enter matrix size (2-10): ");
    scanf("%d", &size);

    if (size < 2 || size > 10)
    {
        return 0;
    }

    int matrix[size][size];
    int *matrixPtr = &matrix[0][0];

    srand((unsigned int)time(NULL));

    for (int index = 0; index < size; ++index)
    {
        for (int index2 = 0; index2 < size; ++index2)
        {
            *(matrixPtr + index * size + index2) = rand() % 256;
        }
    }

    printf("Original Randomly Generated Matrix:\n");
    displayMatrix(matrixPtr, size);
    rotateMatrix(matrixPtr, size);

    printf("\nMatrix after 90° Clockwise Rotation:\n");
    displayMatrix(matrixPtr, size);
    applySmoothingFilter(matrixPtr, size);

    printf("\nMatrix after Applying 3×3 Smoothing Filter:\n");
    displayMatrix(matrixPtr, size);

    return 0;
}
