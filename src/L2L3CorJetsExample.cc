#include "RecoJets/JetAnalyzers/interface/L2L3CorJetsExample.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/GenJetfwd.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include <cmath>
using namespace edm;
using namespace reco;
using namespace std;

L2L3CorJetsExample::L2L3CorJetsExample(const ParameterSet & cfg) :
   GenJetAlgorithm_(cfg.getParameter<string>("GenJetAlgorithm")),
   CaloJetAlgorithm_(cfg.getParameter<string>("CaloJetAlgorithm")), 
   CorJetAlgorithm_(cfg.getParameter<string>("CorJetAlgorithm")), 
   L2JetCorrectionService_(cfg.getParameter<string>("L2JetCorrectionService")),
   L3JetCorrectionService_(cfg.getParameter<string>("L3JetCorrectionService")),  
   HistogramFile_(cfg.getParameter<string>("HistogramFile"))
  {
}

void L2L3CorJetsExample::beginJob(const EventSetup&) 
{
  TString hname,htitle;
  m_file = new TFile(HistogramFile_.c_str(),"RECREATE");
  hname = "h_ptGen";
  htitle = "p_{T} of leading GenJets";
  m_HistNames[hname] = new TH1F(hname,htitle,500,0,1000); 
  hname = "h_ptCal";
  htitle = "p_{T} of leading CaloJets";
  m_HistNames[hname] = new TH1F(hname,htitle,500,0,1000);
  hname = "h_ptCor";
  htitle = "p_{T} of leading CorJets";
  m_HistNames[hname] = new TH1F(hname,htitle,500,0,1000);
  hname = "h_ptCorOnFly";
  htitle = "p_{T} of leading Jets Corrected on the Fly";
  m_HistNames[hname] = new TH1F(hname,htitle,500,0,1000);
}

void L2L3CorJetsExample::fillHist(const TString& histName, const Double_t& value)
{
  std::map<TString, TH1*>::iterator hid = m_HistNames.find(histName);
  if (hid==m_HistNames.end())
    std::cout<<"%fillHist -- Could not find histogram with name: "<<histName<<std::endl;
  else
    hid->second->Fill(value);  
}

void L2L3CorJetsExample::analyze(const Event& evt, const EventSetup& es) 
{
  Handle<CaloJetCollection> caloJets;
  Handle<GenJetCollection> genJets;
  Handle<CaloJetCollection> corJets;
  evt.getByLabel(CaloJetAlgorithm_, caloJets);
  evt.getByLabel(GenJetAlgorithm_, genJets);
  evt.getByLabel(CorJetAlgorithm_, corJets);
  /////////////////////// Calo Jet Collection //////////////////////////
  int jetInd = 0;
  for(CaloJetCollection::const_iterator cal = caloJets->begin(); cal != caloJets->end(); cal++) 
    { 
      fillHist("h_ptCal",cal->pt());
      jetInd++;
      if (jetInd==2) break;
    }
  /////////////////////// Gen Jet Collection //////////////////////////
  jetInd = 0;
  for(GenJetCollection::const_iterator gen = genJets->begin(); gen != genJets->end(); gen++) 
    {  
      fillHist("h_ptGen",gen->pt());   
      jetInd++;
      if (jetInd==2) break;
    } 
  /////////////////////// Corrected Jet Collection //////////////////////////
  jetInd = 0;
  double highestPt=0.0;
  double nextPt=0.0;
  for(CaloJetCollection::const_iterator cor = corJets->begin(); cor != corJets->end(); cor++) 
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
  fillHist("h_ptCor",highestPt);
  fillHist("h_ptCor",nextPt);  
  /////////////////////// Correction on the fly //////////////////////////
  const JetCorrector* corrector_L2 = JetCorrector::getJetCorrector (L2JetCorrectionService_,es);
  const JetCorrector* corrector_L3 = JetCorrector::getJetCorrector (L3JetCorrectionService_,es);  
  highestPt=0.0;
  nextPt=0.0;
  for(CaloJetCollection::const_iterator cal = caloJets->begin(); cal != caloJets->end(); cal++) 
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
  fillHist("h_ptCorOnFly",highestPt);
  fillHist("h_ptCorOnFly",nextPt);
}
void L2L3CorJetsExample::endJob() 
{
  if (m_file !=0) 
    {
      m_file->cd();
      for (std::map<TString, TH1*>::iterator hid = m_HistNames.begin(); hid != m_HistNames.end(); hid++)
        hid->second->Write();
      delete m_file;
      m_file=0;      
    }
}
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(L2L3CorJetsExample);
