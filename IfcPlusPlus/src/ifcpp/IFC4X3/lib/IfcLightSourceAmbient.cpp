/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4X3/include/IfcColourRgb.h"
#include "ifcpp/IFC4X3/include/IfcLabel.h"
#include "ifcpp/IFC4X3/include/IfcLightSourceAmbient.h"
#include "ifcpp/IFC4X3/include/IfcNormalisedRatioMeasure.h"
#include "ifcpp/IFC4X3/include/IfcPresentationLayerAssignment.h"
#include "ifcpp/IFC4X3/include/IfcStyledItem.h"

// ENTITY IfcLightSourceAmbient 
IFC4X3::IfcLightSourceAmbient::IfcLightSourceAmbient( int tag ) { m_tag = tag; }
void IFC4X3::IfcLightSourceAmbient::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_tag << "= IFCLIGHTSOURCEAMBIENT" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LightColour ) { stream << "#" << m_LightColour->m_tag; } else { stream << "$"; }
	stream << ",";
	if( m_AmbientIntensity ) { m_AmbientIntensity->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Intensity ) { m_Intensity->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IFC4X3::IfcLightSourceAmbient::getStepParameter( std::stringstream& stream, bool /*is_select_type*/ ) const { stream << "#" << m_tag; }
void IFC4X3::IfcLightSourceAmbient::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<BuildingEntity> >& map, std::stringstream& errorStream )
{
	const size_t num_args = args.size();
	if( num_args > 0 ){m_Name = IfcLabel::createObjectFromSTEP( args[0], map, errorStream );}
	if( num_args > 1 ){readEntityReference( args[1], m_LightColour, map, errorStream );}
	if( num_args > 2 ){m_AmbientIntensity = IfcNormalisedRatioMeasure::createObjectFromSTEP( args[2], map, errorStream );}
	if( num_args > 3 ){m_Intensity = IfcNormalisedRatioMeasure::createObjectFromSTEP( args[3], map, errorStream );}
	if( num_args != 4 ){ errorStream << "Wrong parameter count for entity IfcLightSourceAmbient, expecting 4, having " << num_args << ". Entity ID: " << m_tag << std::endl; }
}
void IFC4X3::IfcLightSourceAmbient::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IFC4X3::IfcLightSource::getAttributes( vec_attributes );
}
void IFC4X3::IfcLightSourceAmbient::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IFC4X3::IfcLightSource::getAttributesInverse( vec_attributes_inverse );
}
void IFC4X3::IfcLightSourceAmbient::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcLightSource::setInverseCounterparts( ptr_self_entity );
}
void IFC4X3::IfcLightSourceAmbient::unlinkFromInverseCounterparts()
{
	IfcLightSource::unlinkFromInverseCounterparts();
}
