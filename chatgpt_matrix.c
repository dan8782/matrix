#include <stdio.h>
#include <stdlib.h>


typedef struct {
    int rows;
    int cols;
    double **data;
} Matrix;

void print_matrix(Matrix mat);


// Функция для создания матрицы
Matrix create_matrix(int rows, int cols) {
    Matrix mat;
    mat.rows = rows;
    mat.cols = cols;
    mat.data = malloc(rows * sizeof(double*));
    for (int i = 0; i < rows; i++) {
        mat.data[i] = malloc(cols * sizeof(double));
        for (int j = 0; j < cols; j++) {
            mat.data[i][j] = 0;
        }
    }
    return mat;
}

// Функция для очистки матрицы и освобождения памяти
void remove_matrix(Matrix mat) {
    for (int i = 0; i < mat.rows; i++) {
        free(mat.data[i]);
    }
    free(mat.data);
}

// Функция для сравнения матриц
int eq_matrix(Matrix mat1, Matrix mat2) {
    if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
        return 0;
    }
    for (int i = 0; i < mat1.rows; i++) {
        for (int j = 0; j < mat1.cols; j++) {
            if (mat1.data[i][j] != mat2.data[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}

// Функция для сложения матриц
Matrix sum_matrix(Matrix mat1, Matrix mat2) {
    if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
        printf("Error: matrices must have same size\n");
        exit(1);
    }
    Matrix result = create_matrix(mat1.rows, mat1.cols);
    for (int i = 0; i < mat1.rows; i++) {
        for (int j = 0; j < mat1.cols; j++) {
            result.data[i][j] = mat1.data[i][j] + mat2.data[i][j];
        }
    }
    return result;
}

// Функция для вычитания матриц
Matrix sub_matrix(Matrix mat1, Matrix mat2) {
    if (mat1.rows != mat2.rows || mat1.cols != mat2.cols) {
        printf("Error: matrices must have same size\n");
        exit(1);
    }
    Matrix result = create_matrix(mat1.rows, mat1.cols);
    for (int i = 0; i < mat1.rows; i++) {
        for (int j = 0; j < mat1.cols; j++) {
            result.data[i][j] = mat1.data[i][j] - mat2.data[i][j];
        }
    }
    return result;
}

// Функция для умножения матриц
Matrix mult_matrix(Matrix mat1, Matrix mat2) {
    if (mat1.cols != mat2.rows) {
        printf("Error: matrices must have compatible size\n");
        exit(1);
    }
    Matrix result = create_matrix(mat1.rows, mat2.cols);
    for (int i = 0; i < mat1.rows; i++) {
        for (int j = 0; j < mat2.cols; j++) {
            double sum = 0;
            for (int k = 0; k <
            mat1.cols; k++) {
                sum += mat1.data[i][k] * mat2.data[k][j];
            }
            result.data[i][j] = sum;
        }
    }
    return result;
}

// Функция для умножения матрицы на число
Matrix mult_number(Matrix mat, double num) {
    Matrix result = create_matrix(mat.rows, mat.cols);
    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++) {
            result.data[i][j] = mat.data[i][j] * num;
        }
    }
    return result;
}

// Функция для транспонирования матрицы
Matrix transpose(Matrix mat) {
    Matrix result = create_matrix(mat.cols, mat.rows);
    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++) {
            result.data[j][i] = mat.data[i][j];
        }
    }
    return result;
}

// Функция для вычисления определителя матрицы
double determinant(Matrix mat) {
    if (mat.rows != mat.cols) {
        printf("Error: matrix must be square\n");
        exit(1);
    }
    if (mat.rows == 1) {
        return mat.data[0][0];
    }
    if (mat.rows == 2) {
        return mat.data[0][0] * mat.data[1][1] - mat.data[0][1] * mat.data[1][0];
    }
    double det = 0;
    for (int i = 0; i < mat.cols; i++) {
        Matrix submat = create_matrix(mat.rows - 1, mat.cols - 1);
        for (int j = 1; j < mat.rows; j++) {
            for (int k = 0; k < mat.cols; k++) {
                if (k < i) {
                    submat.data[j-1][k] = mat.data[j][k];
                } else if (k > i) {
                    submat.data[j-1][k-1] = mat.data[j][k];
                }
            }
        }
        double sign = (i % 2 == 0) ? 1 : -1;
        det += sign * mat.data[0][i] * determinant(submat);
        remove_matrix(submat);
    }
    return det;
}

// Функция для вычисления матрицы алгебраических дополнений
Matrix calc_complements(Matrix mat) {
    if (mat.rows != mat.cols) {
        printf("Error: matrix must be square\n");
        exit(1);
    }
    Matrix result = create_matrix(mat.rows, mat.cols);
    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++) {
            Matrix submat = create_matrix(mat.rows - 1, mat.cols - 1);
            for (int k = 0; k < mat.rows; k++) {
                if (k == i) continue;
                for (int l = 0; l < mat.cols; l++) {
                    if (l == j) continue;
                    int row = (k < i) ? k : k - 1;
                    int col = (l< j) ? l : l - 1;
                submat.data[row][col] = mat.data[k][l];
            }
        }
        double sign = ((i+j) % 2 == 0) ? 1 : -1;
        result.data[i][j] = sign * determinant(submat);
        remove_matrix(submat);
    	}
	}
	return result;
}

// Функция для поиска обратной матрицы
Matrix inverse_matrix(Matrix mat) {
	if (mat.rows != mat.cols) {
		printf("Error: matrix must be square\n");
		exit(1);
	}	
	double det = determinant(mat);
	if (det == 0) {
		printf("Error: matrix is singular\n");
		exit(1);
	}
	Matrix comats = calc_complements(mat);
	Matrix adj = transpose(comats);
	remove_matrix(comats);
	Matrix inv = mult_number(adj, 1/det);
	remove_matrix(adj);
	return inv;
}

//Пример использования функций:

int main() {
    Matrix mat1 = create_matrix(3, 3);
    mat1.data[0][0] = 1;
    mat1.data[0][1] = 2;
    mat1.data[0][2] = 3;
    mat1.data[1][0] = 4;
    mat1.data[1][1] = 5;
    mat1.data[1][2] = 6;
    mat1.data[2][0] = 7;
    mat1.data[2][1] = 8;
    mat1.data[2][2] = 9;
    print_matrix(mat1);
    Matrix mat2 = create_matrix(3, 3);
    mat2.data[0][0] = 9;
    mat2.data[0][1] = 8;
    mat2.data[0][2] = 7;
    mat2.data[1][0] = 6;
    mat2.data[1][1] = 5;
    mat2.data[1][2] = 4;
    mat2.data[2][0] = 3;
    mat2.data[2][1] = 2;
    mat2.data[2][2] = 1;
    print_matrix(mat2);
    Matrix sum = sum_matrix(mat1, mat2);
    print_matrix(sum);
    Matrix sub = sub_matrix(mat1, mat2);
    print_matrix(sub);
    Matrix mult = mult_matrix(mat1, mat2);
    print_matrix(mult);
    Matrix mult_num = mult_number(mat1, 2);
    print_matrix(mult_num);
    Matrix trans = transpose(mat1);
    print_matrix(trans);
    double det = determinant(mat1);
    printf("Determinant: %f\n", det);
    Matrix comats = calc_complements(mat1);
    print_matrix(comats);
    Matrix inv = inverse_matrix(mat1);
    print_matrix(inv);
    remove_matrix(mat1);
    remove_matrix(mat2);
    remove_matrix(sum);
    remove_matrix(sub);
    remove_matrix(mult);
    remove_matrix(mult_num);
    remove_matrix(trans);
    remove_matrix(comats);
    remove_matrix(inv);
    return 0;
}

void print_matrix(Matrix mat) {
    printf("[\n");
    for (int i = 0; i < mat.rows; i++) {
        printf("  [");
        for (int j = 0; j < mat.cols; j++) {
            printf("%f", mat.data[i][j]);
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