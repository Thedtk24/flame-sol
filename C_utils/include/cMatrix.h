#ifndef CMATRIX_H
#define CMATRIX_H

typedef struct{
    int rows;
    int cols;
    double** data;
} Matrix_t;

Matrix_t* mat_create(int rows, int cols);
void mat_free(Matrix_t* m);
Matrix_t* mat_identity(int n);
Matrix_t* mat_copy(Matrix_t* m);

void mat_print(Matrix_t* m);
double mat_trace(Matrix_t* m);

Matrix_t* mat_add(Matrix_t* a, Matrix_t* b);      
Matrix_t* mat_sub(Matrix_t* a, Matrix_t* b);      
Matrix_t* mat_mul(Matrix_t* a, Matrix_t* b);       
Matrix_t* mat_scalar_mul(Matrix_t* a, double k);  
Matrix_t* mat_transpose(Matrix_t* a);             

double mat_determinant(Matrix_t* m);
Matrix_t* mat_inverse(Matrix_t* m);

int issize_mat(Matrix_t* a, Matrix_t* b);

#endif