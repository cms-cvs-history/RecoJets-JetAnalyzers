// Implementation of template class: JetSubstructurePlotsExample
// Description:  Example of simple EDAnalyzer for jets.
// Author: K. Kousouris
// Date:  25 - August - 2008

#include "RecoJets/JetAnalyzers/interface/JetSubstructurePlotsExample.h"
#include <cmath>
#include <TMath.h>
#include "DataFormats/JetReco/interface/Jet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include <TFile.h>
#include <TLorentzVector.h> 
#include "AnalysisDataFormats/TopObjects/interface/CATopJetTagInfo.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/Vector.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include  <iostream> 
#include <fastjet/JetDefinition.hh>
#include <fastjet/PseudoJet.hh>
#include "fastjet/tools/Filter.hh"
#include <fastjet/ClusterSequence.hh>
#include <fastjet/ActiveAreaSpec.hh>
#include <fastjet/ClusterSequenceArea.hh>
#include <fastjet/tools/JHTopTagger.hh>
#include <fastjet/Selector.hh>
#include "fastjet/tools/Filter.hh"
#include "fastjet/tools/Pruner.hh"
#include "JetSubstructure/SubstructureTools/interface/JetSubstructureTools.h"

////////////////////////////////////////////////////////////////////////////////////////
JetSubstructurePlotsExample::JetSubstructurePlotsExample(edm::ParameterSet const& cfg) :
  jetSrc_   (cfg.getParameter<edm::InputTag>("jetSrc") ),     // jet collection to get
  prunedJetSrc_   (cfg.getParameter<edm::InputTag>("prunedJetSrc") ),     // jet collection to get
  caTopJetSrc_   (cfg.getParameter<edm::InputTag>("caTopJetSrc") ),     // jet collection to get
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
  theDir_->make<TH1F>("hDeltaRSubjet0Subjet1", "#Delta R distance bewteen subjets", 50, 0., 1.0);
  theDir_->make<TH1F>("hMassDrop", "Jet Mass Drop (highest mass subjet mass / jet mass)", 50, 0., 1.0);
  theDir_->make<TH1F>("hSubjetAsymmetry", "Subjet Asymmetry", 50, 0., 1.0);
  theDir_->make<TH1F>("hCATopMass", "CATop Jet mass", 50, 0., 400.);
  theDir_->make<TH1F>("hCATopMinMass", "CATop Jet minmass", 50, 0., 150.);
  theDir_->make<TH1F>("hCATopNsubjets", "CATop Jet Nsubjets", 5, 0., 5.);
  theDir_->make<TH1F>("hCATopPt", "CATop Jet Pt", 50, 0., 1000.);
  theDir_->make<TH1F>("hCATopRapidity", "CATop Jet Rapdity", 50, -5.0, 5.0);
  theDir_->make<TH1F>("hNsubTau32", "Nsubjettiness Tau3/Tau2", 50, 0., 1.);
  theDir_->make<TH1F>("hFilteredMass", "Filtered Jet Mass", 50, 0., 400.);
  theDir_->make<TH1F>("hTrimmedMass", "Trimmed Jet Mass", 50, 0., 400.);
  theDir_->make<TH1F>("hTrimmedArea", "Trimmed Jet Area", 50, 0., 2);
  theDir_->make<TH1F>("hPrunedMass", "Pruned Jet Mass", 50, 0., 400.);
  theDir_->make<TH1F>("hPrunedArea", "Pruned Jet Area", 50, 0., 2.);
  theDir_->make<TH1F>("hFilteredMass", "Filtered Jet Mass", 50, 0., 400.);
  theDir_->make<TH1F>("hFilteredArea", "Filtered Jet Area", 50, 0., 2);
  theDir_->make<TH1F>("hQjetVol", "Qjets volatiliity", 50, 0., 1);
  theDir_->make<TH1F>("hTau3", "Tau3", 50, 0., 1);
  theDir_->make<TH1F>("hTau2", "Tau2", 50, 0., 1);

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
  float weight = 1.0;
  if ( evt.getByLabel("generator", hgen) && hgen.isValid() ) {
    weight = hgen->weight();
  }
  

  //-----------------------------------------------------
  //-- Part 1  - FROM JANUARY CMS DAS
  //--   Access a collection of pruned subjets from PATtuple
  //--   Decluster once to find 2 subjets.
  //--   Calculate mass drop, angle between subjets, subjet asymmetry, pTrel, etc.
  //-----------------------------------------------------



  // Get the jet collection
  edm::Handle<edm::View<reco::Jet> > jets;
  evt.getByLabel(prunedJetSrc_,jets);

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
      if ( subjet0 != 0 && subjet1 != 0  && subjet0->pt()>0. && subjet1->pt()>0.) {


	// Order the subjets by mass, not pt!
  if ( subjet1->mass() > subjet0->mass()) {
	  reco::Jet const * temp = subjet0;
	  subjet0 = subjet1;
	  subjet1 = temp;
  }



	// Get TLorentzVectors to easily compute ptRel and dR to jet axis. 
	TLorentzVector jet_p4( ijet->px(), ijet->py(), ijet->pz(), ijet->energy() );
	TLorentzVector subjet0_p4( subjet0->px(), subjet0->py(), subjet0->pz(), subjet0->energy());
	TLorentzVector subjet1_p4( subjet1->px(), subjet1->py(), subjet1->pz(), subjet1->energy());


	// Compute the delta R between the subjets, and the "hard jet" axis
	float dR0 = subjet0_p4.DeltaR( jet_p4 ) ;
	float dR1 = subjet1_p4.DeltaR( jet_p4 ) ;

  // Compute the delta R between the two subjets
  float dR = subjet0_p4.DeltaR( subjet1_p4 ) ;

	// Compute the relative pt between the subjets, and the "hard jet" axis
	float ptRel0 = subjet0_p4.Perp( jet_p4.Vect() );
	float ptRel1 = subjet1_p4.Perp( jet_p4.Vect() );

  // Compute substructure tagging variables
  float massDrop = subjet0_p4.M()/jet_p4.M();
  float subjetAsymmetry = std::min( subjet0_p4.Perp()*subjet0_p4.Perp(), subjet1_p4.Perp()*subjet1_p4.Perp()) * dR*dR / (jet_p4.M()*jet_p4.M());

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

  // Fill the quantities for jet tagging variables
  theDir_->getObject<TH1>("hDeltaRSubjet0Subjet1")->Fill( dR, weight );
  theDir_->getObject<TH1>("hMassDrop")->Fill( massDrop, weight );
  theDir_->getObject<TH1>("hSubjetAsymmetry")->Fill( subjetAsymmetry, weight );


      }
    }
  }

  //-----------------------------------------------------
  //-- Part 2  - FROM JANUARY CMS DAS
  //--   Access a collection of Cambrdige Aachen R=0.8 jets on which the CMSTopTagging Algorithm has been applied from the PATtuple 
  //--   Plot Mass, MinMass, Nsubjets, etc.
  //-----------------------------------------------------

  edm::Handle<std::vector<pat::Jet> > h_topTag;
  evt.getByLabel( caTopJetSrc_ , h_topTag );

  int jet_number = 0;

  for ( std::vector<pat::Jet>::const_iterator jetBegin = h_topTag->begin(),
      jetEnd = h_topTag->end(), ijet = jetBegin; ijet != jetEnd; ++ijet ) {

    const reco::CATopJetTagInfo * catopTag = dynamic_cast<reco::CATopJetTagInfo const *>(ijet->tagInfo("CATop"));

    float pt       = ijet->pt();
    float eta      = ijet->eta();
    float rapidity = ijet->rapidity();
    float mass     = ijet->mass();
    float minmass  = catopTag->properties().minMass;
    float topmass  = catopTag->properties().topMass;
    int nsubjets    = ijet->numberOfDaughters();
    
    if (pt>350){

      if (jet_number<2){

        theDir_->getObject<TH1>("hCATopMass")->Fill( mass, weight );
        theDir_->getObject<TH1>("hCATopMinMass")->Fill( minmass, weight );
        theDir_->getObject<TH1>("hCATopNsubjets")->Fill( nsubjets, weight );
        theDir_->getObject<TH1>("hCATopPt")->Fill( pt, weight );
        theDir_->getObject<TH1>("hCATopRapidity")->Fill( rapidity, weight );
      }
      jet_number++;
    }
  }


  //-----------------------------------------------------
  //-- Part 3  - NEW FOR HATS
  //--   Access a collection of Cambrdige Aachen R=0.8 PATtuple
  //--   Loop over the jet constituents
  //--   Use  JetSubstructureTools class to recluster jets and calculate jet substructure variables
  //-----------------------------------------------------

  edm::Handle<std::vector<pat::Jet> > h_CA8PF;
  evt.getByLabel( jetSrc_, h_CA8PF );

  int count_CA8PF = 0;
	
  // CA8 jet loop
  for ( std::vector<pat::Jet>::const_iterator jetBegin = h_CA8PF->begin(), jetEnd = h_CA8PF->end(), ijet = jetBegin; ijet != jetEnd; ++ijet ) 
  {		

    // Select high pT jets
    if ( ijet->pt() > 300 )
    {

      // Store the particle flow constituents of this jet in a vector
  		std::vector<edm::Ptr<reco::PFCandidate> > pfCands = ijet->getPFConstituents();
  			
      // initialize some vectors   
  		std::vector<float> vec_px;		
  		std::vector<float> vec_py;		
  		std::vector<float> vec_pz;		
  		std::vector<float> vec_e;
  		std::vector<float> vec_id;
  		
  		//loop over all constituents of jet
  		for (unsigned j = 0; j < pfCands.size (); ++j)
      {
  			float px = pfCands[j]->px(); 
  			float py = pfCands[j]->py(); 
  			float pz = pfCands[j]->pz(); 
  			float e = pfCands[j]->energy(); 
  			float id = pfCands[j]->pdgId(); 

  			vec_px.push_back( px );
  			vec_py.push_back( py );
  			vec_pz.push_back( pz );
  			vec_e.push_back( e );
  			vec_id.push_back( id );
  		}
  		
  		

      // Use external class to calculate substrucutre variables
  		JetSubstructureTools *tmp = new JetSubstructureTools( 0.8, vec_px, vec_py, vec_pz, vec_e, vec_id);

      // trimmed jet (arXiv:0912.1342)
      float rtrim = 0.2;
      float ptfrac = 0.03;

      float trimmed_jet_mass = tmp->getTrimmedJet( rtrim, ptfrac ).m();
      float trimmed_jet_area = tmp->getTrimmedJet( rtrim, ptfrac ).area();

      //pruned jet (arXiv:0903.5081 and arXiv:0912.0033)
      float zcut = 0.1;
      float rcut = 0.5;

      float pruned_jet_mass  = tmp->getPrunedJet( zcut, rcut ).m();
      float pruned_jet_area  = tmp->getPrunedJet( zcut, rcut ).area();

      // fitlered jet
      float rfilt = 0.3;
      int nfilt = 3;

      float filtered_jet_mass = tmp->getFilteredJet( rfilt, nfilt ).m();
      float filtered_jet_area = tmp->getFilteredJet( rfilt, nfilt ).area();

      // Qjet (arXiv:1201.1914)
      int event = evt.id().event();
      float qjet_volatility  = tmp->getQjetVolatility( event );  // seed random number with event number

      // Nsubjettiness (arXiv:1011.2268)
  		float tau2 = tmp->getTau( 2, 1.0 );  //2subjettiness
  		float tau3 = tmp->getTau( 3, 1.0 );  //3subjettiness
      float tau32 = -1;
      if (tau2!=0) tau32 = tau3/tau2;

 
      // Fill histograms   		
  		theDir_->getObject<TH1>("hTrimmedMass")  ->Fill( trimmed_jet_mass  , weight );
  		theDir_->getObject<TH1>("hTrimmedArea")  ->Fill( trimmed_jet_area  , weight );
  		theDir_->getObject<TH1>("hPrunedMass")   ->Fill( pruned_jet_mass   , weight );
  		theDir_->getObject<TH1>("hPrunedArea")   ->Fill( pruned_jet_area   , weight );
      theDir_->getObject<TH1>("hFilteredMass") ->Fill( filtered_jet_mass , weight );
      theDir_->getObject<TH1>("hFilteredArea") ->Fill( filtered_jet_area , weight );
  		theDir_->getObject<TH1>("hQjetVol")      ->Fill( qjet_volatility   , weight );
  		theDir_->getObject<TH1>("hTau3")         ->Fill( tau3              , weight );
  		theDir_->getObject<TH1>("hTau2")         ->Fill( tau2              , weight );
      theDir_->getObject<TH1>("hNsubTau32")    ->Fill( tau32             , weight );
      
      //clean up
  		vec_px.clear();
  		vec_py.clear();
  		vec_pz.clear();
  		vec_e.clear();
      count_CA8PF++;
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
