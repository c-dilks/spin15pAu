// Implementation of L2 trigger daq ids, used for masking 
// L2sum in the trigger files

#ifndef Trigger_
#define Trigger_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <map>
#include <stdexcept>

#include "TSystem.h"
#include "TObject.h"
#include "TTree.h"
#include "TString.h"

class Trigger : public TObject
{
  public:
    Trigger();

    Int_t Mask(Int_t run, char * trigger0, Int_t dword);
    Int_t Mask(Int_t run, Int_t num0, Int_t dword);
    Int_t Index(char * trigger0);
    char * Name(Int_t num0);

    /*
    Int_t RP_Mask(Int_t run, char * trigger0, Int_t dword);
    Int_t RP_Mask(Int_t run, Int_t num0, Int_t dword);
    Int_t RP_Index(char * trigger0);
    char * RP_Name(Int_t num0);
    */

    Int_t N; // number of FMS triggers
    //Int_t NRP;  // number of RP triggers

  protected:
    char spindir[512];
    TTree * id_tr;

    // MAPS:
    // here trigger idx refers to the enumerator in the namespace
    //
    std::map<Int_t, char*> trigger_name; // trigger idx --> trigger name
    std::map<Int_t, char*> trigger_dbname; // trigger idx --> trigger daq name
    std::map<std::string, Int_t> trigger_idx; // trigger name --> trigger idx
    std::map<std::string, Int_t> trigger_dbidx; // trigger name --> trigger daq index
    // mask_map[2]: (it's a nested map) 
    // key = run number, value = trig_idx map
    //                           -->trig_idx map:
    //                              key = trigger index, value = daq trigger id
    std::map<Int_t, std::map<Int_t,Long_t> > mask_map;

    // RP MAPS (same convention as above)
    /*
    std::map<Int_t, char*> rptrigger_name;
    std::map<Int_t, char*> rptrigger_dbname;
    std::map<std::string, Int_t> rptrigger_idx;
    std::map<std::string, Int_t> rptrigger_dbidx;
    std::map<Int_t, std::map<Int_t,Long_t> > rpmask_map;
    */

    ClassDef(Trigger,1);
};

#endif
