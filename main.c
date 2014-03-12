#include <stdio.h>
#include "sparse_matrix.h"

int main(void)
{
  SparseMatrix* m = newSparseMatrix();

  setCell(m,1,1,1);
  setCell(m,1,2,2);
  setCell(m,2,1,3);
  setCell(m,2,2,4);
  setCell(m,5,5,5);
  setCell(m,3,3,9);

  print(m);

  return 0;
}