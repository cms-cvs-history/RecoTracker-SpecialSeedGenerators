#include <cmath>
#include "RecoTracker/SpecialSeedGenerators/interface/CosmicTrackingRegion.h"
#include "RecoTracker/TkTrackingRegions/interface/OuterEstimator.h"

#include "TrackingTools/TrajectoryParametrization/interface/GlobalTrajectoryParameters.h"
#include "TrackingTools/TrajectoryParametrization/interface/LocalTrajectoryParameters.h"
#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"
#include "TrackingTools/GeomPropagators/interface/AnalyticalPropagator.h"
#include "TrackingTools/GeomPropagators/interface/StraightLinePropagator.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"

#include "TrackingTools/DetLayers/interface/BarrelDetLayer.h"
#include "TrackingTools/DetLayers/interface/ForwardDetLayer.h"
#include "TrackingTools/DetLayers/interface/DetLayer.h"
#include "RecoTracker/TkTrackingRegions/interface/HitRCheck.h"
#include "RecoTracker/TkTrackingRegions/interface/HitZCheck.h"
#include "RecoTracker/TkTrackingRegions/interface/HitEtaCheck.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "MagneticField/Engine/interface/MagneticField.h"

#include "RecoTracker/Record/interface/CkfComponentsRecord.h"
#include "RecoTracker/MeasurementDet/interface/MeasurementTracker.h"
#include "TrackingTools/MeasurementDet/interface/LayerMeasurements.h"
#include "TrackingTools/PatternTools/interface/TrajectoryMeasurement.h"
#include "DataFormats/GeometrySurface/interface/BoundPlane.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHit.h"

#include "../interface/EtaPhiMeasurementEstimator.h"
#include "TrackingTools/TrackAssociator/interface/DetIdInfo.h"

template <class T> T sqr( T t) {return t*t;}


using namespace std;
using namespace ctfseeding; 


TrackingRegion::Hits CosmicTrackingRegion::hits(const edm::Event& ev,
						const edm::EventSetup& es,
						const  ctfseeding::SeedingLayer* layer) const
{

  //get and name collections
  //++++++++++++++++++++++++

  //tracking region
  TrackingRegion::Hits result;

  //detector layer
  const DetLayer * detLayer = layer->detLayer();
  LogDebug("CosmicTrackingRegion") << "Looking at hits on subdet/layer " << layer->name();
  EtaPhiMeasurementEstimator est(0.3,0.3);

  //magnetic field
  edm::ESHandle<MagneticField> field;
  es.get<IdealMagneticFieldRecord>().get(field);
  const MagneticField * magField = field.product();

  //region
  const GlobalPoint vtx = origin();
  GlobalVector dir = direction();
  LogDebug("CosmicTrackingRegion") <<"The initial region characteristics are:" << "\n"
				   <<" Origin    = " << origin() << "\n"
				   <<" Direction = " << direction() << "\n" 
				   <<" Eta = " << origin().eta()  << "\n" 
				   <<" Phi = " << origin().phi();
     
  //trajectory state on surface
  float phi = dir.phi();
  Surface::RotationType rot( sin(phi), -cos(phi),           0,
                             0,                0,          -1,
                             cos(phi),  sin(phi),           0);

  Plane::PlanePointer surface = Plane::build(vtx, rot);
  FreeTrajectoryState fts( GlobalTrajectoryParameters(vtx, dir, 1, magField) );
  TrajectoryStateOnSurface tsos(fts, *surface);
  LogDebug("CosmicTrackingRegion") 
    << "The state used to find measurement with the measurement tracker is:\n" << tsos;

  //propagator
  //StraightLinePropagator prop( magField, alongMomentum);
  AnalyticalPropagator prop( magField, alongMomentum);

  //propagation verification
  //+++++++++++++++++++++++++

  //action
  TrajectoryStateOnSurface stateOnLayer = prop.propagate( *tsos.freeState(),
							  detLayer->surface());
  
  //debug
  if (stateOnLayer.isValid()){
    LogDebug("CosmicTrackingRegion") << "The initial state propagates to the layer surface: \n" << stateOnLayer
    << "R   = " << stateOnLayer.globalPosition().perp() << "\n"
    << "Eta = " << stateOnLayer.globalPosition().eta() << "\n"
    << "Phi = " << stateOnLayer.globalPosition().phi();

  }
  else{
    LogDebug("CosmicTrackingRegion") << "The initial state does not propagate to the layer surface.";
  }

  
  typedef DetLayer::DetWithState DetWithState;
  vector<DetWithState> compatDets = detLayer->compatibleDets(tsos, prop, est);
  LogDebug("CosmicTrackingRegion") << "Compatible dets = " << compatDets.size();
  

  //get hits
  //++++++++

  //measurement tracker (find hits)
  edm::ESHandle<MeasurementTracker> measurementTrackerESH;
  es.get<CkfComponentsRecord>().get(measurementTrackerESH);
  const MeasurementTracker * measurementTracker = measurementTrackerESH.product(); 
  measurementTracker->update(ev);
  LayerMeasurements lm(measurementTracker);
  vector<TrajectoryMeasurement> meas = lm.measurements(*detLayer, tsos, prop, est);
  LogDebug("CosmicTrackingRegion") << "Number of Trajectory measurements = " << meas.size()
				   <<" but the last one is always an invalid hit, by construction.";

  //trajectory measurement
  typedef vector<TrajectoryMeasurement>::const_iterator IM;
  for (IM im = meas.begin(); im != meas.end(); im++) {//loop on measurement tracker
    TrajectoryMeasurement::ConstRecHitPointer ptrHit = im->recHit();
    if (ptrHit->isValid()) { 
      LogDebug("CosmicTrackingRegion") << "Hit found in the region at position: "<<ptrHit->globalPosition();
	result.push_back(  ptrHit );
    }//end if isValid()
    else LogDebug("CosmicTrackingRegion") << "No valid hit";
  }//end loop on measurement tracker

  
  //result
  //++++++

  return result;
}

