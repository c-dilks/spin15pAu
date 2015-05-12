#include "RunData.h"
using namespace std;

ClassImp(RunData)

RunData::RunData()
{
  Construct("");
};

RunData::RunData(char * spindir0)
{
  Construct(spindir0);
};

void RunData::Construct(char * spindir0)
{
  // define maxes
  NRUNS = sizeof(runnum_map)/sizeof(*runnum_map);
  NFILLS = sizeof(b_pol_map)/sizeof(*b_pol_map);
  printf("Loop Maxes: NRUNS=%d NFILLS=%d\n",NRUNS,NFILLS);


  // read trees
  //char spindir[256];
  char counts_file[256];
  char rtree_file[256];
  char pol_file[256];
  if(!strcmp(spindir0,""))
  {
    if(gSystem->Getenv("SPINDIR")==NULL){fprintf(stderr,"ERROR: source env vars\n");return;};
    sscanf(gSystem->Getenv("SPINDIR"),"%s",spindir);
  }
  else strcpy(spindir,spindir0);
  printf("spindir=%s\n",spindir);
  sprintf(counts_file,"%s/counts.root",spindir);
  sprintf(rtree_file,"%s/rtree.root",spindir);
  sprintf(pol_file,"%s/pol.root",spindir);
  counts = new TFile(counts_file,"READ");
  rtree = new TFile(rtree_file,"READ");
  pol = new TFile(pol_file,"READ");
  counts_tr = (TTree*) counts->Get("sca");
  rtree_tr = (TTree*) rtree->Get("rellum");
  pol_tr = (TTree*) pol->Get("pol");
  

  // set rtree branch addresses
  rtree_tr->SetBranchAddress("i",&i_rtree);
  rtree_tr->SetBranchAddress("runnum",&runnum);
  rtree_tr->SetBranchAddress("fill",&fill_rtree);
  rtree_tr->SetBranchAddress("t",&t);
  rtree_tr->SetBranchAddress("isConsistent",&isConsistent);
  rtree_tr->SetBranchAddress("pattern",&pattern_no);

  char R_bbc_name[10][16];
  char R_zdc_name[10][16];
  char R_vpd_name[10][16];
  char R_bbc_err_name[10][16];
  char R_zdc_err_name[10][16];
  char R_vpd_err_name[10][16];

  for(Int_t r=1; r<10; r++)
  {
    //sprintf(R_bbc_name[r],"R%d_bbc_mean",r);
    //sprintf(R_zdc_name[r],"R%d_zdc_mean",r);
    //sprintf(R_vpd_name[r],"R%d_vpd_mean",r);
    sprintf(R_bbc_name[r],"R%d_bbcrsc",r);
    sprintf(R_zdc_name[r],"R%d_zdcrsc",r);
    sprintf(R_vpd_name[r],"R%d_vpdrsc",r);
    rtree_tr->SetBranchAddress(R_bbc_name[r],&(R_bbc[r]));
    rtree_tr->SetBranchAddress(R_zdc_name[r],&(R_zdc[r]));
    rtree_tr->SetBranchAddress(R_vpd_name[r],&(R_vpd[r]));

    //sprintf(R_bbc_err_name[r],"R%d_bbc_mean_err",r);
    //sprintf(R_zdc_err_name[r],"R%d_zdc_mean_err",r);
    //sprintf(R_vpd_err_name[r],"R%d_vpd_mean_err",r);
    sprintf(R_bbc_err_name[r],"R%d_bbc_rsc_err",r);
    sprintf(R_zdc_err_name[r],"R%d_zdc_rsc_err",r);
    sprintf(R_vpd_err_name[r],"R%d_vpd_rsc_err",r);
    rtree_tr->SetBranchAddress(R_bbc_err_name[r],&(R_bbc_err[r]));
    rtree_tr->SetBranchAddress(R_zdc_err_name[r],&(R_zdc_err[r]));
    rtree_tr->SetBranchAddress(R_vpd_err_name[r],&(R_vpd_err[r]));
  };




  // set counts tree branch addresses
  counts_tr->SetBranchAddress("i",&i_counts);
  counts_tr->SetBranchAddress("bx",&bx);
  counts_tr->SetBranchAddress("blue",&blue);
  counts_tr->SetBranchAddress("yell",&yell);
  counts_tr->SetBranchAddress("kicked",&kicked);


  // set pol tree branch addresses
  pol_tr->SetBranchAddress("fill",&fill_pol);
  pol_tr->SetBranchAddress("b_pol",&b_pol);
  pol_tr->SetBranchAddress("y_pol",&y_pol);
  pol_tr->SetBranchAddress("b_pol_e",&b_pol_err);
  pol_tr->SetBranchAddress("y_pol_e",&y_pol_err);


  // build spin maps --> gives blue/yell spin + kicked status for 
  //                     any given run index and bXing no.
  for(Int_t q=0; q<NRUNS; q++)
  {
    for(Int_t qq=0; qq<120; qq++)
    {
      blue_spin_map[q][qq]=0;
      yell_spin_map[q][qq]=0;
      kicked_bx_map[q][qq]=0;
    }
    runnum_map[q]=0;
  };
  for(Int_t q=0; q<counts_tr->GetEntries(); q++)
  {
    counts_tr->GetEntry(q);
    blue_spin_map[i_counts-1][bx] = blue;
    yell_spin_map[i_counts-1][bx] = yell;
    if(kicked) kicked_bx_map[i_counts-1][bx] = 1;
    else kicked_bx_map[i_counts-1][bx] = 0;
  };
  printf("spin maps built\n");


  // build run number hash table and spin pattern map
  // -- rtree tree is viewed as a hash table indexed by tree entry number
  // -- run index - 1 = rtree entry number
  Int_t runnum_tmp=0;
  fill_thou=19000; // run15 pAu
  for(Int_t q=0; q<rtree_tr->GetEntries(); q++)
  {
    rtree_tr->GetEntry(q);
    if(runnum_tmp == runnum)
    {
      fprintf(stderr,"ERROR: duplicated run number\n");
      return;
    }
    else runnum_tmp = runnum;

    if(q != i_rtree-1)
    {
      fprintf(stderr,"ERROR: rdat tree not synced with hashing algorithm assumptions\n");
      return;
    }
    else runnum_map[q] = runnum;
    pattern_map[fill_rtree-fill_thou] = pattern_no;
  };
  printf("hash table ok\n");

  
  // build polarimetry map
  for(Int_t q=0; q<NFILLS; q++)
  {
    b_pol_map[q]=0;
    y_pol_map[q]=0;
    b_pol_err_map[q]=0;
    y_pol_err_map[q]=0;
  };
  for(Int_t q=0; q<pol_tr->GetEntries(); q++)
  {
    pol_tr->GetEntry(q);
    if(fill_pol-fill_thou >= 0 && fill_pol-fill_thou < NFILLS)
    {
      b_pol_map[fill_pol-fill_thou] = b_pol;
      y_pol_map[fill_pol-fill_thou] = y_pol;
      b_pol_err_map[fill_pol-fill_thou] = b_pol_err;
      y_pol_err_map[fill_pol-fill_thou] = y_pol_err;
    }
    else
    {
      fprintf(stderr,"ERROR: variable fill_thou is not correct\n");
      return;
    };
  };
  printf("polarimetry map built\n");
}


Int_t RunData::GetFill(Int_t runnum0)
{
  Int_t set;
  Int_t found=0;
  for(Int_t q=0; q<NRUNS; q++)
  {
    if(runnum0 == runnum_map[q]) 
    {
      set=q;
      found=1;
    };
  };

  rtree_tr->GetEntry(set);
  return found*fill_rtree;
}


Int_t RunData::HashRun(Int_t runnum0)
{
  // linear hashing
  // -- returns "-1" if hashing fails
  for(Int_t q=0; q<NRUNS; q++)
  {
    if(runnum_map[q] == runnum0) return q;
  }
  return -1;
};


Float_t RunData::Rellum(Int_t runnum0, Int_t rellumi, char * detector)
{
  Int_t index = HashRun(runnum0);
  if(index>=0)
  {
    rtree_tr->GetEntry(index);
    if(!strcmp(detector,"bbc")) return R_bbc[rellumi];
    else if(!strcmp(detector,"zdc")) return R_zdc[rellumi];
    else if(!strcmp(detector,"vpd")) return R_vpd[rellumi];
    else
    {
      fprintf(stderr,"ERROR: invalid detector\n");
      return 0;
    }
  }
  else return 0;
};


Float_t RunData::RellumErr(Int_t runnum0, Int_t rellumi, char * detector)
{
  Int_t index = HashRun(runnum0);
  if(index>=0)
  {
    rtree_tr->GetEntry(index);
    if(!strcmp(detector,"bbc")) return R_bbc_err[rellumi];
    else if(!strcmp(detector,"zdc")) return R_zdc_err[rellumi];
    else if(!strcmp(detector,"vpd")) return R_vpd_err[rellumi];
    else
    {
      fprintf(stderr,"ERROR: invalid detector\n");
      return 0;
    }
  }
  else return 0;
};


Bool_t RunData::RellumConsistent(Int_t runnum0)
{
  Int_t index = HashRun(runnum0);
  if(index>=0)
  {
    rtree_tr->GetEntry(index);
    return isConsistent;
  }
  else return 0;
};


Float_t RunData::BluePol(Int_t runnum0)
{
  Int_t fill0 = GetFill(runnum0);
  if(fill0>fill_thou)
    return b_pol_map[fill0-fill_thou];
  else return 0;
}


Float_t RunData::YellPol(Int_t runnum0)
{
  Int_t fill0 = GetFill(runnum0);
  if(fill0>fill_thou)
    return y_pol_map[fill0-fill_thou];
  else return 0;
}


Float_t RunData::BluePolErr(Int_t runnum0)
{
  Int_t fill0 = GetFill(runnum0);
  if(fill0>fill_thou)
    return b_pol_err_map[fill0-fill_thou];
  else return 0;
}


Float_t RunData::YellPolErr(Int_t runnum0)
{
  Int_t fill0 = GetFill(runnum0);
  if(fill0>fill_thou)
    return y_pol_err_map[fill0-fill_thou];
  else return 0;
}


Int_t RunData::BlueSpin(Int_t runnum0, Int_t bXing)
{
  Int_t index = HashRun(runnum0);
  if(bXing>=0 && bXing<120) return blue_spin_map[index][bXing];
  else
  {
    fprintf(stderr,"bXing out of range\n");
    return 0;
  };
};


Int_t RunData::YellSpin(Int_t runnum0, Int_t bXing)
{
  Int_t index = HashRun(runnum0);
  if(bXing>=0 && bXing<120) return yell_spin_map[index][bXing];
  else
  {
    fprintf(stderr,"bXing out of range\n");
    return 0;
  };
};


Bool_t RunData::Kicked(Int_t runnum0, Int_t bXing)
{
  Int_t index = HashRun(runnum0);
  if(bXing>=0 && bXing<120) return kicked_bx_map[index][bXing];
  else
  {
    fprintf(stderr,"bXing out of range\n");
    return 0;
  };
};


Int_t RunData::Pattern(Int_t runnum0)
{
  Int_t fill0 = GetFill(runnum0);
  if(fill0>fill_thou)
    return pattern_map[fill0-fill_thou];
  else return 0;
};
