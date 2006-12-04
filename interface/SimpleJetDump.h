#ifndef RecoExamples_SimpleJetDump_h
#define RecoExamples_SimpleJetDump_h
#include <TH1.h>
/* \class SimpleJetDump
 *
 * \author Robert Harris
 *
 * \version 1
 *
 */
#include "FWCore/Framework/interface/EDAnalyzer.h"

class SimpleJetDump : public edm::EDAnalyzer {
public:
  SimpleJetDump( const edm::ParameterSet & );

private:
  void beginJob( const edm::EventSetup & );
  void analyze( const edm::Event& , const edm::EventSetup& );
  void endJob();
  std::string CaloJetAlg, GenJetAlg;
  //Internal parameters
  int evtCount;
};

#endif
