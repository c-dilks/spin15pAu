// plots A_N^pA / A_N^pp for selected directory and event class

void PlotPAU(const char * kinvar="en",
             const char * evclass="pi0",
             const char * binselect="any")
{
  gSystem->Load("src/RunData.so");
  EventClass * ev = new EventClass();
  
  // build filenames
  enum pp_enum {ka,kp};
  Int_t k;
  TString fname[2];
  TString asym_name[2];
  TString binselect_n="";
  if(strcmp(binselect,"any")) binselect_n="_"+TString(binselect);
  fname[ka] = "pAu";
  fname[kp] = "pp";
  for(k=0;k<2;k++) 
  {
    asym_name[k] = "A_{N}^{"+fname[k]+"#rightarrow"+TString(ev->Title(evclass))+"+X}";
    fname[k]="asym_plots_"+fname[k]+"/output_All_"+kinvar+binselect_n+"/spin_"+evclass+".root";
  };
  cout << fname[ka] << endl << fname[kp] << endl;

  TString binselect_t = "";
  if(!strcmp(binselect,"lowpt")) binselect_t = " @ low p_{T}";
  else if(!strcmp(binselect,"midpt")) binselect_t = " @ mid p_{T}";
  else if(!strcmp(binselect,"highpt")) binselect_t = " @ high p_{T}";
  else if(!strcmp(binselect,"lowen")) binselect_t = " @ low E";
  else if(!strcmp(binselect,"miden")) binselect_t = " @ mid E";
  else if(!strcmp(binselect,"highen")) binselect_t = " @ high E";
  else
  {
    if(!strcmp(kinvar,"en")) binselect_t = " @ any p_{T}";
    else if(!strcmp(kinvar,"pt")) binselect_t = " @ any E";
  };

  
  // set plot colors
  Int_t farbe[2];
  farbe[ka] = kRed;
  farbe[kp] = kBlue;


  // open files and graphs
  TFile * infile[2];
  TGraphErrors * gr[2];
  TMultiGraph * mg = new TMultiGraph();

  TString gr_n = kinvar;
  if(!strcmp(kinvar,"en")) gr_n+="_dep_z1_a2_g0_p0";
  else if(!strcmp(kinvar,"pt")) gr_n+="_dep_z1_a2_g0_e0";
  else { fprintf(stderr,"ERROR: unrecognised kinvar\n"); return; }; 
  gr_n = "A_N_blue/"+gr_n;
  cout << gr_n << endl;

  for(k=0;k<2;k++) 
  {
    infile[k] = new TFile(fname[k].Data(),"READ");
    infile[k]->cd("A_N_blue");
    gr[k] = (TGraphErrors*) infile[k]->Get(gr_n.Data());
    gr[k]->SetMarkerColor(kBlack);
    gr[k]->SetLineColor(farbe[k]);
    gr[k]->SetLineWidth(2);
    mg->Add(gr[k]);
  };


  TString kinvar_t;
  if(!strcmp(kinvar,"en")) kinvar_t="E";
  else if(!strcmp(kinvar,"pt")) kinvar_t="p_{T}";
  TString gr_t = TString(ev->Title(evclass))+" A_{N} vs. "+
                 kinvar_t+binselect_t+";"+kinvar_t+";A_{N}";
  mg->SetTitle(gr_t.Data());

  
  TLegend * asym_leg = new TLegend(0.1,0.9,0.4,0.75);
  asym_leg->SetTextSize(0.04);
  for(k=0;k<2;k++) asym_leg->AddEntry(gr[k],asym_name[k],"LPE");

  
  TCanvas * asym_canv = new TCanvas("asym_canv","asym_canv",800,800);
  asym_canv->SetGrid(1,1);
  mg->Draw("ape");
  asym_leg->Draw();
  


  // compute ratio 
  TGraphErrors * rpau = new TGraphErrors();
  TString rpau_t = asym_name[ka]+"/"+asym_name[kp]+" vs. "+kinvar_t+binselect_t+
                   ";"+kinvar_t+";"+asym_name[ka]+"/"+asym_name[kp];
  rpau->SetTitle(rpau_t.Data());
  rpau->SetMarkerColor(kBlack);
  rpau->SetMarkerStyle(kFullCircle);
  rpau->SetLineColor(kGreen+2);
  rpau->SetLineWidth(2);
  Double_t x[2];
  Double_t y[2];
  Double_t xe[2];
  Double_t ye[2];
  Double_t rr,bb,rre,bbe;
  if(gr[ka]->GetN() != gr[kp]->GetN()) 
  {
    fprintf(stderr,"ERROR: pA and pp have different number of entries\n");
    return;
  };
  for(Int_t n=0; n<gr[0]->GetN(); n++)
  {
    for(k=0;k<2;k++)
    {
      gr[k]->GetPoint(n,x[k],y[k]);
      xe[k]=gr[k]->GetErrorX(n);
      ye[k]=gr[k]->GetErrorY(n);
    };

    /* ratio */
    if(y[kp]!=0) rr = y[ka] / y[kp];
    else rr=0;

    /* horizontal point position */
    bb = (x[ka]+x[kp])/2.0;

    /* error on ratio */
    rre = rr * sqrt(pow(ye[ka]/y[ka],2) + pow(ye[kp]/y[kp],2)); 

    /* horizontal error bar */
    bbe=0;

    rpau->SetPoint(n,bb,rr);
    rpau->SetPointError(n,bbe,rre);
  };


  TCanvas * rpau_canv = new TCanvas("rpau_canv","rpau_canv",800,800);
  rpau_canv->SetGrid(1,1);
  rpau->Draw("APE");
  rpau->GetYaxis()->SetTitleOffset(1.4);

};

