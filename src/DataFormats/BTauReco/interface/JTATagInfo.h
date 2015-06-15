#ifndef DataFormats_BTauReco_JTATagInfo_h
#define DataFormats_BTauReco_JTATagInfo_h

#include "DataFormats/BTauReco/interface/RefMacros.h"
#include "DataFormats/BTauReco/interface/BaseTagInfo.h"

namespace reco {
 
class JTATagInfo : public BaseTagInfo {
public:
  
  JTATagInfo(void) : m_jetTracksAssociation() { }

  virtual ~JTATagInfo(void) { }
  
  virtual JTATagInfo* clone(void) const { return new JTATagInfo(*this); }

  virtual TrackRefVector          tracks(void) const { return m_jetTracksAssociation->second; }

  virtual bool hasTracks(void) const { return true; }
  
};

DECLARE_EDM_REFS( JTATagInfo )

}

#endif // DataFormats_BTauReco_JTATagInfo_h
