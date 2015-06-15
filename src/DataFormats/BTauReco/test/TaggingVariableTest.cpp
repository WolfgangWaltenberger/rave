#include "DataFormats/BTauReco/interface/TaggingVariable.h"
#include <iostream>

using namespace std;

int main()
{
  cout << "trackPparRatio: " << reco::TaggingVariableDescription[reco::btag::trackPparRatio] << endl;
  cout << "category: " << reco::TaggingVariableDescription[reco::btag::vertexCategory] << endl;
  cout << "trackSip2dAbCharm: " << reco::TaggingVariableDescription[reco::btag::trackSip2dAbCharm] << endl;
  cout << "jetEta: " << reco::TaggingVariableDescription[reco::btag::jetEta] << endl;
  cout << endl;
  cout << "jetEta:" << reco::TaggingVariableTokens[reco::btag::jetEta] << endl;
  cout << "jetEta:" << reco::TaggingVariableTokens[ reco::getTaggingVariableName("jetEta") ] << endl;

}
