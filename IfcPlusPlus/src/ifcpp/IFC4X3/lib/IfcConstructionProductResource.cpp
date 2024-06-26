/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4X3/include/IfcAppliedValue.h"
#include "ifcpp/IFC4X3/include/IfcConstructionProductResource.h"
#include "ifcpp/IFC4X3/include/IfcConstructionProductResourceTypeEnum.h"
#include "ifcpp/IFC4X3/include/IfcGloballyUniqueId.h"
#include "ifcpp/IFC4X3/include/IfcIdentifier.h"
#include "ifcpp/IFC4X3/include/IfcLabel.h"
#include "ifcpp/IFC4X3/include/IfcOwnerHistory.h"
#include "ifcpp/IFC4X3/include/IfcPhysicalQuantity.h"
#include "ifcpp/IFC4X3/include/IfcRelAggregates.h"
#include "ifcpp/IFC4X3/include/IfcRelAssigns.h"
#include "ifcpp/IFC4X3/include/IfcRelAssignsToResource.h"
#include "ifcpp/IFC4X3/include/IfcRelAssociates.h"
#include "ifcpp/IFC4X3/include/IfcRelDeclares.h"
#include "ifcpp/IFC4X3/include/IfcRelDefinesByObject.h"
#include "ifcpp/IFC4X3/include/IfcRelDefinesByProperties.h"
#include "ifcpp/IFC4X3/include/IfcRelDefinesByType.h"
#include "ifcpp/IFC4X3/include/IfcRelNests.h"
#include "ifcpp/IFC4X3/include/IfcResourceTime.h"
#include "ifcpp/IFC4X3/include/IfcText.h"

// ENTITY IfcConstructionProductResource 
IFC4X3::IfcConstructionProductResource::IfcConstructionProductResource( int tag ) { m_tag = tag; }
void IFC4X3::IfcConstructionProductResource::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_tag << "= IFCCONSTRUCTIONPRODUCTRESOURCE" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_tag; } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ObjectType ) { m_ObjectType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Identification ) { m_Identification->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LongDescription ) { m_LongDescription->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Usage ) { stream << "#" << m_Usage->m_tag; } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_BaseCosts );
	stream << ",";
	if( m_BaseQuantity ) { stream << "#" << m_BaseQuantity->m_tag; } else { stream << "$"; }
	stream << ",";
	if( m_PredefinedType ) { m_PredefinedType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IFC4X3::IfcConstructionProductResource::getStepParameter( std::stringstream& stream, bool /*is_select_type*/ ) const { stream << "#" << m_tag; }
void IFC4X3::IfcConstructionProductResource::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<BuildingEntity> >& map, std::stringstream& errorStream )
{
	const size_t num_args = args.size();
	if( num_args > 0 ){m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0], map, errorStream );}
	if( num_args > 1 ){readEntityReference( args[1], m_OwnerHistory, map, errorStream );}
	if( num_args > 2 ){m_Name = IfcLabel::createObjectFromSTEP( args[2], map, errorStream );}
	if( num_args > 3 ){m_Description = IfcText::createObjectFromSTEP( args[3], map, errorStream );}
	if( num_args > 4 ){m_ObjectType = IfcLabel::createObjectFromSTEP( args[4], map, errorStream );}
	if( num_args > 5 ){m_Identification = IfcIdentifier::createObjectFromSTEP( args[5], map, errorStream );}
	if( num_args > 6 ){m_LongDescription = IfcText::createObjectFromSTEP( args[6], map, errorStream );}
	if( num_args > 7 ){readEntityReference( args[7], m_Usage, map, errorStream );}
	if( num_args > 8 ){readEntityReferenceList( args[8], m_BaseCosts, map, errorStream );}
	if( num_args > 9 ){readEntityReference( args[9], m_BaseQuantity, map, errorStream );}
	if( num_args > 10 ){m_PredefinedType = IfcConstructionProductResourceTypeEnum::createObjectFromSTEP( args[10], map, errorStream );}
	if( num_args != 11 ){ errorStream << "Wrong parameter count for entity IfcConstructionProductResource, expecting 11, having " << num_args << ". Entity ID: " << m_tag << std::endl; }
}
void IFC4X3::IfcConstructionProductResource::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IFC4X3::IfcConstructionResource::getAttributes( vec_attributes );
	vec_attributes.emplace_back( std::make_pair( "PredefinedType", m_PredefinedType ) );
}
void IFC4X3::IfcConstructionProductResource::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IFC4X3::IfcConstructionResource::getAttributesInverse( vec_attributes_inverse );
}
void IFC4X3::IfcConstructionProductResource::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcConstructionResource::setInverseCounterparts( ptr_self_entity );
}
void IFC4X3::IfcConstructionProductResource::unlinkFromInverseCounterparts()
{
	IfcConstructionResource::unlinkFromInverseCounterparts();
}
