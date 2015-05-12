void DrawPlots()
{
  gStyle->SetOptStat(0);
  const Int_t N_TRIG=12;
  enum trig_enum {kAll,
    kJP0,kJP1,kJP2,
    kLgBS1,kLgBS2,kLgBS3,
    kSmBS1,kSmBS2,kSmBS3,
    kDiJP,kDiBS};
  char trig_name[N_TRIG][16];
  strcpy(trig_name[kAll],"All");
  strcpy(trig_name[kJP0],"JP0");
  strcpy(trig_name[kJP1],"JP1");
  strcpy(trig_name[kJP2],"JP2");
  strcpy(trig_name[kLgBS1],"LgBS1");
  strcpy(trig_name[kLgBS2],"LgBS2");
  strcpy(trig_name[kLgBS3],"LgBS3");
  strcpy(trig_name[kSmBS1],"SmBS1");
  strcpy(trig_name[kSmBS2],"SmBS2");
  strcpy(trig_name[kSmBS3],"SmBS3");
  strcpy(trig_name[kDiJP],"DiJP");
  strcpy(trig_name[kDiBS],"DiBS");

  const Int_t N_COMP=2;
  char type_str[N_COMP][32];
  strcpy(type_str[0],"before");
  strcpy(type_str[1],"after");
  Color_t colour[N_COMP];
  colour[0]=kBlue;
  colour[1]=kRed;
  Float_t normalization[N_COMP]; // number of runs to normalize by
  normalization[0]=22;
  normalization[1]=8;


  TFile * infile[N_TRIG][N_COMP];
  char infile_n[N_TRIG][N_COMP][256];
  for(Int_t t=0; t<N_TRIG; t++)
  {
    for(Int_t y=0; y<N_COMP; y++)
    {
      sprintf(infile_n[t][y],"%s_%s.root",trig_name[t],type_str[y]);
      infile[t][y] = new TFile(infile_n[t][y],"READ");
    };
  };

  char jtype_str[3][16];
  char jtype_name[3][16];
  strcpy(jtype_str[0],"sph");
  strcpy(jtype_str[1],"pi0");
  strcpy(jtype_str[2],"thr");
  strcpy(jtype_name[0],"N_{#gamma}=1");
  strcpy(jtype_name[1],"#pi^{0}");
  strcpy(jtype_name[2],"N_{#gamma}>2");
  TH1D * pt_wdist[N_TRIG][3][N_COMP];  // [trigger] [jtype] [before/after]
  TH1D * en_wdist[N_TRIG][3][N_COMP];  // [trigger] [jtype] [before/after]
  TH1D * mm_wdist[N_TRIG][3][N_COMP];  // [trigger] [jtype] [before/after]
  char pt_wdist_n[N_TRIG][3][N_COMP][64];
  char en_wdist_n[N_TRIG][3][N_COMP][64];
  char mm_wdist_n[N_TRIG][3][N_COMP][64];
  char pt_wdist_t[N_TRIG][3][N_COMP][64];
  char en_wdist_t[N_TRIG][3][N_COMP][64];
  char mm_wdist_t[N_TRIG][3][N_COMP][64];
  for(Int_t t=0; t<N_TRIG; t++)
  {
    for(Int_t y=0; y<N_COMP; y++)
    {
      for(Int_t j=0; j<3; j++)
      {
        sprintf(pt_wdist_n[t][j][y],"pt_wdist_tot_%s_g0_e0",jtype_str[j]);
        sprintf(en_wdist_n[t][j][y],"en_wdist_tot_%s_g0_p0",jtype_str[j]);
        sprintf(mm_wdist_n[t][j][y],"mm_wdist_tot_%s_g0_p0_e0",jtype_str[j]);
        pt_wdist[t][j][y] = (TH1D*) infile[t][y]->Get(pt_wdist_n[t][j][y]);
        en_wdist[t][j][y] = (TH1D*) infile[t][y]->Get(en_wdist_n[t][j][y]);
        mm_wdist[t][j][y] = (TH1D*) infile[t][y]->Get(mm_wdist_n[t][j][y]);
        //pt_wdist[t][j][y]->Scale(1/normalization[y]);
        //en_wdist[t][j][y]->Scale(1/normalization[y]);
        //mm_wdist[t][j][y]->Scale(1/normalization[y]);
        //pt_wdist[t][j][y]->Scale(1/pt_wdist[t][j][y]->GetMaximum());
        //en_wdist[t][j][y]->Scale(1/en_wdist[t][j][y]->GetMaximum());
        //mm_wdist[t][j][y]->Scale(1/mm_wdist[t][j][y]->GetMaximum());
        sprintf(pt_wdist_t[t][j][y],"%s triggers %s p_{T} distribution",trig_name[t],jtype_name[j]);
        sprintf(en_wdist_t[t][j][y],"%s triggers %s E distribution",trig_name[t],jtype_name[j]);
        sprintf(mm_wdist_t[t][j][y],"%s triggers %s M distribution",trig_name[t],jtype_name[j]);
        pt_wdist[t][j][y]->SetTitle(pt_wdist_t[t][j][y]);
        en_wdist[t][j][y]->SetTitle(en_wdist_t[t][j][y]);
        mm_wdist[t][j][y]->SetTitle(mm_wdist_t[t][j][y]);
        pt_wdist[t][j][y]->SetLineColor(colour[y]);
        en_wdist[t][j][y]->SetLineColor(colour[y]);
        mm_wdist[t][j][y]->SetLineColor(colour[y]);
      };
    };
  };

  TCanvas * canv[3];
  char canv_n[3][16];
  char canv_png[3][32];
  char drawtype[8];
  for(Int_t j=0; j<3; j++)
  {
    sprintf(canv_n[j],"canv_%s",jtype_str[j]);
    sprintf(canv_png[j],"%s.png",canv_n[j]);
    canv[j] = new TCanvas(canv_n[j],canv_n[j],1200,N_TRIG*300);
    canv[j]->Divide(3,N_TRIG);
    for(Int_t k=1; k<=3*N_TRIG; k++) canv[j]->GetPad(k)->SetGrid(1,1);
    for(Int_t t=0; t<N_TRIG; t++)
    {
      for(Int_t y=0; y<N_COMP; y++) 
      {
        if(y==0) strcpy(drawtype,"");
        else strcpy(drawtype,"same");
        canv[j]->cd(t*3+1); pt_wdist[t][j][y]->Draw(drawtype);
        canv[j]->cd(t*3+2); en_wdist[t][j][y]->Draw(drawtype);
        if(j>0) { canv[j]->cd(t*3+3); mm_wdist[t][j][y]->Draw(drawtype);};
      };
    };
    canv[j]->Print(canv_png[j],"png");
  };
};
