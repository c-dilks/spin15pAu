// tests fiducial volume and draws eta phi dist and FMS map

void FiducialGeomTest(Double_t Cd=1.5)
{
  gSystem->Load("src/RunData.so");
  EventClass * ev = new EventClass();

  const Int_t NBINS=5000;
  TH2D * etaphi = new TH2D("etaphi","#eta-#phi space fiducial cut",
    NBINS,-3.142,3.142,NBINS,2.4,4.5);
  TH2D * fmsmap = new TH2D("fmsmap","x-y space fiducial cut [units: cm]",
    NBINS,-5.8*18,5.8*18,NBINS,-5.8*18,5.8*18);

  Double_t Eta,Phi,ZZ;
  Int_t bb1,bb2,bc;

  for(Int_t p=1; p<etaphi->GetNbinsX(); p++)
  {
    for(Int_t e=1; e<etaphi->GetNbinsY(); e++)
    {
      Eta=etaphi->GetYaxis()->GetBinCenter(e);
      Phi=etaphi->GetXaxis()->GetBinCenter(p);
      bc=0;
      if(ev->FiducialGeom(Eta,Phi,0))
      {
        bc++;
        if(ev->FiducialGeom(Eta,Phi,Cd)) bc++;
      };
      bb1 = fmsmap->FindBin(ev->Xd,ev->Yd);
      fmsmap->SetBinContent(bb1,bc);
      bb2 = etaphi->FindBin(Phi,Eta);
      etaphi->SetBinContent(bb2,bc);
    };
  };
  TCanvas * canv = new TCanvas("fiducial","fiducial",1600,800);
  canv->Divide(2,1);
  for(Int_t c=1; c<=2; c++) canv->GetPad(c)->SetGrid(1,1);
  gStyle->SetOptStat(0);
  canv->cd(1); fmsmap->Draw("col");
  canv->cd(2); etaphi->Draw("col");
  canv->Print("fiducial_geom.png","png");
};
