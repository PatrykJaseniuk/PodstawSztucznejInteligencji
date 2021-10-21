//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ImportZE.h"
#include "AIIniFile.h"
#include "ImportPeriodSrc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
int _pascal neg( LPSTR path )
{
  // funkcja neguje wartoœæ paramteru VALUE
  TAIIniFile *ini = new TAIIniFile( path );
  ini->OpenIni();
  double D = ini->ReadDouble( "DATA", "Value" );
  D = D * -1;
  ini->WriteDouble( "DATA", "Result", D );
  ini->WriteIni();
  delete ini;
  return 1;
}
//---------------------------------------------------------------------------
int _pascal averange( LPSTR path )
{
  // œrednia ..
  TAIIniFile *ini = new TAIIniFile( path );
  ini->OpenIni();
  int I = ini->ReadInteger( "DATA", "VALUECount" );
  if ( I > 0 )
  {
    double *D1 = new double[I];
    for ( int i = 1; i <= I; i++ )
    {
      String S = "VALUE" + (String)i;
      D1[i-1] = ini->ReadDouble("DATA", S );
    }
    // obliczyæ œredni¹
    double D = 0;
    for ( int i = 0; i < I; i++ )
       D += D1[i];
    D = D / I;
    ini->WriteDouble( "DATA", "Result", D );
    ini->WriteIni();
    delete []D1;
  }
  delete ini;
  return 1;
}
//---------------------------------------------------------------------------
int _pascal import_example( LPSTR path )
{
  TAIIniFile *ini = new TAIIniFile( path );
  ini->OpenIni();

  TImportPeriod *dlg = new TImportPeriod(0);
  dlg->ini = ini;
  dlg->ShowModal();
  delete dlg;


  ini->WriteIni();
  delete ini;
  return 1;
}
//---------------------------------------------------------------------------


