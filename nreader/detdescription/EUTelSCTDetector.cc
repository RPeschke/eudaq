
// personal includes ".h"
#include "EUTELESCOPE.h"
#include "EUTelSCTDetector.h"

// system includes <>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;
using namespace eutelescope;

EUTelSCTDetector::EUTelSCTDetector() : EUTelPixelDetector()  {

  _xMin = 0;
  _xMax = 3;

  _yMin = 0;
  _yMax = 1279;



  _signalPolarity = 1;

  _name = "SCTUpgrade";

  _xPitch = 0.01;
  _yPitch = 0.01;//74.5 micro m //$$ todo change value find out which unit


}

bool EUTelSCTDetector::hasSubChannels() const {
  if (  _subChannelsWithoutMarkers.size() != 0 ) return true;
  else return false;
}

std::vector< EUTelROI > EUTelSCTDetector::getSubChannels( bool withMarker ) const {

  if ( withMarker ) return _subChannelsWithMarkers;
  else  return _subChannelsWithoutMarkers;

}

EUTelROI EUTelSCTDetector::getSubChannelBoundary( size_t iChan, bool withMarker ) const {
  if ( withMarker ) return _subChannelsWithMarkers.at( iChan );
  else return _subChannelsWithoutMarkers.at( iChan );

}


void EUTelSCTDetector::setMode( string mode ) {

  _mode = mode;

}

void EUTelSCTDetector::print( ostream& os ) const {

  os << "hello from EUTelSCTDetector \n";
  
}
