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
     , prevY = 1
     , curX
     , prevX = 1
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
          printf("0 ");
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
            printf("0 ");
          }
        }

        prevX = 1;
        break;
      }
      else
      {
        xDiff = cell->_x - prevX;

        if (xDiff > 1)
        {
          for (i = 0; i < xDiff; i++)
          {
            printf("0 ");
          }
        }

        printf("%lu ", cell->_value);

        prevX = cell->_x;
        cell  = cell->_nextCell;
      }
    }

    printf("\n");

    prevY = pivot->_value;
    pivot = pivot->_nextIndex;
  }
}

SparseMatrix* sum(SparseMatrix* m1, SparseMatrix* m2);
SparseMatrix* subtract(SparseMatrix* m1, SparseMatrix* m2);
SparseMatrix* transpose(SparseMatrix* matrix);
SparseMatrix* multiply(SparseMatrix* m1, SparseMatrix* m2);
SparseMatrix* multiplyByScalar(SparseMatrix* matrix, long scalar);