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
    fileNames = cms.untracked.vstring(
'/store/relval/CMSSW_2_1_2/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/IDEAL_V6_10TeV_v7/0000/044A7A03-BE6E-DD11-B3C0-000423D98B08.root')
)
#############   Include the jet corrections ##########
process.load("JetMETCorrections.Configuration.L2L3Corrections_iCSA08_S156_cff")
# set the record's IOV. Must be defined once. Choose ANY correction service. #
process.prefer("L2L3JetCorrectorIcone5") 
#############   Correct Calo Jets on the fly #########
process.calo = cms.EDAnalyzer("CaloJetCorExample",
    JetAlgorithm         = cms.string('iterativeCone5CaloJets'),
    HistoFileName        = cms.string('CaloJetCorOnTheFlyExample.root'),
    JetCorrectionService = cms.string('L2L3JetCorrectorIcone5')
)
#############   Path       ###########################
process.p = cms.Path(process.calo)
#############   Format MessageLogger #################
process.MessageLogger.cerr.FwkReport.reportEvery = 10

