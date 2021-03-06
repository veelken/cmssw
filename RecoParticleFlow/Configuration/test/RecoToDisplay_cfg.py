import FWCore.ParameterSet.Config as cms

process = cms.Process("REPROD")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
#process.load("Configuration.StandardSequences.MagneticField_4T_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.autoCond import autoCond
process.GlobalTag.globaltag = autoCond['startup']
#process.GlobalTag.globaltag = 'START50_V10::All'

#process.Timing =cms.Service("Timing")
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(200)
)

process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(    
    #'root://eoscms//eos/cms/store/relval/CMSSW_5_2_0_pre5/RelValQCD_FlatPt_15_3000/GEN-SIM-RECO/START52_V1-v1/0105/2AAA5F86-8D57-E111-B6E8-003048678B84.root',
    #'root://eoscms//eos/cms/store/relval/CMSSW_5_2_0_pre5/RelValQCD_FlatPt_15_3000/GEN-SIM-RECO/START52_V1-v1/0105/38D32839-8A57-E111-849D-0026189438E4.root'
    #'root://cms-xrd-global.cern.ch//store/relval/CMSSW_6_1_0/SingleGammaPt300ExtRelVal610/GEN-SIM-RECO/START61_V8_NoPuCustomEvC-v1/00000/00110DD9-9390-E211-88AB-5404A63886E6.root'
    'file:/tmp/lgray/EEC27D47-2F0C-E311-B9F2-003048FFD75C.root'
    ),
    eventsToProcess = cms.untracked.VEventRange(),
    #eventsToProcess = cms.untracked.VEventRange('1:1217421-1:1217421'),
    #                                             '1:1220344-1:1220344',
    #                                             '1:1655912-1:1655912',
    #                                             '1:415027-1:415027',
    #                                             '1:460640-1:460640',
    #                                             '1:2054772-1:2054772'),
    secondaryFileNames = cms.untracked.vstring(),
    noEventSort = cms.untracked.bool(True),
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)

#from RecoParticleFlow.Configuration.reco_QCDForPF_cff import fileNames
#process.source.fileNames = fileNames

process.dump = cms.EDAnalyzer("EventContentAnalyzer")


process.load("RecoParticleFlow.Configuration.ReDisplay_EventContent_cff")
process.display = cms.OutputModule("PoolOutputModule",
    process.DisplayEventContent,
    #outputCommands = cms.untracked.vstring('keep *'),
    fileName = cms.untracked.string('display.root')
)

process.load("Configuration.EventContent.EventContent_cff")
process.reco = cms.OutputModule("PoolOutputModule",
    process.RECOSIMEventContent,
    fileName = cms.untracked.string('reco.root')
)

# modify reconstruction sequence
#process.hbhereflag = process.hbhereco.clone()
#process.hbhereflag.hbheInput = 'hbhereco'
#process.towerMakerPF.hbheInput = 'hbhereflag'
#process.particleFlowRecHitHCAL.hcalRecHitsHBHE = cms.InputTag("hbhereflag")
process.particleFlowTmp.useHO = False

# Local re-reco: Produce tracker rechits, pf rechits and pf clusters
process.localReReco = cms.Sequence(process.siPixelRecHits+
                                   process.siStripMatchedRecHits+
                                   #process.hbhereflag+
                                   process.particleFlowCluster+
                                   process.ecalClusters)

# Track re-reco
process.globalReReco =  cms.Sequence(process.offlineBeamSpot+
                                     process.recopixelvertexing+
                                     process.ckftracks+
                                     process.caloTowersRec+
                                     process.vertexreco+
                                     process.recoJets+
                                     process.muonrecoComplete+
                                     process.muoncosmicreco+
                                     process.egammaGlobalReco+
                                     process.pfTrackingGlobalReco+
                                     process.egammaHighLevelRecoPrePF+
                                     process.muoncosmichighlevelreco+
                                     process.metreco)

# Particle Flow re-processing
process.pfReReco = cms.Sequence(process.particleFlowReco+
                                process.egammaHighLevelRecoPostPF+
                                process.muonshighlevelreco+
                                process.particleFlowLinks+
                                process.recoPFJets+
                                process.recoPFMET+
                                process.PFTau)
                                
# Gen Info re-processing
process.load("PhysicsTools.HepMCCandAlgos.genParticles_cfi")
process.load("RecoJets.Configuration.GenJetParticles_cff")
process.load("RecoJets.Configuration.RecoGenJets_cff")
process.load("RecoMET.Configuration.GenMETParticles_cff")
process.load("RecoMET.Configuration.RecoGenMET_cff")
process.load("RecoParticleFlow.PFProducer.particleFlowSimParticle_cff")
process.load("RecoParticleFlow.Configuration.HepMCCopy_cfi")
process.genReReco = cms.Sequence(process.generator+
                                 process.genParticles+
                                 process.genJetParticles+
                                 process.recoGenJets+
                                 process.genMETParticles+
                                 process.recoGenMET+
                                 process.particleFlowSimParticle)

#process.load("RecoParticleFlow.PFProducer.particleFlowCandidateChecker_cfi")
#process.particleFlowCandidateChecker.pfCandidatesReco = cms.InputTag("particleFlow","","REPROD")
#process.particleFlowCandidateChecker.pfCandidatesReReco = cms.InputTag("particleFlow","","REPROD2")
#process.particleFlowCandidateChecker.pfJetsReco = cms.InputTag("ak5PFJets","","REPROD")
#process.particleFlowCandidateChecker.pfJetsReReco = cms.InputTag("ak5PFJets","","REPROD2")
# The complete reprocessing
process.p = cms.Path(process.localReReco+
                     process.globalReReco+
                     process.pfReReco+
                     process.genReReco
                     #+process.particleFlowCandidateChecker
                     )

# And the output.
process.outpath = cms.EndPath(
    process.reco + 
    process.display
)

# And the monitoring
process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
                                        ignoreTotal=cms.untracked.int32(1),
                                        jobReportOutputOnly = cms.untracked.bool(True)
                                        )
process.Timing = cms.Service("Timing",
                             summaryOnly = cms.untracked.bool(True)
                             )

# And the logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.options = cms.untracked.PSet(
    makeTriggerResults = cms.untracked.bool(True),
    wantSummary = cms.untracked.bool(True),
    Rethrow = cms.untracked.vstring('Unknown', 
        'ProductNotFound', 
        'DictionaryNotFound', 
        'InsertFailure', 
        'Configuration', 
        'LogicError', 
        'UnimplementedFeature', 
        'InvalidReference', 
        'NullPointerError', 
        'NoProductSpecified', 
        'EventTimeout', 
        'EventCorruption', 
        'ModuleFailure', 
        'ScheduleExecutionFailure', 
        'EventProcessorFailure', 
        'FileInPathError', 
        'FatalRootError', 
        'NotFound')
)

process.MessageLogger.cerr.FwkReport.reportEvery = 1


