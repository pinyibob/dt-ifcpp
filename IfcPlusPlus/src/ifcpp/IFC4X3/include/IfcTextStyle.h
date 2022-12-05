/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/GlobalDefines.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingObject.h"
#include "IfcPresentationStyle.h"
namespace IFC4X3
{
	class IFCQUERY_EXPORT IfcTextStyleForDefinedFont;
	class IFCQUERY_EXPORT IfcTextStyleTextModel;
	class IFCQUERY_EXPORT IfcTextFontSelect;
	class IFCQUERY_EXPORT IfcBoolean;
	//ENTITY
	class IFCQUERY_EXPORT IfcTextStyle : public IfcPresentationStyle
	{ 
	public:
		IfcTextStyle() = default;
		IfcTextStyle( int id );
		virtual void getStepLine( std::stringstream& stream ) const;
		virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
		virtual void readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<BuildingEntity> >& map, std::stringstream& errorStream );
		virtual void setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self );
		virtual uint8_t getNumAttributes() const { return 5; }
		virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const;
		virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const;
		virtual void unlinkFromInverseCounterparts();
		virtual uint32_t classID() const { return 1447204868; }

		// IfcPresentationStyle -----------------------------------------------------------
		// attributes:
		//  shared_ptr<IfcLabel>					m_Name;						//optional

		// IfcTextStyle -----------------------------------------------------------
		// attributes:
		shared_ptr<IfcTextStyleForDefinedFont>	m_TextCharacterAppearance;	//optional
		shared_ptr<IfcTextStyleTextModel>		m_TextStyle;				//optional
		shared_ptr<IfcTextFontSelect>			m_TextFontStyle;
		shared_ptr<IfcBoolean>					m_ModelOrDraughting;		//optional
	};
}
