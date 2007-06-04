#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_SEAL_MODULE();
#include "RecoTracker/SpecialSeedGenerators/interface/CtfSpecialSeedGenerator.h"

DEFINE_ANOTHER_FWK_MODULE(CtfSpecialSeedGenerator);

#include "RecoTracker/TkTrackingRegions/interface/OrderedHitsGeneratorFactory.h"
#include "RecoTracker/TkTrackingRegions/interface/OrderedHitsGenerator.h"
#include "RecoTracker/SpecialSeedGenerators/interface/GenericTripletGenerator.h"
#include "RecoTracker/SpecialSeedGenerators/interface/GenericPairGenerator.h"

DEFINE_SEAL_PLUGIN(OrderedHitsGeneratorFactory, GenericTripletGenerator, "GenericTripletGenerator");
DEFINE_SEAL_PLUGIN(OrderedHitsGeneratorFactory, GenericPairGenerator, "GenericPairGenerator");

