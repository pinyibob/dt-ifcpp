/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4X3/include/IfcIndexedPolygonalFaceWithVoids.h"
#include "ifcpp/IFC4X3/include/IfcPolygonalFaceSet.h"
#include "ifcpp/IFC4X3/include/IfcPositiveInteger.h"
#include "ifcpp/IFC4X3/include/IfcPresentationLayerAssignment.h"
#include "ifcpp/IFC4X3/include/IfcStyledItem.h"
#include "ifcpp/IFC4X3/include/IfcTextureCoordinateIndices.h"

// ENTITY IfcIndexedPolygonalFaceWithVoids 
IFC4X3::IfcIndexedPolygonalFaceWithVoids::IfcIndexedPolygonalFaceWithVoids( int tag ) { m_tag = tag; }
void IFC4X3::IfcIndexedPolygonalFaceWithVoids::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_tag << "= IFCINDEXEDPOLYGONALFACEWITHVOIDS" << "(";
	stream << "(";
	for( size_t ii = 0; ii < m_CoordIndex.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcPositiveInteger>& type_object = m_CoordIndex[ii];
		if( type_object )
		{
			type_object->getStepParameter( stream, false );
		}
		else
		{
			stream << "$";
		}
	}
	stream << ")";
	stream << ",";
	writeTypeOfIntList2D( stream, m_InnerCoordIndices, false );
	stream << ");";
}
void IFC4X3::IfcIndexedPolygonalFaceWithVoids::getStepParameter( std::stringstream& stream, bool /*is_select_type*/ ) const { stream << "#" << m_tag; }
void IFC4X3::IfcIndexedPolygonalFaceWithVoids::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<BuildingEntity> >& map, std::stringstream& errorStream )
{
	const size_t num_args = args.size();
	if( num_args > 0 ){readTypeOfIntegerList( args[0], m_CoordIndex );}
	if( num_args > 1 ){readTypeOfIntegerList2D( args[1], m_InnerCoordIndices );}
	if( num_args != 2 ){ errorStream << "Wrong parameter count for entity IfcIndexedPolygonalFaceWithVoids, expecting 2, having " << num_args << ". Entity ID: " << m_tag << std::endl; }
}
void IFC4X3::IfcIndexedPolygonalFaceWithVoids::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IFC4X3::IfcIndexedPolygonalFace::getAttributes( vec_attributes );
	shared_ptr<AttributeObjectVector> InnerCoordIndices_vector( new AttributeObjectVector() );
	vec_attributes.emplace_back( std::make_pair( "InnerCoordIndices", InnerCoordIndices_vector ) );
	for( size_t ii=0; ii<m_InnerCoordIndices.size(); ++ii )
	{
		const std::vector<shared_ptr<IfcPositiveInteger> >& vec_ii = m_InnerCoordIndices[ii];
		shared_ptr<AttributeObjectVector> inner_vector( new AttributeObjectVector() );
		InnerCoordIndices_vector->m_vec.push_back( inner_vector );
		std::copy(vec_ii.begin(), vec_ii.end(), std::back_inserter(inner_vector->m_vec));
	}
}
void IFC4X3::IfcIndexedPolygonalFaceWithVoids::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IFC4X3::IfcIndexedPolygonalFace::getAttributesInverse( vec_attributes_inverse );
}
void IFC4X3::IfcIndexedPolygonalFaceWithVoids::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcIndexedPolygonalFace::setInverseCounterparts( ptr_self_entity );
}
void IFC4X3::IfcIndexedPolygonalFaceWithVoids::unlinkFromInverseCounterparts()
{
	IfcIndexedPolygonalFace::unlinkFromInverseCounterparts();
}
