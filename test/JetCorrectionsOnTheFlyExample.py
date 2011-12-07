# PYTHON configuration file for class: JetPlotsExample
# Description:  Example of simple EDAnalyzer for jets.
# Author: K. Kousouris
# Date:  25 - August - 2008
# Modified: Kalanand Mishra
# Date:  11 - January - 2011 (for CMS Data Analysis School jet exercise)


import FWCore.ParameterSet.Config as cms

##  ____        _                       __  __  ____ 
## |  _ \  __ _| |_ __ _    ___  _ __  |  \/  |/ ___|
## | | | |/ _` | __/ _` |  / _ \| '__| | |\/| | |    
## | |_| | (_| | || (_| | | (_) | |    | |  | | |___ 
## |____/ \__,_|\__\__,_|  \___/|_|    |_|  |_|\____|
            
isMC = False
##isMC = True

##   ____             __ _                       _     _           
##  / ___|___  _ __  / _(_) __ _ _   _ _ __ __ _| |__ | | ___  ___ 
## | |   / _ \| '_ \| |_| |/ _` | | | | '__/ _` | '_ \| |/ _ \/ __|
## | |__| (_) | | | |  _| | (_| | |_| | | | (_| | |_) | |  __/\__ \
##  \____\___/|_| |_|_| |_|\__, |\__,_|_|  \__,_|_.__/|_|\___||___/
##                         |___/

PFJetCollection   = "goodPatJetsPFlow"


PlotSuffix = "_Data"
if isMC:
  PlotSuffix = "_MC"
  jecLevels = [
    'GR_R_42_V19_AK5PFchs_L1FastJet.txt',
    'GR_R_42_V19_AK5PFchs_L2Relative.txt',
    'GR_R_42_V19_AK5PFchs_L3Absolute.txt'
  ]
else :
  jecLevels = [
    'GR_R_42_V19_AK5PFchs_L1FastJet.txt',
    'GR_R_42_V19_AK5PFchs_L2Relative.txt',
    'GR_R_42_V19_AK5PFchs_L3Absolute.txt',
    'GR_R_42_V19_AK5PFchs_L2L3Residual.txt'
  ]


inputFile = '/store/user/lpctlbsm/vasquez/Jet/ttbsm_v9_Run2011A-May10ReReco/f8e845a0332c56398831da6c30999af1/ttbsm_42x_data_60_1_EbE.root'

if isMC:
  inputFile ='/store/user/lpctlbsm/srappocc/QCD_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6/ttbsm_v8_Summer11-PU_S3_-START42_V11-v2/d870fa9b0dd695e8eb649b7e725d070f/ttbsm_42x_mc_86_2_fG3.root'
  

##  _            _           _           
## (_)_ __   ___| |_   _  __| | ___  ___ 
## | | '_ \ / __| | | | |/ _` |/ _ \/ __|
## | | | | | (__| | |_| | (_| |  __/\__ \
## |_|_| |_|\___|_|\__,_|\__,_|\___||___/
    
process = cms.Process("Ana")
#############   Format MessageLogger #################
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10


process.TFileService = cms.Service("TFileService",
  fileName = cms.string('jetCorrectionsOnTheFlyExample.root')
)


##  ____             _ ____                           
## |  _ \ ___   ___ | / ___|  ___  _   _ _ __ ___ ___ 
## | |_) / _ \ / _ \| \___ \ / _ \| | | | '__/ __/ _ \
## |  __/ (_) | (_) | |___) | (_) | |_| | | | (_|  __/
## |_|   \___/ \___/|_|____/ \___/ \__,_|_|  \___\___|
                                                   

#############   Set the number of events #############
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
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
process.pf = cms.EDAnalyzer("JetCorrectionsOnTheFly",
    jetSrc = cms.InputTag('goodPatJetsPFlow'),
    rhoSrc = cms.InputTag('kt6PFJets', 'rho'),
    pvSrc  = cms.InputTag('goodOfflinePrimaryVertices'),
    jecPayloadNames = cms.vstring( jecLevels ),
    jecUncName = cms.string('GR_R_42_V19_AK5PFchs_Uncertainty.txt') 
)



#############   Path       ###########################
##  ____       _   _     
## |  _ \ __ _| |_| |__  
## | |_) / _` | __| '_ \ 
## |  __/ (_| | |_| | | |
## |_|   \__,_|\__|_| |_|

process.myseq = cms.Sequence(process.pf )
process.p = cms.Path( process.myseq)

