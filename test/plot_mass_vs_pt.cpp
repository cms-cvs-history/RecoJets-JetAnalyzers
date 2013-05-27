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

  TH2D * MassPt1  = (TH2D*)F1->Get("pf/h2AK5JetMassPt");
  TH2D * MassPt2  = (TH2D*)F1->Get("pf/h2CA8JetMassPt");
  TH2D * MassPt3  = (TH2D*)F1->Get("pf/h2PrunedMassPt");
  TH2D * MassPt4  = (TH2D*)F1->Get("pf/h2TrimmedMassPt");
  TH2D * MassPt5  = (TH2D*)F1->Get("pf/h2FilteredMassPt");


  MassPt1->RebinX(2);
  MassPt2->RebinX(2);
  MassPt3->RebinX(2);
  MassPt4->RebinX(2);
  MassPt5->RebinX(2);

  TProfile * ProfileMassPt1  = MassPt1 ->ProfileX("ProfileMassPt1",  1,-1,"s");
  TProfile * ProfileMassPt2  = MassPt2 ->ProfileX("ProfileMassPt2",  1,-1,"s");
  TProfile * ProfileMassPt3  = MassPt3 ->ProfileX("ProfileMassPt3",  1,-1,"s");
  TProfile * ProfileMassPt4  = MassPt4 ->ProfileX("ProfileMassPt4",  1,-1,"s");
  TProfile * ProfileMassPt5  = MassPt5 ->ProfileX("ProfileMassPt5",  1,-1,"s");

  ProfileMassPt1 -> SetTitle(";Jet p_{T} (GeV/c); Average Jet Mass (GeV/c^{2})");
  ProfileMassPt2 -> SetTitle(";Jet p_{T} (GeV/c); Average Jet Mass (GeV/c^{2})");
  ProfileMassPt3 -> SetTitle(";Jet p_{T} (GeV/c); Average Jet Mass (GeV/c^{2})");
  ProfileMassPt4 -> SetTitle(";Jet p_{T} (GeV/c); Average Jet Mass (GeV/c^{2})");
  ProfileMassPt5 -> SetTitle(";Jet p_{T} (GeV/c); Average Jet Mass (GeV/c^{2})");

  ProfileMassPt1 -> SetMarkerStyle(20);
  ProfileMassPt2 -> SetMarkerStyle(21);
  ProfileMassPt3 -> SetMarkerStyle(22);
  ProfileMassPt4 -> SetMarkerStyle(23);
  ProfileMassPt5 -> SetMarkerStyle(24);

  ProfileMassPt1 -> SetMarkerColor(1);
  ProfileMassPt2 -> SetMarkerColor(2);
  ProfileMassPt3 -> SetMarkerColor(3);
  ProfileMassPt4 -> SetMarkerColor(4);
  ProfileMassPt5 -> SetMarkerColor(6);

  ProfileMassPt1 -> SetLineColor(1);
  ProfileMassPt2 -> SetLineColor(2);
  ProfileMassPt3 -> SetLineColor(3);
  ProfileMassPt4 -> SetLineColor(4);
  ProfileMassPt5 -> SetLineColor(6);

  ProfileMassPt1 -> Fit("pol1");
  ProfileMassPt2 -> Fit("pol1");
  ProfileMassPt3 -> Fit("pol1");
  ProfileMassPt4 -> Fit("pol1");
  ProfileMassPt5 -> Fit("pol1");

  TF1 *f1 = ProfileMassPt1->GetFunction("pol1");
  TF1 *f2 = ProfileMassPt2->GetFunction("pol1");
  TF1 *f3 = ProfileMassPt3->GetFunction("pol1");
  TF1 *f4 = ProfileMassPt4->GetFunction("pol1");
  TF1 *f5 = ProfileMassPt5->GetFunction("pol1");


  f1->SetLineColor(1);
  f2->SetLineColor(2);
  f3->SetLineColor(3);
  f4->SetLineColor(4);
  f5->SetLineColor(6);



  TCanvas *c= new TCanvas("c","",200,10,800,600);
  ProfileMassPt1 -> Draw("");
  ProfileMassPt2 -> Draw("same");
  leg = new TLegend(0.15,0.67,0.4,0.85);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry(ProfileMassPt1,"AK5","P");
  leg->AddEntry(ProfileMassPt2,"CA8","P");
  leg->Draw("same");
  c->SaveAs("AverageMass_vs_pt_CompareAlgos.png");

  TCanvas *c2= new TCanvas("c2","",200,10,800,600);
  ProfileMassPt2 -> Draw("");
  ProfileMassPt3-> Draw("same");
  leg = new TLegend(0.12,0.67,0.45,0.85);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry(ProfileMassPt2,"CA8 - normal","P");
  leg->AddEntry(ProfileMassPt3,"CA8 - pruned","P");
  leg->Draw("same");
  c2->SaveAs("AverageMass_vs_pt_pruned.png");

  TCanvas *c3= new TCanvas("c3","",200,10,800,600);
  ProfileMassPt2 -> Draw("");
  ProfileMassPt4-> Draw("same");
  leg = new TLegend(0.12,0.67,0.45,0.85);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry(ProfileMassPt2,"CA8 - normal","P");
  leg->AddEntry(ProfileMassPt4,"CA8 - trimmed","P");
  leg->Draw("same");
  c3->SaveAs("AverageMass_vs_pt_trimmed.png");

  TCanvas *c4= new TCanvas("c4","",200,10,800,600);
  ProfileMassPt2 -> Draw("");
  ProfileMassPt5-> Draw("same");
  leg = new TLegend(0.12,0.67,0.45,0.85);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry(ProfileMassPt2,"CA8 - normal","P");
  leg->AddEntry(ProfileMassPt5,"CA8 - filtered","P");
  leg->Draw("same");
  c4->SaveAs("AverageMass_vs_pt_filtered.png");



}