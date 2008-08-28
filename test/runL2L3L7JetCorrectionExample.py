# PYTHON configuration file.
# Description:  Example of applying L2+L3+L7 jet corrections.
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
#############   Define the L2 correction service #####
process.L2JetCorrector = cms.ESSource("L2RelativeCorrectionService", 
    tagName = cms.string('iCSA08_S156_L2Relative_Icone5'),
    label = cms.string('L2RelativeJetCorrector')
)
#############   Define the L3 correction service #####
process.L3JetCorrector = cms.ESSource("L3AbsoluteCorrectionService", 
    tagName = cms.string('iCSA08_S156_L3Absolute_Icone5'),
    label = cms.string('L3AbsoluteJetCorrector')
)
#############   Define the L7 correction service #####
process.L7JetCorrector = cms.ESSource("L7PartonCorrectionService", 
    section = cms.string('qJ'),
    tagName = cms.string('L7parton_IC5_080301'),
    label = cms.string('L7PartonJetCorrector')
)
#############   Define the chain corrector service ###
process.L2L3L7JetCorrector = cms.ESSource("JetCorrectionServiceChain",  
    correctors = cms.vstring('L2RelativeJetCorrector','L3AbsoluteJetCorrector','L7PartonJetCorrector'),
    label = cms.string('L2L3L7JetCorrector')
)
#############   Define the chain corrector module ####
process.L2L3L7CorJet = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("iterativeCone5CaloJets"),
    correctors = cms.vstring('L2L3L7JetCorrector')
)
# set the record's IOV. Must be defined once. Choose ANY correction service. #
process.prefer("L2L3L7JetCorrector") 
#############   Plots of corrected Jet collection ####
process.plots = cms.EDAnalyzer("CaloJetPlotsExample",
    JetAlgorithm  = cms.string('L2L3L7CorJet'),
    HistoFileName = cms.string('L2L3L7CaloJetCorExample.root'),
    NJets         = cms.int32(100)
)
#############   Path       ###########################
process.p = cms.Path(process.L2L3L7CorJet * process.plots)
#############   Format MessageLogger #################
process.MessageLogger.cerr.FwkReport.reportEvery = 10
