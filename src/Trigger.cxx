// Implementation of L2 trigger daq ids, used for masking 
// L2sum in the trigger files

#include "Trigger.h"

ClassImp(Trigger)

namespace
{
  const Int_t N_TRIG = 13;
  enum trigger_enum 
  {
    kAll,
    kJP2,kJP1,kJP0,
    kLgBS3,kLgBS2,kLgBS1,
    kSmBS3,kSmBS2,kSmBS1,
    kDiBS,kDiJP,kLED
  };
  /*
  const Int_t N_RPTRIG = 12;
  enum rptrigger_enum
  {
    krpAll,
    krpSD,krpSDT,
    krpRPZMU,krpRPZE,krpRP2MU,krpRP2E,
    krpCPT2,krpET,krpCP,krpCPEI,
    krpZerobias
  };
  */
  const Int_t int_max = 0xFFFFFFFF;
};

Trigger::Trigger()
{
  N = N_TRIG;
  //NRP = N_RPTRIG;


  // get spindir
  if(gSystem->Getenv("SPINDIR")==NULL){fprintf(stderr,"ERROR: source env vars\n");return;};
  sscanf(gSystem->Getenv("SPINDIR"),"%s",spindir);


  // fms trigger names
  trigger_name.insert(std::pair<Int_t,char*>(kAll,"All")); // all triggers except LED
  trigger_name.insert(std::pair<Int_t,char*>(kJP2,"JP2"));
  trigger_name.insert(std::pair<Int_t,char*>(kJP1,"JP1"));
  trigger_name.insert(std::pair<Int_t,char*>(kJP0,"JP0"));
  trigger_name.insert(std::pair<Int_t,char*>(kLgBS3,"LgBS3"));
  trigger_name.insert(std::pair<Int_t,char*>(kLgBS2,"LgBS2"));
  trigger_name.insert(std::pair<Int_t,char*>(kLgBS1,"LgBS1"));
  trigger_name.insert(std::pair<Int_t,char*>(kSmBS3,"SmBS3"));
  trigger_name.insert(std::pair<Int_t,char*>(kSmBS2,"SmBS2"));
  trigger_name.insert(std::pair<Int_t,char*>(kSmBS1,"SmBS1"));
  trigger_name.insert(std::pair<Int_t,char*>(kDiBS,"DiBS"));
  trigger_name.insert(std::pair<Int_t,char*>(kDiJP,"DiJP"));
  trigger_name.insert(std::pair<Int_t,char*>(kLED,"LED"));

  trigger_dbname.insert(std::pair<Int_t,char*>(kAll,"All")); // unused
  trigger_dbname.insert(std::pair<Int_t,char*>(kJP2,"FMS-JP2"));
  trigger_dbname.insert(std::pair<Int_t,char*>(kJP1,"FMS-JP1"));
  trigger_dbname.insert(std::pair<Int_t,char*>(kJP0,"FMS-JP0"));
  trigger_dbname.insert(std::pair<Int_t,char*>(kLgBS3,"FMS-lg-bs3"));
  trigger_dbname.insert(std::pair<Int_t,char*>(kLgBS2,"FMS-lg-bs2"));
  trigger_dbname.insert(std::pair<Int_t,char*>(kLgBS1,"FMS-lg-bs1"));
  trigger_dbname.insert(std::pair<Int_t,char*>(kSmBS3,"FMS-sm-bs3"));
  trigger_dbname.insert(std::pair<Int_t,char*>(kSmBS2,"FMS-sm-bs2"));
  trigger_dbname.insert(std::pair<Int_t,char*>(kSmBS1,"FMS-sm-bs1"));
  trigger_dbname.insert(std::pair<Int_t,char*>(kDiBS,"FMS-DiBS"));
  trigger_dbname.insert(std::pair<Int_t,char*>(kDiJP,"FMS-DiJP"));
  trigger_dbname.insert(std::pair<Int_t,char*>(kLED,"FMS-LED"));

  std::map<char*,Int_t> trigger_name_idx; // trigger name --> trigger idx
  std::map<char*,Int_t> trigger_dbname_idx; // trigger dbname --> trigger idx
  for(Int_t t=0; t<N_TRIG; t++)
  {
    trigger_idx.insert(std::pair<std::string,Int_t>(std::string(trigger_name[t]),t));
    trigger_dbidx.insert(std::pair<std::string,Int_t>(std::string(trigger_dbname[t]),t));
  };


  // rp trigger names
  /*
  rptrigger_name.insert(std::pair<Int_t,char*>(krpAll,"All")); 
  rptrigger_name.insert(std::pair<Int_t,char*>(krpSD,"SD")); 
  rptrigger_name.insert(std::pair<Int_t,char*>(krpSDT,"SDT")); 
  rptrigger_name.insert(std::pair<Int_t,char*>(krpRPZMU,"RPZMU")); 
  rptrigger_name.insert(std::pair<Int_t,char*>(krpRPZE,"RPZE")); 
  rptrigger_name.insert(std::pair<Int_t,char*>(krpRP2MU,"RP2MU")); 
  rptrigger_name.insert(std::pair<Int_t,char*>(krpRP2E,"RP2E")); 
  rptrigger_name.insert(std::pair<Int_t,char*>(krpCPT2,"CPT2")); 
  rptrigger_name.insert(std::pair<Int_t,char*>(krpET,"ET")); 
  rptrigger_name.insert(std::pair<Int_t,char*>(krpCP,"CP")); 
  rptrigger_name.insert(std::pair<Int_t,char*>(krpCPEI,"CPEI")); 
  rptrigger_name.insert(std::pair<Int_t,char*>(krpZerobias,"Zerobias")); 

  rptrigger_dbname.insert(std::pair<Int_t,char*>(krpAll,"All")); // unused
  rptrigger_dbname.insert(std::pair<Int_t,char*>(krpSD,"RP_SD")); 
  rptrigger_dbname.insert(std::pair<Int_t,char*>(krpSDT,"RP_SDT")); 
  rptrigger_dbname.insert(std::pair<Int_t,char*>(krpRPZMU,"RP_RPZMU")); 
  rptrigger_dbname.insert(std::pair<Int_t,char*>(krpRPZE,"RP_RPZE")); 
  rptrigger_dbname.insert(std::pair<Int_t,char*>(krpRP2MU,"RP_RP2MU")); 
  rptrigger_dbname.insert(std::pair<Int_t,char*>(krpRP2E,"RP_RP2E")); 
  rptrigger_dbname.insert(std::pair<Int_t,char*>(krpCPT2,"RP_CPT2")); 
  rptrigger_dbname.insert(std::pair<Int_t,char*>(krpET,"RP_ET")); 
  rptrigger_dbname.insert(std::pair<Int_t,char*>(krpCP,"RP_CP")); 
  rptrigger_dbname.insert(std::pair<Int_t,char*>(krpCPEI,"RP_CPEI")); 
  rptrigger_dbname.insert(std::pair<Int_t,char*>(krpZerobias,"RP_Zerobias")); 

  std::map<char*,Int_t> rptrigger_name_idx; // trigger name --> trigger idx
  std::map<char*,Int_t> rptrigger_dbname_idx; // trigger dbname --> trigger idx
  for(Int_t t=0; t<N_RPTRIG; t++)
  {
    rptrigger_idx.insert(std::pair<std::string,Int_t>(std::string(rptrigger_name[t]),t));
    rptrigger_dbidx.insert(std::pair<std::string,Int_t>(std::string(rptrigger_dbname[t]),t));
  };
  */


  // read trigid.dat
  id_tr = new TTree();
  char trigidfile[512];
  sprintf(trigidfile,"%s/trigid.dat",spindir);
  id_tr->ReadFile(trigidfile,"rn/I:dbidx/I:name/C");
  Int_t rn,dbidx;
  char name[32];
  id_tr->SetBranchAddress("rn",&rn);
  id_tr->SetBranchAddress("dbidx",&dbidx);
  id_tr->SetBranchAddress("name",name);
  std::map<Int_t,Long_t> trig_idx; // trigger idx --> trigger dbidx
  //std::map<Int_t,Long_t> rptrig_idx; // rptrigger idx --> rptrigger dbidx
  id_tr->GetEntry(0); 
  Int_t rn_tmp=rn;
  //Int_t rprn_tmp=rn;
  Int_t idx;
  Long_t mask;
  Long_t allmask=0;
  //Long_t rpallmask=0;
  TString name_str;
  
  // build mask map
  for(Int_t i=0; i<id_tr->GetEntries(); i++)
  {
    id_tr->GetEntry(i);
    name_str = TString(name);

    // check for FMS triggers
    if(name_str(0,3)=="FMS")
    {
      idx = trigger_dbidx.at(std::string(name));

      mask = (long)1 << dbidx;
      if(idx!=kLED) allmask = allmask | mask;

      trig_idx.insert(std::pair<Int_t,Long_t>(idx,mask));
      if(rn != rn_tmp || i+1==id_tr->GetEntries())
      {
        idx = trigger_idx.at(std::string("All"));
        trig_idx.insert(std::pair<Int_t,Long_t>(idx,allmask));
        mask_map.insert(std::pair<Int_t,std::map<Int_t,Long_t> >(rn_tmp,trig_idx));
        rn_tmp=rn;
        trig_idx.clear();
        allmask=0;
      };
    }
    //else fprintf(stderr,"ERROR: not FMS trigger\n");

    // check for RP triggers
    /*
    else if(name_str(0,2)=="RP")
    {
      idx = rptrigger_dbidx.at(std::string(name));

      mask = (long)1 << dbidx;
      rpallmask = rpallmask | mask;

      rptrig_idx.insert(std::pair<Int_t,Long_t>(idx,mask));
      if(rn != rprn_tmp || i+1==id_tr->GetEntries())
      {
        idx = rptrigger_idx.at(std::string("All"));
        rptrig_idx.insert(std::pair<Int_t,Long_t>(idx,rpallmask));
        rpmask_map.insert(std::pair<Int_t,std::map<Int_t,Long_t> >(rprn_tmp,rptrig_idx));
        rprn_tmp=rn;
        rptrig_idx.clear();
        rpallmask=0;
      };
    }
    */
  };
};

// fms accessors
//////////////////
Int_t Trigger::Index(char * trigger0)
{
  return trigger_idx.at(std::string(trigger0));
};

Int_t Trigger::Mask(Int_t run, char * trigger0, Int_t dword)
{
  Int_t retval;
  try { retval = (mask_map.at(run).at(Index(trigger0)) >> (32*dword)) & int_max; }
  catch(const std::out_of_range& e) 
  {
    //fprintf(stderr,"[+] no FMS triggers for run %d\n",run);
    retval=0;
  };
  return retval;
};

Int_t Trigger::Mask(Int_t run, Int_t num0, Int_t dword)
{
  Int_t retval;
  try { retval = (mask_map.at(run).at(num0) >> (32*dword)) & int_max; }
  catch(const std::out_of_range& e) 
  {
    //fprintf(stderr,"[+] no FMS triggers for run %d\n",run);
    retval=0;
  };
  return retval;
};

char * Trigger::Name(Int_t num0)
{
  return trigger_name.at(num0);
};


// rp accessors
//////////////////
/*
Int_t Trigger::RP_Index(char * trigger0)
{
  return rptrigger_idx.at(std::string(trigger0));
};

Int_t Trigger::RP_Mask(Int_t run, char * trigger0, Int_t dword)
{
  Int_t retval;
  try { retval = (rpmask_map.at(run).at(RP_Index(trigger0)) >> (32*dword)) & int_max; }
  catch(const std::out_of_range& e) 
  {
    //fprintf(stderr,"[+] no RP triggers for run %d\n",run);
    retval=0;
  };
  return retval;
};

Int_t Trigger::RP_Mask(Int_t run, Int_t num0, Int_t dword)
{
  Int_t retval;
  try { retval = (rpmask_map.at(run).at(num0) >> (32*dword)) & int_max; }
  catch(const std::out_of_range& e) 
  {
    //fprintf(stderr,"[+] no RP triggers for run %d\n",run);
    retval=0;
  };
  return retval;
};

char * Trigger::RP_Name(Int_t num0)
{
  return rptrigger_name.at(num0);
};
*/
