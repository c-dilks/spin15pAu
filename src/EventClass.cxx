#include "EventClass.h"

ClassImp(EventClass)

namespace
{
  const Int_t N_CLASS = 5;
  enum class_enum
  {
    kSph,
    kPi0,
    kThr,
    kEtm,
    kJps,
  };

  const Double_t Zd = 720;  // distance from IP [cm]
  const Double_t Sd = 3.8; // small cell dimension [cm]
  const Double_t Ld = 5.8; // large cell dimension [cm]

  const Double_t pi0_mass = 0.135; // pi0 mass [GeV]
  const Double_t etm_mass = 0.548; // eta mass [GeV]
  const Double_t jps_mass = 3.097; // j/psi mass [GeV]
};

// constructor
EventClass::EventClass()
{
  N = N_CLASS;

  // event class names
  class_name.insert(std::pair<Int_t,char*>(kSph,"sph")); // single photons
  class_name.insert(std::pair<Int_t,char*>(kPi0,"pi0")); // pi0's
  class_name.insert(std::pair<Int_t,char*>(kThr,"thr")); // three or more photons
  class_name.insert(std::pair<Int_t,char*>(kEtm,"etm")); // eta's
  class_name.insert(std::pair<Int_t,char*>(kJps,"jps")); // j/psi's

  // event class titles
  class_title.insert(std::pair<Int_t,char*>(kSph,"Single #gamma")); // single photons
  class_title.insert(std::pair<Int_t,char*>(kPi0,"#pi^{0}")); // pi0's
  class_title.insert(std::pair<Int_t,char*>(kThr,"N_{#gamma}>2")); // three or more photons
  class_title.insert(std::pair<Int_t,char*>(kEtm,"#eta-meson")); // eta's
  class_title.insert(std::pair<Int_t,char*>(kJps,"J/#psi")); // j/psi's

  // event class index from name
  for(Int_t n=0; n<N_CLASS; n++)
    class_idx.insert(std::pair<std::string,Int_t>(std::string(class_name[n]),n));

  // read kinematic-dependent mass cuts
  env = new Environ();
  mass_tr = new TTree();
  char mass_tr_file[512];
  sprintf(mass_tr_file,"%s/mass_cuts.dat",env->SpinDir);
  mass_tr->ReadFile(mass_tr_file,"kbinL/D:kbinH/D:massL/D:massM/D:massH/D");
  mass_tr->SetBranchAddress("kbinL",&kbinL);
  mass_tr->SetBranchAddress("kbinH",&kbinH);
  mass_tr->SetBranchAddress("massL",&massL);
  mass_tr->SetBranchAddress("massM",&massM);
  mass_tr->SetBranchAddress("massH",&massH);

  // import exclusion list
  exclude_tr = new TTree();
  char exclude_tr_file[512];
  sprintf(exclude_tr_file,"%s/exclusion_list",env->SpinDir);
  exclude_tr->ReadFile(exclude_tr_file,"exc_run/I");
  exclude_tr->SetBranchAddress("exc_run",&exc_run);
};


// sets kinematic variables for the event, used to check cuts with Valid()
void EventClass::SetKinematics(Int_t runnum_,
                               Double_t E12_,
                               Double_t Pt_,
                               Double_t Eta_,
                               Double_t Phi_,
                               Double_t M12_,
                               Double_t Z_,
                               Double_t N12_)
{
  runnum = runnum_;
  E12 = E12_;
  Pt = Pt_;
  Eta = Eta_;
  Phi = Phi_;
  M12 = M12_;
  Z = Z_;
  N12 = N12_;
};


// returns event class index
Int_t EventClass::Idx(char * name)
{
  return class_idx.at(std::string(name));
};

// returns event class name, given index
char * EventClass::Name(Int_t idx)
{
  return class_name.at(idx);
};

// returns event class title, given index
char * EventClass::Title(Int_t idx)
{
  return class_title.at(idx);
};

// returns event class title, given event class name
char * EventClass::Title(char * name)
{
  return class_title.at(Idx(name));
};


//////////////////////////
//   EVENT CLASS CUTS   //
//////////////////////////
// returns true if the event passes the cuts
Bool_t EventClass::Valid(Int_t idx)
{
  // check if on exclusion list
  for(Int_t e=0; e<exclude_tr->GetEntries(); e++)
  {
    exclude_tr->GetEntry(e);
    if(runnum==exc_run) return false;
  };

  // single photon cuts
  if(idx==kSph)
  {
    if( fabs(N12-1)<0.1 &&
        E12>10 &&
        Pt>1 ) return true;
  }

  // pi0 cuts
  else if(idx==kPi0)
  {
    if( fabs(N12-2)<0.1 &&
        Z<0.8 &&
        CheckMass(M12) &&
        E12>10 &&
        Pt>1 ) return true;
  }

  // three or more photons cuts
  else if(idx==kThr)
  {
    if( N12>2 &&
        M12>0.7 && 
        E12>10 &&
        Pt>1 ) return true;
  }

  // eta meson cuts
  else if(idx==kEtm)
  {
    if( fabs(N12-2)<0.1 &&
        Z<0.8 &&
        fabs(M12-etm_mass)<0.15 &&
        FiducialGeom(Eta,Phi,1.5) &&
        E12>10 &&
        Pt>1 ) return true;
  }

  // j/psi cuts
  else if(idx==kJps)
  {
    if( fabs(N12-2)<0.1 &&
        Z<0.3 &&
        M12>2 && M12<3.5 &&
        Eta>3.2 &&
        FiducialGeom(Eta,Phi,1.5) &&
        E12>60 && E12<100 &&
        Pt>1) return true;
  }

  return false;
};
//////////////////////////


// checks Valid(), but ignores Mass cut
Bool_t EventClass::ValidWithoutMcut(Int_t idx)
{
  Double_t M12_tmp = M12; // store M12 to tmp variable
  Bool_t boole;
  // set M12 to optimal meson masses
  if(idx==kPi0) M12 = pi0_mass;
  else if(idx==kEtm) M12 = etm_mass;
  else if(idx==kJps) M12 = jps_mass;
  boole = Valid(idx);
  M12 = M12_tmp; // restore value of M12
  return boole;
};


// checks Valid(), but ignores Z cut
Bool_t EventClass::ValidWithoutZcut(Int_t idx)
{
  Double_t Z_tmp = Z; // store Z to tmp variable
  Double_t boole;
  Z = 0; // set Z to optimal value
  boole = Valid(idx);
  Z = Z_tmp; // restore value of Z
  return boole;
};


// check E-dependent or Pt-dependent mass cut; returns true if passed
Bool_t EventClass::CheckMass(Double_t M12_)
{
  for(Int_t q=0; q<mass_tr->GetEntries(); q++)
  {
    mass_tr->GetEntry(q);
    if( (M12>=massL && M12<=massH) &&
        ( (!strcmp(env->MassCutType,"en") && E12>=kbinL && E12<=kbinH) ||
          (!strcmp(env->MassCutType,"pt") && Pt>=kbinL && Pt<=kbinH) )) return true;
  };
  return false;
};


// checks if Eta and Phi are within fiducial area cut, defined as Cd-cells
// from cell boundaries
Bool_t EventClass::FiducialGeom(Double_t Eta_, Double_t Phi_, Double_t Cd)
{
  Bool_t boole = false;
  Theta = 2*atan2(exp(-1*Eta_),1); // polar angle

  Xd =  Zd * tan(Theta) * cos(Phi_);
  Yd =  Zd * tan(Theta) * sin(Phi_);

  // small cell fiducial area
  if( ( fabs(Xd)<(12-Cd)*Sd && fabs(Yd)<(12-Cd)*Sd ) && 
     !( fabs(Xd)<( 5+Cd)*Sd && fabs(Yd)<( 5+Cd)*Sd )) boole = true;

  // large cell fiducial area
  if( ( fabs(Xd)<(17-Cd)*Ld && fabs(Yd)<(17-Cd)*Ld ) &&
     !( fabs(Xd)<( 8+Cd)*Ld && fabs(Yd)<( 8+Cd)*Ld ) &&
     fabs(Xd+Yd)<(26-sqrt(2)*Cd)*Ld &&
     fabs(Xd-Yd)<(26-sqrt(2)*Cd)*Ld) boole = true;


  // uncomment these lines for cut which was used for spin2014...
  //if(Cd!=0) boole = false;
  //if(Cd!=0 && Eta_>=2.5 && Eta_<=4) boole = true;

  return boole;
};
