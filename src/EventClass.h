#ifndef EventClass_
#define EventClass_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <map>
#include "TSystem.h"
#include "TObject.h"
#include "TTree.h"
#include "Environ.h"

class EventClass : public TObject
{
  public:
    EventClass();
    void SetKinematics(Int_t runnum_,
                       Double_t E12_,
                       Double_t Pt_,
                       Double_t Eta_,
                       Double_t Phi_,
                       Double_t M12_,
                       Double_t Z_,
                       Double_t N12_);
    Int_t Idx(char * name);
    char * Name(Int_t idx);
    char * Title(Int_t idx);
    char * Title(char * name);
    Bool_t Valid(Int_t idx); // returns true if cuts pass
    Bool_t ValidWithoutMcut(Int_t idx); // Valid(), but don't cut on mass
    Bool_t ValidWithoutZcut(Int_t idx); // Valid(), but don't cut on Z
    Bool_t CheckMass(Double_t M12_);
    Bool_t FiducialGeom(Double_t Eta_,Double_t Phi_, Double_t Cd);

    Int_t N;
    Int_t runnum;
    Double_t E12,Pt,Eta,Phi,M12,Z,N12;

    Double_t Theta,Xd,Yd;

  protected:
    std::map<Int_t, char*> class_name; // idx --> name
    std::map<std::string, Int_t> class_idx;  // name --> idx
    std::map<Int_t, char*> class_title; // idx --> title

    Environ * env;
    TTree * mass_tr;
    Double_t kbinL,kbinH,massL,massM,massH;
    TTree * exclude_tr;
    Int_t exc_run;

  ClassDef(EventClass,1);
};

#endif
