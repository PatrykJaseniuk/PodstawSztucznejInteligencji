#include <vcl.h>
#pragma hdrstop

#include "dirclass.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <io.h>
#include <Sysutils.hpp>

#pragma package(smart_init)
//------------------------------------------------------------------------------
TTemporaryFilename::TTemporaryFilename( bool bAutoDel, char * extension, char *dir )
{
   bAutoDelete = bAutoDel;
   CreatePath( extension, dir );
}
//------------------------------------------------------------------------------
TTemporaryFilename::~TTemporaryFilename()
{
  if ( bAutoDelete )
    DeleteFile();
}
//------------------------------------------------------------------------------
void TTemporaryFilename::CreatePath( char * extension, char *dir )
{
   char temp[L_tmpnam];
   tmpnam( temp );
   if ( extension != NULL )
   {
      char *ptr = strchr(temp, '.');
      *ptr = '\0';
      strcat( temp, extension );
   }
   if ( dir == NULL )
   {
     GetTempPath( sizeof(path), path ); // w TMP
/*
     if ( access(path,0) != 0 )
     {
       aiMessageBox( 0, "Brak dostêpu do katalogu tymczasowego proszê sprawdzic zmienne srodowiskowe TMP oraz TEMP", "Stop", MB_OK | MB_ICONSTOP );
     }
*/     
   }
   else
   {
     strcpy( path, dir );
     if ( path[strlen(path)-1] != '\\' )
       strcat( path, "\\" );
   }
   strcat( path, temp);

   String full = ExpandFileName( path );
   strcpy( path, full.c_str() );


}
//------------------------------------------------------------------------------
void TTemporaryFilename::DeleteFile()
{
   if ( access(path,0) == 0 )
     ::DeleteFile( path );
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
TDirectoryStore::TDirectoryStore( char *new_directory )
{
    StoreDirectory( new_directory );
}
//------------------------------------------------------------------------------
TDirectoryStore::~TDirectoryStore()
{
    RestoreDirectory();
}
//------------------------------------------------------------------------------
void TDirectoryStore::StoreDirectory( char * directory )
{
    // zapamiêtaj star¹ œcie¿kê...
    strcpy(old_path, "X:\\");
#pragma warn -sig
    old_path[0] = 'A' + getdisk();
    getcurdir(0, old_path+3);
    // zmieñ na now¹ ...
    chdir_ex( directory );
}
//------------------------------------------------------------------------------
int TDirectoryStore::RestoreDirectory()
{
    return chdir_ex( old_path );
}
//------------------------------------------------------------------------------
int chdir_ex( char *path )
{
    if ( path == NULL || *path == '\0' )
        return -1;
    char *wsk = strrchr( path, ':' );
    if ( wsk != NULL )
    {
        // jest te¿ dysk !
        setdisk( toupper(path[0])-'A' );
    }
    return SetCurrentDir( path );
    //return chdir( path );
}
//------------------------------------------------------------------------------
__fastcall TAISetCursor::TAISetCursor( TCursor cursor )
{
  saveCursor = Screen->Cursor;
  Screen->Cursor = cursor;
}
//------------------------------------------------------------------------------
__fastcall TAISetCursor::~TAISetCursor()
{
  Screen->Cursor = saveCursor;
}
//------------------------------------------------------------------------------

