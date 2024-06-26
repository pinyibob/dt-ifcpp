/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4X3/include/IfcDataOriginEnum.h"
#include "ifcpp/IFC4X3/include/IfcDate.h"
#include "ifcpp/IFC4X3/include/IfcLabel.h"
#include "ifcpp/IFC4X3/include/IfcRecurrencePattern.h"
#include "ifcpp/IFC4X3/include/IfcWorkTime.h"

// ENTITY IfcWorkTime 
IFC4X3::IfcWorkTime::IfcWorkTime( int tag ) { m_tag = tag; }
void IFC4X3::IfcWorkTime::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_tag << "= IFCWORKTIME" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_DataOrigin ) { m_DataOrigin->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_UserDefinedDataOrigin ) { m_UserDefinedDataOrigin->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RecurrencePattern ) { stream << "#" << m_RecurrencePattern->m_tag; } else { stream << "$"; }
	stream << ",";
	if( m_StartDate ) { m_StartDate->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_FinishDate ) { m_FinishDate->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IFC4X3::IfcWorkTime::getStepParameter( std::stringstream& stream, bool /*is_select_type*/ ) const { stream << "#" << m_tag; }
void IFC4X3::IfcWorkTime::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<BuildingEntity> >& map, std::stringstream& errorStream )
{
	const size_t num_args = args.size();
	if( num_args > 0 ){m_Name = IfcLabel::createObjectFromSTEP( args[0], map, errorStream );}
	if( num_args > 1 ){m_DataOrigin = IfcDataOriginEnum::createObjectFromSTEP( args[1], map, errorStream );}
	if( num_args > 2 ){m_UserDefinedDataOrigin = IfcLabel::createObjectFromSTEP( args[2], map, errorStream );}
	if( num_args > 3 ){readEntityReference( args[3], m_RecurrencePattern, map, errorStream );}
	if( num_args > 4 ){m_StartDate = IfcDate::createObjectFromSTEP( args[4], map, errorStream );}
	if( num_args > 5 ){m_FinishDate = IfcDate::createObjectFromSTEP( args[5], map, errorStream );}
	if( num_args != 6 ){ errorStream << "Wrong parameter count for entity IfcWorkTime, expecting 6, having " << num_args << ". Entity ID: " << m_tag << std::endl; }
}
void IFC4X3::IfcWorkTime::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IFC4X3::IfcSchedulingTime::getAttributes( vec_attributes );
	vec_attributes.emplace_back( std::make_pair( "RecurrencePattern", m_RecurrencePattern ) );
	vec_attributes.emplace_back( std::make_pair( "StartDate", m_StartDate ) );
	vec_attributes.emplace_back( std::make_pair( "FinishDate", m_FinishDate ) );
}
void IFC4X3::IfcWorkTime::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IFC4X3::IfcSchedulingTime::getAttributesInverse( vec_attributes_inverse );
}
void IFC4X3::IfcWorkTime::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcSchedulingTime::setInverseCounterparts( ptr_self_entity );
}
void IFC4X3::IfcWorkTime::unlinkFromInverseCounterparts()
{
	IfcSchedulingTime::unlinkFromInverseCounterparts();
}
