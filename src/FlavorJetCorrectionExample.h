#ifndef FlavorJetCorrectionExample_h
#define FlavorJetCorrectionExample_h

/* Example analizer to use L5 flavor JetCorrector services
   Applies different flavor corrections randomly
    F.Ratnikov (UMd)  Nov 16, 2007
*/

#include <string>

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

class FlavorJetCorrectionExample : public edm::EDAnalyzer {
 public:
  explicit FlavorJetCorrectionExample (const edm::ParameterSet& fParameters);
  virtual ~FlavorJetCorrectionExample () {}
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
 private:
  edm::InputTag mInput;
  std::string mUDSCorrectorName;
  std::string mCCorrectorName;
  std::string mBCorrectorName;
  std::string mGCorrectorName;
};

#endif
