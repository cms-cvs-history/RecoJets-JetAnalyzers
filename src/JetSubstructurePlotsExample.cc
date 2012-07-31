// Implementation of template class: JetSubstructurePlotsExample
// Description:  Example of simple EDAnalyzer for jets.
// Author: K. Kousouris
// Date:  25 - August - 2008
#include "RecoJets/JetAnalyzers/interface/JetSubstructurePlotsExample.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include <TFile.h>
#include <TLorentzVector.h> 
#include <cmath>
////////////////////////////////////////////////////////////////////////////////////////
JetSubstructurePlotsExample::JetSubstructurePlotsExample(edm::ParameterSet const& cfg) :
  jetSrc_   (cfg.getParameter<edm::InputTag>("jetSrc") ),     // jet collection to get
  leadJetPtMin_ (cfg.getParameter<double>("leadJetPtMin") ),  // minimum jet pt of leading jet
  jetPtMin_ (cfg.getParameter<double>("jetPtMin") )           // minimum jet pt of all jets
{

  // Get the TFileService to handle plots
  edm::Service<TFileService> fileService;
  theDir_ = &*fileService;

  // Make histograms in that directory
  theDir_->make<TH1F>("hPt", "Jet pt", 50, 0., 1000.);
  theDir_->make<TH1F>("hRapidity", "Jet Rapidity", 50, -5.0, 5.0);
  theDir_->make<TH1F>("hPhi", "Jet Azimuthal Angle", 50, -TMath::Pi(), TMath::Pi());
  theDir_->make<TH1F>("hMass", "Jet Mass", 50, 0., 400.);
  theDir_->make<TH1F>("hArea", "Jet Area", 50, 0., 5.0);
  theDir_->make<TH1F>("hSubjet0Pt", "Subjet pt, highest mass subjet", 50, 0., 1000.);
  theDir_->make<TH1F>("hSubjet0Mass", "Subjet mass, highest mass subjet", 50, 0., 500.);
  theDir_->make<TH1F>("hSubjet0Area", "Subjet area, highest mass subjet", 50, 0., 5.0);
  theDir_->make<TH1F>("hSubjet0DeltaRCore", "Subjet #Delta R to Jet Core, highest mass subjet", 50, 0., 5.0);
  theDir_->make<TH1F>("hSubjet0PtRelCore", "Subjet P_{T}^{REL} to Jet Core, highest mass subjet", 50, 0., 100.);
  theDir_->make<TH1F>("hSubjet1Pt", "Subjet pt, lowest mass subjet", 50, 0., 1000.);
  theDir_->make<TH1F>("hSubjet1Mass", "Subjet mass, lowest mass subjet", 50, 0., 500.);
  theDir_->make<TH1F>("hSubjet1Area", "Subjet area, lowest mass subjet", 50, 0., 5.0);
  theDir_->make<TH1F>("hSubjet1DeltaRCore", "Subjet #Delta R to Jet Core, lowest mass subjet", 50, 0., 5.0);
  theDir_->make<TH1F>("hSubjet1PtRelCore", "Subjet P_{T}^{REL} to Jet Core, lowest mass subjet", 50, 0., 100.);
}
////////////////////////////////////////////////////////////////////////////////////////
void JetSubstructurePlotsExample::beginJob() 
{
}
////////////////////////////////////////////////////////////////////////////////////////
void JetSubstructurePlotsExample::analyze(edm::Event const& evt, edm::EventSetup const& iSetup) 
{
  
  // For Monte Carlo, get the weight of this generated event.
  // The MC sample we're using has an artificial "flat" pt spectrum
  // and so to compare data to MC, we need to weight the events
  // to get a physical pt spectrum.

  edm::Handle<GenEventInfoProduct> hgen;
  double weight = 1.0;
  if ( evt.getByLabel("generator", hgen) && hgen.isValid() ) {
    weight = hgen->weight();
  }
  

  // Get the jet collection
  edm::Handle<edm::View<reco::Jet> > jets;
  evt.getByLabel(jetSrc_,jets);

  // Ensure that we have at least one jet
  if ( jets->size() < 1 ) return;

  // Ensure that the leading jet is above trigger threshold
  edm::View<reco::Jet>::const_iterator ibegin = jets->begin();
  edm::View<reco::Jet>::const_iterator iend = jets->end();
  edm::View<reco::Jet>::const_iterator ijet = ibegin;
  if ( ibegin->pt() < leadJetPtMin_ )
    return;


  // Loop over the "hard" jets
  for ( ; ijet != iend; ++ijet ) {

    if ( ijet->pt() < jetPtMin_ ) continue;

    // Plot the "hard jet" quantities
    theDir_->getObject<TH1>("hPt")->Fill( ijet->pt(), weight );
    theDir_->getObject<TH1>("hRapidity")->Fill( ijet->rapidity(), weight );
    theDir_->getObject<TH1>("hPhi")->Fill( ijet->phi(), weight );
    theDir_->getObject<TH1>("hMass")->Fill( ijet->mass(), weight );
    theDir_->getObject<TH1>("hArea")->Fill( ijet->jetArea(), weight );


    // Examine the subjets of this "hard jet". 
    // In this case, we're looking at the jet pruning algorithm
    // where we have requested 2 subjets. You can change this
    // in your own analysis, and is a configurable parameter. 
    if ( ijet->numberOfDaughters() >= 2 ) {

      reco::Jet const * subjet0 = dynamic_cast<reco::Jet const *>(ijet->daughter(0));
      reco::Jet const * subjet1 = dynamic_cast<reco::Jet const *>(ijet->daughter(1));

      // Ensure that we have two valid subjets
      if ( subjet0 != 0 && subjet1 != 0 ) {


	// Order the subjets by mass, not pt!
	if ( subjet1->mass() > subjet0->mass() ) {
	  reco::Jet const * temp = subjet0;
	  subjet0 = subjet1;
	  subjet1 = temp;
	}

	// Get TLorentzVectors to easily compute ptRel and dR to jet axis. 
	TLorentzVector jet_p4( ijet->px(), ijet->py(), ijet->pz(), ijet->energy() );
	TLorentzVector subjet0_p4( subjet0->px(), subjet0->py(), subjet0->pz(), subjet0->energy());
	TLorentzVector subjet1_p4( subjet1->px(), subjet1->py(), subjet1->pz(), subjet1->energy());

	// Compute the delta R between the subjets, and the "hard jet" axis
	double dR0 = subjet0_p4.DeltaR( jet_p4 ) ;
	double dR1 = subjet1_p4.DeltaR( jet_p4 ) ;

	// Compute the relative pt between the subjets, and the "hard jet" axis
	double ptRel0 = subjet0_p4.Perp( jet_p4.Vect() );
	double ptRel1 = subjet1_p4.Perp( jet_p4.Vect() );

	// Fill the quantities for the leading mass subjet
	theDir_->getObject<TH1>("hSubjet0Pt")->Fill( subjet0_p4.Perp(), weight );
	theDir_->getObject<TH1>("hSubjet0Mass")->Fill( subjet0_p4.M(), weight );
	theDir_->getObject<TH1>("hSubjet0Area")->Fill( subjet0->jetArea(), weight );
	theDir_->getObject<TH1>("hSubjet0DeltaRCore")->Fill( dR0, weight );
	theDir_->getObject<TH1>("hSubjet0PtRelCore")->Fill( ptRel0, weight );

	// Fill the quantities for the lowest mass subjet
	theDir_->getObject<TH1>("hSubjet1Pt")->Fill( subjet1_p4.Perp(), weight );
	theDir_->getObject<TH1>("hSubjet1Mass")->Fill( subjet1_p4.M(), weight );
	theDir_->getObject<TH1>("hSubjet1Area")->Fill( subjet1->jetArea(), weight );
	theDir_->getObject<TH1>("hSubjet1DeltaRCore")->Fill( dR1, weight );
	theDir_->getObject<TH1>("hSubjet1PtRelCore")->Fill( ptRel1, weight );


      }
    }

  }
}
////////////////////////////////////////////////////////////////////////////////////////
void JetSubstructurePlotsExample::endJob() 
{
}
/////////// Register Modules ////////
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(JetSubstructurePlotsExample);
