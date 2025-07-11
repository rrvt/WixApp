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

