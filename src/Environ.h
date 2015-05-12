#ifndef Environ_
#define Environ_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <map>
#include "TSystem.h"

class Environ : public TObject
{
  public:
    Environ();
    char SpinDir[256];
    char TriggerType[32];
    char MassCutType[32];
    Int_t PhiBins,EtaBins,PtBins,EnBins;
    Float_t PhiLow,PhiHigh;
    Float_t EtaLow,EtaHigh;
    Float_t PtLow,PtHigh;
    Float_t EnLow,EnHigh;
    Float_t PhiDiv(Int_t bin);
    Float_t EtaDiv(Int_t bin);
    Float_t EnDiv(Int_t bin);
    Float_t PtDiv(Int_t bin);
    Double_t Error(const char * bintype);

  protected:
    typedef std::map<Int_t,Float_t> DivMap;
    DivMap PhiDivMap,EtaDivMap,PtDivMap,EnDivMap;

    ClassDef(Environ,1);
};

#endif
