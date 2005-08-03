/*
 * File:          bHYPRE_SStructBuildVector.h
 * Symbol:        bHYPRE.SStructBuildVector-v1.0.0
 * Symbol Type:   interface
 * Babel Version: 0.10.8
 * Description:   Client-side glue code for bHYPRE.SStructBuildVector
 * 
 * WARNING: Automatically generated; changes will be lost
 * 
 * babel-version = 0.10.8
 */

#ifndef included_bHYPRE_SStructBuildVector_h
#define included_bHYPRE_SStructBuildVector_h

/**
 * Symbol "bHYPRE.SStructBuildVector" (version 1.0.0)
 */
struct bHYPRE_SStructBuildVector__object;
struct bHYPRE_SStructBuildVector__array;
typedef struct bHYPRE_SStructBuildVector__object* bHYPRE_SStructBuildVector;

/*
 * Includes for all header dependencies.
 */

#ifndef included_sidl_header_h
#include "sidl_header.h"
#endif
#ifndef included_bHYPRE_SStructGrid_h
#include "bHYPRE_SStructGrid.h"
#endif
#ifndef included_sidl_BaseInterface_h
#include "sidl_BaseInterface.h"
#endif
#ifndef included_sidl_ClassInfo_h
#include "sidl_ClassInfo.h"
#endif

#ifndef included_sidl_io_Serializer_h
#include "sidl_io_Serializer.h"
#endif
#ifndef included_sidl_io_Deserializer_h
#include "sidl_io_Deserializer.h"
#endif
#ifdef __cplusplus
extern "C" {
#endif

/**
 * RMI connector function for the class.
 */
bHYPRE_SStructBuildVector
bHYPRE_SStructBuildVector__connect(const char *, sidl_BaseInterface *_ex);
void
bHYPRE_SStructBuildVector_addRef(
  /* in */ bHYPRE_SStructBuildVector self);

void
bHYPRE_SStructBuildVector_deleteRef(
  /* in */ bHYPRE_SStructBuildVector self);

sidl_bool
bHYPRE_SStructBuildVector_isSame(
  /* in */ bHYPRE_SStructBuildVector self,
  /* in */ sidl_BaseInterface iobj);

sidl_BaseInterface
bHYPRE_SStructBuildVector_queryInt(
  /* in */ bHYPRE_SStructBuildVector self,
  /* in */ const char* name);

sidl_bool
bHYPRE_SStructBuildVector_isType(
  /* in */ bHYPRE_SStructBuildVector self,
  /* in */ const char* name);

sidl_ClassInfo
bHYPRE_SStructBuildVector_getClassInfo(
  /* in */ bHYPRE_SStructBuildVector self);

int32_t
bHYPRE_SStructBuildVector_SetCommunicator(
  /* in */ bHYPRE_SStructBuildVector self,
  /* in */ void* mpi_comm);

int32_t
bHYPRE_SStructBuildVector_Initialize(
  /* in */ bHYPRE_SStructBuildVector self);

int32_t
bHYPRE_SStructBuildVector_Assemble(
  /* in */ bHYPRE_SStructBuildVector self);

int32_t
bHYPRE_SStructBuildVector_GetObject(
  /* in */ bHYPRE_SStructBuildVector self,
  /* out */ sidl_BaseInterface* A);

/**
 * Set the vector grid.
 * 
 */
int32_t
bHYPRE_SStructBuildVector_SetGrid(
  /* in */ bHYPRE_SStructBuildVector self,
  /* in */ bHYPRE_SStructGrid grid);

/**
 * Set vector coefficients index by index.
 * 
 * NOTE: Users are required to set values on all processes that
 * own the associated variables.  This means that some data will
 * be multiply defined.
 * 
 * If the vector is complex, then {\tt value} consists of a pair
 * of doubles representing the real and imaginary parts of the
 * complex value.
 * 
 */
int32_t
bHYPRE_SStructBuildVector_SetValues(
  /* in */ bHYPRE_SStructBuildVector self,
  /* in */ int32_t part,
  /* in rarray[dim] */ int32_t* index,
  /* in */ int32_t dim,
  /* in */ int32_t var,
  /* in rarray[one] */ double* values,
  /* in */ int32_t one);

/**
 * Set vector coefficients a box at a time.
 * 
 * NOTE: Users are required to set values on all processes that
 * own the associated variables.  This means that some data will
 * be multiply defined.
 * 
 * If the vector is complex, then {\tt values} consists of pairs
 * of doubles representing the real and imaginary parts of each
 * complex value.
 * 
 */
int32_t
bHYPRE_SStructBuildVector_SetBoxValues(
  /* in */ bHYPRE_SStructBuildVector self,
  /* in */ int32_t part,
  /* in rarray[dim] */ int32_t* ilower,
  /* in rarray[dim] */ int32_t* iupper,
  /* in */ int32_t dim,
  /* in */ int32_t var,
  /* in rarray[nvalues] */ double* values,
  /* in */ int32_t nvalues);

/**
 * Set vector coefficients index by index.
 * 
 * NOTE: Users are required to set values on all processes that
 * own the associated variables.  This means that some data will
 * be multiply defined.
 * 
 * If the vector is complex, then {\tt value} consists of a pair
 * of doubles representing the real and imaginary parts of the
 * complex value.
 * 
 */
int32_t
bHYPRE_SStructBuildVector_AddToValues(
  /* in */ bHYPRE_SStructBuildVector self,
  /* in */ int32_t part,
  /* in rarray[dim] */ int32_t* index,
  /* in */ int32_t dim,
  /* in */ int32_t var,
  /* in rarray[one] */ double* values,
  /* in */ int32_t one);

/**
 * Set vector coefficients a box at a time.
 * 
 * NOTE: Users are required to set values on all processes that
 * own the associated variables.  This means that some data will
 * be multiply defined.
 * 
 * If the vector is complex, then {\tt values} consists of pairs
 * of doubles representing the real and imaginary parts of each
 * complex value.
 * 
 */
int32_t
bHYPRE_SStructBuildVector_AddToBoxValues(
  /* in */ bHYPRE_SStructBuildVector self,
  /* in */ int32_t part,
  /* in rarray[dim] */ int32_t* ilower,
  /* in rarray[dim] */ int32_t* iupper,
  /* in */ int32_t dim,
  /* in */ int32_t var,
  /* in rarray[nvalues] */ double* values,
  /* in */ int32_t nvalues);

/**
 * Gather vector data before calling {\tt GetValues}.
 * 
 */
int32_t
bHYPRE_SStructBuildVector_Gather(
  /* in */ bHYPRE_SStructBuildVector self);

/**
 * Get vector coefficients index by index.
 * 
 * NOTE: Users may only get values on processes that own the
 * associated variables.
 * 
 * If the vector is complex, then {\tt value} consists of a pair
 * of doubles representing the real and imaginary parts of the
 * complex value.
 * 
 */
int32_t
bHYPRE_SStructBuildVector_GetValues(
  /* in */ bHYPRE_SStructBuildVector self,
  /* in */ int32_t part,
  /* in rarray[dim] */ int32_t* index,
  /* in */ int32_t dim,
  /* in */ int32_t var,
  /* out */ double* value);

/**
 * Get vector coefficients a box at a time.
 * 
 * NOTE: Users may only get values on processes that own the
 * associated variables.
 * 
 * If the vector is complex, then {\tt values} consists of pairs
 * of doubles representing the real and imaginary parts of each
 * complex value.
 * 
 */
int32_t
bHYPRE_SStructBuildVector_GetBoxValues(
  /* in */ bHYPRE_SStructBuildVector self,
  /* in */ int32_t part,
  /* in rarray[dim] */ int32_t* ilower,
  /* in rarray[dim] */ int32_t* iupper,
  /* in */ int32_t dim,
  /* in */ int32_t var,
  /* inout rarray[nvalues] */ double* values,
  /* in */ int32_t nvalues);

/**
 * Set the vector to be complex.
 * 
 */
int32_t
bHYPRE_SStructBuildVector_SetComplex(
  /* in */ bHYPRE_SStructBuildVector self);

/**
 * Print the vector to file.  This is mainly for debugging
 * purposes.
 * 
 */
int32_t
bHYPRE_SStructBuildVector_Print(
  /* in */ bHYPRE_SStructBuildVector self,
  /* in */ const char* filename,
  /* in */ int32_t all);

/**
 * Cast method for interface and class type conversions.
 */
struct bHYPRE_SStructBuildVector__object*
bHYPRE_SStructBuildVector__cast(
  void* obj);

/**
 * String cast method for interface and class type conversions.
 */
void*
bHYPRE_SStructBuildVector__cast2(
  void* obj,
  const char* type);

/**
 * Select and execute a method by name
 */
void
bHYPRE_SStructBuildVector__exec(
  /* in */ bHYPRE_SStructBuildVector self,
  /* in */ const char* methodName,
  /* in */ sidl_io_Deserializer inArgs,
  /* in */ sidl_io_Serializer outArgs);
/**
 * static Exec method for reflexity.
 */
void
bHYPRE_SStructBuildVector__sexec(
  /* in */ const char* methodName,
  /* in */ sidl_io_Deserializer inArgs,
  /* in */ sidl_io_Serializer outArgs);
/**
 * Get the URL of the Implementation of this object (for RMI)
 */
char*
bHYPRE_SStructBuildVector__getURL(
  /* in */ bHYPRE_SStructBuildVector self);
struct bHYPRE_SStructBuildVector__array*
bHYPRE_SStructBuildVector__array_createCol(
  int32_t       dimen,
  const int32_t lower[],
  const int32_t upper[]);

struct bHYPRE_SStructBuildVector__array*
bHYPRE_SStructBuildVector__array_createRow(
  int32_t       dimen,
  const int32_t lower[],
  const int32_t upper[]);

struct bHYPRE_SStructBuildVector__array*
bHYPRE_SStructBuildVector__array_create1d(int32_t len);

struct bHYPRE_SStructBuildVector__array*
bHYPRE_SStructBuildVector__array_create1dInit(
  int32_t len, 
  bHYPRE_SStructBuildVector* data);

struct bHYPRE_SStructBuildVector__array*
bHYPRE_SStructBuildVector__array_create2dCol(int32_t m, int32_t n);

struct bHYPRE_SStructBuildVector__array*
bHYPRE_SStructBuildVector__array_create2dRow(int32_t m, int32_t n);

struct bHYPRE_SStructBuildVector__array*
bHYPRE_SStructBuildVector__array_borrow(
  bHYPRE_SStructBuildVector* firstElement,
  int32_t       dimen,
  const int32_t lower[],
  const int32_t upper[],
  const int32_t stride[]);

struct bHYPRE_SStructBuildVector__array*
bHYPRE_SStructBuildVector__array_smartCopy(
  struct bHYPRE_SStructBuildVector__array *array);

void
bHYPRE_SStructBuildVector__array_addRef(
  struct bHYPRE_SStructBuildVector__array* array);

void
bHYPRE_SStructBuildVector__array_deleteRef(
  struct bHYPRE_SStructBuildVector__array* array);

bHYPRE_SStructBuildVector
bHYPRE_SStructBuildVector__array_get1(
  const struct bHYPRE_SStructBuildVector__array* array,
  const int32_t i1);

bHYPRE_SStructBuildVector
bHYPRE_SStructBuildVector__array_get2(
  const struct bHYPRE_SStructBuildVector__array* array,
  const int32_t i1,
  const int32_t i2);

bHYPRE_SStructBuildVector
bHYPRE_SStructBuildVector__array_get3(
  const struct bHYPRE_SStructBuildVector__array* array,
  const int32_t i1,
  const int32_t i2,
  const int32_t i3);

bHYPRE_SStructBuildVector
bHYPRE_SStructBuildVector__array_get4(
  const struct bHYPRE_SStructBuildVector__array* array,
  const int32_t i1,
  const int32_t i2,
  const int32_t i3,
  const int32_t i4);

bHYPRE_SStructBuildVector
bHYPRE_SStructBuildVector__array_get5(
  const struct bHYPRE_SStructBuildVector__array* array,
  const int32_t i1,
  const int32_t i2,
  const int32_t i3,
  const int32_t i4,
  const int32_t i5);

bHYPRE_SStructBuildVector
bHYPRE_SStructBuildVector__array_get6(
  const struct bHYPRE_SStructBuildVector__array* array,
  const int32_t i1,
  const int32_t i2,
  const int32_t i3,
  const int32_t i4,
  const int32_t i5,
  const int32_t i6);

bHYPRE_SStructBuildVector
bHYPRE_SStructBuildVector__array_get7(
  const struct bHYPRE_SStructBuildVector__array* array,
  const int32_t i1,
  const int32_t i2,
  const int32_t i3,
  const int32_t i4,
  const int32_t i5,
  const int32_t i6,
  const int32_t i7);

bHYPRE_SStructBuildVector
bHYPRE_SStructBuildVector__array_get(
  const struct bHYPRE_SStructBuildVector__array* array,
  const int32_t indices[]);

void
bHYPRE_SStructBuildVector__array_set1(
  struct bHYPRE_SStructBuildVector__array* array,
  const int32_t i1,
  bHYPRE_SStructBuildVector const value);

void
bHYPRE_SStructBuildVector__array_set2(
  struct bHYPRE_SStructBuildVector__array* array,
  const int32_t i1,
  const int32_t i2,
  bHYPRE_SStructBuildVector const value);

void
bHYPRE_SStructBuildVector__array_set3(
  struct bHYPRE_SStructBuildVector__array* array,
  const int32_t i1,
  const int32_t i2,
  const int32_t i3,
  bHYPRE_SStructBuildVector const value);

void
bHYPRE_SStructBuildVector__array_set4(
  struct bHYPRE_SStructBuildVector__array* array,
  const int32_t i1,
  const int32_t i2,
  const int32_t i3,
  const int32_t i4,
  bHYPRE_SStructBuildVector const value);

void
bHYPRE_SStructBuildVector__array_set5(
  struct bHYPRE_SStructBuildVector__array* array,
  const int32_t i1,
  const int32_t i2,
  const int32_t i3,
  const int32_t i4,
  const int32_t i5,
  bHYPRE_SStructBuildVector const value);

void
bHYPRE_SStructBuildVector__array_set6(
  struct bHYPRE_SStructBuildVector__array* array,
  const int32_t i1,
  const int32_t i2,
  const int32_t i3,
  const int32_t i4,
  const int32_t i5,
  const int32_t i6,
  bHYPRE_SStructBuildVector const value);

void
bHYPRE_SStructBuildVector__array_set7(
  struct bHYPRE_SStructBuildVector__array* array,
  const int32_t i1,
  const int32_t i2,
  const int32_t i3,
  const int32_t i4,
  const int32_t i5,
  const int32_t i6,
  const int32_t i7,
  bHYPRE_SStructBuildVector const value);

void
bHYPRE_SStructBuildVector__array_set(
  struct bHYPRE_SStructBuildVector__array* array,
  const int32_t indices[],
  bHYPRE_SStructBuildVector const value);

int32_t
bHYPRE_SStructBuildVector__array_dimen(
  const struct bHYPRE_SStructBuildVector__array* array);

int32_t
bHYPRE_SStructBuildVector__array_lower(
  const struct bHYPRE_SStructBuildVector__array* array,
  const int32_t ind);

int32_t
bHYPRE_SStructBuildVector__array_upper(
  const struct bHYPRE_SStructBuildVector__array* array,
  const int32_t ind);

int32_t
bHYPRE_SStructBuildVector__array_length(
  const struct bHYPRE_SStructBuildVector__array* array,
  const int32_t ind);

int32_t
bHYPRE_SStructBuildVector__array_stride(
  const struct bHYPRE_SStructBuildVector__array* array,
  const int32_t ind);

int
bHYPRE_SStructBuildVector__array_isColumnOrder(
  const struct bHYPRE_SStructBuildVector__array* array);

int
bHYPRE_SStructBuildVector__array_isRowOrder(
  const struct bHYPRE_SStructBuildVector__array* array);

struct bHYPRE_SStructBuildVector__array*
bHYPRE_SStructBuildVector__array_slice(
  struct bHYPRE_SStructBuildVector__array* src,
  int32_t        dimen,
  const int32_t  numElem[],
  const int32_t  *srcStart,
  const int32_t  *srcStride,
  const int32_t  *newStart);

void
bHYPRE_SStructBuildVector__array_copy(
  const struct bHYPRE_SStructBuildVector__array* src,
  struct bHYPRE_SStructBuildVector__array* dest);

struct bHYPRE_SStructBuildVector__array*
bHYPRE_SStructBuildVector__array_ensure(
  struct bHYPRE_SStructBuildVector__array* src,
  int32_t dimen,
  int     ordering);

#ifdef __cplusplus
}
#endif
#endif
