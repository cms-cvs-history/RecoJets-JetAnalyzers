//////////////////////////////////////
// script to make ROC (mistag rate vs efficiency) plots comparing W taggers  (jdolen@fnal.gov)
// instructions
// root
// [] .L plot_ROC_wtag.cpp
// [] run()


void run()
{
  
  TFile * BKD  = new TFile("jetSubstructurePlotsExample_QCDMC.root");   
  TFile * SIG  = new TFile("jetSubstructurePlotsExample_RStoWW.root");  


  TH1F * DENOM_BKD  = (TH1F*)BKD->Get("pf/hCount"); 
  TH1F * DENOM_SIG  = (TH1F*)SIG->Get("pf/hCount"); 


  cout<<"-----------------------------------"<<endl;
  cout<<" Q jet volatility "<<endl;
  cout<<"-----------------------------------"<<endl;
  TH1F * NUMER_QJET_BKD  = (TH1F*)BKD->Get("pf/h_cutWMass_QjetVolatility");
  TH1F * NUMER_QJET_SIG  = (TH1F*)SIG->Get("pf/h_cutWMass_QjetVolatility");
  TGraph * ROC1 = plotROC_IntegrateDown(NUMER_QJET_BKD, DENOM_BKD, NUMER_QJET_SIG, DENOM_SIG );


  cout<<"-----------------------------------"<<endl;
  cout<<" Mass drop "<<endl;
  cout<<"-----------------------------------"<<endl;
  TH1F * NUMER_MASSDROP_BKD  = (TH1F*)BKD->Get("pf/h_prunedJet_cutWMass_MassDrop");
  TH1F * NUMER_MASSDROP_SIG  = (TH1F*)SIG->Get("pf/h_prunedJet_cutWMass_MassDrop");
  TGraph * ROC2 = plotROC_IntegrateDown(NUMER_MASSDROP_BKD, DENOM_BKD, NUMER_MASSDROP_SIG, DENOM_SIG);


  cout<<"-----------------------------------"<<endl;
  cout<<" Tau3/Tau2 "<<endl;
  cout<<"-----------------------------------"<<endl;

  TH1F * NUMER_TAU21_BKD  = (TH1F*)BKD->Get("pf/h_cutWMass_Tau21");
  TH1F * NUMER_TAU21_SIG  = (TH1F*)SIG->Get("pf/h_cutWMass_Tau21");
  TGraph * ROC3 = plotROC_IntegrateDown(NUMER_TAU21_BKD, DENOM_BKD, NUMER_TAU21_SIG, DENOM_SIG);

  // compare the W-tagging ROC plots
  ROC1->SetTitle("Mistag rate vs Efficiency - W tagging;Tagging Efficiency; Mistag Rate");
  ROC2->SetTitle("Mistag rate vs Efficiency - W tagging;Tagging Efficiency; Mistag Rate");
  ROC3->SetTitle("Mistag rate vs Efficiency - W tagging;Tagging Efficiency; Mistag Rate");

  ROC1->SetLineColor(1);
  ROC2->SetLineColor(2);
  ROC3->SetLineColor(4);

  ROC1->SetMarkerStyle(20);
  ROC2->SetMarkerStyle(20);
  ROC3->SetMarkerStyle(20);

  ROC1->SetLineWidth(2);
  ROC2->SetLineWidth(2);
  ROC3->SetLineWidth(2);

  TCanvas *c1= new TCanvas("c1","",200,10,900,800);

  ROC1->Draw("al");
  ROC2->Draw("l");
  ROC3->Draw("l");

  leg = new TLegend(0.15,0.67,0.4,0.85);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry(ROC1,"Qjets","L");
  leg->AddEntry(ROC2,"MassDrop","L");
  leg->AddEntry(ROC3,"Tau21","L");
  leg->Draw("same");


}




TGraph *  plotROC_IntegrateDown(TH1 * Numer_bkd, TH1 * Denom_bkd, TH1 * Numer_sig, TH1 * Denom_sig)
{
 
  double denom_bkd = Denom_bkd->Integral();
  double denom_sig = Denom_sig->Integral();
  
  cout<<"denom_bkd "<<denom_bkd<<endl;
  cout<<"denom_sig "<<denom_sig<<endl;
  cout<<"numer bkd integral "<<Numer_bkd->Integral()<<endl;
  cout<<"numer sig integral "<<Numer_sig->Integral()<<endl;


  int nbinsx = Numer_bkd->GetNbinsX();

  double *sig_effic = new double[nbinsx-2];
  double *bkd_effic = new double[nbinsx-2];

  for (int i=0; i<nbinsx-2; i++ )
  {
    double numer_sig = Numer_sig->Integral(1,nbinsx-i);
    double numer_bkd = Numer_bkd->Integral(1,nbinsx-i);
          
    if (denom_sig !=0 ) sig_effic[i]=numer_sig/denom_sig;
    if (denom_bkd !=0 ) bkd_effic[i]=numer_bkd/denom_bkd;

    cout<<"integration upper bound "<<Numer_bkd->GetBinLowEdge(nbinsx-i)<<"  effic " <<sig_effic[i]<<" mistag "<<bkd_effic[i] <<endl;
  }

  TGraph *g=new TGraph(nbinsx,sig_effic,bkd_effic);
  return g;
}