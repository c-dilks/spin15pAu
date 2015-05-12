// plots FMS channel distributions weighted by ADC counts for each run;
// each run's channel distribution normalised by its integral

void HotChannelSearch(Int_t runnum=16085023)
{
  // intitialisation
  TString checkstr = gSystem->ExpandPathName("$SETFMSENV");
  TString dataroot = gSystem->ExpandPathName("$DataRoot");
  if(checkstr!="SETFMSENV") { fprintf(stderr,"source SetFMSEnv first\n"); exit(); };
  gROOT->Macro("${FMSROOT}/start.C");

  //gSystem->Load("src/RunData.so");
  //Trigger * T = new Trigger();
 
  UInt_t nqtdata;
  UInt_t QTdata[3700];
  Bool_t dec;
  UInt_t L2sum[2];

  Qt * QT = new Qt(p_files);
  //memset(QTdata,0,sizeof(QTdata));

  TH1D * ad[4];
  TString ad_n[4];
  Int_t nchan;
  for(Int_t n=0; n<4; n++) 
  {
    nchan = (34-10*(n>1)) * (17-5*(n>1));
    ad_n[n] = Form("chandist%d",n+1);
    ad[n] = new TH1D(ad_n[n].Data(),ad_n[n].Data(),nchan,1,nchan+1);
  };
      
  
  // chain Output files
  TChain * tc = new TChain("h111");
  TString filez = Form("%s/Day%03d/run%d.*.root",dataroot.Data(),runnum/1000%1000,runnum);
  tc->Add(filez.Data());
  tc->SetBranchAddress("nqtdata",&nqtdata);
  tc->SetBranchAddress("QTdata",QTdata);
  tc->SetBranchAddress("L2sum",L2sum);

  dataSet * ds = new dataSet(filez.Data(),p_files,"h111");


  // chain loop
  Int_t nrows,ncols,chan;
  Double_t adc;

  /*
  Int_t ENT = tc->GetEntries();
  ENT = 50000;
  */

  Int_t ENT = ds->Input->GetEntries();
  Int_t shift=50*10000; ENT = 50000;


  for(Int_t e=shift; e<ENT+shift; e++)
  {
    if(e%10000==0) printf("%.2f%%\n",(float)e/ENT*100);
    /*
    tc->GetEntry(e);
    
    if(L2sum[1] & T->Mask(runnum,"All",1))
    {
    */

    ds->GetEntry(e);

      /*
      dec = QT->decodeQT(nqtdata,QTdata,1);
      */

      dec = ds->decodeQT();

      if(dec) printf("pass\n");
      for(Int_t n=0; n<4; n++)
      {
        /*
        nrows = QT->tm(n)->GetNrows();
        ncols = QT->tm(n)->GetNcols();
        */

        nrows = (ds->dMatrixQt(n)).GetNrows();
        ncols = (ds->dMatrixQt(n)).GetNcols();

        for(Int_t nr=0; nr<nrows; nr++)
        {
          for(Int_t nc=0; nc<ncols; nc++)
          {
            chan = nc+nr*(17-5*(n>1))+1;
            adc = (ds->dMatrixQt(n))(nr,nc);
            //printf("n=%d ch=%d adc=%d\n",n,chan,adc);
            ad[n]->Fill(chan,adc);
          };
        };
      };
    //};
  };

  TString outfile_n = Form("chandists/chandist.%d.root",runnum);
  TFile * outfile = new TFile(outfile_n.Data(),"RECREATE");
  for(Int_t n=0; n<4; n++) ad[n]->Write(ad_n[n]);
};
