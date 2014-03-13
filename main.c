#include <stdio.h>
#include "sparse_matrix.h"

int main(void)
{
  SparseMatrix* m1 = newSparseMatrix();

  setCell(m1,1,1,1);
  setCell(m1,1,2,7);
  setCell(m1,2,1,3);
  setCell(m1,2,2,4);
  setCell(m1,3,3,9);

  print(m1);

  printf("\nMultiplicacão por escalar (2):\n");
  print(multiplyByScalar(m1, 2));

  printf("\nSoma de matrizes:\n");
  SparseMatrix* m2 = newSparseMatrix();
  setCell(m2,3,1,8);
  setCell(m2,1,3,10);

  print(m1);
  printf("+\n");
  print(m2);
  printf("=\n");
  print(sum(m1, m2));

  printf("\nSubtração de matrizes:\n");  
  print(m1);
  printf("-\n");
  print(m2);
  printf("=\n");
  print(subtract(m1, m2));

  printf("\nTransposta de uma matriz:\n");
  print(m1);
  printf("{T}\n");
  print(transpose(m1));

  return 0;
}