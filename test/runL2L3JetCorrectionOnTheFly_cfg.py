# PYTHON configuration file for class: JetCorExample
# Description:  Example of simple EDAnalyzer for correcting jets on the fly.
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
#############   Include the jet corrections ##########
process.load("JetMETCorrections.Configuration.L2L3Corrections_Winter09_cff")
# set the record's IOV. Must be defined once. Choose ANY correction service. #
process.prefer("L2L3JetCorrectorIC5Calo") 
#############   Correct Calo Jets on the fly #########
process.calo = cms.EDAnalyzer("CaloJetCorExample",
    JetAlgorithm         = cms.string('iterativeCone5CaloJets'),
    HistoFileName        = cms.string('CaloJetCorOnTheFlyExample.root'),
    JetCorrectionService = cms.string('L2L3JetCorrectorIC5Calo')
)
#############   Path       ###########################
process.p = cms.Path(process.calo)
#############   Format MessageLogger #################
process.MessageLogger.cerr.FwkReport.reportEvery = 10

