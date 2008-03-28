# The following comments couldn't be translated into the new config version:

#also some pairs in the barrel, in case the triplet fail. If the triplet secceds, the pairs should be skipped by the trajectory builder 
#for the TEC we use pairs (Fasthelix not working properly with ss TEC hits)
# genereator for TEC+ outsideIn

#generator for TEC+ insideOut
#generator for TEC- outsideIn
#generator for TEC- insideOut 
import FWCore.ParameterSet.Config as cms

# magnetic field
#include "Geometry/CMSCommonData/data/cmsMagneticFieldXML.cfi"
from MagneticField.Engine.volumeBasedMagneticField_cfi import *
# cms geometry
from Geometry.CMSCommonData.cmsIdealGeometryXML_cfi import *
# tracker geometry
from Geometry.TrackerGeometryBuilder.trackerGeometry_cfi import *
# tracker numbering
from Geometry.TrackerNumberingBuilder.trackerNumberingGeometry_cfi import *
#stripCPE
from RecoLocalTracker.SiStripRecHitConverter.StripCPEfromTrackAngle_cfi import *
#pixelCPE
from RecoLocalTracker.SiPixelRecHits.PixelCPEParmError_cfi import *
#TransientTrackingBuilder
from RecoTracker.TransientTrackingRecHit.TransientTrackingRecHitBuilder_cfi import *
from RecoTracker.TransientTrackingRecHit.TransientTrackingRecHitBuilderWithoutRefit_cfi import *
import copy
from RecoTracker.SpecialSeedGenerators.CombinatorialSeedGeneratorForCosmics_cfi import *
#recHitMatcher
#include "RecoLocalTracker/SiStripRecHitConverter/data/SiStripRecHitMatcher.cfi"
#seeding module
combinatorialcosmicseedfinderP5 = copy.deepcopy(combinatorialcosmicseedfinder)
#replace combinatorialcosmicseedfinderP5.SetMomentum = false
combinatorialcosmicseedfinderP5.UseScintillatorsConstraint = False
combinatorialcosmicseedfinderP5.OrderedHitsFactoryPSets = cms.VPSet(cms.PSet(
    ComponentName = cms.string('GenericTripletGenerator'),
    LayerPSet = cms.PSet(
        layerInfo,
        layerList = cms.vstring('TOB4+TOB5+TOB6', 'TOB3+TOB5+TOB6', 'TOB3+TOB4+TOB5', 'TOB2+TOB4+TOB5', 'TOB3+TOB4+TOB6', 'TOB2+TOB4+TOB6')
    ),
    PropagationDirection = cms.string('alongMomentum'),
    NavigationDirection = cms.string('outsideIn')
), cms.PSet(
    ComponentName = cms.string('GenericPairGenerator'),
    LayerPSet = cms.PSet(
        layerInfo,
        layerList = cms.vstring('TOB5+TOB6', 'TOB4+TOB5')
    ),
    PropagationDirection = cms.string('alongMomentum'),
    NavigationDirection = cms.string('outsideIn')
), cms.PSet(
    ComponentName = cms.string('GenericPairGenerator'),
    LayerPSet = cms.PSet(
        layerList = cms.vstring('TEC1_pos+TEC2_pos', 'TEC2_pos+TEC3_pos', 'TEC3_pos+TEC4_pos', 'TEC4_pos+TEC5_pos', 'TEC5_pos+TEC6_pos', 'TEC6_pos+TEC7_pos', 'TEC7_pos+TEC8_pos', 'TEC8_pos+TEC9_pos'),
        TEC = cms.PSet(
            minRing = cms.int32(5),
            matchedRecHits = cms.InputTag("siStripMatchedRecHits","matchedRecHit"),
            useRingSlector = cms.untracked.bool(True),
            TTRHBuilder = cms.string('WithTrackAngle'),
            rphiRecHits = cms.InputTag("siStripMatchedRecHits","rphiRecHit"),
            maxRing = cms.int32(7)
        )
    ),
    PropagationDirection = cms.string('alongMomentum'),
    NavigationDirection = cms.string('outsideIn')
), cms.PSet(
    ComponentName = cms.string('GenericPairGenerator'),
    LayerPSet = cms.PSet(
        layerList = cms.vstring('TEC1_pos+TEC2_pos', 'TEC2_pos+TEC3_pos', 'TEC3_pos+TEC4_pos', 'TEC4_pos+TEC5_pos', 'TEC5_pos+TEC6_pos', 'TEC6_pos+TEC7_pos', 'TEC7_pos+TEC8_pos', 'TEC8_pos+TEC9_pos'),
        TEC = cms.PSet(
            minRing = cms.int32(5),
            matchedRecHits = cms.InputTag("siStripMatchedRecHits","matchedRecHit"),
            useRingSlector = cms.untracked.bool(True),
            TTRHBuilder = cms.string('WithTrackAngle'),
            rphiRecHits = cms.InputTag("siStripMatchedRecHits","rphiRecHit"),
            maxRing = cms.int32(7)
        )
    ),
    PropagationDirection = cms.string('alongMomentum'),
    NavigationDirection = cms.string('insideOut')
), cms.PSet(
    ComponentName = cms.string('GenericPairGenerator'),
    LayerPSet = cms.PSet(
        layerList = cms.vstring('TEC1_neg+TEC2_neg', 'TEC2_neg+TEC3_neg', 'TEC3_neg+TEC4_neg', 'TEC4_neg+TEC5_neg', 'TEC5_neg+TEC6_neg', 'TEC6_neg+TEC7_neg', 'TEC7_neg+TEC8_neg', 'TEC8_neg+TEC9_neg'),
        TEC = cms.PSet(
            minRing = cms.int32(5),
            matchedRecHits = cms.InputTag("siStripMatchedRecHits","matchedRecHit"),
            useRingSlector = cms.untracked.bool(True),
            TTRHBuilder = cms.string('WithTrackAngle'),
            rphiRecHits = cms.InputTag("siStripMatchedRecHits","rphiRecHit"),
            maxRing = cms.int32(7)
        )
    ),
    PropagationDirection = cms.string('alongMomentum'),
    NavigationDirection = cms.string('outsideIn')
), cms.PSet(
    ComponentName = cms.string('GenericPairGenerator'),
    LayerPSet = cms.PSet(
        layerList = cms.vstring('TEC1_neg+TEC2_neg', 'TEC2_neg+TEC3_neg', 'TEC3_neg+TEC4_neg', 'TEC4_neg+TEC5_neg', 'TEC5_neg+TEC6_neg', 'TEC6_neg+TEC7_neg', 'TEC7_neg+TEC8_neg', 'TEC8_neg+TEC9_neg'),
        TEC = cms.PSet(
            minRing = cms.int32(5),
            matchedRecHits = cms.InputTag("siStripMatchedRecHits","matchedRecHit"),
            useRingSlector = cms.untracked.bool(True),
            TTRHBuilder = cms.string('WithTrackAngle'),
            rphiRecHits = cms.InputTag("siStripMatchedRecHits","rphiRecHit"),
            maxRing = cms.int32(7)
        )
    ),
    PropagationDirection = cms.string('alongMomentum'),
    NavigationDirection = cms.string('insideOut')
))

