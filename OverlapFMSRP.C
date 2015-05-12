// takes overlap matrices in diag.root and divides each overlap amount
// by the total number of RP-unbiased FMS trigger

void OverlapFMSRP(const char * filename="diag.root")
{
  TFile * infile = new TFile(filename,"READ");
  gSystem->Load("src/RunData.so");
  EventClass * E = new EventClass();
  Int_t N_CLASS_tmp = E->N;
  const Int_t N_CLASS = N_CLASS_tmp;

  Int_t NX,NY;
  Int_t xp,yp,zp;
  Int_t bn;
  Double_t bc;
  TH2D * mx_in[N_CLASS];
  TH2D * mx_out[N_CLASS];
  TString mx_in_name[N_CLASS];
  TString mx_get_name[N_CLASS];
  TString mx_out_name[N_CLASS];
  for(Int_t c=0; c<N_CLASS; c++)
  {
    mx_in_name[c] = Form("%s_trig_fmsrp_mix",E->Name(c));
    mx_out_name[c] = mx_in_name[c]+"_frac";
    mx_get_name[c] = "overlap_matrices/"+mx_in_name[c];
    mx_in[c] = (TH2D*) infile->Get(mx_get_name[c].Data());

    // assume the right most column is the RP-unbiased FMS counts
    // ... but there is underflow and overflow (picture a frame of 
    // bins around the entire histogram) in the count
    //
    // for that reason, this calculation is rather delicate...

    // total number of x-bins and y-bins
    NX = mx_in[c]->GetNbinsX();
    NY = mx_in[c]->GetNbinsY();

    mx_out[c] = new TH2D(mx_out_name[c].Data(),mx_in[c]->GetTitle(),NX-1,0,NX-1,NY,0,NY);
    for(Int_t xx=1; xx<=NX-1; xx++)
      mx_out[c]->GetXaxis()->SetBinLabel(xx,mx_in[c]->GetXaxis()->GetBinLabel(xx));
    for(Int_t yy=1; yy<=NY; yy++)
      mx_out[c]->GetYaxis()->SetBinLabel(yy,mx_in[c]->GetYaxis()->GetBinLabel(yy));

    // bin coordinates below depend on overflow & underflow in mx_in, so
    // we add 2 to NX and NY to compensate
    NX+=2;
    NY+=2;
    

    // start loop from first non under/overflow bin (nb bin count starts @ 0)
    // end loop at last non underflow/overflow bin
    for(Int_t nn=NX+1; nn<=NX*(NY-1)-1; nn++)
    {
      mx_in[c]->GetBinXYZ(nn,xp,yp,zp);
      //xp = (nn-1)%NX+1;
      //yp = (nn-1)/NX+1;
      if(xp>0&&xp<NX-2)
      {
        // FindBin args must be 1 less than results from GetBinXYZ
        bn = mx_in[c]->FindBin(NX-2-1,yp-1);
        div = mx_in[c]->GetBinContent(bn);
        bc = (div>0) ? mx_in[c]->GetBinContent(nn) / div : 0;

        if(c==0) printf("x=%d y=%d bc=%f\n",xp,yp,bc);

        bn = mx_out[c]->FindBin(xp-1,yp-1);
        mx_out[c]->SetBinContent(bn,bc);
      };
    };
  };

  TFile * outfile = new TFile("diag_overlap.root","RECREATE");
  for(Int_t c=0; c<N_CLASS; c++) mx_in[c]->Write();
  for(Int_t c=0; c<N_CLASS; c++) mx_out[c]->Write();
    
};

