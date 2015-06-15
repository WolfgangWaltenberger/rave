#ifndef BTauReco_JetTag_h
#define BTauReco_JetTag_h

/** Rave's JetTag version
  *
  */

namespace reco {
  class JetTag {
    public:
      JetTag();
      JetTag ( double discriminator );
      virtual JetTag * clone() const;
      virtual ~JetTag();
      double discriminator() const;
    private:
      double m_discriminator;
  };
}

#endif
