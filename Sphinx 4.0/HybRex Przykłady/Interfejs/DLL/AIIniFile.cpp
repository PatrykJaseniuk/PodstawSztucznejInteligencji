//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AIIniFile.h"
#include <io.h>
#include <stdio.h>
#include <dir.h>
#include "dirclass.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
typedef struct iniElement
{
   AnsiString SecName;
   TStringList*   Keys;
   TStringList*   Values;
} aiIniElement;
//---------------------------------------------------------------------------
__fastcall TAIIniFile::TAIIniFile(const AnsiString fileName)
{
  FileName = fileName;
  Sections = new TList();
}
//---------------------------------------------------------------------------
AnsiString __fastcall TAIIniFile::ReadString(const AnsiString SecName,
   const AnsiString Key, const AnsiString Default)
{
  AnsiString Str = Default;

  int pos = findSectionIndex( SecName );
  if ( pos != -1 )
  {
    aiIniElement* el = (aiIniElement*)Sections->Items[pos];
    if ( el != NULL )
    {
      int Index = findKeyIndex( pos, Key );
      if ( Index != -1 )
        Str = el->Values->Strings[Index];
    }
  }
  return Str;
}
//---------------------------------------------------------------------------
int __fastcall TAIIniFile::ReadInteger(const AnsiString SecName, const AnsiString Key, int Default)
{
  int Ret = Default;
  int pos = findSectionIndex( SecName );
  if ( pos != -1 )
  {
    aiIniElement* el = (aiIniElement*)Sections->Items[pos];
    if ( el != NULL )
    {
      int Index = findKeyIndex( pos, Key );
      if ( Index != -1 )
        Ret = el->Values->Strings[Index].ToInt();
    }
  }
  return Ret;
}
//---------------------------------------------------------------------------
void __fastcall TAIIniFile::WriteString(const AnsiString SecName,
    const AnsiString Key, const AnsiString Value)
{
  addKey( SecName, Key, Value );
}
//---------------------------------------------------------------------------
void __fastcall TAIIniFile::WriteInteger(const AnsiString SecName, const AnsiString Key, int Value)
{
  String sValue = (String)Value;
  addKey( SecName, Key, sValue );
}
//---------------------------------------------------------------------------
int __fastcall TAIIniFile::findSectionIndex( const AnsiString SecName )
{
  String sSecName = SecName;
  CutEndSpaces( sSecName );
  for ( int i = 0; i < Sections->Count; i++ )
  {
    aiIniElement* el = (aiIniElement*)Sections->Items[i];
    if ( strcmp(el->SecName.c_str(), sSecName.c_str()) == 0 )
      return i;
  }
  return -1;
}
//---------------------------------------------------------------------------
int __fastcall TAIIniFile::findKeyIndex( int iSectionPos, const AnsiString Key )
{
  String sKey = Key;
  CutEndSpaces( sKey );

  if ( iSectionPos != -1 )
  {
    aiIniElement* el = (aiIniElement*)Sections->Items[iSectionPos];
    if ( el != NULL )
    {
      for ( int i = 0; i < el->Keys->Count; i++ )
        if ( strcmp( el->Keys->Strings[i].c_str(), sKey.c_str()) == 0 )
          return i;
     }
  }
  return -1;
}
//---------------------------------------------------------------------------
double __fastcall TAIIniFile::ReadDouble(const AnsiString Section, const AnsiString Ident, double Default )
{
  double dRet = Default;

  int pos = findSectionIndex( Section );
  if ( pos != -1 )
  {
    aiIniElement* el = (aiIniElement*)Sections->Items[pos];
    if ( el != NULL )
    {
      int Index = findKeyIndex( pos, Ident );
      if ( Index != -1 )
      {
        String s = el->Values->Strings[Index];
        char temp[512];
        strcpy( temp, s.c_str() );
        char *comma = strchr( temp, '.');
        if ( comma != NULL )
          *comma = ',';
        s = temp;
        dRet = s.ToDouble();
      }
    }
  }
  return dRet;
}
//---------------------------------------------------------------------------
void __fastcall TAIIniFile::WriteDouble(const AnsiString SecName, const AnsiString Key, double Value )
{
  String sValue = (String)Value;
  addKey( SecName, Key, sValue );
}
//---------------------------------------------------------------------------
int __fastcall TAIIniFile::GetSectionCount( const AnsiString Section )
{
  int pos = findSectionIndex( Section );
  if ( pos != -1 )
  {
    aiIniElement* el = (aiIniElement*)Sections->Items[pos];
    if ( el != NULL )
      return el->Keys->Count;
  }
  return -1;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TAIIniFile::GetKeyName( const AnsiString Section, int Index )
{
  int pos = findSectionIndex( Section );
  String Str = "";
  if ( pos != -1 )
  {
    aiIniElement* el = (aiIniElement*)Sections->Items[pos];
    if ( el != NULL )
    {
      if ( Index >= 0 && Index < el->Keys->Count )
        Str = el->Keys->Strings[Index];
    }
  }
  return Str;
}
//---------------------------------------------------------------------------
int __fastcall TAIIniFile::GetSectionsCount()
{
  return Sections->Count;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TAIIniFile::GetSectionName( int index )
{
  String Str = "";
  if ( index >= 0 && index < Sections->Count )
  {
    aiIniElement* el = (aiIniElement*)Sections->Items[index];
    Str = el->SecName;
  }
  return Str;
}
//---------------------------------------------------------------------------
void __fastcall TAIIniFile::ClearSection( const AnsiString Section )
{
  int pos = findSectionIndex( Section );
  if ( pos != -1 )
  {
    aiIniElement* el = (aiIniElement*)Sections->Items[pos];
    Sections->Items[pos] = NULL;
    el->Keys->Clear();
    delete el->Keys;
    el->Values->Clear();
    delete el->Values;
    delete el;
    Sections->Delete( pos );
  }
}
//---------------------------------------------------------------------------
void TAIIniFile::deleteList()
{
  while ( Sections->Count > 0 )
  {
    aiIniElement* el = (aiIniElement*)Sections->Items[Sections->Count-1];
    Sections->Items[Sections->Count-1] = NULL;
    el->Keys->Clear();
    delete el->Keys;
    el->Values->Clear();
    delete el->Values;
    delete el;
    Sections->Delete( Sections->Count-1 );
  }
}
//---------------------------------------------------------------------------
int __fastcall TAIIniFile::OpenIni()
{
  deleteList();

  // zmieñ katalog na windows
  char dir[1024];

  if ( FileName.LastDelimiter("\\") == 0 )
  {
    GetWindowsDirectory(dir, sizeof(dir));
  }
  else
  {
    // bie¿ z nazwy pliku ?
    dir[0] = '\0';
  }
  TDirectoryStore store(dir);


  if ( access( FileName.c_str(), 04 ) != 0 )
    return -1;
  FILE *f;
  f = fopen( FileName.c_str(), "r" );
  if(!f)
    return -1;
  char line[1024];
  String SecName;
  while ( !feof(f) )
  {
    fgets( line, sizeof(line), f );
    if ( strlen(line) <= 0 )
      continue;
    if ( line[0]=='[' )
    {
      // sekcja ?
      char *begin = &line[1];
      char *end = strrchr( line, ']' );
      if ( end != NULL )
      {
        *end = '\0';
        SecName = begin;
        continue;
      }
    }
    // no to rozbijamy liniê i próbujemy wstawiæ na listê ...
    char *equals = strchr(line,'=');
    if ( equals == NULL )
      continue; // pomijamy liniê
    *equals = '\0';
/*
    char *value = equals;
    value++;
    equals--;

/*
    while ( *equals == ' ' || *equals == '\n ')
    {
      *equals = '\0';
      equals--;
    }
*/
    String Key = line;
    String Value = ++equals;
//    CutEndSpaces( Key );
//    CutEndSpaces( Value );

    int iLast = Value.Length() - 1;
    while ( iLast >= 0 )
    {
      if ( Value.c_str()[iLast] == '\n' )
      {
        Value.Delete(iLast+1, 1);
        iLast--;
      }
      else
        break;
    }


    if ( Key.Length() > 0 )
      addKey( SecName, Key, Value );
  }
  fclose( f );
  return 1;
}
//---------------------------------------------------------------------------
int __fastcall TAIIniFile::WriteIni()
{
  // zmieñ katalog na windows
  char dir[1024];

  if ( FileName.LastDelimiter("\\") == 0 )
  {
    GetWindowsDirectory(dir, sizeof(dir));
  }
  else
  {
    // bie¿ z nazwy pliku ?
    dir[0] = '\0';
  }
  TDirectoryStore store(dir);


  FILE *f;
  f = fopen( FileName.c_str(), "w" );
  if(!f)
    return -1;
  for ( int i = 0; i < Sections->Count; i++ )
  {
    // sekcja ...
    aiIniElement* el = (aiIniElement*)Sections->Items[i];
    String Title = (String)"[" + el->SecName + (String)"]\n";
    fputs( Title.c_str(), f );
    // wartoœci ...
    for ( int k = 0; k < el->Keys->Count; k++ )
    {
      String Line = el->Keys->Strings[k] + (String)"=" + el->Values->Strings[k] + (String)"\n";
      fputs( Line.c_str(), f );
    }
  }
  fclose(f);
  return 1;
}
//---------------------------------------------------------------------------
int __fastcall TAIIniFile::addKey( AnsiString SecName, AnsiString Key, AnsiString Value )
{
//  String S = SecName + "=" + (String)SecName.Length() + "\n" + Key + "=" +(String)Key.Length() + "\n" + Value + "=" + (String)Value.Length();
//  MessageBox( 0, S.c_str(), "addKey", MB_OK );


  // wpierw szukamy na liœcie sekcji, je¿eli nie ma to dodajemy now¹ sekcjê
  int pos = findSectionIndex( SecName );
  if ( pos == -1 )
  {
     aiIniElement* el = new aiIniElement;
     el->SecName = SecName;
     el->Keys = new TStringList;
     el->Values = new TStringList;
     pos = Sections->Add( el );
  }
  // potem szukamy klucza, je¿eli nie ma to dodajemy
  aiIniElement* el = (aiIniElement*)Sections->Items[pos];
  if ( el != NULL )
  {
    int Index = findKeyIndex( pos, Key );
    if ( Index != -1 )
    {
      // ustaw now¹ wartoœæ
      if ( Value.Length() > 0 )
      {
        el->Values->Strings[Index] = Value;
      }
      else
      {
        // usuñ wartoœci
        el->Keys->Delete( Index );
        el->Values->Delete( Index );
      }
    }
    else
    {
      if ( Value.Length() > 0 )
      {
        el->Keys->Add( Key );
        el->Values->Add( Value );
      }
    }
  }
  return 1;
}
//---------------------------------------------------------------------------
void CutEndSpaces( AnsiString &key )
{
  int iLast = key.Length() - 1;
  while ( iLast >= 0 )
  {
    if ( key.c_str()[iLast] == ' ' || key.c_str()[iLast] == '\n' )
    {
      key.Delete(iLast+1, 1);
      iLast--;
    }
    else
      break;
  }
  while ( key.Length() > 0 && (key.c_str()[0] == ' ' || key.c_str()[0] == '\n') )
  {
    key.Delete(1,1);
  }
}
//---------------------------------------------------------------------------
bool __fastcall TAIIniFile::ExistsKey( const AnsiString Section, const AnsiString Ident )
{
  int pos = findSectionIndex( Section );
  if ( pos != -1 )
    return  (findKeyIndex( pos, Ident ) != -1);
  return false;
}
//---------------------------------------------------------------------------

