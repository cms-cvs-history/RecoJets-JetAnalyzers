# PYTHON configuration file for class: JetSubstructurePlotsExample
# Description:  Example of simple EDAnalyzer for substructure jets.
# Author: S. Rappoccio
# Date:  8 Dec 2011


import FWCore.ParameterSet.Config as cms

##  ____        _                       __  __  ____ 
## |  _ \  __ _| |_ __ _    ___  _ __  |  \/  |/ ___|
## | | | |/ _` | __/ _` |  / _ \| '__| | |\/| | |    
## | |_| | (_| | || (_| | | (_) | |    | |  | | |___ 
## |____/ \__,_|\__\__,_|  \___/|_|    |_|  |_|\____|
            
#isQCDMC = True
isQCDMC = False
isZprimeMC = True
#isZprimeMC = False
#isTTbarMC = True
isTTbarMC = False
#isRStoWW = True
isRStoWW = False
##   ____             __ _                       _     _           
##  / ___|___  _ __  / _(_) __ _ _   _ _ __ __ _| |__ | | ___  ___ 
## | |   / _ \| '_ \| |_| |/ _` | | | | '__/ _` | '_ \| |/ _ \/ __|
## | |__| (_) | | | |  _| | (_| | |_| | | | (_| | |_) | |  __/\__ \
##  \____\___/|_| |_|_| |_|\__, |\__,_|_|  \__,_|_.__/|_|\___||___/
##                         |___/


if isZprimeMC:
  PlotSuffix = "_ZprimeMC"
  inputFile = 'file:/uscms_data/d1/jdolen/HATS/ZPrimeToTTJets_M2000GeV_W20GeV_tlbsm_53x_v3_mc_1_1_ZzP.root'
  nEvents = 200

if isQCDMC:
  PlotSuffix = "_QCDMC"  
  inputFile ='file:/uscms_data/d1/jdolen/HATS/QCD_Pt-15to3000_TuneZ2star_Flat_8TeV_pythia6_00F65A31.root'
  nEvents = 1000

if isTTbarMC:
  PlotSuffix = "_TTbarMC"  
  inputFile = 'file:/uscms_data/d1/jdolen/HATS/TT_Mtt_1000toInf_tlbsm_53x_v3_mc_96_1_abQ.root'
  nEvents =200

if isRStoWW:
  PlotSuffix = "_RStoWW"
  inputFile = 'file:/uscms_data/d1/jdolen/HATS/RStoWW1.root','file:/uscms_data/d1/jdolen/HATS/RStoWW2.root','file:/uscms_data/d1/jdolen/HATS/RStoWW3.root','file:/uscms_data/d1/jdolen/HATS/RStoWW4.root','file:/uscms_data/d1/jdolen/HATS/RStoWW5.root','file:/uscms_data/d1/jdolen/HATS/RStoWW6.root','file:/uscms_data/d1/jdolen/HATS/RStoWW7.root','file:/uscms_data/d1/jdolen/HATS/RStoWW8.root','file:/uscms_data/d1/jdolen/HATS/RStoWW9.root','file:/uscms_data/d1/jdolen/HATS/RStoWW10.root','file:/uscms_data/d1/jdolen/HATS/RStoWW11.root','file:/uscms_data/d1/jdolen/HATS/RStoWW12.root',
  nEvents = 200


##  _            _           _           
## (_)_ __   ___| |_   _  __| | ___  ___ 
## | | '_ \ / __| | | | |/ _` |/ _ \/ __|
## | | | | | (__| | |_| | (_| |  __/\__ \
## |_|_| |_|\___|_|\__,_|\__,_|\___||___/
    
process = cms.Process("Ana")
#############   Format MessageLogger #################
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100


process.TFileService = cms.Service("TFileService",
  fileName = cms.string('jetSubstructurePlotsExample' + PlotSuffix + '.root')
)


##  ____             _ ____                           
## |  _ \ ___   ___ | / ___|  ___  _   _ _ __ ___ ___ 
## | |_) / _ \ / _ \| \___ \ / _ \| | | | '__/ __/ _ \
## |  __/ (_) | (_) | |___) | (_) | |_| | | | (_|  __/
## |_|   \___/ \___/|_|____/ \___/ \__,_|_|  \___\___|
                                                   

#############   Set the number of events #############
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(nEvents)
)
#############   Define the source file ###############
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(inputFile)
)
process.source.inputCommands = cms.untracked.vstring("keep *","drop *_MEtoEDMConverter_*_*")


##  ____  _       _       
## |  _ \| | ___ | |_ ___ 
## | |_) | |/ _ \| __/ __|
## |  __/| | (_) | |_\__ \
## |_|   |_|\___/ \__|___/

#############   PF Jets    ###########################
process.pf = cms.EDAnalyzer("JetSubstructurePlotsExample",
    jetSrc = cms.InputTag('goodPatJetsCA8PF'),
    prunedJetSrc = cms.InputTag('goodPatJetsCA8PrunedPFPacked'),
    caTopJetSrc = cms.InputTag('goodPatJetsCATopTagPFPacked'),
    leadJetPtMin = cms.double(300.0),
    jetPtMin = cms.double(50.0),
	runQjets = cms.bool(True)
)



#############   Path       ###########################
##  ____       _   _     
## |  _ \ __ _| |_| |__  
## | |_) / _` | __| '_ \ 
## |  __/ (_| | |_| | | |
## |_|   \__,_|\__|_| |_|

process.myseq = cms.Sequence(process.pf )
process.p = cms.Path( process.myseq)

