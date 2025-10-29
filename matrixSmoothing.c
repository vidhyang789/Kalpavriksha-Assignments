#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void initializeMatrix(int **matrix, const int matrixSize)
{
    for (int i = 0; i < matrixSize; ++i)
    {
        for (int j = 0; j < matrixSize; ++j)
        {
            *(*(matrix + i) + j) = rand() % 256;
        }
    }
}

void allocateMemory(int **matrix, const int matrixSize)
{
    for (int i = 0; i < matrixSize; ++i)
    {
        *(matrix + i) = (int *)malloc(matrixSize * sizeof(int));
    }
}

static void displayMatrix(int **matrix, const int matrixSize)
{
    for (int index = 0; index < matrixSize; ++index)
    {
        for (int index2 = 0; index2 < matrixSize; ++index2)
        {
            printf("%4d", *(*(matrix + index) + index2));
        }
        printf("\n");
    }
}

static void rotateMatrix(int **matrix, const int matrixSize)
{
    for (int layer = 0; layer < matrixSize / 2; ++layer)
    {
        int first = layer;
        int last = matrixSize - 1 - layer;
        for (int index = first; index < last; ++index)
        {
            int offset = index - first;
            int *top = *(matrix + first) + index;
            int *left = *(matrix + (last - offset)) + first;
            int *bottom = *(matrix + last) + (last - offset);
            int *right = *(matrix + index) + last;

            int temp = *top;
            *top = *left;
            *left = *bottom;
            *bottom = *right;
            *right = temp;
        }
    }
}

static void smootheningFilter(int **matrix, const int matrixSize)
{
    int *previous_row = (int *)malloc(matrixSize * sizeof(int));
    int *current_row = (int *)malloc(matrixSize * sizeof(int));

    for (int col_index = 0; col_index < matrixSize; col_index++)
    {
        *(previous_row + col_index) = *(*(matrix + 0) + col_index);
    }

    for (int row_index = 0; row_index < matrixSize; row_index++)
    {
        for (int col_index = 0; col_index < matrixSize; col_index++)
        {
            *(current_row + col_index) = *(*(matrix + row_index) + col_index);
        }

        for (int col_index = 0; col_index < matrixSize; col_index++)
        {
            int sum = 0;
            int count = 0;

            for (int rowShift = -1; rowShift <= 1; rowShift++)
            {
                for (int colShift = -1; colShift <= 1; colShift++)
                {
                    int neighbour_row_index = row_index + rowShift;
                    int neighbour_col_index = col_index + colShift;

                    if (neighbour_row_index >= 0 && neighbour_row_index < matrixSize &&
                        neighbour_col_index >= 0 && neighbour_col_index < matrixSize)
                    {
                        int value;
                        if (neighbour_row_index == row_index - 1)
                        {
                            value = *(previous_row + neighbour_col_index);
                        }
                        else if (neighbour_row_index == row_index)
                        {
                            value = *(current_row + neighbour_col_index);
                        }
                        else
                        {
                            value = *(*(matrix + neighbour_row_index) + neighbour_col_index);
                        }
                        sum += value;
                        count++;
                    }
                }
            }
            *(*(matrix + row_index) + col_index) = sum / count;
        }

        int *temp = previous_row;
        previous_row = current_row;
        current_row = temp;
    }

    free(previous_row);
    free(current_row);
}

int main()
{
    int matrixSize;
    printf("Enter matrix matrixSize (2-10): ");
    scanf("%d", &matrixSize);

    if (matrixSize >= 2 || matrixSize <= 10)
    {
        int **matrix = (int **)malloc(matrixSize * sizeof(int *));

        allocateMemory(matrix, matrixSize);

        srand((unsigned int)time(NULL));
        initializeMatrix(matrix, matrixSize);

        printf("Original Randomly Generated Matrix:\n");
        displayMatrix(matrix, matrixSize);

        rotateMatrix(matrix, matrixSize);
        printf("\nMatrix after 90° Clockwise Rotation:\n");
        displayMatrix(matrix, matrixSize);

        smootheningFilter(matrix, matrixSize);
        printf("\nMatrix after Applying 3×3 Smoothing Filter:\n");
        displayMatrix(matrix, matrixSize);

        for (int i = 0; i < matrixSize; ++i)
        {
            free(*(matrix + i));
        }

        free(matrix);
    }
    return 0;
}
