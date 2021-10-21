//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ImportPeriodSrc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TImportPeriod *ImportPeriod;
//---------------------------------------------------------------------------
__fastcall TImportPeriod::TImportPeriod(TComponent* Owner)
        : TForm(Owner)
{
  ini = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TImportPeriod::FormShow(TObject *Sender)
{
 //
 Month->ItemIndex = 0;
 if ( ini != NULL )
 {
   ini->WriteInteger("PERIODS", "AddPeriodsCount",0); // zeruj ...
 }
}
//---------------------------------------------------------------------------
void __fastcall TImportPeriod::Button1Click(TObject *Sender)
{
  //
  if ( ini != NULL && Month->ItemIndex != -1 )
  {
    int AddPeriodsCount = ini->ReadInteger("PERIODS", "AddPeriodsCount");
    String title = Month->Items->Strings[Month->ItemIndex] + (String)" " + (String)UpDown1->Position;
    String key = "Period" + (String)AddPeriodsCount;
    ini->WriteString("PERIODS", key,title);
    String valueKey = "Value" + (String)AddPeriodsCount;

    // a teraz wartoœæ która bêdzie wstawiana do nowego zestawu 
    ini->WriteInteger("DATA", valueKey,(Month->ItemIndex+UpDown1->Position*100+1) );

    AddPeriodsCount++;
    ini->WriteInteger("PERIODS", "AddPeriodsCount",AddPeriodsCount);
    if ( Month->ItemIndex == 11 )
    {
      UpDown1->Position++;
      Month->ItemIndex = 0;
    }
    else
      Month->ItemIndex++;
  }
}
//---------------------------------------------------------------------------
