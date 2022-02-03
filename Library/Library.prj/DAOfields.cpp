// Access DAO Record Fields


#include "stdafx.h"
#include "DAOfields.h"


DAOfield* DAOfldsIter::getDatum() {
FieldP field;

  if (iterX < 0 || fields->Count <= iterX) return 0;

  field = fields->GetItem(iterX);   if (!field) return 0;

  fieldDsc.field = field;
//  fieldDsc.value = field->GetValue();   notNull(fieldDsc.value);
//  fieldDsc.attr  = field->Attributes;

  return &fieldDsc;
  }


// Translate Access Database type value into a c++ type

String DAOfield::getType(DataTypeEnum type) {

  switch (type) {
    case dbBoolean        : return _T("bool");
    case dbByte           : return _T("Byte");
    case dbInteger        : return _T("int");
    case dbLong           : return _T("long");
    case dbCurrency       : return _T("Currency");
    case dbSingle         : return _T("float");
    case dbDouble         : return _T("double");
    case dbDate           : return _T("Date");
    case dbBinary         : return _T("dbBinary");
    case dbText           : return _T("String");
    case dbLongBinary     : return _T("dbLongBinary");
    case dbMemo           : return _T("String");
    case dbGUID           : return _T("dbGUID");
    case dbBigInt         : return _T("dbBigInt");
    case dbVarBinary      : return _T("dbVarBinary");
    case dbChar           : return _T("char");
    case dbNumeric        : return _T("dbNumeric");
    case dbDecimal        : return _T("dbDecimal");
    case dbFloat          : return _T("float");
    case dbTime           : return _T("dbTime");
    case dbTimeStamp      : return _T("dbTimeStamp");
    case dbAttachment     : return _T("dbAttachment");
    case dbComplexByte    : return _T("dbComplexByte");
    case dbComplexInteger : return _T("dbComplexInteger");
    case dbComplexLong    : return _T("dbComplexLong");
    case dbComplexSingle  : return _T("dbComplexSingle");
    case dbComplexDouble  : return _T("dbComplexDouble");
    case dbComplexGUID    : return _T("dbComplexGUID");
    case dbComplexDecimal : return _T("dbComplexDecimal");
    case dbComplexText    : return _T("dbComplexText");
    default               : break;
    }
  String t = _T("<"); t += type; t += _T(">");  return t;
  }

