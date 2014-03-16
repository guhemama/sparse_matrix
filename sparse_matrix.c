#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sparse_matrix.h"

/**
 * Aloca memória para a matriz e inicializa valores
 */
SparseMatrix* newSparseMatrix()
{
  SparseMatrix* m = malloc(sizeof(SparseMatrix));

  m->_firstIndex = NULL;
  m->_maximumX   = 0;
  m->_maximumY   = 0;

  return m;
}

/**
 * Aloca memória para um índice e inicializa valores
 */
Index* newIndex(long value)
{
  Index* idx = malloc(sizeof(Index));

  idx->_value      = value;
  idx->_cell       = NULL;
  idx->_nextIndex  = NULL;

  return idx;
}

/**
 * Aloca memória para uma célula e inicializa valores
 */
Cell* newCell(long x, long y, long value)
{
  Cell* cell = malloc(sizeof(Cell));

  cell->_x     = x;
  cell->_y     = y;
  cell->_value = value;

  return cell;
}

/**
 * Seta o valor de uma célula da matriz. Se já existir apenas altera o valor.
 */
void setCell(SparseMatrix* matrix, long x, long y, long value)
{
  Cell* cell = getCell(matrix, x, y);

  // Se existir, a célula troca o valor
  if (cell != NULL)
  {
    cell->_value = value;
    return;
  }

  // Verifica os índices máximos da matriz, e os troca se preciso
  if (x > matrix->_maximumX)
  {
    matrix->_maximumX = x;
  }

  if (y > matrix->_maximumY)
  {
    matrix->_maximumY = y;
  }

  // Cria uma nova célula
  cell = newCell(x, y, value);

  // Verifica se o índice (linha) atual existe
  Index* index     = matrix->_firstIndex;
  Index* cellIndex = NULL;

  while (index != NULL)
  {
    // Se a linha for igual, existe
    if (index->_value == y)
    {
      cellIndex = index;
      break;
    }

    index = index->_nextIndex;
  }

  // Se o índice não existir, cria um novo
  if (cellIndex == NULL)
  {
    cellIndex = newIndex(y);
  }

  // Adiciona o índice a matriz
  // Se o primeiro for vazio, tá certo
  if (matrix->_firstIndex == NULL)
  {
    matrix->_firstIndex = cellIndex;
  }
  // Senão insere o novo índice na ordem
  else
  {
    Index* currentIndex  = matrix->_firstIndex;
    Index* previousIndex = matrix->_firstIndex;

    while (true)
    {
      // Se for maior, vai para frente, senão insere
      if (currentIndex == NULL || cellIndex->_value < currentIndex->_value)
      {
        previousIndex->_nextIndex = cellIndex;
        cellIndex->_nextIndex     = currentIndex;
        break;
      }
      else
      {
        previousIndex = currentIndex;
        currentIndex  = currentIndex->_nextIndex;
      }
    }
  }

  // Insere a célula
  if (cellIndex->_cell == NULL)
  {
    cellIndex->_cell = cell;
  }
  else
  {
    Cell* currentCell  = cellIndex->_cell;
    Cell* previousCell = currentCell;

    while (true)
    {
      // Se o índice x for maior, vai para frente, senão insere
      if (currentCell == NULL || cell->_x < currentCell->_x)
      {
        previousCell->_nextCell = cell;
        cell->_nextCell         = currentCell;
        break;
      }
      else
      {
        previousCell = currentCell;
        currentCell  = currentCell->_nextCell;
      }
    }
  }
}

/**
 * Busca o valor de uma célula.
 */
Cell* getCell(SparseMatrix* matrix, long x, long y)
{
  Index* idx = matrix->_firstIndex;

  while (idx != NULL)
  {
    if (idx->_value == y)
    {
      Cell* cell = idx->_cell;

      while (cell != NULL)
      {
        if (cell->_x == x)
        {
          return cell;
        }

        cell = cell->_nextCell;
      }

      return NULL;
    }

    idx = idx->_nextIndex;
  }

  return NULL;
}

/**
 * Pega o valor de uma célula. Retorna 0 se não existe.
 */
long getCellValue(SparseMatrix* matrix, long x, long y)
{
  Cell* cell = getCell(matrix, x, y);

  if (cell == NULL)
  {
    return 0;
  }

  return cell->_value;
}

/**
 * Imprime a matriz a partir do eixo das ordenadas
 */
void print(SparseMatrix* matrix)
{
  Index* pivot = matrix->_firstIndex;

  int i, j;

  long curY
     , prevY = 0
     , curX
     , prevX = 0
     , xDiff
     , yDiff
     , counter;

  while (pivot != NULL)
  {
    // Se a diferença entre índices de linhas for maior que 1,
    // imprime n linhas de caracteres zerados
    yDiff = pivot->_value - prevY - 1;

    if (yDiff >= 1)
    {
      for (i = 0; i < yDiff; i++)
      {
        for (j = 0; j < matrix->_maximumX; j++)
        {
          printf("0\t");
        }

        printf("\n");
      }
    }

    Cell* cell = pivot->_cell;

    while (true)
    {
      // Se a célula é nula, checa em qual chegamos
      if (cell == NULL)
      {
        // Se chegou ao final, quebra loop
        xDiff = matrix->_maximumX - prevX;

        if (xDiff > 0)
        {
          // Senáo, imprime zeros restantes
          for (i = 0; i < xDiff; i++)
          {
            printf("0\t");
          }
        }

        prevX = 0;
        break;
      }
      else
      {
        xDiff = cell->_x - prevX - 1;

        if (xDiff > 0)
        {
          for (i = 0; i < xDiff; i++)
          {
            printf("0\t");
          }
        }

        printf("%ld\t", cell->_value);

        prevX = cell->_x;
        cell  = cell->_nextCell;
      }
    }

    printf("\n");

    prevY = pivot->_value;
    pivot = pivot->_nextIndex;
  }
}

/**
 * Soma duas matrizes, desde que com dimensões iguais
 * @param  m1 [description]
 * @param  m2 [description]
 * @return    [description]
 */
SparseMatrix* sum(SparseMatrix* m1, SparseMatrix* m2)
{
  if (m1->_maximumX != m2->_maximumX || m1->_maximumY != m2->_maximumY)
  {
    printf("As matrizes a serem somadas devem ter o mesmo número de linhas e colunas.\n");
    exit(1);
  }

  SparseMatrix* matrix = newSparseMatrix();

  long x, y, sum;

  for (x = 1; x <= m1->_maximumX; x++)
  {
    for (y = 1; y <= m1->_maximumY; y++)
    {
      sum = getCellValue(m1, x, y) + getCellValue(m2, x, y);

      if (sum != 0)
      {
        setCell(matrix, x, y, sum);
      }
    }

  }

  return matrix;
}

/**
 * Subtrai duas matrizes, desde que com dimensões iguais
 * @param  m1 [description]
 * @param  m2 [description]
 * @return    [description]
 */
SparseMatrix* subtract(SparseMatrix* m1, SparseMatrix* m2)
{
  if (m1->_maximumX != m2->_maximumX || m1->_maximumY != m2->_maximumY)
  {
    printf("As matrizes a serem somadas devem ter o mesmo número de linhas e colunas.\n");
    exit(1);
  }

  SparseMatrix* matrix = newSparseMatrix();

  long x, y, diff;

  for (x = 1; x <= m1->_maximumX; x++)
  {
    for (y = 1; y <= m1->_maximumY; y++)
    {
      diff = getCellValue(m1, x, y) - getCellValue(m2, x, y);

      if (diff != 0)
      {
        setCell(matrix, x, y, diff);
      }
    }

  }

  return matrix;
}

/**
 * Cria a transposta de uma matriz
 * @param  matrix [description]
 * @return        [description]
 */
SparseMatrix* transpose(SparseMatrix* matrix)
{
  SparseMatrix* m = newSparseMatrix();
  long x, y, cellValue;

  for (x = 1; x <= matrix->_maximumX; x++)
  {
    for (y = 1; y <= matrix->_maximumY; y++)
    {
      cellValue = getCellValue(matrix, x, y);

      if (cellValue != 0)
      {
        setCell(m, y, x, cellValue);
      }
    }
  }

  return m;
}

/**
 * Multiplica duas matrizes
 * @param  m1 [description]
 * @param  m2 [description]
 * @return    [description]
 */
SparseMatrix* multiply(SparseMatrix* m1, SparseMatrix* m2)
{
  SparseMatrix* result = newSparseMatrix();

  // Verifica se colunas1 == linhas2
  if (m1->_maximumX != m2->_maximumY)
  {
    printf("As matrizes a serem multiplicadas devem ter o mesmo número de colunas e linhas, respectivamente.\n");
    exit(1);
  }

  long x, y, k;

  // Quantidade de elementos de cada linha/coluna
  long max = m1->_maximumX;

  for (x = 1; x <= m1->_maximumY; x++)
  {
    for (y = 1; y <= m2->_maximumX; y++)
    {
      long cellValue = 0;

      // a11.b11 + a21.b12 + a31.b13
      for (k = 1; k <= max; k++)
      {
        cellValue += getCellValue(m1, k, x) * getCellValue(m2, y, k);
      }

      setCell(result, y, x, cellValue);
    }
  }

  return result;
}

/**
 * Multiplica uma matriz por um escalar
 * @param  matrix [description]
 * @param  scalar [description]
 * @return        [description]
 */
SparseMatrix* multiplyByScalar(SparseMatrix* matrix, long scalar)
{
  SparseMatrix* m = newSparseMatrix();
  long x, y, cellValue;

  for (x = 1; x <= matrix->_maximumX; x++)
  {
    for (y = 1; y <= matrix->_maximumY; y++)
    {
      cellValue = getCellValue(matrix, x, y);

      if (cellValue != 0)
      {
        cellValue *= scalar;
        setCell(m, x, y, cellValue);
      }
    }
  }

  return m;
}