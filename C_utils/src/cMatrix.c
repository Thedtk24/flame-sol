#include <stdio.h>
#include <stdlib.h>
#include "../include/cMatrix.h"

Matrix_t* mat_create(int rows, int cols){
    if(rows <= 0 || cols <= 0) return NULL;

    Matrix_t* mat = malloc(sizeof(Matrix_t));
    if(!mat) return NULL;

    mat -> rows = rows;
    mat -> cols = cols;

    mat -> data = (double**)malloc(rows * sizeof(double*));
    if(!mat -> data){
        free(mat);
        return NULL;
    }

    for(int i = 0; i < rows; i++){
        mat -> data[i] = (double*)malloc(cols * sizeof(double));
        if(!mat -> data[i]){
            for(int k = 0; k < i; k++) free(mat -> data[k]);
            free(mat -> data);
            free(mat);
            return NULL;
        }
        for(int j = 0; j < cols; j++){
            mat -> data[i][j] = 0.0;
        }
    }

    return mat;
}

void mat_free(Matrix_t* mat) {
    if (!mat) return; 
    for (int i = 0; i < mat -> rows; i++) {
        free(mat -> data[i]);
    }
    free(mat -> data);
    free(mat);
}

void mat_print(Matrix_t* mat){
    if(!mat){
        printf("Matrice vide (NULL)\n");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < mat -> rows; i++){
        printf("| ");
        for(int j = 0; j < mat -> cols; j++){
            printf("%8.2f ", mat -> data[i][j]);
        }
        printf("|\n");
    }

    printf("\n");
}

Matrix_t* mat_identity(int n){
    if (n <= 0) return NULL;
    Matrix_t* mat = mat_create(n, n);
    if(!mat) return NULL;
    for(int i = 0; i < n; i++){
        mat -> data[i][i] = 1.0;
    }

    return mat;
}

Matrix_t* mat_copy(Matrix_t* mat){
    if (!mat) return NULL;
    Matrix_t* new_mat = mat_create(mat -> rows, mat -> cols);
    if (!new_mat) return NULL;
    for(int i = 0; i < mat -> rows; i++){
        for(int j = 0; j < mat -> cols; j++){
            new_mat -> data[i][j] = mat -> data[i][j];
        }
    }

    return new_mat;
}

double mat_trace(Matrix_t* mat){
    if(mat -> rows != mat -> cols){
        printf("Ereur dimensions\n");
        exit(EXIT_FAILURE);
    }
    double somme = 0;
    for(int i = 0; i < mat -> rows; i++){
        somme = somme + mat -> data[i][i];
    }
    return somme;
}

int issize_mat(Matrix_t* a, Matrix_t* b) {
    if (!a || !b) {
        return 0; 
    }
    return (a->rows == b->rows && a->cols == b->cols);
}


Matrix_t* mat_add(Matrix_t* a, Matrix_t* b){
    if(!issize_mat(a, b)) return NULL;
    Matrix_t* add = mat_create(a ->rows, a -> cols);
    if (!add) return NULL;
    for(int i = 0; i < a -> rows; i++){
        for(int j = 0; j < a -> cols; j++){
            add -> data[i][j] = a -> data[i][j] + b -> data[i][j];
        }
    }

    return add;
}

Matrix_t* mat_sub(Matrix_t* a, Matrix_t* b){
    if(!issize_mat(a, b)) return NULL;
    Matrix_t* sub = mat_create(a ->rows, a -> cols);
    if (!sub) return NULL;
    for(int i = 0; i < a -> rows; i++){
        for(int j = 0; j < a -> cols; j++){
            sub -> data[i][j] = a -> data[i][j] - b -> data[i][j];
        }
    }

    return sub;
}

Matrix_t* mat_mul(Matrix_t* a, Matrix_t* b){
    if(!a || !b) return NULL;
    if(a -> cols != b -> rows){
        printf("Erreur dimensions\n");
        exit(EXIT_FAILURE);
    }
    Matrix_t* mul = mat_create(a -> rows, b -> cols);
    if(!mul) return NULL;
    for (int i = 0; i < a -> rows; i++) {
        for (int j = 0; j < b -> cols; j++) {
            double sum = 0.0;
            for (int k = 0; k < a -> cols; k++) {
                sum += a -> data[i][k] * b -> data[k][j];
            }
            mul -> data[i][j] = sum;
        }
    }

    return mul;
}

Matrix_t* mat_scalar_mul(Matrix_t* a, double k){
    if(!a) return NULL;
    Matrix_t* res = mat_create(a -> rows, a -> cols);
    if(!res) return NULL;
    for(int i = 0; i < a -> rows; i++){
        for(int j = 0; j < a -> cols; j++){
            res -> data[i][j] = k * a -> data[i][j];
        }
    }

    return res;
}

Matrix_t* mat_transpose(Matrix_t* a){
    if (!a) return NULL;
    Matrix_t* transpose = mat_create(a -> cols, a -> rows);
    if (!transpose) return NULL;
    for (int i = 0; i < a -> rows; i++) {
        for (int j = 0; j < a -> cols; j++) {
            transpose -> data[j][i] = a -> data[i][j];
        }
    }
    return transpose;
}

static Matrix_t* mat_mineur(Matrix_t* m, int range_retire, int collone_retire) {
    int n = m -> rows;
    Matrix_t* mineur = mat_create(n - 1, n - 1);
    int r = 0;
    for (int i = 0; i < n; i++) {
        if (i == range_retire) continue;
        int c = 0;
        for (int j = 0; j < n; j++) {
            if (j == collone_retire) continue;
            mineur -> data[r][c] = m -> data[i][j];
            c++;
        }
        r++;
    }
    return mineur;
}

double mat_determinant(Matrix_t* m) {
    if (!m || m -> rows != m -> cols) return 0.0;
    int n = m -> rows;
    if (n == 1) {
        return m -> data[0][0];
    }
    if (n == 2) {
        return m -> data[0][0] * m -> data[1][1] - m -> data[0][1] * m -> data[1][0];
    }
    double det = 0.0;
    for (int j = 0; j < n; j++) {
        Matrix_t* mineur = mat_mineur(m, 0, j);
        double cofactor = ((j % 2 == 0) ? 1 : -1) * m -> data[0][j] * mat_determinant(mineur);
        det = det + cofactor;
        mat_free(mineur);
    }
    return det;
}

Matrix_t* mat_inverse(Matrix_t* m) {
    if (!m || m -> rows != m -> cols) return NULL;
    int n = m -> rows;
    double det = mat_determinant(m);
    if (det == 0) return NULL;
    Matrix_t* cofactors = mat_create(n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            Matrix_t* mineur = mat_mineur(m, i, j);
            double c = ((i + j) % 2 == 0 ? 1 : -1) * mat_determinant(mineur);
            cofactors->data[i][j] = c;
            mat_free(mineur);
        }
    }
    Matrix_t* adj = mat_transpose(cofactors);
    mat_free(cofactors);
    Matrix_t* inv = mat_scalar_mul(adj, 1.0 / det);
    mat_free(adj);
    return inv;
}