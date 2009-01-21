# PYTHON configuration file.
# Description:  Example of applying default (L2+L3) jet corrections on JetPlusTrack.
# Author: K. Kousouris
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
#############   Include the JPT ######################
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("JetMETCorrections.Configuration.JetPlusTrackCorrections_cff")
process.load("JetMETCorrections.Configuration.ZSPJetCorrections219_cff")
#############   Include the jet corrections ##########
process.load("JetMETCorrections.Configuration.L2L3Corrections_Winter09_cff")
# set the record's IOV. Must be defined once. Choose ANY correction service. #
process.prefer("L2L3JetCorrectorIC5JPT") 
#############   User analyzer (corrected jets) #######
process.corrected = cms.EDAnalyzer("CaloJetPlotsExample",
    JetAlgorithm  = cms.string('L2L3CorJetIC5JPT'),
    HistoFileName = cms.string('CorJetHisto.root'),
    NJets         = cms.int32(2)
)
#############   Path       ###########################
process.p = cms.Path(process.ZSPJetCorrections * process.JetPlusTrackCorrections * process.L2L3CorJetIC5JPT * process.corrected)
#############   Format MessageLogger #################
process.MessageLogger.cerr.FwkReport.reportEvery = 10

