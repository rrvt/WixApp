// Access DAO Record Fields


#pragma once
#include "DAOAccess.h"
#include "DAOrecords.h"


const long FAU = dbFixedField | dbAutoIncrField | dbUpdatableField;


class DAOfield {
FieldP field;

public:

  DAOfield() : field(0) {}

  String    name()  {return field->Name;}
  variant_t value() {variant_t v = field->Value;  notNull(v); return v;}
  long      attr()  {return field->Attributes;}
  bool      index() {return (field->Attributes & FAU) == FAU;}
  String    type()  {return getType((DataTypeEnum) field->Type);}

  DAOfield& operator= (variant_t v)
                               {try {field->Value = v;} catch(ComError& e) {daoError(e);}; return *this;}

private:

  String getType(DataTypeEnum type);      // Translate Access Database type value into a c++ type

  friend class DAOfldsIter;
  };


class DAOfldsIter {
int      iterX;
FieldsP  fields;
DAOfield fieldDsc;

public:

  DAOfldsIter(FieldsP fieldsP) : fields(fieldsP) { }
 ~DAOfldsIter() { }

  int index() {return iterX;}

  DAOfield* operator() ()    {iterX = 0;                          return getDatum();}
  DAOfield* operator++ (int) {if (iterX < fields->Count) iterX++; return getDatum();}

private:

  DAOfield* getDatum();

  DAOfldsIter() : fields(*(FieldsP*)0) { }
  };

