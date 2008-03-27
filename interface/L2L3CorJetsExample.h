#ifndef RecoExamples_L2L3CorJetsExample_h
#define RecoExamples_L2L3CorJetsExample_h
#include <TH1.h>
#include "FWCore/Framework/interface/EDAnalyzer.h"

class L2L3CorJetsExample : public edm::EDAnalyzer 
{
public:
  L2L3CorJetsExample(const edm::ParameterSet&);

private:
  void beginJob(const edm::EventSetup &);
  void analyze(const edm::Event&,const edm::EventSetup&);
  void endJob();
  std::string CaloJetAlgorithm, CorJetAlgorithm, L2JetCorrectionService, L3JetCorrectionService, GenJetAlgorithm;
  TH1F h_ptCal, h_ptGen, h_ptCor, h_ptCorOnFly;
  TFile* m_file;
};

#endif
