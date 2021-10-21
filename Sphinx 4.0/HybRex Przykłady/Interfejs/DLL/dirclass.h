#ifndef _TDIRCLASS_H_
#define _TDIRCLASS_H_

#include <dir.h>
#include <windows.h>

int PACKAGE chdir_ex( char *path );

//---------------------------------------------------------------------------
class PACKAGE TTemporaryFilename
{
  public:
    TTemporaryFilename( bool bAutoDel = false, char * extension = ".$$$", char *dir = NULL );
    ~TTemporaryFilename();
    void CreatePath( char * extension = ".$$$", char *dir = NULL );
    void DeleteFile();
    char *GetFileName() { return path; }

  protected:
    bool bAutoDelete;
    char path[MAX_PATH];
};

//---------------------------------------------------------------------------
class PACKAGE TDirectoryStore
{
    public:
        TDirectoryStore( char *new_directory = NULL );
        ~TDirectoryStore();
        void StoreDirectory( char * directory );
        int RestoreDirectory();
    protected :
        char drive;
        char old_path[MAX_PATH];
};

//---------------------------------------------------------------------------
class PACKAGE TAISetCursor
{
 public:
   __fastcall TAISetCursor( TCursor cursor );
   __fastcall ~TAISetCursor();
 protected:
   TCursor saveCursor;
};
//---------------------------------------------------------------------------


#endif
