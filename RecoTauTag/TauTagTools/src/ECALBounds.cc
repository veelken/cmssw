#include "RecoTauTag/TauTagTools/interface/ECALBounds.h"

#include <iostream>
#include <iomanip>

namespace {
  constexpr float epsilon = 0.001; // should not matter at all
  const Surface::RotationType rot; // unit rotation matrix
}

ECALBounds::ECALBounds()
{
  //std::cout << "<ECALBounds::ECALBounds>:" << std::endl;
  //theCylinder = new BoundCylinder(ECALBounds::barrel_innerradius(), Surface::PositionType(0,0,0),rot, 
  //				  new SimpleCylinderBounds(ECALBounds::barrel_innerradius()-epsilon, 
  //							   ECALBounds::barrel_innerradius()+epsilon, 
  //							   -ECALBounds::barrel_halfLength(), 
  //							   ECALBounds::barrel_halfLength()));
  //theNegativeDisk = new BoundDisk(Surface::PositionType(0,0,-ECALBounds::endcap_innerZ()),rot, 
  //				    new SimpleDiskBounds(0,ECALBounds::endcap_outerradius(),-epsilon,epsilon));
  //thePositiveDisk = new BoundDisk(Surface::PositionType(0,0,ECALBounds::endcap_innerZ()),rot, 
  //		                    new SimpleDiskBounds(0,ECALBounds::endcap_outerradius(),-epsilon,epsilon));
  theCylinder = new Cylinder(Surface::PositionType(0,0,0),rot,
			     SimpleCylinderBounds(ECALBounds::barrel_innerradius()-epsilon, 
						  ECALBounds::barrel_innerradius()+epsilon, 
						  -ECALBounds::barrel_halfLength(), 
						  ECALBounds::barrel_halfLength()));
  theNegativeDiskBounds = new SimpleDiskBounds(0,ECALBounds::endcap_outerradius(),-epsilon,epsilon);
  theNegativeDisk = new Plane(Surface::PositionType(0,0,-ECALBounds::endcap_innerZ()),rot, 
			      theNegativeDiskBounds);
  thePositiveDiskBounds = new SimpleDiskBounds(0,ECALBounds::endcap_outerradius(),-epsilon,epsilon);
  thePositiveDisk = new Plane(Surface::PositionType(0,0,ECALBounds::endcap_innerZ()),rot, 
			      thePositiveDiskBounds);
}

ECALBounds::~ECALBounds()
{
  //std::cout << "<ECALBounds::~ECALBounds>:" << std::endl;
  delete theCylinder;
  delete theNegativeDiskBounds;
  delete theNegativeDisk;
  delete thePositiveDiskBounds;
  delete thePositiveDisk;
}
