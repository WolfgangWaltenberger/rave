#include <functional>
#include <algorithm>

#ifndef DISABLE_GNU_EXTENSIONS
//#include <ext/functional>
#endif

#include "FWCore/Utilities/interface/EDMException.h"
#include "DataFormats/GeometryVector/interface/VectorUtil.h"
#include "DataFormats/BTauReco/interface/TaggingVariable.h"

#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"

using namespace reco;

SecondaryVertexTagInfo::SecondaryVertexTagInfo(
		const std::vector<VertexData> &svData,
		unsigned int vertexCandidates ) :
	m_svData(svData),
	m_vertexCandidates(vertexCandidates)
{
}

TaggingVariableList SecondaryVertexTagInfo::taggingVariables() const
{
	TaggingVariableList vars;

	for(std::vector<VertexData>::const_iterator iter = m_svData.begin();
	    iter != m_svData.end(); iter++) {
		vars.insert(btau::flightDistance2dVal,
					iter->dist2d.value(), true);
		vars.insert(btau::flightDistance2dSig,
					iter->dist2d.significance(), true);
		vars.insert(btau::flightDistance3dVal,
					iter->dist3d.value(), true);
		vars.insert(btau::flightDistance3dSig,
					iter->dist3d.significance(), true);
	}

	vars.insert(btau::jetNSecondaryVertices, m_vertexCandidates, true);

	vars.finalize();
	return vars;
}
