#include "s21_matrix.h"


int main(int argc, char const *argv[])
{
	matrix_t Mat;
	s21_create_matrix(3,4, &Mat);
	print_matrix(Mat);
	return 0;
}


int s21_create_matrix(int rows, int columns, matrix_t *result){
    matrix_t mat;
    mat.rows = rows;
    mat.cols = columns;
    mat.matrix = malloc(rows * sizeof(double*));
    for (int i = 0; i < rows; i++) {
        mat.matrix[i] = malloc(columns * sizeof(double));
        for (int j = 0; j < columns; j++) {
            mat.matrix[i][j] = 0;
        }
    }
	*result = mat;
	return 0;
}


void print_matrix(matrix_t mat) {
    printf("[\n");
    for (int i = 0; i < mat.rows; i++) {
        printf("  [");
        for (int j = 0; j < mat.cols; j++) {
            printf("%f", mat.matrix[i][j]);
            if (j != mat.cols - 1) {
                printf(", ");
            }
        }
        printf("]");
        if (i != mat.rows - 1) {
            printf(",");
        }
        printf("\n");
    }
    printf("]\n");
}