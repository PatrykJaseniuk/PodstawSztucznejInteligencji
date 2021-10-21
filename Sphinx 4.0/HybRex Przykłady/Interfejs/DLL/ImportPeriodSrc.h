//---------------------------------------------------------------------------

#ifndef ImportPeriodSrcH
#define ImportPeriodSrcH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
#include "AIIniFile.h"
//---------------------------------------------------------------------------
class TImportPeriod : public TForm
{
__published:	// IDE-managed Components
        TComboBox *Month;
        TEdit *Edit1;
        TUpDown *UpDown1;
        TButton *Button1;
        TButton *Button2;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TImportPeriod(TComponent* Owner);

  TAIIniFile *ini;      
};
//---------------------------------------------------------------------------
extern PACKAGE TImportPeriod *ImportPeriod;
//---------------------------------------------------------------------------
#endif
