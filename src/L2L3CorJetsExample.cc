#include "RecoJets/JetAnalyzers/interface/L2L3CorJetsExample.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TCanvas.h>
#include <cmath>
using namespace edm;
using namespace reco;
using namespace std;

L2L3CorJetsExample::L2L3CorJetsExample(const ParameterSet & cfg) :
   CaloJetAlgorithm(cfg.getParameter<string>("CaloJetAlgorithm")), 
   CorJetAlgorithm(cfg.getParameter<string>("CorJetAlgorithm")), 
   L2JetCorrectionService(cfg.getParameter<string>("L2JetCorrectionService")),
   L3JetCorrectionService(cfg.getParameter<string>("L3JetCorrectionService")),  
   GenJetAlgorithm(cfg.getParameter<string>("GenJetAlgorithm"))
  {
}

void L2L3CorJetsExample::beginJob(const EventSetup&) 
{
  m_file = new TFile("histo.root","RECREATE"); 
  h_ptCal =  TH1F("h_ptCal","p_{T} of leading CaloJets",500,0,1000);
  h_ptGen =  TH1F("h_ptGen","p_{T} of leading GenJets",500,0,1000);
  h_ptCor =  TH1F("h_ptCor","p_{T} of leading CorJets",500,0,1000);
  h_ptCorOnFly =  TH1F("h_ptCorOnFly","p_{T} of leading Jets Corrected on the Fly",500,0,1000);
}

void L2L3CorJetsExample::analyze(const Event& evt, const EventSetup& es) 
{
  Handle<CaloJetCollection> caloJets;
  Handle<GenJetCollection> genJets;
  Handle<CaloJetCollection> corJets;
  evt.getByLabel(CaloJetAlgorithm, caloJets);
  evt.getByLabel(GenJetAlgorithm, genJets);
  evt.getByLabel(CorJetAlgorithm, corJets);
  /////////////////////// Calo Jet Collection //////////////////////////
  int jetInd = 0;
  for(CaloJetCollection::const_iterator cal = caloJets->begin(); cal != caloJets->end() && jetInd<2; ++ cal) 
    {
      h_ptCal.Fill(cal->pt());   
      jetInd++;
    }
  /////////////////////// Gen Jet Collection //////////////////////////
  jetInd = 0;
  for(GenJetCollection::const_iterator gen = genJets->begin(); gen != genJets->end() && jetInd<2; ++ gen) 
    {
      h_ptGen.Fill( gen->pt() );   
      jetInd++;
    }
  /////////////////////// Corrected Jet Collection //////////////////////////
  jetInd = 0;
  double highestPt=0.0;
  double nextPt=0.0;
  for(CaloJetCollection::const_iterator cor = corJets->begin(); cor != corJets->end(); ++ cor) 
    {
      double corPt=cor->pt();
      if (corPt>highestPt)
        {
          nextPt=highestPt;
          highestPt=corPt;
        }
      else if (corPt>nextPt) 
        nextPt = corPt;
    }
  h_ptCor.Fill(highestPt);   
  h_ptCor.Fill(nextPt);
  /////////////////////// Correction on the fly //////////////////////////
  const JetCorrector* corrector_L2 = JetCorrector::getJetCorrector (L2JetCorrectionService,es);
  const JetCorrector* corrector_L3 = JetCorrector::getJetCorrector (L3JetCorrectionService,es);  
  highestPt=0.0;
  nextPt=0.0;
  for(CaloJetCollection::const_iterator cal = caloJets->begin(); cal != caloJets->end(); ++ cal) 
   {
     double scale_L2 = corrector_L2->correction(cal->p4());
     double scale_L3 = corrector_L3->correction(scale_L2*cal->p4());
     double corPt = scale_L3*scale_L2*cal->pt();
     if (corPt>highestPt)
       {
         nextPt = highestPt;
         highestPt = corPt;
       }
     else if (corPt>nextPt)
       nextPt = corPt;
   } 
  h_ptCorOnFly.Fill(highestPt);   
  h_ptCorOnFly.Fill(nextPt); 
}

void L2L3CorJetsExample::endJob() 
{
  m_file->Write(); 
}
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(L2L3CorJetsExample);
