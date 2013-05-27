{
  //-----------------------------------------
  // Hands-on training session (HATS) - May 29 2013
  // Plot 2D histograms of jet mass vs number of primary vertices (questions: jdolen@fnal.gov)
  // 
  // run with:
  //  root -x plot_mass_vs_nvtx.cpp
  //


  gStyle->SetOptStat(0); 
  gROOT->Reset();

  TFile * F1  = new TFile("jetSubstructurePlotsExample_QCDMC.root");   ////~190126 events

  TH2D * MassNvtx1  = (TH2D*)F1->Get("pf/h2AK5JetMassNvtx");
  TH2D * MassNvtx2  = (TH2D*)F1->Get("pf/h2CA8JetMassNvtx");
  TH2D * MassNvtx3  = (TH2D*)F1->Get("pf/h2PrunedMassNvtx");
  TH2D * MassNvtx4  = (TH2D*)F1->Get("pf/h2TrimmedMassNvtx");
  TH2D * MassNvtx5  = (TH2D*)F1->Get("pf/h2FilteredMassNvtx");


  MassNvtx1->RebinX(2);
  MassNvtx2->RebinX(2);
  MassNvtx3->RebinX(2);
  MassNvtx4->RebinX(2);
  MassNvtx5->RebinX(2);

  TProfile * ProfileMassNvtx1  = MassNvtx1 ->ProfileX("ProfileMassNvtx1",  1,-1,"s");
  TProfile * ProfileMassNvtx2  = MassNvtx2 ->ProfileX("ProfileMassNvtx2",  1,-1,"s");
  TProfile * ProfileMassNvtx3  = MassNvtx3 ->ProfileX("ProfileMassNvtx3",  1,-1,"s");
  TProfile * ProfileMassNvtx4  = MassNvtx4 ->ProfileX("ProfileMassNvtx4",  1,-1,"s");
  TProfile * ProfileMassNvtx5  = MassNvtx5 ->ProfileX("ProfileMassNvtx5",  1,-1,"s");

  ProfileMassNvtx1 -> SetTitle(";Number of primary vertices; Average Jet Mass (GeV/c^{2})");
  ProfileMassNvtx2 -> SetTitle(";Number of primary vertices; Average Jet Mass (GeV/c^{2})");
  ProfileMassNvtx3 -> SetTitle(";Number of primary vertices; Average Jet Mass (GeV/c^{2})");
  ProfileMassNvtx4 -> SetTitle(";Number of primary vertices; Average Jet Mass (GeV/c^{2})");
  ProfileMassNvtx5 -> SetTitle(";Number of primary vertices; Average Jet Mass (GeV/c^{2})");

  ProfileMassNvtx1 -> SetMarkerStyle(20);
  ProfileMassNvtx2 -> SetMarkerStyle(21);
  ProfileMassNvtx3 -> SetMarkerStyle(22);
  ProfileMassNvtx4 -> SetMarkerStyle(23);
  ProfileMassNvtx5 -> SetMarkerStyle(24);

  ProfileMassNvtx1 -> SetMarkerColor(1);
  ProfileMassNvtx2 -> SetMarkerColor(2);
  ProfileMassNvtx3 -> SetMarkerColor(3);
  ProfileMassNvtx4 -> SetMarkerColor(4);
  ProfileMassNvtx5 -> SetMarkerColor(6);

  ProfileMassNvtx1 -> SetLineColor(1);
  ProfileMassNvtx2 -> SetLineColor(2);
  ProfileMassNvtx3 -> SetLineColor(3);
  ProfileMassNvtx4 -> SetLineColor(4);
  ProfileMassNvtx5 -> SetLineColor(6);

  ProfileMassNvtx1 -> Fit("pol1");
  ProfileMassNvtx2 -> Fit("pol1");
  ProfileMassNvtx3 -> Fit("pol1");
  ProfileMassNvtx4 -> Fit("pol1");
  ProfileMassNvtx5 -> Fit("pol1");

  TF1 *f1 = ProfileMassNvtx1->GetFunction("pol1");
  TF1 *f2 = ProfileMassNvtx2->GetFunction("pol1");
  TF1 *f3 = ProfileMassNvtx3->GetFunction("pol1");
  TF1 *f4 = ProfileMassNvtx4->GetFunction("pol1");
  TF1 *f5 = ProfileMassNvtx5->GetFunction("pol1");


  f1->SetLineColor(1);
  f2->SetLineColor(2);
  f3->SetLineColor(3);
  f4->SetLineColor(4);
  f5->SetLineColor(6);



  TCanvas *c= new TCanvas("c","",200,10,800,600);
  ProfileMassNvtx2 -> Draw("");
  ProfileMassNvtx1 -> Draw("same");
  leg = new TLegend(0.15,0.67,0.4,0.85);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry(ProfileMassNvtx1,"AK5","P");
  leg->AddEntry(ProfileMassNvtx2,"CA8","P");
  leg->Draw("same");
  c->SaveAs("AverageMass_vs_nvtx_CompareAlgos.png");

  TCanvas *c2= new TCanvas("c2","",200,10,800,600);
  ProfileMassNvtx2 -> Draw("");
  ProfileMassNvtx3-> Draw("same");
  leg = new TLegend(0.12,0.67,0.45,0.85);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry(ProfileMassNvtx2,"CA8 - normal","P");
  leg->AddEntry(ProfileMassNvtx3,"CA8 - pruned","P");
  leg->Draw("same");
  c2->SaveAs("AverageMass_vs_nvtx_pruned.png");

  TCanvas *c3= new TCanvas("c3","",200,10,800,600);
  ProfileMassNvtx2 -> Draw("");
  ProfileMassNvtx4-> Draw("same");
  leg = new TLegend(0.12,0.67,0.45,0.85);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry(ProfileMassNvtx2,"CA8 - normal","P");
  leg->AddEntry(ProfileMassNvtx4,"CA8 - trimmed","P");
  leg->Draw("same");
  c3->SaveAs("AverageMass_vs_nvtx_trimmed.png");

  TCanvas *c4= new TCanvas("c4","",200,10,800,600);
  ProfileMassNvtx2 -> Draw("");
  ProfileMassNvtx5-> Draw("same");
  leg = new TLegend(0.12,0.67,0.45,0.85);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry(ProfileMassNvtx2,"CA8 - normal","P");
  leg->AddEntry(ProfileMassNvtx5,"CA8 - filtered","P");
  leg->Draw("same");
  c4->SaveAs("AverageMass_vs_nvtx_filtered.png");


}