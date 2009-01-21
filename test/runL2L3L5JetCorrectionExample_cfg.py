# PYTHON configuration file.
# Description:  Example of applying L2+L3+L5 jet corrections.
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
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/mc/Winter09/QCDDiJetPt380to470/GEN-SIM-DIGI-RECO/IDEAL_V11_FastSim_v1/0041/0894C59C-51D0-DD11-B828-003048322B3E.root')
)
#############   Define the L2 correction service #####
process.L2RelativeJetCorrector = cms.ESSource("L2RelativeCorrectionService", 
    tagName = cms.string('Winter09_L2Relative_IC5Calo'),
    label = cms.string('L2RelativeJetCorrector')
)
#############   Define the L3 correction service #####
process.L3AbsoluteJetCorrector = cms.ESSource("L3AbsoluteCorrectionService", 
    tagName = cms.string('Winter09_L3Absolute_IC5Calo'),
    label = cms.string('L3AbsoluteJetCorrector')
)
#############   Define the L5 correction service #####
process.L5JetCorrector = cms.ESSource("L5FlavorCorrectionService",
    section = cms.string('b'), 
    tagName = cms.string('L5Flavor_fromTTbar_iterativeCone5'),
    label = cms.string('L5FlavorJetCorrector')
)
#############   Define the chain corrector service ###
process.L2L3L5JetCorrector = cms.ESSource("JetCorrectionServiceChain",  
    correctors = cms.vstring('L2RelativeJetCorrector','L3AbsoluteJetCorrector','L5FlavorJetCorrector'),
    label = cms.string('L2L3L5JetCorrector')
)
#############   Define the chain corrector module ####
process.L2L3L5CorJet = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("iterativeCone5CaloJets"),
    correctors = cms.vstring('L2L3L5JetCorrector')
)
# set the record's IOV. Must be defined once. Choose ANY correction service. #
process.prefer("L2L3L5JetCorrector") 
#############   Plots of corrected Jet collection ####
process.plots = cms.EDAnalyzer("CaloJetPlotsExample",
    JetAlgorithm  = cms.string('L2L3L5CorJet'),
    HistoFileName = cms.string('L2L3L5CaloJetCorExample.root'),
    NJets         = cms.int32(100)
)
#############   Path       ###########################
process.p = cms.Path(process.L2L3L5CorJet * process.plots)
#############   Format MessageLogger #################
process.MessageLogger.cerr.FwkReport.reportEvery = 10
