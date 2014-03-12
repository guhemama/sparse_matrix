typedef struct Cell
{
  long         _x;
  long         _y;
  long         _value;
  struct Cell* _nextCell;
} Cell;

typedef struct Index
{
  long          _value;
  Cell*         _cell;
  struct Index* _nextIndex;
} Index;

typedef struct SparseMatrix
{
  Index* _firstIndex;
  long   _maximumX;
  long   _maximumY;
} SparseMatrix;

SparseMatrix* newSparseMatrix();
Index* newIndex(long value);
Cell* newCell(long x, long y, long value);

SparseMatrix* sum(SparseMatrix* m1, SparseMatrix* m2);
SparseMatrix* subtract(SparseMatrix* m1, SparseMatrix* m2);
SparseMatrix* transpose(SparseMatrix* matrix);
SparseMatrix* multiply(SparseMatrix* m1, SparseMatrix* m2);
SparseMatrix* multiplyByScalar(SparseMatrix* matrix, long scalar);

void setCell(SparseMatrix* matrix, long x, long y, long value);
Cell* getCell(SparseMatrix* matrix, long x, long y);
long getCellValue(SparseMatrix* matrix, long x, long y);

void print(SparseMatrix* matrix);