# PYTHON configuration file.
# Description:  Example of applying EMF (L4) jet corrections.
# Author: K. Kousouris
# Date:  25 - August - 2008
import FWCore.ParameterSet.Config as cms

process = cms.Process("Ana")
process.load("FWCore.MessageService.MessageLogger_cfi")
#############   Set the number of events #############
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)
#############   Define the source file ###############
#process.load("RecoJets.JetAnalyzers.RelValQCD_Pt_80_120_cfi")
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
'/store/relval/CMSSW_2_1_2/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/IDEAL_V6_10TeV_v7/0000/044A7A03-BE6E-DD11-B3C0-000423D98B08.root')
)
#############   Include the default jet corrections ##
process.load("JetMETCorrections.Configuration.L2L3Corrections_iCSA08_S156_cff")
#############   Define the EMF correction service ####
process.L4JetCorrector = cms.ESSource("L4EMFCorrectionService", 
    tagName = cms.string('CMSSW_152_L4EMF'),
    label = cms.string('L4EMFJetCorrector')
)
#############   Define the EMF corrector module ######
process.L4JetCorJet = cms.EDProducer("CaloJetCorrectionProducer", 
    src = cms.InputTag("L2L3ChainJetCorJetIcone5"), 
    correctors = cms.vstring('L4EMFJetCorrector')
)
# set the record's IOV. Must be defined once. Choose ANY correction service. #
process.prefer("L2L3JetCorrectorIcone5") 
#############   Plots of corrected Jet collection ####
process.plots = cms.EDAnalyzer("CaloJetPlotsExample",
    JetAlgorithm  = cms.string('L4JetCorJet'),
    HistoFileName = cms.string('L4CaloJetCorExample.root'),
    NJets         = cms.int32(100)
)
#############   Path       ###########################
process.p = cms.Path(process.L2L3ChainJetCorJetIcone5 * process.L4JetCorJet * process.plots)
#############   Format MessageLogger #################
process.MessageLogger.cerr.FwkReport.reportEvery = 10
