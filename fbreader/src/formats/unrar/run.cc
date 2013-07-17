/* put in the same directory as the unrar source files
 * compiling with:
 *   make clean
 *   make lib
 *   g++ rartest.cpp -o rartest libunrar.so -lboost_filesystem
 */

#include  <cstring>
#include  <iostream>
#include  <fstream>
#include <stdlib.h>

#define _UNIX
#define  RARDLL
#include "dll.hpp"

using namespace std;

int main(int argc, char* argv[]) {
  if (argc != 2)
    return 0;
  ifstream archiveStream(argv[1]);
  if (!archiveStream.is_open())
    cout << "fstream couldn't open file\n";

  // declare and set parameters
  HANDLE rarFile;  // I renamed this macro in dll.hpp for my own purposes
  RARHeaderDataEx fileInfo;
  RAROpenArchiveDataEx archiveInfo;
  memset(&archiveInfo, 0, sizeof(archiveInfo));
  archiveInfo.CmtBuf = NULL;
  archiveInfo.OpenMode = RAR_OM_EXTRACT;
  archiveInfo.ArcName = argv[1];

  // Open file
  rarFile = RAROpenArchiveEx(&archiveInfo);
  if (archiveInfo.OpenResult != 0) {
    RARCloseArchive(rarFile);
    cout  << "unrar couldn't open" << endl;
    exit(1);
  }
  fileInfo.CmtBuf = NULL;

  // loop through archive
  int numFiles = 0;
  int RHCode;
  int PFCode;
  char currDir[255] = ".";

  while(true) {
    RHCode = RARReadHeaderEx(rarFile, &fileInfo);
    if (RHCode != 0) break;
   // RARReadHeaderEx(rarFile2, &fileInfo);

    numFiles++;

    cout << dec << fileInfo.Method << " " << fileInfo.FileName << " (" << fileInfo.UnpSize << ")" << endl;

    // works for some files, but not for others
    PFCode = RARProcessFile(rarFile, RAR_TEST, NULL, NULL);


    if (PFCode != 0) {
      RARCloseArchive(rarFile);
      cout  << "error processing this file\n" << endl;
      exit(1);
    }

    // properly extracts to a directory...
    RARProcessFile(rarFile, RAR_EXTRACT, currDir, fileInfo.FileName);

  }

  if (RHCode != ERAR_END_ARCHIVE) {
    cout  << "error traversing through archive: " << RHCode << endl;
  }

  RARCloseArchive(rarFile);

  cout  << dec << "num files: " << numFiles << endl;

}

