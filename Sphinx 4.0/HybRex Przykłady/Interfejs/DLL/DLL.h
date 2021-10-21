//---------------------------------------------------------------------------
#ifndef DLLH
#define DLLH
//---------------------------------------------------------------------------
/*
#ifdef __DLL__
   #define DLL_EXP __declspec(eksport)
#else
   #define DLL_EXP __declspec(import)
#endif
*/
//---------------------------------------------------------------------------
#include <vcl.h>
#include <vcl/dstring.h>

//---------------------------------------------------------------------------
#define NO_CHANGE_METHOD                0
#define REGRESSION                      1
#define A_R                             2
#define EXPONENTIAL_SMOOTHING           3
#define NEURAL_NET                      4
#define MULTIDIMENSION_REGRESSION       5
#define MULTIDIMENSION_NEURAL_NET       6
//----------------------------------------------------------------------------
#define NO_CHANGE_METHOD_DES                "Szereg Ÿród³owy"
#define REGRESSION_TIME_SERIES_DES          "Regresja dla szeregu czasowego"
#define REGRESSION_DES                      "Regresja"
#define STEP_REGRESSION_DES                 "Regresja krokowa"
#define AR_DES                              "Model autoregresji"
#define EXPONENTIAL_SMOOTHING_DES           "Wyg³adzanie wyk³adniczne"
#define MULTIDIMENSION_REGRESSION_DES       "Regresja wielowymiarowa"
#define NEURAL_NET_DES                      "Sieæ neuronowa"
#define MULTIDIMENSION_NEURAL_NET_DES       "Nieliniowa regresja wielowymiarowa"

//----------------------------------------------------------------------------

#define SHEET_NAME_INPUT  "input"
#define SHEET_NAME_OUTPUT "output"
#define SHEET_NAME_PARAM  "parametry"

//---------------------------------------------------------------------------

#define NO_FILTER           0
#define DIFFERENT_FILTER    1
#define MA_FILTER           2
//----------------------------------------------------------------------------

#define LINEAR_REGRESSION       0
#define LOGARITHM_REGRESSION    1
#define EXP_REGRESSION          2
#define POLYNOMIAL_REGRESSION   3
#define STEP_REGRESSION         4
#define POWER_REGRESSION        5
#define INVERSE_REGRESSION      6
//----------------------------------------------------------------------------

//*************************************************  ERRORS  ********************************************************************
#define     ERROR_MESSAGE       "B³¹d programu"
#define     INFO_MESSAGE        "Informacja dla u¿ytkownika"


#define     DLL_ERROR_1    -1 // NOL niezidentyfikowany obiekt logiczny.
#define     DLL_ERROR_2    -2 // Could not allocate. Bye ... it's somethig wrong with memory
#define     DLL_ERROR_3    -3 // Dzielnie przez zero.
#define     DLL_ERROR_4    -4 // B³¹d operacji mnozenia macierzy.
#define     DLL_ERROR_5    -5 // B³¹d operacji odwracania macierzy.
#define     DLL_ERROR_6    -6 // minimalny odcinek dla wyznaczenia rownania regresji to dwa punkty
#define     DLL_ERROR_7    -7 // no.. jak trend wielomianowy to po co identyfikowac rownanie stalej y=a
#define     DLL_ERROR_8    -8 // troche za daleko w przyszlosc .... horyzont prognozy 2x wiekszy od liczby danych
#define     DLL_ERROR_9    -9 // rz¹d wielomianu jest wy¿szy ni¿ liczba danych
#define     DLL_ERROR_10  -10 // rz¹d autoregresji jest wiekszy niz podwojna liczba danych, oznacza to zle uwarunkowanie macierzy glownej
#define     DLL_ERROR_11  -11 // brak dostepu do projektu sieci neuronowej
#define     DLL_ERROR_12  -12 // liczba danych mniejsza niz dwie liczby, nalezy zbadac czy ma to sens
#define     DLL_ERROR_13  -13 // w neuronowym modelu wielowymiarowym niezgodna liczba wejsc z liczba podana w projekcie !
#define     DLL_ERROR_14  -14 // w neuronowym modelu jednowymiarowym opoznienie jest wieksze od liczby danych, lub wyjscie jest rozne od jedynki
#define     DLL_ERROR_15  -15 // w liniowym modelu wielowymiarowym liczba danych jest mniejsza lub rowna liczbie kolumn macierzy danych ppIn
//#define     DLL_ERROR_16  -13 // w neuronowym modelu wielowymiarowym niezgodna liczba wejsc z liczba podana w projekcie !
//#define     DLL_ERROR_17  -13 // w neuronowym modelu wielowymiarowym niezgodna liczba wejsc z liczba podana w projekcie !




//=====================================================================================================================================================================================================

extern "C" __declspec(dllexport)    int  RegressionTimeSeries( double *x, double *y, long n, int iNext, int iMethod, int iDegree, float* pfParam);
extern "C" __declspec(dllexport)    int  RegressionOneVariable( double** x, double *y, long n, int iMethod);
extern "C" __declspec(dllexport)    int  ExpotentialSmoothing( double *x, double *y, int iLong, int iNext, int iLocalMethod, float alpha);
extern "C" __declspec(dllexport)    int  AR( double* x, double *y, long lInput, long iNext, int iDelay, float* pfParam);
extern "C" __declspec(dllexport)    int  NAR( double* pIn, double *pOut, long lInput, long iNext, int *iDelay, AnsiString pName);
extern "C" __declspec(dllexport)    int  NARX( double** pIn, double *pOut, long lInput, long iNext, int iDelay, AnsiString pName);
extern "C" __declspec(dllexport)    int  MultidimensionRegression(double** ppIn, double* pOut, int n, int iCol, int iNext, float* pfParams);
extern "C" __declspec(dllexport)    int  RegressionLU(double** ppIn, double* pOut, int n, int iCol, int iNext, float* pfParams);
extern "C" __declspec(dllexport)    int  Multiply(double** ppA, int m, int n, double** ppB, int k, double** ppC);
extern "C" __declspec(dllexport)    int  LMultiply(long double** ppA, int m, int n, long double** ppB, int k, long double** ppC);
extern "C" __declspec(dllexport)    int  InverseMatrix(double** ppA, int n);
extern "C" __declspec(dllexport)    int  LInverseMatrix(long double** ppA, int n);
extern "C" __declspec(dllexport)    int  Corr(double* pX, double* pY, int iStart, int iEnd, float *pfRateParam);
extern "C" __declspec(dllexport)    double** NewMatrix(int iRows, int iCols);
extern "C" __declspec(dllexport)    long double** LNewMatrix(int iRows, int iCols);
extern "C" __declspec(dllexport)    void RemoveMatrix(double **/*&*/ppMatrix,int iRows, int iCols);
extern "C" __declspec(dllexport)    void LRemoveMatrix(long double **/*&*/ppMatrix,int iRows, int iCols);
extern "C" __declspec(dllexport)    void ShowMatrix(double** ppP, int iRows, int iCols);
extern "C" __declspec(dllexport)    void ShowVector(double* ppP, int iRows);
extern "C" __declspec(dllexport)    int Aproksymator(double *x, double *y, int n, int method, double *xp, double *yp, int sizep);




#endif


