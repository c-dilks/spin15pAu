// compares RP trigger dependent asymmetries
//
// asym: A_Sigma, R_blue, R_yellow
//       A_TT, A_N_blue, A_N_yellow

void CompareRPasym(char * kinvar="en", char * evclass="pi0",char * asym="A_N_blue")
{
  const Int_t NRP = 9;
  enum rp_enum {kAll,kET,kIT,kSDE,kSDW,kEOR,kWOR,kEXOR,kWXOR};

  // draw switch
  Bool_t draw[NRP];
  draw[kAll] = 1;

  draw[kET] = 1;
  draw[kIT] = 1;

  draw[kSDE] = 0;
  draw[kSDW] = 0;

  draw[kEOR] = 0;
  draw[kWOR] = 0;

  draw[kEXOR] = 1;
  draw[kWXOR] = 1;

  /////////////////////////

  char rp_name[NRP][16];
  strcpy(rp_name[kAll],"all");
  strcpy(rp_name[kET],"ET");
  strcpy(rp_name[kIT],"IT");
  strcpy(rp_name[kSDE],"SDE");
  strcpy(rp_name[kSDW],"SDW");
  strcpy(rp_name[kEOR],"EOR");
  strcpy(rp_name[kWOR],"WOR");
  strcpy(rp_name[kEXOR],"EXOR");
  strcpy(rp_name[kWXOR],"WXOR");

  Int_t rp_plot_color[NRP];
  rp_plot_color[kAll] = (Int_t) kRed;
  rp_plot_color[kET] = (Int_t) kOrange;
  rp_plot_color[kIT] = (Int_t) kCyan+2;
  rp_plot_color[kSDE] = (Int_t) kGreen+2;
  rp_plot_color[kSDW] = (Int_t) kBlue;
  rp_plot_color[kEOR] = (Int_t) kGreen+2;
  rp_plot_color[kWOR] = (Int_t) kBlue;
  rp_plot_color[kEXOR] = (Int_t) kGreen+2;
  rp_plot_color[kWXOR] = (Int_t) kBlue;

  Int_t A,Z;
  if(!strcmp(asym,"A_Sigma")) { Z=0; A=3; }
  else if(!strcmp(asym,"R_blue")) { Z=0; A=2; }
  else if(!strcmp(asym,"R_yellow")) { Z=0; A=1; }
  else if(!strcmp(asym,"A_TT")) { Z=1; A=3; }
  else if(!strcmp(asym,"A_N_blue")) { Z=1; A=2; }
  else if(!strcmp(asym,"A_N_yellow")) { Z=1; A=1; }
  else
  {
    fprintf(stderr,"ERROR: asym directory not valid\n");
    return;
  };
  
  char gr_n[32];
  if(!strcmp(kinvar,"en")) sprintf(gr_n,"%s/en_dep_z%d_a%d_g0_p0",asym,Z,A);
  else if(!strcmp(kinvar,"pt")) sprintf(gr_n,"%s/pt_dep_z%d_a%d_g0_e0",asym,Z,A);
  else 
  {
    fprintf(stderr,"ERROR: kinvar not valid\n");
    return;
  };

  TFile * infile[NRP]; 
  char infile_n[NRP][256];
  TGraphErrors * gr[NRP];
  TMultiGraph * multi_gr = new TMultiGraph();
  char multi_gr_title[64];
  sprintf(multi_gr_title,"%s %s vs. %s",evclass,asym,kinvar);
  multi_gr->SetTitle(multi_gr_title);
  TLegend * leg = new TLegend(0.1,0.9,0.2,0.6);
  for(Int_t n=0; n<NRP; n++)
  {
    sprintf(infile_n[n],"output_%s/spin_%s.root",rp_name[n],evclass);
    printf("%s\n",infile_n[n]);
    infile[n] = new TFile(infile_n[n],"READ");
    //infile[n]->cd(asym);
    gr[n] = (TGraphErrors*) infile[n]->Get(gr_n);
    printf("infile @ %p   gr[%d] @ %p\n",(void*)infile[n],n,(void*)gr[n]);
    gr[n]->SetMarkerColor(rp_plot_color[n]);
    gr[n]->SetLineColor(rp_plot_color[n]);
    gr[n]->SetMarkerSize(1.3);
    gr[n]->SetMarkerStyle(kFullCircle);
    gr[n]->SetLineWidth(2);
    if(draw[n])
    {
      leg->AddEntry(gr[n],rp_name[n],"LPE");
      multi_gr->Add(gr[n]);
    };
  };

  TCanvas * canv = new TCanvas("canv","canv",800,800);
  canv->SetGrid(1,1);
  multi_gr->Draw("ape");
  leg->Draw();
};
