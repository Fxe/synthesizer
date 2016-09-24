#include "blas_utilities.h"
#include <string>
#ifdef UNIX
#include <time.h>
#include <sys/time.h>
#elif WIN32
#include <Windows.h>
#endif

double wall_time()
{
#ifdef UNIX

#elif WIN32
  LARGE_INTEGER time, freq;
  if (!QueryPerformanceFrequency(&freq)){
    //  Handle error
    return 0;
  }
  if (!QueryPerformanceCounter(&time)){
    //  Handle error
    return 0;
  }
  return (double)time.QuadPart / freq.QuadPart;
#endif
}

double cpd_time()
{
#ifdef UNIX

#elif WIN32

#endif
  return 0;
}

double blas::WallTime()
{
  return wall_time();
}

double blas::CpuTime()
{
  return cpd_time();
}

double* blas::dInitArray(const int s, const double value)
{
  double* v = new double[s];
  for (int i = 0; i < s; i++)
  {
    v[i] = value;
  }
  return v;
}

double** blas::dInitMatrix(const int r, const int c, const double value)
{
  double** m = new double*[r];
  // naive way ! non contiguos
  for (int i = 0; i < r; i++)
  {
    m[i] = dInitArray(c, value);
  }
  // better ! too lazy to implement
  //m[0] = new double[r * c];
  return m;
}

