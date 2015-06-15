#ifndef DataFormats_BTauReco_SecondaryVertexTagInfo_h
#define DataFormats_BTauReco_SecondaryVertexTagInfo_h

#include <utility>
#include <vector>

#include "DataFormats/BTauReco/interface/RefMacros.h"
#include "DataFormats/BTauReco/interface/BaseTagInfo.h"
#include "DataFormats/BTauReco/interface/TaggingVariable.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/Measurement1D.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"

namespace reco {
 
class SecondaryVertexTagInfo : public BaseTagInfo {
    public:
	struct VertexData {
		Vertex				vertex;
		Measurement1D			dist2d, dist3d;
		GlobalVector			direction;
	};

	SecondaryVertexTagInfo() {}
	virtual ~SecondaryVertexTagInfo() {}

	SecondaryVertexTagInfo(
			const std::vector<VertexData> &svData,
			unsigned int vertexCandidates );

	const Vertex &secondaryVertex(unsigned int index) const
	{ return m_svData[index].vertex; }

	unsigned int nVertices() const { return m_svData.size(); }
	unsigned int nVertexCandidates() const { return m_vertexCandidates; }

	Measurement1D flightDistance(unsigned int index, bool in2d = false) const
	{ return in2d ? m_svData[index].dist2d : m_svData[index].dist3d; }
	const GlobalVector & flightDirection(unsigned int index) const
	{ return m_svData[index].direction; }

	virtual TaggingVariableList taggingVariables() const;

    private:
	std::vector<VertexData>			m_svData;
	unsigned int				m_vertexCandidates;
};

}

#endif // DataFormats_BTauReco_SecondaryVertexTagInfo_h
