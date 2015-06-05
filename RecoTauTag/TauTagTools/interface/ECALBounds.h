#ifndef RecoTauTag_TauTagTools_ECALBounds_H
#define RecoTauTag_TauTagTools_ECALBounds_H

#include "DataFormats/GeometrySurface/interface/ReferenceCounted.h"
#include "DataFormats/GeometrySurface/interface/Surface.h"
#include "DataFormats/GeometrySurface/interface/BoundCylinder.h"
#include "DataFormats/GeometrySurface/interface/BoundDisk.h"
#include "DataFormats/GeometrySurface/interface/SimpleCylinderBounds.h"
#include "DataFormats/GeometrySurface/interface/SimpleDiskBounds.h"

class Cylinder;
class Disk;

class ECALBounds {
public:
  ECALBounds();
  ~ECALBounds();

  const Cylinder& barrelBound() const    { return *theCylinder;}
  const Plane& negativeEndcapDisk() const {return *theNegativeDisk;}
  const Plane& positiveEndcapDisk() const { return *thePositiveDisk;}
  /** Hard-wired numbers defining the envelope of the sensitive volumes.
   */
  float barrel_innerradius() const    {return 129.0f;}
  float barrel_outerradius() const     {return 175.f;}
  float barrel_halfLength() const {return 270.89f;}
  float endcap_innerradius() const     {return 31.6f;}
  float endcap_outerradius() const     {return 171.1f;}
  float endcap_innerZ() const {return 314.40f;}
  float endcap_outerZ() const {return 388.f;}
  /** Hard-wired numbers defining eta cracks.
   */
  std::pair<float,float> crack_absEtaIntervalA() const {return std::pair<float,float>(0.000f,0.018f);}
  std::pair<float,float> crack_absEtaIntervalB() const {return std::pair<float,float>(0.423f,0.461f);}
  std::pair<float,float> crack_absEtaIntervalC() const {return std::pair<float,float>(0.770f,0.806f);}
  std::pair<float,float> crack_absEtaIntervalD() const {return std::pair<float,float>(1.127f,1.163f);}
  std::pair<float,float> crack_absEtaIntervalE() const {return std::pair<float,float>(1.460f,1.558f);}
 private:
  //BoundCylinder* theCylinder;
  //BoundDisk* theNegativeDisk;
  //BoundDisk* thePositiveDisk;
  Cylinder* theCylinder;
  SimpleDiskBounds* theNegativeDiskBounds;
  Plane* theNegativeDisk;
  SimpleDiskBounds* thePositiveDiskBounds;
  Plane* thePositiveDisk;
};

#endif
