// Example of jet substructure example: JetSubstructurePlotsExample
// Description:  Example of simple EDAnalyzer to check into jet substructure
// Author: S. Rappoccio
// Date: 07 Dec 2011
#ifndef JetSubstructurePlotsExample_h
#define JetSubstructurePlotsExample_h


#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include "TNamed.h"
#include <TMath.h>
#include <TLorentzVector.h> 

#include <vector>
#include <map>
#include <cmath>
#include  <iostream> 

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/JetReco/interface/Jet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "AnalysisDataFormats/TopObjects/interface/CATopJetTagInfo.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/Vector.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

class JetSubstructurePlotsExample : public edm::EDAnalyzer 
   {
     public:
       JetSubstructurePlotsExample(edm::ParameterSet const& cfg);
     private:
       void beginJob();
       void analyze(edm::Event const& e, edm::EventSetup const& iSetup);
       void endJob();

       edm::InputTag            jetSrc_;      /// Jet source to plot
       edm::InputTag            prunedJetSrc_;      /// Jet source to plot
       edm::InputTag            caTopJetSrc_;      /// Jet source to plot
       double                   leadJetPtMin_;/// Minimum leading jet pt, in place to be above trigger threshold
       double                   jetPtMin_;    /// Minimum jet pt to plot
	   bool                     runQjets_;
       TFileDirectory *         theDir_;      /// Histogram directory for this module
   };
#endif
