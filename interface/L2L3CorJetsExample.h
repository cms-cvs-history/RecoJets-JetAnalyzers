#ifndef RecoExamples_L2L3CorJetsExample_h
#define RecoExamples_L2L3CorJetsExample_h
#include "TH1.h"
#include "TFile.h"
#include "TNamed.h"
#include <map>
#include "FWCore/Framework/interface/EDAnalyzer.h"

class L2L3CorJetsExample : public edm::EDAnalyzer 
{
public:
  L2L3CorJetsExample(const edm::ParameterSet&);

private:
  void beginJob(const edm::EventSetup &);
  void analyze(const edm::Event&,const edm::EventSetup&);
  void endJob();
  void fillHist(const TString& histName, const Double_t& value);
  std::string GenJetAlgorithm_;
  std::string CaloJetAlgorithm_;
  std::string CorJetAlgorithm_; 
  std::string L2JetCorrectionService_;
  std::string L3JetCorrectionService_;
  std::string HistogramFile_; 
  std::map<TString, TH1*> m_HistNames;
  TFile* m_file;
};

#endif
