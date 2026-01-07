#include "C_utils/include/cMatrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 10

void fill_random(Matrix_t *m) {
  if (!m)
    return;
  #pragma omp parallel for schedule(dynamic)
  for (int i = 0; i < m->rows; i++) {
    for (int j = 0; j < m->cols; j++) {
      m->data[i][j] = ((double)rand() / RAND_MAX) * 20.0 - 10.0;
    }
  }
}

int main() {
  srand(time(NULL));

  fprintf(stdout, "=== Resolution de systeme d'equation matricielle ===\n");
  fprintf(stdout, "Taille des matrices : %dx%d\n", N, N);

  Matrix_t *A = mat_create(N, N);
  Matrix_t *B = mat_create(N, N);

  if (!A || !B) {
    fprintf(stderr, "Erreur critique : echec d'allocation memoire.\n");
    return 1;
  }

  fill_random(A);
  fill_random(B);

  fprintf(stdout, "Calcul de l'inverse de A...\n");
  Matrix_t *invA = mat_inverse(A);

  if (invA == NULL) {
    fprintf(stderr,
            "La matrice A est singuliere ou une erreur est survenue.\n");
    mat_free(A);
    mat_free(B);
    return 1;
  }

  fprintf(stdout, "Resolution du systeme (X = invA * B)...\n");
  Matrix_t *X = mat_mul(invA, B);

  if (X) {
    fprintf(stdout, "Systeme resolu avec succes.\n");

    double tr = mat_trace(X);
    fprintf(stdout, "Trace de la matrice solution X : %f\n", tr);

    mat_free(X);
  } else {
    fprintf(stderr, "Erreur lors de la multiplication finale.\n");
  }

  mat_free(A);
  mat_free(B);
  mat_free(invA);

  fprintf(stdout, "=== Fin du programme ===\n");
  return 0;
}