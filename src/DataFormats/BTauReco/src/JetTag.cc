#include "DataFormats/BTauReco/interface/JetTag.h"

reco::JetTag::JetTag() : m_discriminator ( 0. )
{}

reco::JetTag::JetTag( double d ) : m_discriminator ( d )
{}

double reco::JetTag::discriminator() const
{
  return m_discriminator;
}

reco::JetTag * reco::JetTag::clone() const
{
  return new reco::JetTag ( *this );
}

reco::JetTag::~JetTag() {}
