//---------------------------------------------------------------------------

#ifndef AIIniFileH
#define AIIniFileH

//---------------------------------------------------------------------------
class PACKAGE TAIIniFile  // TIniFile
{
public:
	virtual AnsiString __fastcall ReadString(const AnsiString Section, const AnsiString Ident,
                  const AnsiString Default = "" );
	virtual void __fastcall WriteString(const AnsiString Section, const AnsiString Ident, const AnsiString
		Value);
        virtual int __fastcall ReadInteger(const AnsiString Section, const AnsiString Ident, int Default = 0 );
        virtual void __fastcall WriteInteger(const AnsiString Section, const AnsiString Ident, int Value);

        virtual double __fastcall ReadDouble(const AnsiString Section, const AnsiString Ident, double Default = 0 );
        virtual void __fastcall WriteDouble(const AnsiString Section, const AnsiString Ident, double Value);

  // Funkcje dostêpu do kolejnych elementów
        int __fastcall GetSectionCount( const AnsiString Section );
        AnsiString __fastcall GetKeyName( const AnsiString Section, int index );
        int __fastcall GetSectionsCount();
        AnsiString __fastcall GetSectionName( int index );
        void __fastcall ClearSection( const AnsiString SecName );
        bool __fastcall ExistsKey( const AnsiString Section, const AnsiString Ident );
public:

	__fastcall TAIIniFile(const AnsiString fileName);

        int __fastcall OpenIni();
        int __fastcall WriteIni();

        int __fastcall findSectionIndex( const AnsiString SecName );
        int __fastcall findKeyIndex( int iSectionPos, const AnsiString Ident );
public:

	__fastcall virtual ~TAIIniFile(void) { deleteList(); delete Sections; }
protected:
       void deleteList();
       int __fastcall addKey( AnsiString SecName, AnsiString Key, AnsiString Value );


  AnsiString FileName;
  TList *Sections;
};

//---------------------------------------------------------------------------
void CutEndSpaces( AnsiString &key );
//---------------------------------------------------------------------------
#endif
