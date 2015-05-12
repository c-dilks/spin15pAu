// takes kinematic variable and event class and plots the distribution
// of that variable for each run in a 2d histogram; plots are normalised
// to equalise the scale

void HotTowerSearch(char * var="Eta")
{
  const Int_t N_BINS=100;
  gSystem->Load("src/RunData.so");
  RunData * RD = new RunData();
  Trigger * T = new Trigger();
  Environ * env = new Environ();
  EventClass * ev = new EventClass();

  Int_t N_CLASS_tmp = ev->N;
  const Int_t N_CLASS = N_CLASS_tmp;

  TChain * tc = new TChain("str");
  tc->Add("redset/Red*.root");
  //tc->Add("redset/RedOutputset080s1.root");

  Float_t E12,Pt,Eta,Phi,M12,Z,N12;
  Int_t runnum;
  UInt_t L2sum[2];
  tc->SetBranchAddress("runnum",&runnum);
  tc->SetBranchAddress("E12",&E12);
  tc->SetBranchAddress("Pt",&Pt);
  tc->SetBranchAddress("Eta",&Eta);
  tc->SetBranchAddress("Phi",&Phi);
  tc->SetBranchAddress("M12",&M12);
  tc->SetBranchAddress("Z",&Z);
  tc->SetBranchAddress("N12",&N12);
  tc->SetBranchAddress("L2sum",L2sum);


  Float_t bin_low,bin_high;
  Float_t * kinvar;

  if(!strcmp(var,"Eta"))
  {
    bin_low = env->EtaLow;
    bin_high = env->EtaHigh;
    kinvar = &Eta;
  }
  else if(!strcmp(var,"Phi"))
  {
    bin_low = env->PhiLow;
    bin_high = env->PhiHigh;
    kinvar = &Phi;
  }
  else if(!strcmp(var,"Pt"))
  {
    bin_low = env->PtLow;
    bin_high = env->PtHigh;
    kinvar = &Pt;
  }
  else if(!strcmp(var,"E12"))
  {
    bin_low = env->EnLow;
    bin_high = env->EnHigh;
    kinvar = &E12;
  }

  Int_t count=0;
  Int_t runnum_tmp=0;

  TH1D * h[N_CLASS][2000]; // assumes max number of runs
  char h_name[N_CLASS][2000][32];
  for(Int_t i=0; i<tc->GetEntries(); i++)
  {
    tc->GetEntry(i);
    ev->SetKinematics(runnum,E12,Pt,Eta,Phi,M12,Z,N12);
    if(runnum!=runnum_tmp)
    {
      if(runnum_tmp!=0) count++;
      for(Int_t c=0; c<N_CLASS; c++)
      {
        sprintf(h_name[c][count],"h_%d_%s",runnum,ev->Name(c));
        h[c][count] = new TH1D(h_name[c][count],h_name[c][count],N_BINS,bin_low,bin_high);
      };
      printf("%d %d\n",count,runnum);
      runnum_tmp = runnum;
    };
    for(Int_t c=0; c<N_CLASS; c++)
    {
      if(ev->Valid(c) && L2sum[1]&T->Mask(runnum,"All",1))
        h[c][count]->Fill(*kinvar);
    };
  };

  char h2_t[N_CLASS][64];
  char h2_n[N_CLASS][32];
  TH2D * h2[N_CLASS];
  for(Int_t c=0; c<N_CLASS; c++)
  {
    sprintf(h2_t[c],"%s %s vs. run index",ev->Title(c),var);
    sprintf(h2_n[c],"h2_%s",ev->Name(c));
    h2[c] = new TH2D(h2_n[c],h2_t[c],count+1,0,count+1,N_BINS,bin_low,bin_high);
    Float_t binpos;
    Int_t binn;
    for(Int_t k=0; k<count+1; k++)
    {
      h[c][k]->Scale(1/h[c][k]->Integral());
      for(Int_t b=0; b<h[c][k]->GetNbinsX(); b++)
      {
        binpos = h[c][k]->GetBinCenter(b);
        binn = h2[c]->FindBin(k,binpos);
        h2[c]->SetBinContent(binn,h[c][k]->GetBinContent(b));
      };
    };
  };

  TCanvas * canv = new TCanvas("canv","canv",1000,N_CLASS*400);
  gStyle->SetOptStat(0);
  canv->Divide(1,N_CLASS);
  for(Int_t c=0; c<N_CLASS; c++)
  {
    canv->cd(c+1);
    h2[c]->Draw("colz");
  };
};

