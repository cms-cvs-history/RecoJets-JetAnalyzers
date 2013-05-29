//////////////////////////////////////
// script to make ROC (mistag rate vs efficiency) plots comparing top taggers (j)
// instructions
// root
// [] .L plot_ROC_toptag.cpp
// [] run()


void run()
{
  
  TFile * BKD  = new TFile("jetSubstructurePlotsExample_QCDMC.root");   
  TFile * SIG  = new TFile("jetSubstructurePlotsExample_ZprimeMC.root");  


  TH1F * DENOM_BKD  = (TH1F*)BKD->Get("pf/hCount"); 
  TH1F * DENOM_SIG  = (TH1F*)SIG->Get("pf/hCount"); 


  TH1F * NUMER_QJET_BKD  = (TH1F*)BKD->Get("pf/h_cutTopMass_QjetVolatility");
  TH1F * NUMER_QJET_SIG  = (TH1F*)SIG->Get("pf/h_cutTopMass_QjetVolatility");
  TGraph * ROC1 = plotROC_IntegrateDown(NUMER_QJET_BKD, DENOM_BKD, NUMER_QJET_SIG, DENOM_SIG);

  TH1F * NUMER_CMS_BKD  = (TH1F*)BKD->Get("pf/h_cutTopMass_CATopMinMass");
  TH1F * NUMER_CMS_SIG  = (TH1F*)SIG->Get("pf/h_cutTopMass_CATopMinMass");
  TH1F * DENOM_CMS_BKD  = (TH1F*)BKD->Get("pf/hCATopRapidity");
  TH1F * DENOM_CMS_SIG  = (TH1F*)SIG->Get("pf/hCATopRapidity");
  TGraph * ROC2 = plotROC_IntegratUp(NUMER_CMS_BKD, DENOM_CMS_BKD, NUMER_CMS_SIG, DENOM_CMS_SIG);

  TH1F * NUMER_TAU32_BKD  = (TH1F*)BKD->Get("pf/h_cutTopMass_Tau32");
  TH1F * NUMER_TAU32_SIG  = (TH1F*)SIG->Get("pf/h_cutTopMass_Tau32");
  TGraph * ROC3 = plotROC_IntegrateDown(NUMER_TAU32_BKD, DENOM_BKD, NUMER_TAU32_SIG, DENOM_SIG);

  // compare the top-tagging ROC plots
  ROC1->SetTitle("Mistag rate vs Efficiency - Top tagging;Tagging Efficiency; Mistag Rate");
  ROC2->SetTitle("Mistag rate vs Efficiency - Top tagging;Tagging Efficiency; Mistag Rate");
  ROC3->SetTitle("Mistag rate vs Efficiency - Top tagging;Tagging Efficiency; Mistag Rate");

  ROC1->SetLineColor(1);
  ROC2->SetLineColor(2);
  ROC3->SetLineColor(4);

  ROC1->SetMarkerStyle(20);
  ROC2->SetMarkerStyle(20);
  ROC3->SetMarkerStyle(20);

  ROC1->SetLineWidth(2);
  ROC2->SetLineWidth(2);
  ROC3->SetLineWidth(2);

  TCanvas *c1= new TCanvas("c1","",200,10,800,800);

  ROC1->Draw("alp");
  ROC2->Draw("l");
  ROC3->Draw("l");

  leg = new TLegend(0.15,0.67,0.4,0.85);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry(ROC1,"Qjets","L");
  leg->AddEntry(ROC2,"MinMass","L");
  leg->AddEntry(ROC3,"Tau32","L");
  leg->Draw("same");






}




TGraph *  plotROC_IntegrateDown(TH1 * Numer_bkd, TH1 * Denom_bkd, TH1 * Numer_sig, TH1 * Denom_sig)
{
 
  double denom_bkd = Denom_bkd->Integral();
  double denom_sig = Denom_sig->Integral();
  
  // cout<<"denom_bkd "<<denom_bkd<<endl;
  // cout<<"denom_sig "<<denom_sig<<endl;
  // cout<<"numer bkd integral "<<Numer_bkd->Integral()<<endl;
  // cout<<"numer sig integral "<<Numer_sig->Integral()<<endl;


  int nbinsx = Numer_bkd->GetNbinsX();

  double *sig_effic = new double[nbinsx-2];
  double *bkd_effic = new double[nbinsx-2];

  for (int i=0; i<nbinsx-2; i++ )
  {
    double numer_sig = Numer_sig->Integral(1,nbinsx-i);
    double numer_bkd = Numer_bkd->Integral(1,nbinsx-i);
          
    if (denom_sig !=0 ) sig_effic[i]=numer_sig/denom_sig;
    else sig_effic[i]= 0;
    if (denom_bkd !=0 ) bkd_effic[i]=numer_bkd/denom_bkd;
    else bkd_effic[i]= 0;
    //cout<<"integration upper bound "<<Numer_bkd->GetBinLowEdge(nbinsx-i)<<"  effic " <<sig_effic[i]<<" mistag "<<bkd_effic[i] <<endl;
  }

  TGraph *g=new TGraph(nbinsx,sig_effic,bkd_effic);
  return g;
}



TGraph *  plotROC_IntegratUp(TH1 * Numer_bkd, TH1 * Denom_bkd, TH1 * Numer_sig, TH1 * Denom_sig)
{
 
  double denom_bkd = Denom_bkd->Integral();
  double denom_sig = Denom_sig->Integral();
  
  // cout<<"denom_bkd "<<denom_bkd<<endl;
  // cout<<"denom_sig "<<denom_sig<<endl;
  // cout<<"numer bkd integral "<<Numer_bkd->Integral()<<endl;
  // cout<<"numer sig integral "<<Numer_sig->Integral()<<endl;


  int nbinsx = Numer_bkd->GetNbinsX();
  // cout<<"nbinsx "<<nbinsx<<endl;
  double *sig_effic = new double[nbinsx];
  double *bkd_effic = new double[nbinsx];

  for (int i=1; i<=nbinsx; i++ )
  {
    double numer_sig = Numer_sig->Integral(i,nbinsx); 
    double numer_bkd = Numer_bkd->Integral(i,nbinsx); 
          
    if (denom_sig !=0 ) sig_effic[i-1]=numer_sig/denom_sig;  
    if (denom_bkd !=0 ) bkd_effic[i-1]=numer_bkd/denom_bkd; 

    // cout<<"bin "<<i<<"  integration upper bound "<<Numer_bkd->GetBinLowEdge(i)<<"  effic " <<sig_effic[i]<<" mistag "<<bkd_effic[i] <<endl;
  }

  TGraph *g=new TGraph(nbinsx,sig_effic,bkd_effic);
  return g;
}