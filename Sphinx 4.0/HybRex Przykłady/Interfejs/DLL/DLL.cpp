//---------------------------------------------------------------------------
#pragma hdrstop
#include "math.h"
#include "DLL.h"
#include "nn.h"

#include <exception>
#include <iostream.h>
#include <stdio.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//#include "TF.h"
#include "Polyreg.h"
//---------------------------------------------------------------------------
#define DEBUG 0
#define DEBUG_NEURAL 0
//if you wanna remove memory
//#define DEBUG 2
//#define DEBUG 3
//#define MATRIX
//#define TEST_DLL

#define AR_TYPE 1

//---------------------------------------------------------------------------

// pamiec na zewnatrz
int Aproksymator(double *x, double *y, int n, int method, double *xp, double *yp, int sizep)
{
        int iResult=1;

        double *alpha, *beta, *gamma, *delta, *poly;
        alpha = NewVector(0, method);
        beta = NewVector(0, method);
        gamma = NewVector(0, method);
        delta = NewVector(0, method);
        poly = NewVector(0, n-1);

        try
        {
                PolyRegress(x, y, alpha, gamma, delta, poly, method, n-1, 0);
                OrthoToPower(alpha, gamma, delta, beta, method);
                for(int i=0; i<sizep; i++)
                   yp[i] = PolyOrdinate(xp[i], alpha, gamma, delta, method);
        }
        catch(...)
        {
                return -1;
        }

        FreeVector(alpha,0, method);
        FreeVector(beta,0, method);
        FreeVector(gamma,0, method);
        FreeVector(delta, 0, method);
        FreeVector(poly,0, n-1);

        return iResult;
}

int  RegressionTimeSeries( double *x, double *y, long n, int iNext, int iMethod, int iDegree, float* pfParam)
{
    int iResult=1;


    //Main selection of errors
    if(n<3)//case of const value, somethig like that y=a
    {
        return DLL_ERROR_6;
    }

    if(iNext>2*n)//predictor could be unstable
    {
        return DLL_ERROR_8;
    }



    double  p0=0,p1=0,s1=0,s2=0,s3=0,s4=0,w1=0,w2=0,w=0,iCount=0;
    double  *t = new double[n+iNext];

    //pomocnicza zmienna indeksujaca
    for( int i=0; i<(n+iNext); i++)
        t[i]=++iCount;

    switch(iMethod)
    {
        case LINEAR_REGRESSION :
                {
                    for(long i=0 ; i<n ; i++)
                    {
                        s1 += t[i];
                        s2 += x[i];
                        s3 += t[i]*t[i];
                        s4 += t[i]*x[i];
                    }
                        w =  n*s3 - s1*s1;
                        w1 = s2*s3 - s4*s1;
                        w2 =  n*s4 - s1*s2;

                    {
                       if(w==0)
                       {
                          delete[] t;
                          return DLL_ERROR_3;
                       }
                       else
                       {
                          p0 = w1/(w+1.e-40);
                          p1 = w2/(w+1.e-40);
                       }
                    }
                    for( long i=0; i<(n+iNext); i++)
                        y[i]=p0+p1*t[i];

                    pfParam[0]=p0;
                    pfParam[1]=p1;

                    break;
                }
        case LOGARITHM_REGRESSION :
                {
                    for(long i=0 ; i<n ; i++)
                    {
                        s1 += log(t[i])*log(t[i]);
                        s2 += log(t[i]);
                        s3 += log(t[i])*x[i];
                        s4 += x[i];
                    }

                    w =  n*s1 - s2*s2;
                    w1 = n*s3 - s2*s4;
                    w2 = s1*s4 - s2*s3;

                    if(w==0)
                    {
                       delete[] t;
                       return DLL_ERROR_3;
                    }
                    else
                    {
                       p0 = w1/(w+1.e-40);
                       p1 = w2/(w+1.e-40);
                    }

                    for(long i=0; i<(n+iNext); i++)
                        y[i]=p0*log(t[i])+p1;

                    pfParam[0]=p0;
                    pfParam[1]=p1;

                    break;
                }



        case  EXP_REGRESSION:
                {

                    for(long i=0 ; i<n ; i++)
                    {
                        s1 += t[i];
                        s2 += log(x[i]);
                        s3 += t[i]*t[i];
                        s4 += t[i]*log(x[i]);
                    }
                        w =  n*s3 - s1*s1;
                        w1 = s2*s3 - s4*s1;
                        w2 =  n*s4 - s1*s2;


                    if(w==0)
                    {
                        delete[] t;
                        return DLL_ERROR_3;
                    }
                    else
                    {
                        p0 = exp(w1/(w+1.e-40));
                        p1 = w2/(w+1.e-40);
                    }

                    for( long i=0; i<(n+iNext); i++)
                        y[i]=p0*exp(p1*t[i]);

                    pfParam[0]=p0;
                    pfParam[1]=p1;

                    break;
                }

        case  POWER_REGRESSION:
                {

                    for(long i=0 ; i<n ; i++)
                    {
                        s1 += log(t[i]);
                        s2 += log(x[i]);
                        s3 += log(t[i])*log(t[i]);
                        s4 += log(t[i])*log(x[i]);
                    }
                        w =  n*s3 - s1*s1;
                        w1 = s2*s3 - s4*s1;
                        w2 =  n*s4 - s1*s2;


                    if(w==0)
                    {
                        delete[] t;
                        return DLL_ERROR_3;
                    }
                    else
                    {
                        p0 = exp(w1/(w+1.e-40));
                        p1 = w2/(w+1.e-40);
                    }

                    for( long i=0; i<(n+iNext); i++)
                        y[i]=p0*pow(t[i],p1);

                    pfParam[0]=p0;
                    pfParam[1]=p1;

                    break;
                }

        case  INVERSE_REGRESSION:
                {

                    for(long i=0 ; i<n ; i++)
                    {
                        // t[i] zawsze>0
                        s1 += 1/t[i];
                        s2 += x[i];
                        s3 += (1/t[i])*(1/t[i]);
                        s4 += (1/t[i])*x[i];
                    }
                        w =  n*s3 - s1*s1;
                        w1 = s2*s3 - s4*s1;
                        w2 =  n*s4 - s1*s2;


                    if(w==0)
                    {
                        delete[] t;
                        return DLL_ERROR_3;
                    }
                    else
                    {
                        p0 = w1/(w+1.e-40);
                        p1 = w2/(w+1.e-40);
                    }

                    for( long i=0; i<(n+iNext); i++)
                        y[i]=p0+(p1/t[i]);

                    pfParam[0]=p0;
                    pfParam[1]=p1;

                    break;
                }



        case  STEP_REGRESSION:
                {
                      // identfication number of points in the trend set of data
                      int s=iDegree;

                      // fill first s rows
                      for(int i=0; i<s; i++)
                          y[i]=x[i];

                      for(int i=0; i<n-s; i++)
                      {
                          p0=0;
                          p1=0;

                          for(int j=i ; j<i+s; j++)
                          {
                              s1 += t[j];
                              s2 += x[j];
                              s3 += t[j]*t[j];
                              s4 += t[j]*x[j];
                          }

                          w =  (s)*s3 - s1*s1;
                          w1 = s2*s3 - s4*s1;
                          w2 =  (s)*s4 - s1*s2;

                          if(w==0)
                          {
                             delete[] t;
                             return DLL_ERROR_3;
                          }
                          else
                          {
                             p0 = w1/(w+1.e-40);
                             p1 = w2/(w+1.e-40);
                          }
                          s1=0.0;
                          s2=0.0;
                          s3=0.0;
                          s4=0.0;
                          w=0.0;
                          w1=0.0;
                          w2=0.0;

                          y[s+i]=p0+p1*t[s+i];

                       }

                       pfParam[0]=0.0;
                       pfParam[1]=0.0;

                       // and now the rest of data
                       for(int k=n; k<(iNext+n); k++)
                               y[k]=p0+p1*t[k];



                    break;
                }
         case POLYNOMIAL_REGRESSION:
                {

                        if(iDegree<2)
                        {
                           delete[] t;
                           return DLL_ERROR_7;
                        }
                        double *alpha, *beta, *gamma, *delta, *poly;
                        //liczba parametrów jest o jeden wiêksza od rzedu regresji
                        // dlatego robimy:
                        int q=iDegree-1;

                        alpha = NewVector(0, q);
                        beta = NewVector(0, q);
                        gamma = NewVector(0, q);
                        delta = NewVector(0, q);
                        poly = NewVector(0, n+iNext-1);

                        PolyRegress(t, x, alpha, gamma, delta, poly, q, n-1, iNext);
                        OrthoToPower(alpha, gamma, delta, beta, q);

                        // and now generate output
                        memcpy(y, poly, (n+iNext)*sizeof(double));
                        // memcpy(pfParam, beta, (q+1)*sizeof(float));
                        for (int i=0; i<q+1; i++)   //zastapic memcpy
                             pfParam[i]=(float)beta[i];

                        FreeVector(alpha,0, q);
                        FreeVector(beta,0, q);
                        FreeVector(gamma,0, q);
                        FreeVector(delta, 0, q);
                        FreeVector(poly,0, n+iNext-1);

                 }

        default :;
    }

    delete[] t;
    return iResult;
}


int MultidimensionRegression(double** ppIn, double* pOut, int n, int iCol, int iNext, float* pfParams)
{

                    int iResult=1;
                    double **ppX, **ppXT, **ppY, **ppV1, **ppV2, **ppP, **ppYY;
#if DEBUG == 2
                    double a,b,c,d,e,f,g,h;
                    double check_ppY, check_pOut, check_ppX, check_ppP;
#endif

                    int iNumberOfCol=iCol;

                    //n=n-1;

                    //poniewaz dochodza jedynki w 1 kolumnie
                    //n=n+1;
                    //n=3;
                    //iDelay=2;
                    //double tab[4]={4,3,2,1};
                    // give memory for matrix
                    // n - number of col
                    // delay - numer of row
                    // ppT[delay][n]
                    // place matrix:
                    // i this is row
                    // j this is col
                    ppX=NewMatrix(n,iNumberOfCol);
                    ppXT=NewMatrix(iNumberOfCol,n);
                    ppY=NewMatrix(n,1);
                    ppV1=NewMatrix(iNumberOfCol,iNumberOfCol);
                    ppV2=NewMatrix(iNumberOfCol,iNumberOfCol);
                    ppYY=NewMatrix(n,1);
                    ppP=NewMatrix(iNumberOfCol,iNumberOfCol/*too many cols*/);

                    // fill first col matrix
                    for(int i=0; i<n; i++)
                    ppX[i][0]=1.0;

                    /*
                    // make non-linear matrix
                    // iDelay < n
                    double dPower;
                    for (int i=0; i<n+iNext; i++)
                        for(int j=1; j<iDegree+1; j++)
                        {
                            dPower=i+1;
                            for(int s=1 ; s<j; s++)
                                dPower=dPower*dPower;
                            ppX[i][j]=dPower;
                        }
                     */
                    for (int i=0; i<n ; i++)
                    {
                        for(int j=1 ; j<iCol; j++)
                        {
                             ppX[i][j]=ppIn[j-1][i];
#if DEBUG == 2
                            check_ppX=ppX[i][j];
#endif

                        }
                    }


                    // make output vector
                    for (int i=0; i<n; i++)
                    {
                        ppY[i][0]=ppIn[iCol-1][i];
#if DEBUG == 2
                        check_ppY=ppY[i][0];
#endif
                    }


                    //transpozycja
                    for (int i=0; i<n; i++)

                        for(int j=0; j<iNumberOfCol; j++)
                            ppXT[j][i]=ppX[i][j];

#if DEBUG == 2
   a=ppX[0][0];
   b=ppX[0][1];
   c=ppX[1][0];
   d=ppX[1][1];
   e=ppX[2][0];
   f=ppX[2][1];

   a=ppXT[0][0];
   b=ppXT[0][1];
   c=ppXT[0][2];
/*
   d=ppXT[1][0];
   e=ppXT[1][1];
   f=ppXT[1][2];
*/
#endif

                    try
                    {
                        Multiply(ppXT, iNumberOfCol, n-iNext, ppX, iNumberOfCol,  ppV1);
                        InverseMatrix(ppV1, iNumberOfCol);
                        Multiply(ppXT, iNumberOfCol, n-iNext, ppY, 1, ppV2);
                        Multiply(ppV1, iNumberOfCol, iNumberOfCol, ppV2, iNumberOfCol, ppP);
#if DEBUG == 2
                        for(int i=0; i<iCol; i++)
                        {
                           check_ppP=ppP[i][0];
                        }
#endif
                    }
                    catch(...)
                    {
                        MessageBox( 0, "B³¹d w regresji wielowymiarowej", "B³¹d", MB_OK | MB_ICONSTOP );
                        throw;
                    }

                    for (int i=0; i<n; i++)
                        pOut[i]=0;

                    double dSum;
                    /*
                    for (int i=0; i<n+iNext; i++)
                    {
                        dSum=0;
                        for(int g=0; g<iNumberOfCol; g++)
                        {
                            b=ppX[i][i+g];
                            dSum = dSum + ppX[i][g]*ppP[g][0];
                            //y[i] = y[i] + ppX[i][g]*ppB[g][0];
                            //;
                        }
                        pOut[i]=dSum;
                    }
                    */

                    double** ppOut=NewMatrix(n,iNumberOfCol);
                    Multiply(ppX, n, iNumberOfCol, ppP, 1, ppOut);

                    for (int i=0; i<n; i++)
                    {
                        pOut[i]=ppOut[i][0];
#if DEBUG == 2
                        check_pOut=pOut[i];
#endif
                    }

                    RemoveMatrix(ppOut,n,iNumberOfCol);

                    // iCol - 1 because there's one output
                    for (int i=0; i<iCol; i++)
                        pfParams[i]=ppP[i][0];


                    RemoveMatrix(ppX,n,iNumberOfCol);
                    RemoveMatrix(ppXT,iNumberOfCol,n);
                    RemoveMatrix(ppY,n,1);
                    RemoveMatrix(ppV1,iNumberOfCol,iNumberOfCol);
                    RemoveMatrix(ppV2,iNumberOfCol,iNumberOfCol);
                    RemoveMatrix(ppYY,n,1);
                    RemoveMatrix(ppP,iNumberOfCol,iNumberOfCol);


    return iResult;
//#endif
}

int  ExpotentialSmoothing( double *x, double *y, int iLong, int iNext, int iLocalMethod, float alpha)
{
    int iResult=-1;

#if DEBUG == 2
    double a,b,c,d,e,f,g,h;
#endif

    double dFirstValue=0;
    int    iLongOfFirst=(int)(iLong/2);

    //a=iLongOfFirst;
    y[0]=0;
    //e=x[0];

    // count first value
    for(int i=0; i<iLongOfFirst ; i++)
    {
        dFirstValue += x[i];
#if DEBUG == 2
        f=x[i];
        g=y[0];
#endif
    }

#if DEBUG == 2
    b=dFirstValue;
#endif

    y[0]=dFirstValue/iLongOfFirst;

#if DEBUG == 2
    b=y[0];
#endif

    // main loop
    double dy;
    for(int i=1; i<iLong+iNext; i++)
    {
        dy=y[i-1];
        y[i]=alpha*x[i-1]+(1-alpha)*dy;
#if DEBUG == 2
        c=y[i];
#endif
    }

    return iResult;
}



#if AR_TYPE == 1

int  AR( double* x, double *y, long lInput, long iNext, int iDelay, float* pfParam)
{
    int iResult=1;

    //This AR/ARX algorithm use only standard operation on matrix
    //for increes power you should use topelitz decomposition of delay matrix

    double **ppX, **ppXT, **ppY, **ppV1, **ppV2, **ppP, **ppYY;
    double* pY;

#if DEBUG == 2
    double a,b,c,d,e,f,g,h;
#endif

    long n=lInput-iDelay;

    if((2*iDelay)>lInput)
    {
        return DLL_ERROR_10;
    }

    //poniewaz dochodza jedynki w 1 kolumnie
    //because ones in first col
    //n=n+1;
    //n=3;
    //iDelay=2;
    //double tab[4]={4,3,2,1};
    // give memory for matrix
    // n - number of col
    // delay - numer of row
    // ppT[delay][n]
    // place matrix:
    // i this is row
    // j this is col
    ppX=NewMatrix(n,iDelay);
    ppXT=NewMatrix(iDelay,n);
    ppY=NewMatrix(n,1);
    ppV1=NewMatrix(iDelay,iDelay);
    ppV2=NewMatrix(iDelay, iDelay);
    ppYY=NewMatrix(lInput+iNext,1);
    ppP=NewMatrix(iDelay,1/*iDelay*//*!too many cols*/);


    //make delay matrix
    // iDelay < n
    for (int i=0; i<iDelay; i++)
        for(int j=0; j<n; j++)
            ppX[j][i]=x[i+j];

    // make output vector
    for (int i=iDelay; i<lInput; i++)
        ppY[i-iDelay][0]=x[i];

    //transpozycja
    for (int i=0; i<n; i++)
        for(int j=0; j<iDelay; j++)
            ppXT[j][i]=ppX[i][j];


#if DEBUG == 2
   a=ppX[0][0];
   b=ppX[0][1];
   c=ppX[1][0];
   d=ppX[1][1];
   e=ppX[2][0];
   f=ppX[2][1];

   a=ppXT[0][0];
   b=ppXT[0][1];
   c=ppXT[0][2];
   d=ppXT[1][0];
   e=ppXT[1][1];
   f=ppXT[1][2];
#endif

// **********  start of the main process  ********************
// ***********************************************************
        if(Multiply(ppXT, iDelay, n, ppX, iDelay,  ppV1) != 1)
        {
               RemoveMatrix(ppX,n,iDelay);
               RemoveMatrix(ppXT,iDelay,n);
               RemoveMatrix(ppY,n,1);
               RemoveMatrix(ppV1,iDelay,iDelay);
               RemoveMatrix(ppV2,iDelay, iDelay);
               RemoveMatrix(ppYY,lInput+iNext,1);
               RemoveMatrix(ppP,iDelay,1);
               return DLL_ERROR_4;
        }


#if DEBUG
   a=ppV1[0][0];
   b=ppV1[0][1];
   c=ppV1[1][0];
   d=ppV1[1][1];
   //ppV1[1][1]=-1;
   //ppV1[1][2]=1;
   //ppV1[2][0]=-1;
   //ppV1[2][1]=-1;
   //ppV1[2][2]=2;
#endif

        if(InverseMatrix(ppV1, iDelay)!=1)
        {
               RemoveMatrix(ppX,n,iDelay);
               RemoveMatrix(ppXT,iDelay,n);
               RemoveMatrix(ppY,n,1);
               RemoveMatrix(ppV1,iDelay,iDelay);
               RemoveMatrix(ppV2,iDelay, iDelay);
               RemoveMatrix(ppYY,lInput+iNext,1);
               RemoveMatrix(ppP,iDelay,1);
               return DLL_ERROR_5;
        }


#if DEBUG
   a=ppV1[0][0];
   b=ppV1[0][1];
   c=ppV1[1][0];
   d=ppV1[1][1];
#endif

        if(Multiply(ppXT, iDelay, n, ppY, 1, ppV2)!=1)
        {
               RemoveMatrix(ppX,n,iDelay);
               RemoveMatrix(ppXT,iDelay,n);
               RemoveMatrix(ppY,n,1);
               RemoveMatrix(ppV1,iDelay,iDelay);
               RemoveMatrix(ppV2,iDelay, iDelay);
               RemoveMatrix(ppYY,lInput+iNext,1);
               RemoveMatrix(ppP,iDelay,1);
               return DLL_ERROR_4;
        }

#if DEBUG
   a=ppV2[0][0];
   b=ppV2[0][1];
   c=ppV2[1][0];
   d=ppV2[1][1];
#endif


            if(Multiply(ppV1, iDelay, iDelay, ppV2, 1, ppP)!=1)
            {
               RemoveMatrix(ppX,n,iDelay);
               RemoveMatrix(ppXT,iDelay,n);
               RemoveMatrix(ppY,n,1);
               RemoveMatrix(ppV1,iDelay,iDelay);
               RemoveMatrix(ppV2,iDelay, iDelay);
               RemoveMatrix(ppYY,lInput+iNext,1);
               RemoveMatrix(ppP,iDelay,1);
               return DLL_ERROR_4;
            }

#if DEBUG
   a=ppP[0][0];
   b=ppP[1][0];
#endif

/*
#if DEBUG == 2
   ppV[0][0]=1;
   ppV[0][1]=1;
   ppV[0][2]=-1;
   ppV[1][0]=2;
   ppV[1][1]=-1;
   ppV[1][2]=1;
   ppV[2][0]=-1;
   ppV[2][1]=-1;
   ppV[2][2]=2;
#endif
*/

//************** end of the main process *******************************

    for (int i=0; i<iDelay; i++)
        pfParam[i]=ppP[i][0];

    for (int i=iDelay; i<lInput+iNext; i++)
        y[i]=0;

    for (int i=iDelay; i<lInput; i++)
        for(int g=0; g<iDelay; g++)
        {
            y[i] += x[i+g-iDelay] * ppP[g][0];
        }

    double dSum=0.0;
    for (int i=lInput; i<lInput+iNext ; i++)
    {
        dSum=0.0;
        for(int g=0; g<iDelay; g++)
        {
            dSum += y[i+g-iDelay] * ppP[g][0];
        }
        y[i]=dSum;
     }

#if DEBUG
   a=y[0];
   b=y[1];
   c=y[2];
   d=y[3];
#endif

    //ShowVector(y,lInput+iNext);

    RemoveMatrix(ppX,n,iDelay);
    RemoveMatrix(ppXT,iDelay,n);
    RemoveMatrix(ppY,n,1);
    RemoveMatrix(ppV1,iDelay,iDelay);
    RemoveMatrix(ppV2,iDelay, iDelay);
    RemoveMatrix(ppYY,lInput+iNext,1);
    RemoveMatrix(ppP,iDelay,1);

    return iResult;
}
#endif
//==============================================================================

#if AR_TYPE == 0
//==============================================================================
int  AR( double* x, double *y, long lInput, long iNext, int iDelay, float* pfParam)
{
    int iResult=1;

    //This AR/ARX algorithm use only standard operation on matrix
    //for increes power you should use topelitz decomposition of delay matrix

    long double **ppX, **ppXT, **ppY, **ppV1, **ppV2, **ppP, **ppYY;
    long double* pY;

#if DEBUG == 2
    double a,b,c,d,e,f,g,h;
#endif

    long n=lInput-iDelay;

    if((2*iDelay)>lInput)
    {
        return DLL_ERROR_10;
    }

    //poniewaz dochodza jedynki w 1 kolumnie
    //because ones in first col
    //n=n+1;
    //n=3;
    //iDelay=2;
    //double tab[4]={4,3,2,1};
    // give memory for matrix
    // n - number of col
    // delay - numer of row
    // ppT[delay][n]
    // place matrix:
    // i this is row
    // j this is col
    ppX=LNewMatrix(n,iDelay);
    ppXT=LNewMatrix(iDelay,n);
    ppY=LNewMatrix(n,1);
    ppV1=LNewMatrix(iDelay,iDelay);
    ppV2=LNewMatrix(iDelay, iDelay);
    ppYY=LNewMatrix(lInput+iNext,1);
    ppP=LNewMatrix(iDelay,1/*iDelay*//*!too many cols*/);


    //make delay matrix
    // iDelay < n
    for (int i=0; i<iDelay; i++)
        for(int j=0; j<n; j++)
            ppX[j][i]=x[i+j];

    // make output vector
    for (int i=iDelay; i<lInput; i++)
        ppY[i-iDelay][0]=x[i];

    //transpozycja
    for (int i=0; i<n; i++)
        for(int j=0; j<iDelay; j++)
            ppXT[j][i]=ppX[i][j];


#if DEBUG == 2
   a=ppX[0][0];
   b=ppX[0][1];
   c=ppX[1][0];
   d=ppX[1][1];
   e=ppX[2][0];
   f=ppX[2][1];

   a=ppXT[0][0];
   b=ppXT[0][1];
   c=ppXT[0][2];
   d=ppXT[1][0];
   e=ppXT[1][1];
   f=ppXT[1][2];
#endif

// **********  start of the main process  ********************
// ***********************************************************
        if(LMultiply(ppXT, iDelay, n, ppX, iDelay,  ppV1) != 1)
        {
               RemoveMatrix(ppX,n,iDelay);
               RemoveMatrix(ppXT,iDelay,n);
               RemoveMatrix(ppY,n,1);
               RemoveMatrix(ppV1,iDelay,iDelay);
               RemoveMatrix(ppV2,iDelay, iDelay);
               RemoveMatrix(ppYY,lInput+iNext,1);
               RemoveMatrix(ppP,iDelay,1);
               return DLL_ERROR_4;
        }


#if DEBUG
   a=ppV1[0][0];
   b=ppV1[0][1];
   c=ppV1[1][0];
   d=ppV1[1][1];
   //ppV1[1][1]=-1;
   //ppV1[1][2]=1;
   //ppV1[2][0]=-1;
   //ppV1[2][1]=-1;
   //ppV1[2][2]=2;
#endif

        if(LInverseMatrix(ppV1, iDelay)!=1)
        {
               RemoveMatrix(ppX,n,iDelay);
               RemoveMatrix(ppXT,iDelay,n);
               RemoveMatrix(ppY,n,1);
               RemoveMatrix(ppV1,iDelay,iDelay);
               RemoveMatrix(ppV2,iDelay, iDelay);
               RemoveMatrix(ppYY,lInput+iNext,1);
               RemoveMatrix(ppP,iDelay,1);
               return DLL_ERROR_5;
        }

#if DEBUG
   a=ppV1[0][0];
   b=ppV1[0][1];
   c=ppV1[1][0];
   d=ppV1[1][1];
#endif

        if(LMultiply(ppXT, iDelay, n, ppY, 1, ppV2)!=1)
        {
               RemoveMatrix(ppX,n,iDelay);
               RemoveMatrix(ppXT,iDelay,n);
               RemoveMatrix(ppY,n,1);
               RemoveMatrix(ppV1,iDelay,iDelay);
               RemoveMatrix(ppV2,iDelay, iDelay);
               RemoveMatrix(ppYY,lInput+iNext,1);
               RemoveMatrix(ppP,iDelay,1);
               return DLL_ERROR_4;
        }

#if DEBUG
   a=ppV2[0][0];
   b=ppV2[0][1];
   c=ppV2[1][0];
   d=ppV2[1][1];
#endif

           
            if(LMultiply(ppV1, iDelay, iDelay, ppV2, 1, ppP)!=1)
            {
               RemoveMatrix(ppX,n,iDelay);
               RemoveMatrix(ppXT,iDelay,n);
               RemoveMatrix(ppY,n,1);
               RemoveMatrix(ppV1,iDelay,iDelay);
               RemoveMatrix(ppV2,iDelay, iDelay);
               RemoveMatrix(ppYY,lInput+iNext,1);
               RemoveMatrix(ppP,iDelay,1);
               return DLL_ERROR_4;
            }

#if DEBUG
   a=ppP[0][0];
   b=ppP[1][0];
#endif

/*
#if DEBUG == 2
   ppV[0][0]=1;
   ppV[0][1]=1;
   ppV[0][2]=-1;
   ppV[1][0]=2;
   ppV[1][1]=-1;
   ppV[1][2]=1;
   ppV[2][0]=-1;
   ppV[2][1]=-1;
   ppV[2][2]=2;
#endif
*/

//************** end of the main process *******************************

    for (int i=0; i<iDelay; i++)
        pfParam[i]=ppP[i][0];

    for (int i=iDelay; i<lInput+iNext; i++)
        y[i]=0;

    for (int i=iDelay; i<lInput; i++)
        for(int g=0; g<iDelay; g++)
        {
            y[i]+=x[i+g-iDelay]*ppP[g][0];
        }

    double dSum=0.0;
    for (int i=lInput; i<lInput+iNext ; i++)
    {
        dSum=0.0;
        for(int g=0; g<iDelay; g++)
        {
            dSum+=y[i+g-iDelay]*ppP[g][0];
        }
        y[i]=dSum;
     }

#if DEBUG
   a=y[0];
   b=y[1];
   c=y[2];
   d=y[3];
#endif


    LRemoveMatrix(ppX,n,iDelay);
    LRemoveMatrix(ppXT,iDelay,n);
    LRemoveMatrix(ppY,n,1);
    LRemoveMatrix(ppV1,iDelay,iDelay);
    LRemoveMatrix(ppV2,iDelay, iDelay);
    LRemoveMatrix(ppYY,lInput+iNext,1);
    LRemoveMatrix(ppP,iDelay,1);

    return iResult;
}
#endif
//==============================================================================


#if AR_TYPE == 2
int  AR( double* x, double *y, long lInput, long iNext, int iDelay, float* pfParam)
{
    //void YuleWalker(int p, int q, double *gamma, double *alpha)
/*
    alpha = NewVector(0, lInput);
    pacv = NewVector(0, lInput);
    //acovar = NewVector(0, 20);
    xx=NewVector(0, lInput);
    memcpy(xx,x,lInput*sizeof(double));

    BurgEstimation(alpha, pacv, x, iDelay, lInput);

    for(int i=0; i<(lInput+iNext) ; i++)
      y[i]=0.0;

    RationalAcquisition(q, p, mu, alpha, stability);
    ARMAgenerator(lInput, 0, iDelay, 0, alpha, NULL, y);

    FreeVector(alpha, 0, lInput);
    FreeVector(pacv, 0, lInput);
*/


    double **a;
    double *b;
    int i, j, k; 
    
    b = NewVector(1, p);
    a = NewMatrix(1, p, 1, p);
    
    for (i = 1; i <= p; i++)
      { 
       b[i] = -gamma[q + i];
       for (j = 1; j <= p; j++)
         { 
          k = abs(q + i - j);
          a[i] [j] = gamma[k];
         }
      }
      
    LUsolve(1, p, a, alpha, b);
    
    alpha[0] = 1;
    
    FreeVector(b, 1, p);
    FreeMatrix(a, 1, p, 1, p);

    return 1;
}
#endif


int Corr(double* pX, double* pY, int iStart, int iEnd, float *pfRateParam)
{
    int iResult=1;

    long double dX=0, dY=0, dCov=0, dSx=0, dSy=0, dSxx=0, dSyy=0;

    pfRateParam[0]=0.0;//mean
    pfRateParam[1]=0.0;//Dev
    pfRateParam[2]=0.0;//Corr


    //MeanAndStdDev(const double * Data, const int Data_Size, Extended &Mean, Extended &StdDev);

    if((iEnd-iStart-1)>0)
    {
       if (pY)
       {
           //mean
           for (int i=iStart ; i<iEnd ;i++)
          {
        	dX+=pX[i];
                dY+=pY[i];
          }
          dX/=iEnd;
          dY/=iEnd;

          pfRateParam[0]=(float)dX;//mean

	  //deviation
	  for (int i=iStart ; i<iEnd ; i++)
          {
                dSx=pX[i] - dX;
                dSy=pY[i] - dY;
      	        dSxx+=dSx*dSx;
                dSyy+=dSy*dSy;
                dCov+=dSx*dSy;
          }


          pfRateParam[1]=(float)sqrt(dSxx/(iEnd-1));//dev

          if((dSx>0) && (dSy>0))
                  pfRateParam[2]=(float)(dCov/sqrt(dSxx*dSyy) );//cor
          else
                  pfRateParam[2]=0.0;//cov
       }
       else
       {
          //mean
          for (int i=iStart ; i<iEnd ;i++)
          {
               dX+=pX[i];
          }
          dX/=iEnd;

          pfRateParam[0]=(float)dX;

          //deviation
          for (int i=iStart ; i<iEnd ; i++)
          {
                dSx=pX[i] - dX;
      	        dSxx+=dSx*dSx;
          }

          pfRateParam[1]=(float)sqrt(dSxx/(iEnd-1));//dev
          pfRateParam[2]=0.0;//?

       }
     }//end if


      return iResult;
}

int NAR( double* pIn, double *pOut, long lInput, long iNext, int *iDelay, AnsiString pName)
{
    int iResult=1;
    //iDelay jest prekazywane na zewn¹trz
    double **ppX, **ppXT, **ppY, **ppV1, **ppV2, **ppP, **ppYY;
    double* pY;
    int iNet=0;
    int iIn=0;
    int iOut=0;
    TNNFunctions* pTNNFunctions;

#if DEBUG_NEURAL
    double a,b,c,d,e,f,g,h;
#endif

    try
    {
            pTNNFunctions = new TNNFunctions();
            iNet = pTNNFunctions->InitNetwork(pName.c_str());

            if(iNet>0)
            {
                pTNNFunctions->GetModelOrder ( iNet, iIn, iOut );

                //zabezpieczenie wejscia i wyjscia
                if((iIn > lInput) || (iOut != 1))
                {
                   for(int i=0; i<(lInput+iNext); i++)
                   {
                       pOut[i]=0.0;
                   }
                   *iDelay=0;
                   return DLL_ERROR_14;
                }
            }
            else
            {
              for(int i=0; i<(lInput+iNext); i++)
              {
                 pOut[i]=0.0;
              }
              *iDelay=0;
              return DLL_ERROR_11;
            }
    }
    catch(...)
    {
        //MessageBox( 0, "Neural network error !!", "B³¹d", MB_OK | MB_ICONSTOP );
        return DLL_ERROR_1;
        //throw;
    }

    *iDelay=iIn;
    long n=lInput-iIn;
    //poniewaz dochodza jedynki w 1 kolumnie
    //n=n+1;
    //n=3;
    //iDelay=2;
    //double tab[4]={4,3,2,1};
    // give memory for matrix
    // n - number of col
    // delay - numer of row
    // ppT[delay][n]
    // place matrix:
    // i this is row
    // j this is col
    ppX=NewMatrix(n+iNext,iIn);


    //make delay matrix
    // iDelay < n
    for (int i=0; i<iIn; i++)
        for(int j=0; j<n; j++)
        {
            ppX[j][i]=pIn[i+j];
#if DEBUG_NEURAL
            d=ppX[j][i];
#endif
        }


    try
    {

#if DEBUG_NEURAL
    a=ppX[0][0];
    b=ppX[1][0];
    c=ppX[2][0];
    //.....

    d=ppX[0][1];
    e=ppX[1][1];
    f=ppX[2][1];
#endif

    double* pDelayInput = new double[iIn];
    double* pDelayOutput = new double[1];
    double* pY = new double[lInput+iNext];

    for(int i=0; i<n+iNext ; i++)
    {
        for(int j=0 ; j<iIn ; j++)
        {
                pDelayInput[iIn-j-1]=ppX[i][j];
#if DEBUG_NEURAL
                g=ppX[i][j];
                a=pDelayInput[j];
#endif
        }

        pTNNFunctions->RunNetwork2( iNet, pDelayInput, iIn, pDelayOutput, 1 );
        pY[i]=pDelayOutput[0];

#if DEBUG_NEURAL
        b=pDelayOutput[0];
        c=pY[i];
#endif

        if((i>=(n-1))&&(i<(n+iNext-1)))
        {
            for(int k=0; k<iIn-1; k++)
            {
                ppX[i+1][k]=ppX[i][k+1];
#if DEBUG_NEURAL
                g=ppX[i+1][k];
                //ppX[i][k]=ppX[i-1][k+1];
                //g=ppX[i][k];
#endif
            }
            ppX[i+1][iIn-1]=pY[i];
#if DEBUG_NEURAL
            g=ppX[i+1][iIn-1];
            //ppX[i][iDelay-1]=pOut[i-1];
            //g=ppX[i][iDelay-1];
#endif
        }

    }

    for(int i=iIn; i<lInput+iNext; i++)
        pOut[i]=pY[i-iIn];


    pTNNFunctions->DeleteNetwork(iNet);
    delete[] pDelayInput;
    delete[] pDelayOutput;
    delete pTNNFunctions;
    }
    catch(...)
    {
        //MessageBox( 0, "Neural network error !!", "B³¹d", MB_OK | MB_ICONSTOP );
        //throw;
        return DLL_ERROR_1;
    }

    return iResult;
}

int NARX( double** pIn, double *pOut, long lInput, long iNext, int iDelay, AnsiString pName)
{
    int iResult=1;

    double **ppX, **ppXT, **ppY, **ppV1, **ppV2, **ppP, **ppYY;
    double* pY;
    int iNet, iIn, iOut;
    TNNFunctions* pTNNFunctions;

#if DEBUG_NEURAL
    double a,b,c,d,e,f,g,h;
#endif


    try
    {
            pTNNFunctions = new TNNFunctions();
            iNet = pTNNFunctions->InitNetwork(pName.c_str());
            if(iNet>0)
            {
                pTNNFunctions->GetModelOrder ( iNet, iIn, iOut );
                if((iDelay-1) != iIn)
                {
                    for(int i=0; i<(lInput+iNext); i++)
                    {
                        pOut[i]=0.0;
                    }
                    return DLL_ERROR_13;
                }
                else
                {
                   iDelay=iIn;
                }

            }
            else
            {
              for(int i=0; i<(lInput+iNext); i++)
              {
                 pOut[i]=0.0;
              }
              return DLL_ERROR_11;
            }
    }
    catch(...)
    {
        //MessageBox( 0, "Neural network error !!", "B³¹d", MB_OK | MB_ICONSTOP );
        DLL_ERROR_1;
        throw;
    }



    //long n=lInput-iDelay;
    long n=lInput;
    long nn=n;

    //poniewaz dochodza jedynki w 1 kolumnie
    //n=n+1;
    //n=3;
    //iDelay=2;
    //double tab[4]={4,3,2,1};
    // give memory for matrix
    // n - number of col
    // delay - numer of row
    // ppT[delay][n]
    // place matrix:
    // i this is row
    // j this is col
    ppX=NewMatrix(nn,iDelay);
    ppY=NewMatrix(nn,1);

    //make delay matrix
    // iDelay < n
    for (int i=0; i<iDelay; i++)
        for(int j=0; j<nn; j++)
        {
            ppX[j][i]=pIn[i][j];
#if DEBUG_NEURAL
            d=ppX[j][i];
#endif
        }

    try
    {


#if DEBUG_NEURAL
    a=ppX[0][0];
    b=ppX[1][0];
    c=ppX[2][0];
    //.....

    d=ppX[0][1];
    e=ppX[1][1];
    f=ppX[2][1];
#endif


    double* pDelayInput = new double[iDelay];
    double* pDelayOutput = new double[1];
    double* pY = new double[nn];

    for(int i=0; i<nn ; i++)
    {
        pTNNFunctions->RunNetwork2( iNet, ppX[i], iDelay, ppY[i], 1 );
        pY[i]=pDelayOutput[0];

#if DEBUG_NEURAL
        b=pDelayOutput[0];
        c=pY[i];
#endif        
    }

    for(int i=0; i<nn; i++)
        pOut[i]=ppY[i][0];


    pTNNFunctions->DeleteNetwork(iNet);
    delete[] pDelayInput;
    delete[] pDelayOutput;
    delete pTNNFunctions;
    }
    catch(...)
    {
        //MessageBox( 0, "Neural network error !!", "B³¹d", MB_OK | MB_ICONSTOP );
        DLL_ERROR_1;
        throw;
    }

    return iResult;
}



double** NewMatrix(int iRows, int iCols)
{
    double **ppMatrix;

    // exceptions
    try {
            // give memory for matrix
            ppMatrix=new double*[iRows];
            for (int i=0; i<iRows; i++)
                    ppMatrix[i]=new double[iCols];
        }
    catch (std::bad_alloc)
        {
            MessageBox( 0, "Could not allocate. Bye ...", "B³¹d", MB_OK | MB_ICONSTOP );
            exit(-1);
            //return DLL_ERROR_2;
        }

    // memset(ppMatrix, 0, iRows*iCols*sizeof(double));
    // reset on zero
    for(int i=0; i<iRows; i++)
       for(int j=0; j<iCols; j++)
          ppMatrix[i][j]=0.0;

    return ppMatrix;
}

/*-----------------------------------------------------------------------------
/                Werjsa tworzenia macierzy dla typu rozszerzonego
/-----------------------------------------------------------------------------*/

long double** LNewMatrix(int iRows, int iCols)
{
    long double **ppMatrix;

    // exceptions
    try {
            // give memory for matrix
            ppMatrix=new long double*[iRows];
            for (int i=0; i<iRows; i++)
                    ppMatrix[i]=new long double[iCols];
        }
    catch (std::bad_alloc)
        {
            MessageBox( 0, "Could not allocate. Bye ...", "B³¹d", MB_OK | MB_ICONSTOP );
            exit(-1);
            //return DLL_ERROR_2;
        }

    // memset(ppMatrix, 0, iRows*iCols*sizeof(double));
    // reset on zero
    for(int i=0; i<iRows; i++)
       for(int j=0; j<iCols; j++)
          ppMatrix[i][j]=0.0;

    return ppMatrix;
}


void RemoveMatrix(double **/*&*/ppMatrix,int iRows, int iCols)
{
    // remove memory for matrix

    for (int i=0; i<iRows; i++)
        delete[] ppMatrix[i];

    delete[] ppMatrix;
}


/*-----------------------------------------------------------------------------
/                Werjsa usuwania macierzy dla typu rozszerzonego
/-----------------------------------------------------------------------------*/
void LRemoveMatrix(long double **/*&*/ppMatrix,int iRows, int iCols)
{
    // remove memory for matrix

    for (int i=0; i<iRows; i++)
        delete[] ppMatrix[i];

    delete[] ppMatrix;
}



int Multiply(double** ppA, int m, int n, double** ppB, int k, double** ppC)
{
    int iResult=1;
    double dSum;
    // m -liczba wierszy w ppA
    // n - liczba kolumn w ppA
    // n - liczba wierszy w ppB
    // k - liczba kolumn w ppB
    try
    {
        for(int i=0; i<m;  i++)
            {
                for(int j=0; j<k ; j++)
                {
                    dSum=0.0;
                    for(int s=0; s<n ; s++)
                       dSum += ppA[i][s]*ppB[s][j];

                    ppC[i][j]=dSum;
                 }
            }
    }
    catch(...)
    {
        //MessageBox( 0, "Multiple error", "B³¹d", MB_OK | MB_ICONSTOP );
        return DLL_ERROR_4;
        //throw;
    }

    return iResult;
}

/*-----------------------------------------------------------------------------
/                Werjsa mnozenia macierzy dla typu rozszerzonego
/-----------------------------------------------------------------------------*/
int LMultiply(long double** ppA, int m, int n, long double** ppB, int k, long double** ppC)
{
    int iResult=1;
    long double dSum;
    // m -liczba wierszy w ppA
    // n - liczba kolumn w ppA
    // n - liczba wierszy w ppB
    // k - liczba kolumn w ppB
    try
    {
            for(int i=0; i<m;  i++)
    {
        for(int j=0; j<k ; j++)
        {
            dSum=0.0;
            for(int s=0; s<n ; s++)
                dSum += ppA[i][s]*ppB[s][j];

            ppC[i][j]=dSum;
         }
    }
    }
    catch(...)
    {
        //MessageBox( 0, "Multiple error", "B³¹d", MB_OK | MB_ICONSTOP );
        return DLL_ERROR_4;
        //throw;
    }

    return iResult;
}



int InverseMatrix(double** ppA,  int n)
{
    int iResult=1;
    double lambda, p;
    int i, j, k;
    try
    {
       for (i = 0; i < n; i++)
       {
         p = ppA[i][i];
         for (k = 0; k < n; k++)
         {
          if (k != i)
            {
             lambda = ppA[k][i]/p;
             for (j = 0; j < n; j++)
                ppA[k][j] -=  lambda * ppA[i] [j];
             ppA[k][i] = -lambda;
            }
         }
         for (j = 0; j < n; j++)
           ppA[i][j] /=  p;
       ppA[i][i] = 1/p;
     }
     }
     catch(...)
     {
            return DLL_ERROR_5;
     }

    return iResult;
}

/*-----------------------------------------------------------------------------
/                Werjsa odwracania macierzy dla typu rozszerzonego
/-----------------------------------------------------------------------------*/

int LInverseMatrix(long double** ppA,  int n)
{
    int iResult=1;
    long double lambda, p;
    int i, j, k;
    try
    {
       for (i = 0; i < n; i++)
       {
         p = ppA[i][i];
         for (k = 0; k < n; k++)
         {
          if (k != i)
            {
             lambda = ppA[k][i]/p;
             for (j = 0; j < n; j++)
                ppA[k][j] -=  lambda * ppA[i] [j];
             ppA[k][i] = -lambda;
            }
         }
         for (j = 0; j < n; j++)
           ppA[i][j] /=  p;
       ppA[i][i] = 1/p;
     }
     }
     catch(...)
     {
            return DLL_ERROR_5;
     }

    return iResult;
}


void ShowMatrix(double** ppP, int iRows, int iCols)
{
       int dec, sign, ndig = 5;
       char* pStr = new char[40];
       char* pRows = new char[10];
       char* pCols = new char[10];



        for (long i=0; i<iRows; i++)
           for(long j=0; j<iCols; j++)
           {
                strcpy(pStr,"M(");
                itoa(i, pRows, 10);
                itoa(j, pCols, 10);
                strcat(pStr, pRows);
                strcat(pStr, ",");
                strcat(pStr, pCols);
                strcat(pStr, ") = ");
                strcat(pStr, fcvt(ppP[i][j],ndig, &dec, &sign));
                MessageBox( 0, pStr, "Show matrix", MB_OK | MB_ICONSTOP );

           }

        delete[] pStr;
        delete[] pRows;
        delete[] pCols;
}

void ShowVector(double* ppP, int iRows)
{
       int dec, sign, ndig = 5;
       char* pStr = new char[40];
       char* pRows = new char[10];
       char* pCols = new char[10];



        for (long i=0; i<iRows; i++)
        {
                strcpy(pStr,"M(");
                itoa(i, pRows, 10);

                strcat(pStr, pRows);
                strcat(pStr, ",");

                strcat(pStr, ") = ");
                strcat(pStr, fcvt(ppP[i],ndig, &dec, &sign));
                MessageBox( 0, pStr, "Show Vector", MB_OK | MB_ICONSTOP );

        }

        delete[] pStr;
        delete[] pRows;
        delete[] pCols;
}

double *NewVector(int nl, int nh)
{
    double *v = (double *)malloc((unsigned) (nh - nl + 1) * sizeof(double));
    if (!v)
       fprintf(stderr, "could not allocate memory in NewVector().");
    return (v - nl);
}

void FreeVector(double *v, int nl, int nh)
{
    nh = nh; /* This is only to show good grace */
    free((char*) (v + nl));
}


