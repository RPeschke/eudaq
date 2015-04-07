#include "eudaq/DataConverterPlugin.hh"
#include "eudaq/PluginManager.hh"

#include <iostream>

namespace eudaq {

  unsigned DataConverterPlugin::GetTriggerID(eudaq::Event const &) const {
    return (unsigned)-1;
  }

  DataConverterPlugin::DataConverterPlugin(std::string subtype)
    : m_eventtype(make_pair(Event::str2id("_RAW"), subtype))
  {
    //std::cout << "DEBUG: Registering DataConverterPlugin: " << Event::id2str(m_eventtype.first) << ":" << m_eventtype.second << std::endl;
    PluginManager::GetInstance().RegisterPlugin(this);
  }

  DataConverterPlugin::DataConverterPlugin(unsigned type, std::string subtype)
    : m_eventtype(make_pair(type, subtype))
  {
    //std::cout << "DEBUG: Registering DataConverterPlugin: " << Event::id2str(m_eventtype.first) << ":" << m_eventtype.second << std::endl;
    PluginManager::GetInstance().RegisterPlugin(this);
  }

  std::shared_ptr<eutelescope::EUTelTrackerDataInterfacerImpl<eutelescope::EUTelGenericSparsePixel>> ConverPlaneToLCIOGenericPixel(StandardPlane & plane, lcio::TrackerDataImpl& zsFrame)
  {
    // this is the structure that will host the sparse pixel
    auto sparseFrame = std::make_shared<eutelescope::EUTelTrackerDataInterfacerImpl<eutelescope::EUTelGenericSparsePixel>>(&zsFrame);

#ifdef _DEBUG

    eutelescope::EUTelGenericSparsePixel thisHit(0, 0, 0, 0); //empty pixel so that the collection is never empty. LCIO crashes in debug mode when the collection is empty 
    sparseFrame->addSparsePixel(&thisHit);
#endif // _DEBUG

    for (size_t iPixel = 0; iPixel < plane.HitPixels(); ++iPixel) {
      eutelescope::EUTelGenericSparsePixel thisHit1(plane.GetX(iPixel), plane.GetY(iPixel), plane.GetPixel(iPixel), 0);
      sparseFrame->addSparsePixel(&thisHit1);
    }
    return sparseFrame;
  }

}//namespace eudaq
