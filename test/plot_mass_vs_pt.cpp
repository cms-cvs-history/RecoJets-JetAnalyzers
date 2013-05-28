{
  //-----------------------------------------
  // Hands-on training session (HATS) - May 29 2013
  // Plot 2D histograms of jet mass vs number of primary vertices 
  // 
  // run with:
  //  root -x plot_mass_vs_pt.cpp
  //


  gStyle->SetOptStat(0); 
  gROOT->Reset();

  TCanvas *c= new TCanvas("c","",200,10,800,600);

  TFile * F1  = new TFile("jetSubstructurePlotsExample_QCDMC.root");   ////~190126 events

  TH2D * MassPt1  = (TH2D*)F1->Get("pf/h2AK5JetMassPt");
  TH2D * MassPt2  = (TH2D*)F1->Get("pf/h2CA8JetMassPt");

  MassPt1->RebinX(2);
  MassPt2->RebinX(2);

  MassPt1->GetXaxis()->SetRangeUser(300,1000);
  MassPt2->GetXaxis()->SetRangeUser(300,1000);


  TProfile * ProfileMassPt1  = MassPt1 ->ProfileX("ProfileMassPt1",  1,-1,"s");
  TProfile * ProfileMassPt2  = MassPt2 ->ProfileX("ProfileMassPt2",  1,-1,"s");
  
  ProfileMassPt1 -> SetTitle(";Jet p_{T} (GeV/c); Average Jet Mass (GeV/c^{2})");
  ProfileMassPt2 -> SetTitle(";Jet p_{T} (GeV/c); Average Jet Mass (GeV/c^{2})");

  ProfileMassPt1 -> SetMarkerStyle(20);
  ProfileMassPt2 -> SetMarkerStyle(21);
 
  ProfileMassPt1 -> SetMarkerColor(2);
  ProfileMassPt2 -> SetMarkerColor(1);
  
  ProfileMassPt1 -> SetLineColor(2);
  ProfileMassPt2 -> SetLineColor(1);
 
  ProfileMassPt1->GetYaxis()->SetRangeUser(0,220);
  ProfileMassPt2->GetYaxis()->SetRangeUser(0,220);
 
  ProfileMassPt1 -> Fit("pol1");
  ProfileMassPt2 -> Fit("pol1");
  
  TF1 *f1 = ProfileMassPt1->GetFunction("pol1");
  TF1 *f2 = ProfileMassPt2->GetFunction("pol1");
 
  f1->SetLineColor(2);
  f2->SetLineColor(1);
  


 
  ProfileMassPt1 -> Draw("E1");
  ProfileMassPt2 -> Draw("E1same");
  leg = new TLegend(0.15,0.67,0.4,0.85);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry(ProfileMassPt1,"AK5","P");
  leg->AddEntry(ProfileMassPt2,"CA8","P");
  leg->Draw("same");
  c->SaveAs("AverageMass_vs_pt_CompareAlgos.png");


}
