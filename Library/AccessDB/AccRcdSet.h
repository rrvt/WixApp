// Part of the infastructure of examining tables and fields.


#pragma once
#include "afxdb.h"


enum SetResult {SuccessSet, SuccesWithInfoSet, NoDataSet, ErrorSet, InvalidHandleSet, StillExecutingSet};


class AccRcdSet : public CRecordset {

DECLARE_DYNAMIC(AccRcdSet)

protected:

bool      opened;
SetResult result;

public:

  AccRcdSet(CDatabase* database) : CRecordset(database), opened(false), result(SuccessSet)
                                                                              {m_nDefaultType = dynaset;}
 ~AccRcdSet() {close();}

  void AllocAndCacheFieldInfo();

  void close();

private:

  virtual void CheckRowsetError(RETCODE nRetCode);
  };

