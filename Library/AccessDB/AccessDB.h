// Open an Access Database for the purposes of determining the table names and fields within the tables

#pragma once


class AccessDB : public CDatabase {

int  loginTimeout;
int  queryTimeout;
bool opened;

public:

  AccessDB();
 ~AccessDB() {close();}

  bool open(TCchar* path);
  void close();

  CDatabase* db() {return this;}

  bool isOpen() {return opened;}
  };



extern AccessDB accessDB;


#if 0
//MyTable*        myTable;            // --> handling tables or columns in a database:

ErrCode         lastRetCode;        // --> error analyzation:
CString         lastErrMsg;

//  bool openTables();
//  bool GetTables(MyTable*& table);
//CDatabase       myDatabase;         // --> database handling:  The all important database object
#endif
