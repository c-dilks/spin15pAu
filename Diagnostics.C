// shows the dependence of various kinematics on geometry
// e.g., pt vs. eta
// --> outputs all plots to diag_[trigger].pdf

void Diagnostics(const char * RP_select="")
{
  char outfilename[64];
  if(!strcmp(RP_select,"")) sprintf(outfilename,"diag.root");
  else sprintf(outfilename,"diag_%s.root",RP_select);
  TFile * outfile = new TFile(outfilename,"RECREATE");

  const Int_t NBINS=400; // NUMBER OF BINS
  const Int_t NBINS_VS_RUN=100; // number of bins for variable vs. run index plots
  const Int_t MAXRUNS=500; // arbitrary max number of runs (so we don't waste time counting them...)

  gSystem->Load("src/RunData.so");
  RunData * RD = new RunData();
  Trigger * T = new Trigger();
  Environ * env = new Environ();
  EventClass * ev = new EventClass();
  TCUbits * tcu = new TCUbits();


  // open chain
  TChain * tr = new TChain("str");
  tr->Add("./redset/Red*.root");
  Float_t E12,Pt,Eta,Phi,M12,Z,b_pol,y_pol;
  Bool_t kicked,isConsistent;
  Int_t runnum,bx;
  UInt_t L2sum[2];
  UInt_t lastdsm[8];
  Float_t N12;

  Float_t E12_min,Pt_min,Eta_min,Phi_min;
  Float_t E12_max,Pt_max,Eta_max,Phi_max;
  E12_min=Pt_min=Eta_min=Phi_min=1000;
  E12_max=Pt_max=Eta_max=Phi_max=0;

  tr->SetBranchAddress("runnum",&runnum);
  tr->SetBranchAddress("Bunchid7bit",&bx);
  tr->SetBranchAddress("E12",&E12);
  tr->SetBranchAddress("Pt",&Pt);
  tr->SetBranchAddress("Eta",&Eta);
  tr->SetBranchAddress("Phi",&Phi);
  tr->SetBranchAddress("M12",&M12);
  tr->SetBranchAddress("Z",&Z);
  tr->SetBranchAddress("N12",&N12);
  tr->SetBranchAddress("L2sum",L2sum);
  tr->SetBranchAddress("lastdsm",lastdsm);

  // trigger list
  Int_t N_TRIG_tmp = T->N;
  const Int_t N_TRIG = N_TRIG_tmp;

  // event classes
  Int_t N_CLASS_tmp = ev->N;
  const Int_t N_CLASS = N_CLASS_tmp;


  // get kinematic ranges using the current binning set by Bin_Splitter.C
  Phi_min = env->PhiLow;
  Phi_max = env->PhiHigh;
  Eta_min = env->EtaLow;
  Eta_max = env->EtaHigh;
  E12_min = env->EnLow;
  E12_max = env->EnHigh;
  Pt_min = env->PtLow;
  Pt_max = env->PtHigh;

  
  // define histograms
  TH2D * pt_vs_eta[N_CLASS][N_TRIG];
  TH2D * en_vs_eta[N_CLASS][N_TRIG];
  TH2D * pt_vs_phi[N_CLASS][N_TRIG];
  TH2D * en_vs_phi[N_CLASS][N_TRIG];
  TH2D * eta_vs_phi[N_CLASS][N_TRIG];
  TH2D * pt_vs_en[N_CLASS][N_TRIG];
  TH2D * y_vs_x[N_CLASS][N_TRIG];

  TH2D * z_vs_eta[N_CLASS][N_TRIG];
  TH2D * z_vs_phi[N_CLASS][N_TRIG];
  TH2D * mass_vs_en[N_CLASS][N_TRIG];
  TH2D * mass_vs_pt[N_CLASS][N_TRIG];
  TH1D * mass_dist[N_CLASS][N_TRIG];
  TH1D * z_dist[N_CLASS][N_TRIG];

  TH1D * pt_temp[N_CLASS][N_TRIG];
  TH1D * en_temp[N_CLASS][N_TRIG];
  TH1D * eta_temp[N_CLASS][N_TRIG];
  TH1D * phi_temp[N_CLASS][N_TRIG];
  TH1D * mass_temp[N_CLASS][N_TRIG];
  TH1D * z_temp[N_CLASS][N_TRIG];

  TH2D * pt_vs_run[N_CLASS][N_TRIG];
  TH2D * en_vs_run[N_CLASS][N_TRIG];
  TH2D * eta_vs_run[N_CLASS][N_TRIG];
  TH2D * phi_vs_run[N_CLASS][N_TRIG];
  TH2D * mass_vs_run[N_CLASS][N_TRIG];
  TH2D * z_vs_run[N_CLASS][N_TRIG];

  char pt_vs_eta_n[N_CLASS][N_TRIG][64];
  char en_vs_eta_n[N_CLASS][N_TRIG][64];
  char pt_vs_phi_n[N_CLASS][N_TRIG][64];
  char en_vs_phi_n[N_CLASS][N_TRIG][64];
  char eta_vs_phi_n[N_CLASS][N_TRIG][64];
  char pt_vs_en_n[N_CLASS][N_TRIG][64];
  char y_vs_x_n[N_CLASS][N_TRIG][64];
  char z_vs_eta_n[N_CLASS][N_TRIG][64];
  char z_vs_phi_n[N_CLASS][N_TRIG][64];
  char mass_vs_en_n[N_CLASS][N_TRIG][64];
  char mass_vs_pt_n[N_CLASS][N_TRIG][64];
  char mass_dist_n[N_CLASS][N_TRIG][64];
  char z_dist_n[N_CLASS][N_TRIG][64];

  char pt_vs_eta_t[N_CLASS][N_TRIG][256];
  char en_vs_eta_t[N_CLASS][N_TRIG][256];
  char pt_vs_phi_t[N_CLASS][N_TRIG][256];
  char en_vs_phi_t[N_CLASS][N_TRIG][256];
  char eta_vs_phi_t[N_CLASS][N_TRIG][256];
  char pt_vs_en_t[N_CLASS][N_TRIG][256];
  char y_vs_x_t[N_CLASS][N_TRIG][256];
  char z_vs_eta_t[N_CLASS][N_TRIG][256];
  char z_vs_phi_t[N_CLASS][N_TRIG][256];
  char mass_vs_en_t[N_CLASS][N_TRIG][256];
  char mass_vs_pt_t[N_CLASS][N_TRIG][256];
  char mass_dist_t[N_CLASS][N_TRIG][256];
  char z_dist_t[N_CLASS][N_TRIG][256];

  char pt_temp_n[N_CLASS][N_TRIG][64];
  char en_temp_n[N_CLASS][N_TRIG][64];
  char eta_temp_n[N_CLASS][N_TRIG][64];
  char phi_temp_n[N_CLASS][N_TRIG][64];
  char mass_temp_n[N_CLASS][N_TRIG][64];
  char z_temp_n[N_CLASS][N_TRIG][64];

  char pt_vs_run_n[N_CLASS][N_TRIG][64];
  char en_vs_run_n[N_CLASS][N_TRIG][64];
  char eta_vs_run_n[N_CLASS][N_TRIG][64];
  char phi_vs_run_n[N_CLASS][N_TRIG][64];
  char mass_vs_run_n[N_CLASS][N_TRIG][64];
  char z_vs_run_n[N_CLASS][N_TRIG][64];

  char pt_vs_run_t[N_CLASS][N_TRIG][256];
  char en_vs_run_t[N_CLASS][N_TRIG][256];
  char eta_vs_run_t[N_CLASS][N_TRIG][256];
  char phi_vs_run_t[N_CLASS][N_TRIG][256];
  char mass_vs_run_t[N_CLASS][N_TRIG][256];
  char z_vs_run_t[N_CLASS][N_TRIG][256];

  char RP_select_t[32];
  if(!strcmp(RP_select,"")) strcpy(RP_select_t,"");
  else sprintf(RP_select_t," --- %s RP only",RP_select);

  Float_t low_mass,high_mass;

  for(Int_t t=0; t<N_TRIG; t++)
  {
    for(Int_t c=0; c<N_CLASS; c++)
    {
      low_mass=0; // default
      high_mass=1; // default
      if(c==ev->Idx("thr")) 
      {
        low_mass=0.7; // to match cutoff in EventClass.cxx
        high_mass=6;
      }
      else if(c==ev->Idx("etm")) 
      {
        low_mass=0.3;
      }
      else if(c==ev->Idx("jps"))
      {
        low_mass=0;
        high_mass=6;
      };

      sprintf(pt_vs_eta_n[c][t],"%s_%s_pt_vs_eta",T->Name(t),ev->Name(c));
      sprintf(en_vs_eta_n[c][t],"%s_%s_en_vs_eta",T->Name(t),ev->Name(c));
      sprintf(pt_vs_phi_n[c][t],"%s_%s_pt_vs_phi",T->Name(t),ev->Name(c));
      sprintf(en_vs_phi_n[c][t],"%s_%s_en_vs_phi",T->Name(t),ev->Name(c));
      sprintf(eta_vs_phi_n[c][t],"%s_%s_eta_vs_phi",T->Name(t),ev->Name(c));
      sprintf(pt_vs_en_n[c][t],"%s_%s_pt_vs_en",T->Name(t),ev->Name(c));
      sprintf(y_vs_x_n[c][t],"%s_%s_y_vs_x",T->Name(t),ev->Name(c));
      sprintf(z_vs_eta_n[c][t],"%s_%s_z_vs_eta",T->Name(t),ev->Name(c));
      sprintf(z_vs_phi_n[c][t],"%s_%s_z_vs_phi",T->Name(t),ev->Name(c));
      sprintf(mass_vs_en_n[c][t],"%s_%s_mass_vs_en",T->Name(t),ev->Name(c));
      sprintf(mass_vs_pt_n[c][t],"%s_%s_mass_vs_pt",T->Name(t),ev->Name(c));
      sprintf(mass_dist_n[c][t],"%s_%s_mass_dist",T->Name(t),ev->Name(c));
      sprintf(z_dist_n[c][t],"%s_%s_z_dist",T->Name(t),ev->Name(c));

      sprintf(pt_vs_eta_t[c][t],"%s %s --- p_{T} vs. #eta%s;#eta;p_{T}",
        T->Name(t),ev->Title(c),RP_select_t);
      sprintf(en_vs_eta_t[c][t],"%s %s --- E vs. #eta%s;#eta;E",
        T->Name(t),ev->Title(c),RP_select_t);
      sprintf(pt_vs_phi_t[c][t],"%s %s --- p_{T} vs. #phi%s;#phi;p_{T}",
        T->Name(t),ev->Title(c),RP_select_t);
      sprintf(en_vs_phi_t[c][t],"%s %s --- E vs. #phi%s;#phi;E",
        T->Name(t),ev->Title(c),RP_select_t);
      sprintf(eta_vs_phi_t[c][t],"%s %s --- #eta vs. #phi%s;#phi;#eta",
        T->Name(t),ev->Title(c),RP_select_t);
      sprintf(pt_vs_en_t[c][t],"%s %s --- p_{T} vs. E%s;E;p_{T}",
        T->Name(t),ev->Title(c),RP_select_t);
      sprintf(y_vs_x_t[c][t],"%s %s --- y vs. x%s;x;y",
        T->Name(t),ev->Title(c),RP_select_t);

      sprintf(z_vs_eta_t[c][t],"%s %s --- Z vs. #eta (%s cuts w/o Z-cut)%s;#eta;Z",
        T->Name(t),ev->Title(c),ev->Title(c),RP_select_t);
      sprintf(z_vs_phi_t[c][t],"%s %s --- Z vs. #phi (%s cuts w/o Z-cut)%s;#phi;Z",
        T->Name(t),ev->Title(c),ev->Title(c),RP_select_t);
      sprintf(mass_vs_en_t[c][t],"%s %s --- M vs. E (%s cuts w/o M-cut)%s;E;M",
        T->Name(t),ev->Title(c),ev->Title(c),RP_select_t);
      sprintf(mass_vs_pt_t[c][t],"%s %s --- M vs. p_{T} (%s cuts w/o M-cut)%s;p_{T};M",
        T->Name(t),ev->Title(c),ev->Title(c),RP_select_t);
      sprintf(mass_dist_t[c][t],"%s %s --- M distribution (%s cuts w/o M-cut)%s;M",
        T->Name(t),ev->Title(c),ev->Title(c),RP_select_t);
      sprintf(z_dist_t[c][t],"%s %s --- Z distribution (%s cuts w/o Z-cut)%s;Z",
        T->Name(t),ev->Title(c),ev->Title(c),RP_select_t);

      pt_vs_eta[c][t] = new TH2D(pt_vs_eta_n[c][t],pt_vs_eta_t[c][t],
        NBINS,Eta_min,Eta_max,NBINS,Pt_min,Pt_max);
      en_vs_eta[c][t] = new TH2D(en_vs_eta_n[c][t],en_vs_eta_t[c][t],
        NBINS,Eta_min,Eta_max,NBINS,E12_min,E12_max);
      pt_vs_phi[c][t] = new TH2D(pt_vs_phi_n[c][t],pt_vs_phi_t[c][t],
        NBINS,Phi_min,Phi_max,NBINS,Pt_min,Pt_max);
      en_vs_phi[c][t] = new TH2D(en_vs_phi_n[c][t],en_vs_phi_t[c][t],
        NBINS,Phi_min,Phi_max,NBINS,E12_min,E12_max);
      eta_vs_phi[c][t] = new TH2D(eta_vs_phi_n[c][t],eta_vs_phi_t[c][t],
        NBINS,Phi_min,Phi_max,NBINS,Eta_min,Eta_max);
      pt_vs_en[c][t] = new TH2D(pt_vs_en_n[c][t],pt_vs_en_t[c][t],
        NBINS,E12_min,E12_max,NBINS,Pt_min,Pt_max);
      y_vs_x[c][t] = new TH2D(y_vs_x_n[c][t],y_vs_x_t[c][t],
        NBINS,-110,110,NBINS,-110,110);
      z_vs_eta[c][t] = new TH2D(z_vs_eta_n[c][t],z_vs_eta_t[c][t],
        NBINS,Eta_min,Eta_max,NBINS,0,1);
      z_vs_phi[c][t] = new TH2D(z_vs_phi_n[c][t],z_vs_phi_t[c][t],
        NBINS,Phi_min,Phi_max,NBINS,0,1);
      mass_vs_en[c][t] = new TH2D(mass_vs_en_n[c][t],mass_vs_en_t[c][t],
        NBINS,E12_min,E12_max,NBINS,low_mass,high_mass);
      mass_vs_pt[c][t] = new TH2D(mass_vs_pt_n[c][t],mass_vs_pt_t[c][t],
        NBINS,Pt_min,Pt_max,NBINS,low_mass,high_mass);

      z_dist[c][t] = new TH1D(z_dist_n[c][t],z_dist_t[c][t],NBINS,0,1);
      mass_dist[c][t] = new TH1D(mass_dist_n[c][t],mass_dist_t[c][t],NBINS,low_mass,high_mass);
      printf("VVVVV\n");
      printf("^^^^^\n");

      printf("----- c=%d t=%d\n",c,t);
      sprintf(pt_temp_n[c][t],"%s_%s_pt_temp",T->Name(t),ev->Name(c));
      sprintf(en_temp_n[c][t],"%s_%s_en_temp",T->Name(t),ev->Name(c));
      sprintf(eta_temp_n[c][t],"%s_%s_eta_temp",T->Name(t),ev->Name(c));
      sprintf(phi_temp_n[c][t],"%s_%s_phi_temp",T->Name(t),ev->Name(c));
      sprintf(mass_temp_n[c][t],"%s_%s_mass_temp",T->Name(t),ev->Name(c));
      sprintf(z_temp_n[c][t],"%s_%s_z_temp",T->Name(t),ev->Name(c));

      printf("%s\n",pt_temp_n[c][t]);
      printf("%s\n",en_temp_n[c][t]);
      printf("%s\n",eta_temp_n[c][t]);
      printf("%s\n",phi_temp_n[c][t]);
      printf("%s\n",mass_temp_n[c][t]);
      printf("%s\n",z_temp_n[c][t]);

      sprintf(pt_vs_run_n[c][t],"%s_%s_pt_vs_run",T->Name(t),ev->Name(c));
      sprintf(en_vs_run_n[c][t],"%s_%s_en_vs_run",T->Name(t),ev->Name(c));
      sprintf(eta_vs_run_n[c][t],"%s_%s_eta_vs_run",T->Name(t),ev->Name(c));
      sprintf(phi_vs_run_n[c][t],"%s_%s_phi_vs_run",T->Name(t),ev->Name(c));
      sprintf(mass_vs_run_n[c][t],"%s_%s_mass_vs_run",T->Name(t),ev->Name(c));
      sprintf(z_vs_run_n[c][t],"%s_%s_z_vs_run",T->Name(t),ev->Name(c));

      sprintf(pt_vs_run_t[c][t],"%s %s --- p_{T} vs. run index%s",
        T->Name(t),ev->Title(c),RP_select);
      sprintf(en_vs_run_t[c][t],"%s %s --- E vs. run index%s",
        T->Name(t),ev->Title(c),RP_select);
      sprintf(eta_vs_run_t[c][t],"%s %s --- #eta vs. run index%s",
        T->Name(t),ev->Title(c),RP_select);
      sprintf(phi_vs_run_t[c][t],"%s %s --- #phi vs. run index%s",
        T->Name(t),ev->Title(c),RP_select);
      sprintf(mass_vs_run_t[c][t],"%s %s --- M vs. run index%s",
        T->Name(t),ev->Title(c),RP_select);
      sprintf(z_vs_run_t[c][t],"%s %s --- Z vs. run index%s",
        T->Name(t),ev->Title(c),RP_select);

      pt_temp[c][t] = new TH1D(pt_temp_n[c][t],pt_temp_n[c][t],
        NBINS_VS_RUN,Pt_min,Pt_max);
      en_temp[c][t] = new TH1D(en_temp_n[c][t],en_temp_n[c][t],
        NBINS_VS_RUN,E12_min,E12_max);
      eta_temp[c][t] = new TH1D(eta_temp_n[c][t],eta_temp_n[c][t],
        NBINS_VS_RUN,Eta_min,Eta_max);
      phi_temp[c][t] = new TH1D(phi_temp_n[c][t],phi_temp_n[c][t],
        NBINS_VS_RUN,Phi_min,Phi_max);
      mass_temp[c][t] = new TH1D(mass_temp_n[c][t],mass_temp_n[c][t],
        NBINS_VS_RUN,low_mass,high_mass);
      z_temp[c][t] = new TH1D(z_temp_n[c][t],z_temp_n[c][t],
        NBINS_VS_RUN,0,1);

      pt_vs_run[c][t] = new TH2D(pt_vs_run_n[c][t],pt_vs_run_t[c][t],
        MAXRUNS,0,MAXRUNS,NBINS_VS_RUN,Pt_min,Pt_max);
      en_vs_run[c][t] = new TH2D(en_vs_run_n[c][t],en_vs_run_t[c][t],
        MAXRUNS,0,MAXRUNS,NBINS_VS_RUN,E12_min,E12_max);
      eta_vs_run[c][t] = new TH2D(eta_vs_run_n[c][t],eta_vs_run_t[c][t],
        MAXRUNS,0,MAXRUNS,NBINS_VS_RUN,Eta_min,Eta_max);
      phi_vs_run[c][t] = new TH2D(phi_vs_run_n[c][t],phi_vs_run_t[c][t],
        MAXRUNS,0,MAXRUNS,NBINS_VS_RUN,Phi_min,Phi_max);
      mass_vs_run[c][t] = new TH2D(mass_vs_run_n[c][t],mass_vs_run_t[c][t],
        MAXRUNS,0,MAXRUNS,NBINS_VS_RUN,low_mass,high_mass);
      z_vs_run[c][t] = new TH2D(z_vs_run_n[c][t],z_vs_run_t[c][t],
        MAXRUNS,0,MAXRUNS,NBINS_VS_RUN,0,1);
    };
  };


  // energy-dependent mass distributions for pions 
  TH1D * mass_dist_for_enbin[N_TRIG][10];
  char mass_dist_for_enbin_n[N_TRIG][10][64];
  char mass_dist_for_enbin_t[N_TRIG][10][256];
  for(Int_t t=0; t<N_TRIG; t++)
  {
    for(Int_t ee=0; ee<10; ee++)
    {
      sprintf(mass_dist_for_enbin_n[t][ee],"%s_mass_dist_for_enbin%d",T->Name(t),ee);
      sprintf(mass_dist_for_enbin_t[t][ee],
        "%s M_{#gamma#gamma} distribution for E_{#gamma#gamma}#in[%d,%d) GeV",
        T->Name(t),ee*10,(ee+1)*10);
      mass_dist_for_enbin[t][ee] = 
        new TH1D(mass_dist_for_enbin_n[t][ee],mass_dist_for_enbin_t[t][ee],NBINS,0,1);
    };
  };

  // pt-dependent mass distributions for pions
  TH1D * mass_dist_for_ptbin[N_TRIG][10];
  char mass_dist_for_ptbin_n[N_TRIG][10][64];
  char mass_dist_for_ptbin_t[N_TRIG][10][256];
  for(Int_t t=0; t<N_TRIG; t++)
  {
    for(Int_t pp=0; pp<10; pp++)
    {
      sprintf(mass_dist_for_ptbin_n[t][pp],"%s_mass_dist_for_ptbin%d",T->Name(t),pp);
      sprintf(mass_dist_for_ptbin_t[t][pp],
        "%s M_{#gamma#gamma} distribution for p_{T}#in[%d,%d) GeV/c",
        T->Name(t),pp,pp+1);
      mass_dist_for_ptbin[t][pp] = new TH1D(mass_dist_for_ptbin_n[t][pp],mass_dist_for_ptbin_t[t][pp],NBINS,0,1);
    };
  };

  // trigger distribution
  char trig_dist_t[64];
  sprintf(trig_dist_t,"Trigger Counts%s",RP_select_t);
  TH1D * trig_dist = new TH1D("trig_dist",trig_dist_t,N_TRIG,0,N_TRIG);
  for(Int_t t=0; t<N_TRIG; t++) trig_dist->GetXaxis()->SetBinLabel(t+1,T->Name(t));

  // trigger mix for FMS and RP
  Int_t N_RP_tmp = tcu->NRP;
  const Int_t N_RP = N_RP_tmp;

  TH2D * trig_fms_mix[N_CLASS]; // overlap matrix of FMS triggers
  TH2D * trig_rp_mix[N_CLASS]; // overlap matrix of RP triggers
  TH2D * trig_fmsrp_mix[N_CLASS]; // overlap matrix of FMS vs. RP triggers
                                  // (with extra column "n/a" for no RP trigger demand
                                 
  char trig_fms_mix_n[N_CLASS][64];
  char trig_rp_mix_n[N_CLASS][64];
  char trig_fmsrp_mix_n[N_CLASS][64];
  char trig_fms_mix_t[N_CLASS][256];
  char trig_rp_mix_t[N_CLASS][256];
  char trig_fmsrp_mix_t[N_CLASS][256];

  for(Int_t c=0; c<N_CLASS; c++)
  {
    sprintf(trig_fms_mix_n[c],"%s_trig_fms_mix",ev->Name(c));
    sprintf(trig_rp_mix_n[c],"%s_trig_rp_mix",ev->Name(c));
    sprintf(trig_fmsrp_mix_n[c],"%s_trig_fmsrp_mix",ev->Name(c));

    sprintf(trig_fms_mix_t[c],"%s FMS trigger overlap matrix",ev->Title(c));
    sprintf(trig_rp_mix_t[c],"%s RP trigger overlap matrix (using only OR of FMS triggers)",ev->Title(c));
    sprintf(trig_fmsrp_mix_t[c],"%s RP-FMS trigger overlap matrix",ev->Title(c));

    trig_fms_mix[c] = new TH2D(trig_fms_mix_n[c],trig_fms_mix_t[c],
      N_TRIG,0,N_TRIG,N_TRIG,0,N_TRIG);
    trig_rp_mix[c] = new TH2D(trig_rp_mix_n[c],trig_rp_mix_t[c],
      N_RP,0,N_RP,N_RP,0,N_RP);
    trig_fmsrp_mix[c] = new TH2D(trig_fmsrp_mix_n[c],trig_fmsrp_mix_t[c],
      N_RP+1,0,N_RP+1,N_TRIG,0,N_TRIG);

    for(Int_t t=0; t<N_TRIG; t++) 
    {
      trig_fms_mix[c]->GetXaxis()->SetBinLabel(t+1,T->Name(t));
      trig_fms_mix[c]->GetYaxis()->SetBinLabel(t+1,T->Name(t));
      trig_fmsrp_mix[c]->GetYaxis()->SetBinLabel(t+1,T->Name(t));
    };
    for(Int_t t=0; t<N_RP; t++)
    {
      trig_rp_mix[c]->GetXaxis()->SetBinLabel(t+1,tcu->RPname(t));
      trig_rp_mix[c]->GetYaxis()->SetBinLabel(t+1,tcu->RPname(t));
      trig_fmsrp_mix[c]->GetXaxis()->SetBinLabel(t+1,tcu->RPname(t));
    };
    trig_fmsrp_mix[c]->GetXaxis()->SetBinLabel(N_RP+1,"n/a");
  };


  // fill histograms
  Int_t runnum_tmp=0;
  Int_t runcount=0;
  Double_t pt_bc,en_bc,eta_bc,phi_bc,z_bc,mass_bc;
  Int_t pt_bn,en_bn,eta_bn,phi_bn,z_bn,mass_bn;

  Int_t ENT = tr->GetEntries();
  //ENT = 100000; // uncomment to do a short loop for testing
  if(!strcmp(RP_select,"")) system("touch diag_run_table.dat; rm diag_run_table.dat");
  for(Int_t x=0; x<ENT; x++)
  {
    if((x%100000)==0) printf("filling histograms: %.2f%%\n",100*((Float_t)x)/((Float_t)ENT));
    tr->GetEntry(x);
    kicked = RD->Kicked(runnum,bx);

    // get new polarisation and check rellum
    // also fill kinematic vs. run plots
    if(runnum!=runnum_tmp || x+1==ENT)
    {
      b_pol = RD->BluePol(runnum);
      y_pol = RD->YellPol(runnum);
      isConsistent = RD->RellumConsistent(runnum);

      if(runnum_tmp!=0) runcount++;
      if(runcount>0)
      {
        for(Int_t t=0; t<N_TRIG; t++)
        {
          for(Int_t c=0; c<N_CLASS; c++)
          {
            pt_temp[c][t]->Scale(1/pt_temp[c][t]->Integral());
            en_temp[c][t]->Scale(1/en_temp[c][t]->Integral());
            eta_temp[c][t]->Scale(1/eta_temp[c][t]->Integral());
            phi_temp[c][t]->Scale(1/phi_temp[c][t]->Integral());
            z_temp[c][t]->Scale(1/z_temp[c][t]->Integral());
            mass_temp[c][t]->Scale(1/mass_temp[c][t]->Integral());
            for(Int_t b=1; b<NBINS_VS_RUN; b++)
            {
              pt_bc = pt_temp[c][t]->GetBinCenter(b);
              en_bc = en_temp[c][t]->GetBinCenter(b);
              eta_bc = eta_temp[c][t]->GetBinCenter(b);
              phi_bc = phi_temp[c][t]->GetBinCenter(b);
              z_bc = z_temp[c][t]->GetBinCenter(b);
              mass_bc = mass_temp[c][t]->GetBinCenter(b);

              pt_bn = pt_vs_run[c][t]->FindBin(runcount,pt_bc);
              en_bn = en_vs_run[c][t]->FindBin(runcount,en_bc);
              eta_bn = eta_vs_run[c][t]->FindBin(runcount,eta_bc);
              phi_bn = phi_vs_run[c][t]->FindBin(runcount,phi_bc);
              z_bn = z_vs_run[c][t]->FindBin(runcount,z_bc);
              mass_bn = mass_vs_run[c][t]->FindBin(runcount,mass_bc);

              pt_vs_run[c][t]->SetBinContent(pt_bn,pt_temp[c][t]->GetBinContent(b));
              en_vs_run[c][t]->SetBinContent(en_bn,en_temp[c][t]->GetBinContent(b));
              eta_vs_run[c][t]->SetBinContent(eta_bn,eta_temp[c][t]->GetBinContent(b));
              phi_vs_run[c][t]->SetBinContent(phi_bn,phi_temp[c][t]->GetBinContent(b));
              z_vs_run[c][t]->SetBinContent(z_bn,z_temp[c][t]->GetBinContent(b));
              mass_vs_run[c][t]->SetBinContent(mass_bn,mass_temp[c][t]->GetBinContent(b));

              //printf("%s %s -- pt_bn=%d runcount=%d pt_bc=%f content=%f\n",
                //T->Name(t),ev->Name(c),pt_bn,runcount,pt_bc,pt_temp[c][t]->GetBinContent(b));

            };
            pt_temp[c][t]->Reset();
            en_temp[c][t]->Reset();
            eta_temp[c][t]->Reset();
            phi_temp[c][t]->Reset();
            z_temp[c][t]->Reset();
            mass_temp[c][t]->Reset();
          };
        };

        printf("%d -----------------------\n",runnum_tmp);
        if(!strcmp(RP_select,""))
        {
          gSystem->RedirectOutput("diag_run_table.dat","a");
          printf("%d %d\n",runcount,runnum_tmp);
          gSystem->RedirectOutput(0);
        };
      };
      runnum_tmp=runnum;
    }

    // rellum / pol cut
    if( kicked==0 && isConsistent==1 && b_pol>0 && y_pol>0)
    {
      // fill trigger plot
      for(Int_t t=0; t<N_TRIG; t++)
      {
        if(L2sum[1] & T->Mask(runnum,t,1) &&
           (!strcmp(RP_select,"") || tcu->FiredRP(RP_select)))
          trig_dist->Fill(t);
      };

      // below here we have if statements for each event class; within each
      // if block, we have trigger loops, since it's more efficient to first pick the event and 
      // then loop through all triggers

      ev->SetKinematics(runnum,E12,Pt,Eta,Phi,M12,Z,N12);
      tcu->SetBits(lastdsm);
      for(Int_t c=0; c<N_CLASS; c++)
      {
        // fill main kinematic correlation plots
        if(ev->Valid(c))
        {
          ev->FiducialGeom(Eta,Phi,0); // compute x and y coordinates
          for(Int_t t=0; t<N_TRIG; t++)
          {
            if(L2sum[1] & T->Mask(runnum,t,1))
            {
              if(!strcmp(RP_select,"") || tcu->FiredRP(RP_select))
              {
                pt_vs_eta[c][t]->Fill(Eta,Pt);
                en_vs_eta[c][t]->Fill(Eta,E12);
                pt_vs_phi[c][t]->Fill(Phi,Pt);
                en_vs_phi[c][t]->Fill(Phi,E12);
                eta_vs_phi[c][t]->Fill(Phi,Eta);
                pt_vs_en[c][t]->Fill(E12,Pt);
                y_vs_x[c][t]->Fill(ev->Xd,ev->Yd);

                pt_temp[c][t]->Fill(Pt);
                en_temp[c][t]->Fill(E12);
                eta_temp[c][t]->Fill(Eta);
                phi_temp[c][t]->Fill(Phi);
              };

              // fill trigger overlap matrices
              for(Int_t tt=0; tt<N_TRIG; tt++)
              {
                if(L2sum[1] & T->Mask(runnum,tt,1))
                  trig_fms_mix[c]->Fill(t,tt);
              };
              for(Int_t r=0; r<N_RP; r++)
              {
                if(tcu->FiredRP(r))
                {
                  trig_fmsrp_mix[c]->Fill(r,t);

                  // only fill trig_rp_mix for OR of FMS triggers
                  if(!strcmp(T->Name(t),"All"))
                  {
                    for(Int_t rr=0; rr<N_RP; rr++)
                    {
                      if(tcu->FiredRP(rr)) trig_rp_mix[c]->Fill(r,rr);
                    };
                  };
                };
              };
              trig_fmsrp_mix[c]->Fill(N_RP,t); // fill "n/a" column
            };
          };
        };

        // fill mass plots
        if(c!=ev->Idx("sph") && ev->ValidWithoutMcut(c))
        {
          for(Int_t t=0; t<N_TRIG; t++)
          {
            if(L2sum[1] & T->Mask(runnum,t,1) &&
               (!strcmp(RP_select,"") || tcu->FiredRP(RP_select)))
            {
              mass_dist[c][t]->Fill(M12);
              mass_vs_en[c][t]->Fill(E12,M12);
              mass_vs_pt[c][t]->Fill(Pt,M12);
              mass_temp[c][t]->Fill(M12);
              // fill kinematic-dependent mass distributions for pions
              if(c==ev->Idx("pi0"))
              {
                for(Int_t ee=0; ee<10; ee++)
                {
                  if(E12>=(ee*10) && E12<((ee+1)*10)) mass_dist_for_enbin[t][ee]->Fill(M12);
                };
                for(Int_t pp=0; pp<10; pp++)
                {
                  if(Pt>=pp && Pt<(pp+1)) mass_dist_for_ptbin[t][pp]->Fill(M12);
                };
              };
            };
          };
        };

        // fill z plots
        if((c==ev->Idx("pi0") || c==ev->Idx("etm") || c==ev->Idx("jps")) && ev->ValidWithoutZcut(c))
        {
          for(Int_t t=0; t<N_TRIG; t++)
          {
            if(L2sum[1] & T->Mask(runnum,t,1) &&
               (!strcmp(RP_select,"") || tcu->FiredRP(RP_select)))
            {
              z_vs_eta[c][t]->Fill(Eta,Z);
              z_vs_phi[c][t]->Fill(Phi,Z);
              z_dist[c][t]->Fill(Z);
              z_temp[c][t]->Fill(Z);
            };
          };
        };
      };
    };
  };


  // build TObjArrays
  TObjArray * pt_vs_eta_arr[N_CLASS];
  TObjArray * en_vs_eta_arr[N_CLASS];
  TObjArray * pt_vs_phi_arr[N_CLASS];
  TObjArray * en_vs_phi_arr[N_CLASS];
  TObjArray * eta_vs_phi_arr[N_CLASS];
  TObjArray * pt_vs_en_arr[N_CLASS];
  TObjArray * y_vs_x_arr[N_CLASS];
  TObjArray * z_vs_eta_arr[N_CLASS];
  TObjArray * z_vs_phi_arr[N_CLASS];
  TObjArray * mass_vs_en_arr[N_CLASS];
  TObjArray * mass_vs_pt_arr[N_CLASS];
  TObjArray * mass_dist_arr[N_CLASS];
  TObjArray * z_dist_arr[N_CLASS];
  TObjArray * mass_dist_for_enbin_arr[10];
  TObjArray * mass_dist_for_ptbin_arr[10];

  TObjArray * pt_vs_run_arr[N_CLASS];
  TObjArray * en_vs_run_arr[N_CLASS];
  TObjArray * eta_vs_run_arr[N_CLASS];
  TObjArray * phi_vs_run_arr[N_CLASS];
  TObjArray * z_vs_run_arr[N_CLASS];
  TObjArray * mass_vs_run_arr[N_CLASS];

  char pt_vs_eta_arr_n[N_CLASS][32];
  char en_vs_eta_arr_n[N_CLASS][32];
  char pt_vs_phi_arr_n[N_CLASS][32];
  char en_vs_phi_arr_n[N_CLASS][32];
  char eta_vs_phi_arr_n[N_CLASS][32];
  char pt_vs_en_arr_n[N_CLASS][32];
  char y_vs_x_arr_n[N_CLASS][32];
  char z_vs_eta_arr_n[N_CLASS][32];
  char z_vs_phi_arr_n[N_CLASS][32];
  char mass_vs_en_arr_n[N_CLASS][32];
  char mass_vs_pt_arr_n[N_CLASS][32];
  char mass_dist_arr_n[N_CLASS][32];
  char z_dist_arr_n[N_CLASS][32];
  char mass_dist_for_enbin_arr_n[10][32];
  char mass_dist_for_ptbin_arr_n[10][32];
  char pt_vs_run_arr_n[N_CLASS][32];
  char en_vs_run_arr_n[N_CLASS][32];
  char eta_vs_run_arr_n[N_CLASS][32];
  char phi_vs_run_arr_n[N_CLASS][32];
  char z_vs_run_arr_n[N_CLASS][32];
  char mass_vs_run_arr_n[N_CLASS][32];

  for(Int_t c=0; c<N_CLASS; c++)
  {
    sprintf(pt_vs_eta_arr_n[c],"%s_pt_vs_eta_arr",ev->Name(c));
    sprintf(en_vs_eta_arr_n[c],"%s_en_vs_eta_arr",ev->Name(c));
    sprintf(pt_vs_phi_arr_n[c],"%s_pt_vs_phi_arr",ev->Name(c));
    sprintf(en_vs_phi_arr_n[c],"%s_en_vs_phi_arr",ev->Name(c));
    sprintf(eta_vs_phi_arr_n[c],"%s_eta_vs_phi_arr",ev->Name(c));
    sprintf(pt_vs_en_arr_n[c],"%s_pt_vs_en_arr",ev->Name(c));
    sprintf(y_vs_x_arr_n[c],"%s_y_vs_x_arr",ev->Name(c));
    sprintf(z_vs_eta_arr_n[c],"%s_z_vs_eta_arr",ev->Name(c));
    sprintf(z_vs_phi_arr_n[c],"%s_z_vs_phi_arr",ev->Name(c));
    sprintf(mass_vs_en_arr_n[c],"%s_mass_vs_en_arr",ev->Name(c));
    sprintf(mass_vs_pt_arr_n[c],"%s_mass_vs_pt_arr",ev->Name(c));
    sprintf(mass_dist_arr_n[c],"%s_mass_dist_arr",ev->Name(c));
    sprintf(z_dist_arr_n[c],"%s_z_dist_arr",ev->Name(c));
    sprintf(pt_vs_run_arr_n[c],"%s_pt_vs_run_arr",ev->Name(c));
    sprintf(en_vs_run_arr_n[c],"%s_en_vs_run_arr",ev->Name(c));
    sprintf(eta_vs_run_arr_n[c],"%s_eta_vs_run_arr",ev->Name(c));
    sprintf(phi_vs_run_arr_n[c],"%s_phi_vs_run_arr",ev->Name(c));
    sprintf(z_vs_run_arr_n[c],"%s_z_vs_run_arr",ev->Name(c));
    sprintf(mass_vs_run_arr_n[c],"%s_mass_vs_run_arr",ev->Name(c));
    pt_vs_eta_arr[c] = new TObjArray();
    en_vs_eta_arr[c] = new TObjArray();
    pt_vs_phi_arr[c] = new TObjArray();
    en_vs_phi_arr[c] = new TObjArray();
    eta_vs_phi_arr[c] = new TObjArray();
    pt_vs_en_arr[c] = new TObjArray();
    y_vs_x_arr[c] = new TObjArray();
    z_vs_eta_arr[c] = new TObjArray();
    z_vs_phi_arr[c] = new TObjArray();
    mass_vs_en_arr[c] = new TObjArray();
    mass_vs_pt_arr[c] = new TObjArray();
    mass_dist_arr[c] = new TObjArray();
    z_dist_arr[c] = new TObjArray();
    pt_vs_run_arr[c] = new TObjArray();
    en_vs_run_arr[c] = new TObjArray();
    eta_vs_run_arr[c] = new TObjArray();
    phi_vs_run_arr[c] = new TObjArray();
    z_vs_run_arr[c] = new TObjArray();
    mass_vs_run_arr[c] = new TObjArray();
  };
  for(Int_t k=0; k<10; k++)
  {
    sprintf(mass_dist_for_enbin_arr_n[k],"mass_dist_for_enbin_%d_arr",k);
    sprintf(mass_dist_for_ptbin_arr_n[k],"mass_dist_for_ptbin_%d_arr",k);
    mass_dist_for_enbin_arr[k] = new TObjArray();
    mass_dist_for_ptbin_arr[k] = new TObjArray();
  };

  for(Int_t t=0; t<N_TRIG; t++)
  {
    for(Int_t c=0; c<N_CLASS; c++)
    {
      pt_vs_eta_arr[c]->AddLast(pt_vs_eta[c][t]);
      en_vs_eta_arr[c]->AddLast(en_vs_eta[c][t]);
      pt_vs_phi_arr[c]->AddLast(pt_vs_phi[c][t]);
      en_vs_phi_arr[c]->AddLast(en_vs_phi[c][t]);
      eta_vs_phi_arr[c]->AddLast(eta_vs_phi[c][t]);
      pt_vs_en_arr[c]->AddLast(pt_vs_en[c][t]);
      y_vs_x_arr[c]->AddLast(y_vs_x[c][t]);
      z_vs_eta_arr[c]->AddLast(z_vs_eta[c][t]);
      z_vs_phi_arr[c]->AddLast(z_vs_phi[c][t]);
      mass_vs_en_arr[c]->AddLast(mass_vs_en[c][t]);
      mass_vs_pt_arr[c]->AddLast(mass_vs_pt[c][t]);
      mass_dist_arr[c]->AddLast(mass_dist[c][t]);
      z_dist_arr[c]->AddLast(z_dist[c][t]);
      pt_vs_run_arr[c]->AddLast(pt_vs_run[c][t]);
      en_vs_run_arr[c]->AddLast(en_vs_run[c][t]);
      eta_vs_run_arr[c]->AddLast(eta_vs_run[c][t]);
      phi_vs_run_arr[c]->AddLast(phi_vs_run[c][t]);
      z_vs_run_arr[c]->AddLast(z_vs_run[c][t]);
      mass_vs_run_arr[c]->AddLast(mass_vs_run[c][t]);
    };
    for(Int_t k=0; k<10; k++)
    {
      mass_dist_for_enbin_arr[k]->AddLast(mass_dist_for_enbin[t][k]);
      mass_dist_for_ptbin_arr[k]->AddLast(mass_dist_for_ptbin[t][k]);
    };
  };


  // write output
  outfile->cd();
  trig_dist->Write();
  outfile->mkdir("overlap_matrices");
  outfile->cd("overlap_matrices");
  for(Int_t c=0; c<N_CLASS; c++) trig_fms_mix[c]->Write();
  for(Int_t c=0; c<N_CLASS; c++) trig_rp_mix[c]->Write();
  for(Int_t c=0; c<N_CLASS; c++) trig_fmsrp_mix[c]->Write();
  outfile->cd();
  outfile->mkdir("hot_tower_search");
  outfile->cd("hot_tower_search");
  for(Int_t c=0; c<N_CLASS; c++)
  {
    pt_vs_run_arr[c]->Write(pt_vs_run_arr_n[c],TObject::kSingleKey);
    en_vs_run_arr[c]->Write(en_vs_run_arr_n[c],TObject::kSingleKey);
    eta_vs_run_arr[c]->Write(eta_vs_run_arr_n[c],TObject::kSingleKey);
    phi_vs_run_arr[c]->Write(phi_vs_run_arr_n[c],TObject::kSingleKey);
    z_vs_run_arr[c]->Write(z_vs_run_arr_n[c],TObject::kSingleKey);
    mass_vs_run_arr[c]->Write(mass_vs_run_arr_n[c],TObject::kSingleKey);
  };
  outfile->cd();
  for(Int_t c=0; c<N_CLASS; c++)
  {
    pt_vs_eta_arr[c]->Write(pt_vs_eta_arr_n[c],TObject::kSingleKey);
    en_vs_eta_arr[c]->Write(en_vs_eta_arr_n[c],TObject::kSingleKey);
    pt_vs_phi_arr[c]->Write(pt_vs_phi_arr_n[c],TObject::kSingleKey);
    en_vs_phi_arr[c]->Write(en_vs_phi_arr_n[c],TObject::kSingleKey);
    eta_vs_phi_arr[c]->Write(eta_vs_phi_arr_n[c],TObject::kSingleKey);
    pt_vs_en_arr[c]->Write(pt_vs_en_arr_n[c],TObject::kSingleKey);
    y_vs_x_arr[c]->Write(y_vs_x_arr_n[c],TObject::kSingleKey);
    z_vs_eta_arr[c]->Write(z_vs_eta_arr_n[c],TObject::kSingleKey);
    z_vs_phi_arr[c]->Write(z_vs_phi_arr_n[c],TObject::kSingleKey);
    mass_vs_en_arr[c]->Write(mass_vs_en_arr_n[c],TObject::kSingleKey);
    mass_vs_pt_arr[c]->Write(mass_vs_pt_arr_n[c],TObject::kSingleKey);
    mass_dist_arr[c]->Write(mass_dist_arr_n[c],TObject::kSingleKey);
    z_dist_arr[c]->Write(z_dist_arr_n[c],TObject::kSingleKey);
  };
  for(Int_t k=0; k<10; k++)
  {
    mass_dist_for_enbin_arr[k]->Write(mass_dist_for_enbin_arr_n[k],TObject::kSingleKey);
  };
  for(Int_t k=0; k<10; k++)
  {
    mass_dist_for_ptbin_arr[k]->Write(mass_dist_for_ptbin_arr_n[k],TObject::kSingleKey);
  };
};
