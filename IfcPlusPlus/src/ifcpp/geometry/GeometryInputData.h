﻿/* -*-c++-*- IfcQuery www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include <vector>
#include <ifcpp/geometry/AppearanceData.h>
#include <ifcpp/geometry/GeometrySettings.h>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BuildingException.h>
#include <ifcpp/IFC4X3/include/IfcObjectPlacement.h>
#include <ifcpp/IFC4X3/include/IfcProduct.h>
#include <ifcpp/IFC4X3/include/IfcRepresentation.h>
#include <ifcpp/IFC4X3/include/IfcTextStyle.h>
#include "IncludeCarveHeaders.h"
#include "GeomUtils.h"

struct MeshSetInfo
{
	MeshSetInfo()
	{
	}
	MeshSetInfo(const MeshSetInfo& other)
	{
		copyFromOther(other);
	}

	void copyFromOther(const MeshSetInfo& other)
	{
		numClosedEdges = other.numClosedEdges;
		numOpenEdges = other.numOpenEdges;
		numFaces = other.numFaces;
		zeroAreaFaces = other.zeroAreaFaces;
		degenerateEdges = other.degenerateEdges;
		finEdges = other.finEdges;
		finFaces = other.finFaces;
		surfaceArea = other.surfaceArea;
		allPointersValid = other.allPointersValid;
		meshSetValid = other.meshSetValid;
		maxNumEdgesExceeded = other.maxNumEdgesExceeded;
		maxNumberOfEdgesPerFace = other.maxNumberOfEdgesPerFace;
		details = other.details;
		report_callback = other.report_callback;
		entity = other.entity;
	}
	MeshSetInfo(StatusCallback* _report_callback, BuildingEntity* _entity)
	{
		report_callback = _report_callback;
		entity = _entity;
	}

	bool isEqual(const MeshSetInfo& other, bool checkOnlyNumberOfEdgesAndFaces = true) const
	{
		if (numClosedEdges != other.numClosedEdges) return false;
		if (numOpenEdges != other.numOpenEdges) return false;
		if (numFaces != other.numFaces) return false;
		if (checkOnlyNumberOfEdgesAndFaces)
		{
			if (zeroAreaFaces.size() != other.zeroAreaFaces.size() ) return false;
			if (degenerateEdges.size() != other.degenerateEdges.size()) return false;
			if (finEdges.size() != other.finEdges.size()) return false;
			if (finFaces.size() != other.finFaces.size()) return false;
		}
		else
		{
			if (zeroAreaFaces != other.zeroAreaFaces) return false;
			if (degenerateEdges != other.degenerateEdges) return false;
			if (finEdges != other.finEdges) return false;
			if (finFaces != other.finFaces) return false;
		}
		if (std::abs(surfaceArea - other.surfaceArea) > EPS_M12 ) return false;
		if (allPointersValid != other.allPointersValid) return false;
		if (meshSetValid != other.meshSetValid) return false;
		if (maxNumEdgesExceeded != other.maxNumEdgesExceeded) return false;
		if (maxNumberOfEdgesPerFace != other.maxNumberOfEdgesPerFace) return false;
		//if (details != other.details) return false;
		return true;
	}

	size_t numClosedEdges = 0;
	size_t numOpenEdges = 0;
	size_t numFaces = 0;
	std::set<carve::mesh::Face<3>* > zeroAreaFaces;
	std::set<carve::mesh::Edge<3>* > degenerateEdges;
	std::set<carve::mesh::Edge<3>* > finEdges;
	std::set<carve::mesh::Face<3>* > finFaces;
	double surfaceArea = 0;
	bool allPointersValid = true;
	bool meshSetValid = false;
	bool maxNumEdgesExceeded = false;
	size_t maxNumberOfEdgesPerFace = 0; // if it is 3, meshset is triangulated
	std::string details;
	StatusCallback* report_callback = nullptr;
	BuildingEntity* entity = nullptr;
	shared_ptr<carve::mesh::MeshSet<3> > meshset;

	void resetInfoResult()
	{
		numClosedEdges = 0;
		numOpenEdges = 0;
		numFaces = 0;
		zeroAreaFaces.clear();
		degenerateEdges.clear();
		finEdges.clear();
		finFaces.clear();
		surfaceArea = 0;
		allPointersValid = true;
		meshSetValid = false;
		maxNumEdgesExceeded = false;
		maxNumberOfEdgesPerFace = false;
		details = "";
	}
};

class TextItemData
{
public:
	std::string m_text;
	carve::math::Matrix m_text_position;
};

inline void premultMatrix( const carve::math::Matrix& matrix_to_append, carve::math::Matrix& target_matrix )
{
	target_matrix = matrix_to_append*target_matrix;
}
class PolyInputCache3D;

/**
*\brief Class TransformData: holds a matrix of a coordinate system and a pointer to the corresponding IFC placement entity
*/
class TransformData
{
public:
	TransformData(){}
	TransformData( const carve::math::Matrix& matrix, weak_ptr<BuildingEntity>& placement_entity, int placement_id ) : m_placement_tag( placement_id ), m_matrix(matrix), m_placement_entity(placement_entity)
	{
	}
	TransformData( const shared_ptr<TransformData>& other )
	{
		if( other )
		{
			m_matrix = other->m_matrix;
			m_placement_entity = other->m_placement_entity;
			m_placement_tag = other->m_placement_tag;
		}
	}
	bool isSameTransform( shared_ptr<TransformData>& transform_data )
	{
		if( transform_data )
		{
			if( !transform_data->m_placement_entity.expired() )
			{
				if( !m_placement_entity.expired() )
				{
					shared_ptr<BuildingEntity> ent1( m_placement_entity );
					shared_ptr<BuildingEntity> ent2( transform_data->m_placement_entity );
					return ent1 == ent2;
				}
			}
		}
		return false;
	}
	
	carve::math::Matrix			m_matrix;
	weak_ptr<BuildingEntity>	m_placement_entity;
	int							m_placement_tag = -1;
};

bool checkPolyhedronData(const shared_ptr<carve::input::PolyhedronData>& poly_data, const GeomProcessingParams& params, std::string& details);
bool fixPolyhedronData(const shared_ptr<carve::input::PolyhedronData>& poly_data, bool removeZeroAreaFaces, const GeomProcessingParams& params);
bool reverseFacesInPolyhedronData(const shared_ptr<carve::input::PolyhedronData>& poly_data);

class PolyInputCache3D
{
public:
	PolyInputCache3D( double epsMergePoints = -1 )
	{
		m_eps = epsMergePoints;
		m_poly_data = shared_ptr<carve::input::PolyhedronData>( new carve::input::PolyhedronData() );
	}

	size_t addPointZ( const vec3& pt, std::map<double, size_t>& map_z )
	{
		double vertex_z = pt.z;

		auto low = map_z.lower_bound(vertex_z);
		if (low == map_z.end()) 
		{
			if( map_z.size() > 0 )
			{
				double lastElement = map_z.rbegin()->first;
				double dz = lastElement - vertex_z;
				if( std::abs(dz) <= m_eps )
				{
					size_t existingIndex = map_z.rbegin()->second;
					return existingIndex;
				}
			}

			size_t vertex_index = m_poly_data->addVertex( pt );
			map_z.insert( {{ vertex_z, vertex_index } } );
			return vertex_index;
		}
		else if (low == map_z.begin())
		{
			double existingZ = low->first;
			double dz = existingZ - vertex_z;
			if( std::abs(dz) <= m_eps )
			{
				size_t& existingIndex = low->second;
				return existingIndex;
			}
			else
			{
				size_t vertex_index = m_poly_data->addVertex( pt );
				map_z.insert( {{ vertex_z, vertex_index }});
				return vertex_index;
			}
		}
		else
		{
			auto prev = std::prev(low);
			double dzPrev = vertex_z - prev->first;
			double dzLow = low->first - vertex_z;
			if( std::abs(dzPrev) < std::abs(dzLow) )
			{
				if( std::abs(dzPrev) <= m_eps )
				{
					size_t& existingIndex = prev->second;
					return existingIndex;
				}
				else
				{
					size_t vertex_index = m_poly_data->addVertex( pt );
					map_z.insert( {{ vertex_z, vertex_index }});
					return vertex_index;
				}
			}
			else
			{
				if( std::abs(dzLow) <= m_eps )
				{
					size_t& existingIndex = low->second;
					return existingIndex;
				}
				else
				{
					size_t vertex_index = m_poly_data->addVertex( pt );
					map_z.insert(  {{ vertex_z, vertex_index }} );
					return vertex_index;
				}
			}
		}
	}

	size_t addPointYZ( const vec3& pt, std::map<double, std::map<double, size_t> >& map_yz )
	{
		double vertex_y = pt.y;
		double vertex_z = pt.z;

		auto low = map_yz.lower_bound(vertex_y);
		if (low == map_yz.end()) 
		{
			if( map_yz.size() > 0 )
			{
				double lastElement = map_yz.rbegin()->first;
				double dy = lastElement - vertex_y;
				if( std::abs(dy) < m_eps )
				{
					auto& map_z = map_yz.rbegin()->second;
					return addPointZ( pt, map_z );
				}
			}

			size_t vertex_index = m_poly_data->addVertex( pt );
			map_yz.insert({{ vertex_y, {{ vertex_z, vertex_index }} } } );
			return vertex_index;
		}
		else if (low == map_yz.begin())
		{
			double existingY = low->first;
			double dy = existingY - vertex_y;
			if( std::abs(dy) <= m_eps )
			{
				std::map<double, size_t>& map_z = low->second;
				return addPointZ( pt, map_z );
			}
			else
			{
				size_t vertex_index = m_poly_data->addVertex( pt );
				map_yz.insert({{  vertex_y, {{ vertex_z, vertex_index }} } });
				return vertex_index;
			}
		}
		else
		{
			auto prev = std::prev(low);
			double dyPrev = vertex_y - prev->first;
			double dyLow = low->first - vertex_y;
			if( std::abs(dyPrev) <= std::abs(dyLow) )
			{
				if( std::abs(dyPrev) <= m_eps )
				{
					std::map<double, size_t>& map_z = prev->second;
					return addPointZ( pt, map_z );
				}
				else
				{
					size_t vertex_index = m_poly_data->addVertex( pt );
					map_yz.insert( {{ vertex_y, {{ vertex_z, vertex_index }} } });
					return vertex_index;
				}
			}
			else
			{
				if( std::abs(dyLow) <= m_eps )
				{
					std::map<double, size_t>& map_z = low->second;
					return addPointZ( pt, map_z );
				}
				else
				{
					size_t vertex_index = m_poly_data->addVertex( pt );
					map_yz.insert( {{ vertex_y, {{ vertex_z, vertex_index }} } } );
					return vertex_index;
				}
			}
		}
	}
	
	size_t addPoint(const vec3& pt)
	{
		double vertex_x = pt.x;
		double vertex_y = pt.y;
		double vertex_z = pt.z;

		if( m_eps > EPS_M16 )
		{
			if( m_existing_vertices_coords.size() > 0 )
			{
				// std::map::lower_bound returns an iterator pointing to the first element that is equal or greater than key
				auto low = m_existing_vertices_coords.lower_bound(vertex_x);
				if( low == m_existing_vertices_coords.end() )
				{
					if( m_existing_vertices_coords.size() > 0 )
					{
						double lastElement = m_existing_vertices_coords.rbegin()->first;
						double dx = lastElement - vertex_x;
						if( std::abs(dx) <= m_eps )
						{
							auto& map_yz = m_existing_vertices_coords.rbegin()->second;
							return addPointYZ(pt, map_yz);
						}
					}

					size_t vertex_index = m_poly_data->addVertex(pt);
					m_existing_vertices_coords.insert({ vertex_x, {{ vertex_y, {{ vertex_z, vertex_index }} } } });
					return vertex_index;
				}
				else if( low == m_existing_vertices_coords.begin() )
				{
					double existingX = low->first;
					double dx = existingX - vertex_x;
					if( std::abs(dx) <= m_eps )
					{
						std::map<double, std::map<double, size_t> >& map_yz = low->second;
						return addPointYZ(pt, map_yz);
					}
					else
					{
						size_t vertex_index = m_poly_data->addVertex(pt);
						m_existing_vertices_coords.insert({ vertex_x, {{ vertex_y, {{ vertex_z, vertex_index }} } } });
						return vertex_index;
					}
				}
				else
				{
					auto prev = std::prev(low);
					double dxPrev = vertex_x - prev->first;
					double dxLow = low->first - vertex_x;
					if( std::abs(dxPrev) < std::abs(dxLow) )
					{
						if( std::abs(dxPrev) <= m_eps )
						{
							std::map<double, std::map<double, size_t> >& map_yz = prev->second;
							return addPointYZ(pt, map_yz);
						}
						else
						{
							size_t vertex_index = m_poly_data->addVertex(pt);
							m_existing_vertices_coords.insert({ vertex_x, {{ vertex_y, {{ vertex_z, vertex_index }} } } });
							return vertex_index;
						}
					}
					else
					{
						if( std::abs(dxLow) <= m_eps )
						{
							std::map<double, std::map<double, size_t> >& map_yz = low->second;
							return addPointYZ(pt, map_yz);
						}
						else
						{
							size_t vertex_index = m_poly_data->addVertex(pt);
							m_existing_vertices_coords.insert({ vertex_x, {{ vertex_y, {{ vertex_z, vertex_index }} } } });
							return vertex_index;
						}
					}
				}
			}
		}

		// add point to polyhedron
		size_t vertex_index = m_poly_data->addVertex(pt);
		m_existing_vertices_coords.insert({ vertex_x, {{ vertex_y, {{ vertex_z, vertex_index }} } } });
		return vertex_index;
	}

	size_t addPointZ(const vec3& pt, const vec3& uv, std::map<double, size_t>& map_z)
	{
		double vertex_z = pt.z;

		auto low = map_z.lower_bound(vertex_z);
		if (low == map_z.end())
		{
			if (map_z.size() > 0)
			{
				double lastElement = map_z.rbegin()->first;
				double dz = lastElement - vertex_z;
				if (std::abs(dz) <= m_eps)
				{
					size_t existingIndex = map_z.rbegin()->second;
					return existingIndex;
				}
			}

			size_t vertex_index = m_poly_data->addVertex(pt,uv);
			map_z.insert({ { vertex_z, vertex_index } });
			return vertex_index;
		}
		else if (low == map_z.begin())
		{
			double existingZ = low->first;
			double dz = existingZ - vertex_z;
			if (std::abs(dz) <= m_eps)
			{
				size_t& existingIndex = low->second;
				return existingIndex;
			}
			else
			{
				size_t vertex_index = m_poly_data->addVertex(pt, uv);
				map_z.insert({ { vertex_z, vertex_index } });
				return vertex_index;
			}
		}
		else
		{
			auto prev = std::prev(low);
			double dzPrev = vertex_z - prev->first;
			double dzLow = low->first - vertex_z;
			if (std::abs(dzPrev) < std::abs(dzLow))
			{
				if (std::abs(dzPrev) <= m_eps)
				{
					size_t& existingIndex = prev->second;
					return existingIndex;
				}
				else
				{
					size_t vertex_index = m_poly_data->addVertex(pt, uv);
					map_z.insert({ { vertex_z, vertex_index } });
					return vertex_index;
				}
			}
			else
			{
				if (std::abs(dzLow) <= m_eps)
				{
					size_t& existingIndex = low->second;
					return existingIndex;
				}
				else
				{
					size_t vertex_index = m_poly_data->addVertex(pt, uv);
					map_z.insert({ { vertex_z, vertex_index } });
					return vertex_index;
				}
			}
		}
	}

	size_t addPointYZ(const vec3& pt, const vec3& uv, std::map<double, std::map<double, size_t> >& map_yz)
	{
		double vertex_y = pt.y;
		double vertex_z = pt.z;

		auto low = map_yz.lower_bound(vertex_y);
		if (low == map_yz.end())
		{
			if (map_yz.size() > 0)
			{
				double lastElement = map_yz.rbegin()->first;
				double dy = lastElement - vertex_y;
				if (std::abs(dy) < m_eps)
				{
					auto& map_z = map_yz.rbegin()->second;
					return addPointZ(pt, uv, map_z);
				}
			}

			size_t vertex_index = m_poly_data->addVertex(pt, uv);
			map_yz.insert({ { vertex_y, {{ vertex_z, vertex_index }} } });
			return vertex_index;
		}
		else if (low == map_yz.begin())
		{
			double existingY = low->first;
			double dy = existingY - vertex_y;
			if (std::abs(dy) <= m_eps)
			{
				std::map<double, size_t>& map_z = low->second;
				return addPointZ(pt, uv, map_z);
			}
			else
			{
				size_t vertex_index = m_poly_data->addVertex(pt, uv);
				map_yz.insert({ {  vertex_y, {{ vertex_z, vertex_index }} } });
				return vertex_index;
			}
		}
		else
		{
			auto prev = std::prev(low);
			double dyPrev = vertex_y - prev->first;
			double dyLow = low->first - vertex_y;
			if (std::abs(dyPrev) <= std::abs(dyLow))
			{
				if (std::abs(dyPrev) <= m_eps)
				{
					std::map<double, size_t>& map_z = prev->second;
					return addPointZ(pt, uv, map_z);
				}
				else
				{
					size_t vertex_index = m_poly_data->addVertex(pt, uv);
					map_yz.insert({ { vertex_y, {{ vertex_z, vertex_index }} } });
					return vertex_index;
				}
			}
			else
			{
				if (std::abs(dyLow) <= m_eps)
				{
					std::map<double, size_t>& map_z = low->second;
					return addPointZ(pt, uv, map_z);
				}
				else
				{
					size_t vertex_index = m_poly_data->addVertex(pt,uv);
					map_yz.insert({ { vertex_y, {{ vertex_z, vertex_index }} } });
					return vertex_index;
				}
			}
		}
	}

	size_t addPoint(const vec3& pt,const vec3& uv)
	{
		double vertex_x = pt.x;
		double vertex_y = pt.y;
		double vertex_z = pt.z;

		if (m_eps > EPS_M16)
		{
			if (m_existing_vertices_coords.size() > 0)
			{
				// std::map::lower_bound returns an iterator pointing to the first element that is equal or greater than key
				auto low = m_existing_vertices_coords.lower_bound(vertex_x);
				if (low == m_existing_vertices_coords.end())
				{
					if (m_existing_vertices_coords.size() > 0)
					{
						double lastElement = m_existing_vertices_coords.rbegin()->first;
						double dx = lastElement - vertex_x;
						if (std::abs(dx) <= m_eps)
						{
							auto& map_yz = m_existing_vertices_coords.rbegin()->second;
							return addPointYZ(pt,uv, map_yz);
						}
					}

					size_t vertex_index = m_poly_data->addVertex(pt,uv);
					m_existing_vertices_coords.insert({ vertex_x, {{ vertex_y, {{ vertex_z, vertex_index }} } } });
					return vertex_index;
				}
				else if (low == m_existing_vertices_coords.begin())
				{
					double existingX = low->first;
					double dx = existingX - vertex_x;
					if (std::abs(dx) <= m_eps)
					{
						std::map<double, std::map<double, size_t> >& map_yz = low->second;
						return addPointYZ(pt, uv, map_yz);
					}
					else
					{
						size_t vertex_index = m_poly_data->addVertex(pt, uv);
						m_existing_vertices_coords.insert({ vertex_x, {{ vertex_y, {{ vertex_z, vertex_index }} } } });
						return vertex_index;
					}
				}
				else
				{
					auto prev = std::prev(low);
					double dxPrev = vertex_x - prev->first;
					double dxLow = low->first - vertex_x;
					if (std::abs(dxPrev) < std::abs(dxLow))
					{
						if (std::abs(dxPrev) <= m_eps)
						{
							std::map<double, std::map<double, size_t> >& map_yz = prev->second;
							return addPointYZ(pt, uv, map_yz);
						}
						else
						{
							size_t vertex_index = m_poly_data->addVertex(pt, uv);
							m_existing_vertices_coords.insert({ vertex_x, {{ vertex_y, {{ vertex_z, vertex_index }} } } });
							return vertex_index;
						}
					}
					else
					{
						if (std::abs(dxLow) <= m_eps)
						{
							std::map<double, std::map<double, size_t> >& map_yz = low->second;
							return addPointYZ(pt, uv, map_yz);
						}
						else
						{
							size_t vertex_index = m_poly_data->addVertex(pt, uv);
							m_existing_vertices_coords.insert({ vertex_x, {{ vertex_y, {{ vertex_z, vertex_index }} } } });
							return vertex_index;
						}
					}
				}
			}
		}

		// add point to polyhedron
		size_t vertex_index = m_poly_data->addVertex(pt, uv);
		m_existing_vertices_coords.insert({ vertex_x, {{ vertex_y, {{ vertex_z, vertex_index }} } } });
		return vertex_index;
	}

	void clearAllData()
	{
		m_poly_data->clearFaces();
		m_existing_vertices_coords.clear();
	}

	double m_eps = 0;
	shared_ptr<carve::input::PolyhedronData> m_poly_data;
	std::map<double, std::map<double, std::map<double, size_t> > > m_existing_vertices_coords;
};

void polyhedronFromMesh(const carve::mesh::Mesh<3>* mesh, PolyInputCache3D& polyInput);
void polyhedronFromMeshSet(const shared_ptr<carve::mesh::MeshSet<3>>& meshset, PolyInputCache3D& polyInput);
void polyhedronFromMeshSet(const shared_ptr<carve::mesh::MeshSet<3>>& meshset, const std::set<carve::mesh::Face<3>* >& setSkipFaces, PolyInputCache3D& polyInput);
void polyhedronFromMeshSet(const shared_ptr<carve::mesh::MeshSet<3>>& meshset, const std::set<carve::mesh::Face<3>* >& setSkipFaces, const std::set<carve::mesh::Face<3>* >& setFlipFaces, PolyInputCache3D& polyInput);

class RepresentationData;
class ProductShapeData;

/**
*\brief Class ItemShapeData: holds input data of one IFC geometric representation item.
* Parent-child relationship of ItemShapeData, RepresentationData, ProductShapeData:
*        ...
*          |-> ProductShapeData [1...n]
*                   |-> ProductShapeData [1...n]
*                           |-> RepresentationData [1...n]
*                                     |-> ItemShapeData [1...n]
*/
class ItemShapeData
{
public:
	ItemShapeData(){}
	~ItemShapeData(){}

	std::vector<shared_ptr<carve::input::PolylineSetData> > m_polylines;
	std::vector<shared_ptr<carve::mesh::MeshSet<3> > >		m_meshsets;
	std::vector<shared_ptr<carve::mesh::MeshSet<3> > >		m_meshsets_open;
	std::vector<shared_ptr<AppearanceData> >				m_vec_item_appearances;
	std::vector<shared_ptr<TextItemData> >					m_vec_text_literals;
	weak_ptr<RepresentationData>							m_parent_representation;  // Pointer to representation object that this item belongs to
	shared_ptr<IFC4X3::IfcRepresentationItem>				m_ifc_item;
	std::vector<shared_ptr<carve::input::VertexData> >	m_vertex_points;
	std::set<int> m_usedInRepresentations;

	weak_ptr<ProductShapeData>								m_parent_product;
	weak_ptr<ItemShapeData>									m_parent_item;  // Pointer to representation object that this item belongs to
	std::vector<shared_ptr<ItemShapeData> >					m_child_items;
	weak_ptr<IFC4X3::IfcRepresentation>						m_ifc_representation;

public:
	bool isEmpty()
	{
		if( m_vertex_points.size() > 0 )			{ return false; }
		if( m_polylines.size() > 0 )				{ return false; }
		if( m_meshsets.size() > 0 )					{ return false; }
		if( m_meshsets_open.size() > 0 )			{ return false; }
		if( m_vec_item_appearances.size() > 0 )		{ return false; }
		if( m_vec_text_literals.size() > 0 )		{ return false; }

		return true;
	}

	void addChildItem(shared_ptr<ItemShapeData>& item_data, shared_ptr<ItemShapeData>& ptr_self)
	{
		if( ptr_self.get() != this )
		{
			std::cout << __FUNCTION__ << "ptr_self != this" << std::endl;
		}
		m_child_items.push_back( item_data );
		item_data->m_parent_item = ptr_self;
	}

	void addOpenOrClosedPolyhedron(const shared_ptr<carve::input::PolyhedronData>& poly_data, const GeomProcessingParams& params);
	
	void addOpenPolyhedron( const shared_ptr<carve::input::PolyhedronData>& poly_data, const GeomProcessingParams& params)
	{
		if( poly_data->getVertexCount() < 3 )
		{
			return;
		}

		std::string details;
		bool correct = checkPolyhedronData(poly_data, params, details);
		if( !correct )
		{
			fixPolyhedronData(poly_data, true, params);
#ifdef _DEBUG
			std::string details2;
			bool correct2 = checkPolyhedronData(poly_data, params, details2);
			if( !correct2 )
			{
				std::cout << "fixPolyhedronData failed: " << details2 << std::endl;
			}
#endif
		}

		shared_ptr<carve::mesh::MeshSet<3> > meshset( poly_data->createMesh( carve::input::opts(), params.epsMergePoints) );
		m_meshsets_open.push_back( meshset );
	}

	bool addClosedPolyhedron(const shared_ptr<carve::input::PolyhedronData>& poly_data, const GeomProcessingParams& params, shared_ptr<GeometrySettings>& geom_settings);

	void addPoint( const vec3& point )
	{
		shared_ptr<carve::input::VertexData> vertex_data;
		if( m_vertex_points.size() > 0 )
		{
			if( !m_vertex_points[0] )
			{
				m_vertex_points[0] = shared_ptr<carve::input::VertexData>( new carve::input::VertexData() );
			}
			vertex_data = m_vertex_points[0];
		}
		else
		{
			vertex_data = shared_ptr<carve::input::VertexData>( new carve::input::VertexData() );
			m_vertex_points.push_back( vertex_data );
		}

		vertex_data->points.push_back( point );
	}

	void applyTransformToItem( const shared_ptr<TransformData>& transform, bool matrix_identity_checked = false )
	{
		if( !transform )
		{
			return;
		}
		if( !matrix_identity_checked )
		{
			if( GeomUtils::isMatrixIdentity( transform->m_matrix ) )
			{
				return;
			}
		}
		applyTransformToItem( transform->m_matrix, true );
	}

	void applyTransformToItem( const carve::math::Matrix& mat, double CARVE_EPSILON, bool matrix_identity_checked = false )
	{
		if( !matrix_identity_checked )
		{
			if( GeomUtils::isMatrixIdentity( mat ) )
			{
				return;
			}
		}

		for( size_t ii = 0; ii < m_vertex_points.size(); ++ii )
		{
			shared_ptr<carve::input::VertexData>& vertex_data = m_vertex_points[ii];
			for( size_t j = 0; j<vertex_data->points.size(); ++j )
			{
				vec3& point = vertex_data->points[j];
				point = mat*point;
			}
		}

		for( size_t polyline_i = 0; polyline_i < m_polylines.size(); ++polyline_i )
		{
			shared_ptr<carve::input::PolylineSetData>& polyline_data = m_polylines[polyline_i];
			for( size_t j = 0; j<polyline_data->points.size(); ++j )
			{
				vec3& point = polyline_data->points[j];
				point = mat*point;
			}
		}

		//is negative if coordinate system changes handedness (for example as result of mirroring)
		//in this case invert the meshes to not make them look inside out (only noticeable if using
		//back face culling)
		bool const invert_meshes = 0 > carve::geom::dotcross(
			carve::geom::VECTOR(mat.m[0][0], mat.m[1][0], mat.m[2][0]),
			carve::geom::VECTOR(mat.m[0][1], mat.m[1][1], mat.m[2][1]),
			carve::geom::VECTOR(mat.m[0][2], mat.m[1][2], mat.m[2][2]));

		for( size_t i_meshsets = 0; i_meshsets < m_meshsets_open.size(); ++i_meshsets )
		{
			shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = m_meshsets_open[i_meshsets];
			if (!item_meshset)
			{
				continue;
			}

			for( size_t i = 0; i < item_meshset->vertex_storage.size(); ++i )
			{
				vec3& point = item_meshset->vertex_storage[i].v;
				point = mat*point;
			}
			for( size_t i = 0; i < item_meshset->meshes.size(); ++i )
			{
				item_meshset->meshes[i]->recalc(CARVE_EPSILON);
				if(invert_meshes)
				{
					item_meshset->meshes[i]->invert();
				}
			}
		}

		for( size_t i_meshsets = 0; i_meshsets < m_meshsets.size(); ++i_meshsets )
		{
			shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = m_meshsets[i_meshsets];
			if (!item_meshset)
			{
				continue;
			}

			for( size_t i = 0; i < item_meshset->vertex_storage.size(); ++i )
			{
				vec3& point = item_meshset->vertex_storage[i].v;
				point = mat*point;
			}
			for( size_t i = 0; i < item_meshset->meshes.size(); ++i )
			{
				item_meshset->meshes[i]->recalc(CARVE_EPSILON);
				if(invert_meshes)
				{
					item_meshset->meshes[i]->invert();
					//calcOrientation resets isNegative flag (usually)
					item_meshset->meshes[i]->calcOrientation();
				}
			}
		}

		for( size_t text_i = 0; text_i < m_vec_text_literals.size(); ++text_i )
		{
			shared_ptr<TextItemData>& text_literals = m_vec_text_literals[text_i];
			text_literals->m_text_position = mat*text_literals->m_text_position;
		}

		for (auto child : m_child_items)
		{
			child->applyTransformToItem(mat, CARVE_EPSILON, matrix_identity_checked);
		}
	}

	shared_ptr<ItemShapeData> getItemShapeDataDeepCopy()
	{
		shared_ptr<ItemShapeData> copy_item( new ItemShapeData() );

		for( size_t ii = 0; ii < m_vertex_points.size(); ++ii )
		{
			shared_ptr<carve::input::VertexData>& data = m_vertex_points[ii];
			copy_item->m_vertex_points.push_back( shared_ptr<carve::input::VertexData>( new carve::input::VertexData( *( data.get() ) ) ) );
		}

		for( size_t ii = 0; ii < m_polylines.size(); ++ii )
		{
			shared_ptr<carve::input::PolylineSetData>& polyline_data = m_polylines[ii];
			copy_item->m_polylines.push_back( shared_ptr<carve::input::PolylineSetData>( new carve::input::PolylineSetData( *( polyline_data.get() ) ) ) );
		}

		for( auto it_meshsets = m_meshsets_open.begin(); it_meshsets != m_meshsets_open.end(); ++it_meshsets )
		{
			shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = ( *it_meshsets );
			copy_item->m_meshsets.push_back( shared_ptr<carve::mesh::MeshSet<3> >( item_meshset->clone() ) );
		}

		for( auto it_meshsets = m_meshsets.begin(); it_meshsets != m_meshsets.end(); ++it_meshsets )
		{
			shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = ( *it_meshsets );
			copy_item->m_meshsets.push_back( shared_ptr<carve::mesh::MeshSet<3> >( item_meshset->clone() ) );
		}

		for( size_t ii = 0; ii < m_vec_text_literals.size(); ++ii )
		{
			shared_ptr<TextItemData>& text_data = m_vec_text_literals[ii];

			shared_ptr<TextItemData> text_data_copy( new TextItemData() );
			text_data_copy->m_text = text_data->m_text.c_str();
			text_data_copy->m_text_position = text_data->m_text_position;
			copy_item->m_vec_text_literals.push_back( text_data_copy );
		}

		std::copy( m_vec_item_appearances.begin(), m_vec_item_appearances.end(), std::back_inserter( copy_item->m_vec_item_appearances ) );

		return copy_item;
	}
	
	/** copies the content of other instance and adds it to own content */
	void addItemData( const shared_ptr<ItemShapeData>& other )
	{
		std::copy( other->m_vertex_points.begin(), other->m_vertex_points.end(), std::back_inserter( m_vertex_points ) );
		std::copy( other->m_polylines.begin(), other->m_polylines.end(), std::back_inserter( m_polylines ) );
		std::copy( other->m_meshsets.begin(), other->m_meshsets.end(), std::back_inserter( m_meshsets ) );
		std::copy( other->m_meshsets_open.begin(), other->m_meshsets_open.end(), std::back_inserter( m_meshsets_open ) );
		std::copy( other->m_vec_item_appearances.begin(), other->m_vec_item_appearances.end(), std::back_inserter( m_vec_item_appearances ) );
		std::copy( other->m_vec_text_literals.begin(), other->m_vec_text_literals.end(), std::back_inserter( m_vec_text_literals ) );
	}

	const std::vector<shared_ptr<carve::input::VertexData> >& getVertexPoints() { return m_vertex_points; }

	void computeBoundingBox( carve::geom::aabb<3>& bbox, std::set<ItemShapeData*>& setVisited ) const
	{
		for( size_t ii = 0; ii < m_vertex_points.size(); ++ii )
		{
			const shared_ptr<carve::input::VertexData>& vertex_data = m_vertex_points[ii];
			for( size_t j = 0; j<vertex_data->points.size(); ++j )
			{
				const vec3& point = vertex_data->points[j];
				if( bbox.isEmpty() )
				{
					bbox = carve::geom::aabb<3>( point, carve::geom::VECTOR( 0, 0, 0 ) );
				}
				else
				{
					bbox.unionAABB( carve::geom::aabb<3>( point, carve::geom::VECTOR( 0, 0, 0 ) ) );
				}
			}
		}

		for( size_t polyline_i = 0; polyline_i < m_polylines.size(); ++polyline_i )
		{
			const shared_ptr<carve::input::PolylineSetData>& polyline_data = m_polylines[polyline_i];
			for( size_t j = 0; j<polyline_data->points.size(); ++j )
			{
				const vec3& point = polyline_data->points[j];
				if( bbox.isEmpty() )
				{
					bbox = carve::geom::aabb<3>( point, carve::geom::VECTOR( 0, 0, 0 ) );
				}
				else
				{
					bbox.unionAABB( carve::geom::aabb<3>( point, carve::geom::VECTOR( 0, 0, 0 ) ) );
				}
			}
		}

		for( size_t i_meshsets = 0; i_meshsets < m_meshsets_open.size(); ++i_meshsets )
		{
			const shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = m_meshsets_open[i_meshsets];
			if (!item_meshset)
			{
				continue;
			}
			carve::geom::aabb<3> meshBBox = item_meshset->getAABB();
			if( bbox.isEmpty() )
			{
				bbox = meshBBox;
			}
			else
			{
				if( !meshBBox.isEmpty() )
				{
					bbox.unionAABB(meshBBox);
				}
			}
		}

		for( size_t i_meshsets = 0; i_meshsets < m_meshsets.size(); ++i_meshsets )
		{
			const shared_ptr<carve::mesh::MeshSet<3> >& item_meshset = m_meshsets[i_meshsets];
			if (!item_meshset)
			{
				continue;
			}
			carve::geom::aabb<3> meshBBox = item_meshset->getAABB();
			if( bbox.isEmpty() )
			{
				bbox = meshBBox;
			}
			else
			{
				if( !meshBBox.isEmpty() )
				{
					bbox.unionAABB(meshBBox);
				}
			}
		}

		for( size_t text_i = 0; text_i < m_vec_text_literals.size(); ++text_i )
		{
			const shared_ptr<TextItemData>& text_literals = m_vec_text_literals[text_i];
			const carve::math::Matrix& mat = text_literals->m_text_position;
			vec3 text_pos = carve::geom::VECTOR( mat._41, mat._42, mat._43 );
			if( bbox.isEmpty() )
			{
				bbox = carve::geom::aabb<3>( text_pos, carve::geom::VECTOR( 0, 0, 0 ) );
			}
			else
			{
				bbox.unionAABB( carve::geom::aabb<3>( text_pos, carve::geom::VECTOR( 0, 0, 0 ) ) );
			}
		}

		for (auto child : m_child_items)
		{
			if (setVisited.find(child.get()) != setVisited.end())
			{
				continue;
			}
			setVisited.insert(child.get());

			carve::geom::aabb<3> meshBBox;
			child->computeBoundingBox(meshBBox, setVisited);
			if (bbox.isEmpty())
			{
				bbox = meshBBox;
			}
			else
			{
				if (!meshBBox.isEmpty())
				{
					bbox.unionAABB(meshBBox);
				}
			}
		}
	}

	bool hasGeometricRepresentation(bool includeChildren)
	{
		if (m_meshsets.size() > 0) return true;
		if (m_meshsets_open.size() > 0) return true;
		if (m_vec_text_literals.size() > 0) return true;
		if (m_vertex_points.size() > 0) return true;
		if (m_polylines.size() > 0) return true;

		if (includeChildren)
		{
			for (auto child : m_child_items)
			{
				if (child->hasGeometricRepresentation(includeChildren))
				{
					return true;
				}
			}
		}
		return false;
	}

	void clearMeshGeometry()
	{
		m_meshsets.clear();
		m_meshsets_open.clear();
		m_vec_text_literals.clear();
		m_vec_item_appearances.clear();
		m_vertex_points.clear();
		m_polylines.clear();

		for (auto child : m_child_items)
		{
			child->clearMeshGeometry();
		}
	}
};

class RepresentationData
{
public:
	RepresentationData() {}
	~RepresentationData() {}

	weak_ptr<IFC4X3::IfcRepresentation>				m_ifc_representation;
	std::vector<shared_ptr<ItemShapeData> >			m_vec_item_data;
	std::vector<shared_ptr<AppearanceData> >		m_vec_representation_appearances;
	std::string										m_representation_identifier;
	std::string										m_representation_type;
	weak_ptr<ProductShapeData>						m_parent_product;  // Pointer to product object that this representation belongs to

	shared_ptr<RepresentationData> getRepresentationDataDeepCopy()
	{
		shared_ptr<RepresentationData> copy_representation(new RepresentationData());
		copy_representation->m_ifc_representation = m_ifc_representation;
		for (size_t ii = 0; ii < m_vec_item_data.size(); ++ii)
		{
			shared_ptr<ItemShapeData>& item_data = m_vec_item_data[ii];
			copy_representation->m_vec_item_data.push_back(item_data->getItemShapeDataDeepCopy());
		}
		std::copy(m_vec_representation_appearances.begin(), m_vec_representation_appearances.end(), std::back_inserter(copy_representation->m_vec_representation_appearances));
		return copy_representation;
	}

	void addChildItem(shared_ptr<ItemShapeData>& item_data, shared_ptr<RepresentationData>& ptr_self)
	{
		if (ptr_self.get() != this)
		{
			std::cout << __FUNCTION__ << "ptr_self != this" << std::endl;
		}
		m_vec_item_data.push_back(item_data);
		item_data->m_parent_representation = ptr_self;
	}

	void appendRepresentationData(shared_ptr<RepresentationData>& other, shared_ptr<RepresentationData>& ptr_self)
	{
		if (ptr_self.get() != this)
		{
			std::cout << __FUNCTION__ << "ptr_self != this" << std::endl;
		}
		for (auto item_data : other->m_vec_item_data)
		{
			item_data->m_parent_representation = ptr_self;
			m_vec_item_data.push_back(item_data);
		}
		// TODO: Check if placement is same
		std::copy(other->m_vec_representation_appearances.begin(), other->m_vec_representation_appearances.end(), std::back_inserter(m_vec_representation_appearances));
	}

	void addAppearance(shared_ptr<AppearanceData>& appearance)
	{
		if (!appearance)
		{
			return;
		}
		int append_id = appearance->m_step_style_id;
		for (size_t ii = 0; ii < m_vec_representation_appearances.size(); ++ii)
		{
			shared_ptr<AppearanceData>& appearance = m_vec_representation_appearances[ii];
			if (appearance->m_step_style_id == append_id)
			{
				return;
			}
		}
		m_vec_representation_appearances.push_back(appearance);
	}

	void clearAppearanceData()
	{
		m_vec_representation_appearances.clear();
	}

	void clearAll()
	{
		m_vec_representation_appearances.clear();
		m_ifc_representation.reset();
		m_vec_item_data.clear();
		m_representation_identifier = "";
		m_representation_type = "";
	}

	void applyTransformToRepresentation(const carve::math::Matrix& matrix, bool matrix_identity_checked = false)
	{
		if (!matrix_identity_checked)
		{
			if (GeomUtils::isMatrixIdentity(matrix))
			{
				return;
			}
		}
		for (size_t i_item = 0; i_item < m_vec_item_data.size(); ++i_item)
		{
			m_vec_item_data[i_item]->applyTransformToItem(matrix, matrix_identity_checked);
		}
	}

	void computeBoundingBox(carve::geom::aabb<3>& bbox) const
	{
		for (size_t ii = 0; ii < m_vec_item_data.size(); ++ii)
		{
			const shared_ptr<ItemShapeData>& item_data = m_vec_item_data[ii];
			std::set<ItemShapeData*> setVisited;
			item_data->computeBoundingBox(bbox, setVisited);
		}
	}
};

class ProductShapeData 
{
public:
	std::string m_entity_guid;
	weak_ptr<IFC4X3::IfcObjectDefinition>			m_ifc_object_definition;
	weak_ptr<IFC4X3::IfcObjectPlacement>			m_object_placement;
	std::vector<shared_ptr<RepresentationData> >	m_vec_representations;
	bool											m_added_to_spatial_structure = false;
	weak_ptr<ProductShapeData>						m_parent;
	std::vector<shared_ptr<TransformData> >			m_vec_transforms;
	std::vector<shared_ptr<ProductShapeData> >		m_vec_children;
	std::vector<shared_ptr<AppearanceData> >		m_vec_product_appearances;

	std::vector<shared_ptr<ItemShapeData> >			m_geometric_items;
	weak_ptr<IFC4X3::IfcRepresentation>				m_ifc_representation;

	ProductShapeData() {}
	ProductShapeData( std::string entity_guid ) : m_entity_guid(entity_guid) { }

	const std::vector<shared_ptr<ProductShapeData> >& getChildren() { return m_vec_children; }
	shared_ptr<ProductShapeData> getDeepCopy()
	{
		shared_ptr<ProductShapeData> copy_data( new ProductShapeData(m_entity_guid) );
		for( size_t item_i = 0; item_i < m_vec_representations.size(); ++item_i )
		{
			shared_ptr<RepresentationData>& representation_data = m_vec_representations[item_i];
			shared_ptr<RepresentationData> representation_data_copy = representation_data->getRepresentationDataDeepCopy();
			copy_data->m_vec_representations.push_back( representation_data_copy );
		}

		std::copy( m_vec_product_appearances.begin(), m_vec_product_appearances.end(), std::back_inserter( copy_data->m_vec_product_appearances ) );
		for( auto child_product_data : m_vec_children )
		{
			shared_ptr<ProductShapeData> child_copy = child_product_data->getDeepCopy();
			copy_data->addChildProduct(child_copy, copy_data);
		}
		copy_data->m_parent = m_parent;

		for( auto transform : m_vec_transforms )
		{
			if( transform )
			{
				shared_ptr<TransformData> transform_deep_copy( new TransformData( transform->m_matrix, transform->m_placement_entity, transform->m_placement_tag ) );
				copy_data->m_vec_transforms.push_back( transform_deep_copy );
			}
		}

		return copy_data;
	}

	void addGeometricItem(shared_ptr<ItemShapeData>& item, shared_ptr<ProductShapeData>& ptr_self)
	{
		if (ptr_self.get() != this)
		{
			std::cout << __FUNCTION__ << ": ptr_self.get() != this" << std::endl;
		}
		m_geometric_items.push_back(item);
		item->m_parent_product = ptr_self;
	}

	void addAppearance( shared_ptr<AppearanceData>& appearance )
	{
		if( !appearance )
		{
			return;
		}
		int append_id = appearance->m_step_style_id;
		for( size_t ii = 0; ii < m_vec_product_appearances.size(); ++ii )
		{
			shared_ptr<AppearanceData>& appearance = m_vec_product_appearances[ii];
			if( appearance->m_step_style_id == append_id )
			{
				return;
			}
		}
		m_vec_product_appearances.push_back( appearance );
	}

	void clearAppearanceData()
	{
		m_vec_product_appearances.clear();
	}

	void clearMeshGeometry()
	{
		m_vec_product_appearances.clear();
		m_object_placement.reset();
		
		for( size_t item_i = 0; item_i < m_vec_representations.size(); ++item_i )
		{
			shared_ptr<RepresentationData>& item_data = m_vec_representations[item_i];
			item_data->m_vec_item_data.clear();
		}
		
		m_added_to_spatial_structure = false;
	}

	void clearAll()
	{
		m_vec_product_appearances.clear();
		m_ifc_object_definition.reset();
		m_object_placement.reset();
		m_vec_children.clear();
		m_vec_representations.clear();
		m_added_to_spatial_structure = false;
	}
	
	bool isContainedInParentsList( shared_ptr<ProductShapeData>& product_data_check )
	{
		if( !m_parent.expired() )
		{
			shared_ptr<ProductShapeData> product_data_parent( m_parent );
			if( product_data_parent == product_data_check )
			{
				return true;
			}
			product_data_parent->isContainedInParentsList( product_data_check );
		}
		return false;
	}

	void addChildProduct( shared_ptr<ProductShapeData>& add_child, shared_ptr<ProductShapeData>& ptr_self )
	{
		if( ptr_self.get() != this )
		{
			std::cout << __FUNCTION__ << ": ptr_self.get() != this" << std::endl;
		}
		if( isContainedInParentsList( add_child ) )
		{
			std::cout << __FUNCTION__ << ": isContainedInParentsList" << std::endl;
			return;
		}

		for( size_t ii = 0; ii < m_vec_children.size(); ++ii )
		{
			const shared_ptr<ProductShapeData>& existing_child = m_vec_children[ii];
			if( existing_child == add_child )
			{
#ifdef _DEBUG
				if( existing_child->m_entity_guid.compare( add_child->m_entity_guid ) == 0 )
				{
					std::cout << __FUNCTION__ << ": child already added, tag: " << add_child->m_entity_guid << std::endl;
				}
				else
				{
					std::cout << __FUNCTION__ << ": tag mismatch: " << add_child->m_entity_guid << " != " << existing_child->m_entity_guid << std::endl;
				}
#endif
				return;
			}
		}

		m_vec_children.push_back( add_child );
		add_child->m_parent = ptr_self;
	}

	/**
	* \brief method getTransform: Computes the transformation matrix, that puts the geometry of this product into global coordinates
	* All transformation matrices of all parent coordinate systems are multiplied.
	*/
	carve::math::Matrix getTransform()
	{
		carve::math::Matrix transform_matrix;
		if( m_vec_transforms.size() > 0 )
		{
			for( shared_ptr<TransformData>& transform : m_vec_transforms )
			{
				if( transform )
				{
					transform_matrix = transform->m_matrix*transform_matrix;
				}
			}
		}
		return transform_matrix;
	}

	carve::math::Matrix getRelativeTransform(const shared_ptr<ProductShapeData>& other)
	{
		carve::math::Matrix transform_matrix;
		if (!other)
		{
			return transform_matrix;
		}
		
		if (m_vec_transforms.size() > 0)
		{
			std::vector<shared_ptr<TransformData> >	diff_transforms;
			auto it_self = m_vec_transforms.rbegin();
			auto it_other = other->m_vec_transforms.rbegin();
			bool sameSoFar = true;
			for (size_t ii_self = 0; ii_self < m_vec_transforms.size(); ++ii_self)
			{
				if (it_self == m_vec_transforms.rend())
				{
					break;
				}
				
				shared_ptr<TransformData>& transform_self = *it_self;

				if( sameSoFar )
				{
					if( it_other != other->m_vec_transforms.rend() )
					{
						shared_ptr<TransformData>& transform_other = *it_other;

						if( transform_self->m_placement_tag >= 0 && transform_self->m_placement_tag == transform_other->m_placement_tag )
						{
							// skip matrices that are the same at both products, to avoid unnecessary multiplications and numerical inaccuracies
							++it_self;
							++it_other;
							continue;
						}
						sameSoFar = false;
					}
				}
				
				if (transform_self)
				{
					transform_matrix = transform_self->m_matrix*transform_matrix;
				}
				++it_self;
			}
		}
		return transform_matrix;
	}

	void addTransform( shared_ptr<TransformData>& transform_data )
	{
		if( !transform_data )
		{
			return;
		}

		for( auto existing_transform : m_vec_transforms )
		{
			if( existing_transform )
			{
				if( transform_data->isSameTransform( existing_transform ) )
				{
					return;
				}
			}
		}
		m_vec_transforms.insert( m_vec_transforms.begin(), transform_data );
	}

	void applyTransformToProduct( const carve::math::Matrix& matrix, bool matrix_identity_checked, bool applyToChildren )
	{
		if( !matrix_identity_checked )
		{
			if( GeomUtils::isMatrixIdentity( matrix ) )
			{
				return;
			}
		}
		for( size_t i_item = 0; i_item < m_vec_representations.size(); ++i_item )
		{
			m_vec_representations[i_item]->applyTransformToRepresentation( matrix, true );
		}

		if( applyToChildren )
		{
			for( auto child_product_data : m_vec_children )
			{
				child_product_data->applyTransformToProduct(matrix, true, applyToChildren );
			}
		}
	}
	const std::vector<shared_ptr<AppearanceData> >& getAppearances() { return m_vec_product_appearances; }

	bool isEmpty( bool check_also_children ) const
	{
		if(m_geometric_items.size() > 0 )
		{
			return false;
		}
		if( m_vec_product_appearances.size() > 0 )
		{
			return false;
		}
		if( check_also_children )
		{
			if( m_vec_product_appearances.size() > 0 )
			{
				for( size_t ii = 0; ii < m_vec_children.size(); ++ii )
				{
					const shared_ptr<ProductShapeData>& child = m_vec_children[ii];
					bool child_empty = child->isEmpty( check_also_children );
					if( !child_empty )
					{
						return false;
					}
				}
			}
		}
		return true;
	}

	bool hasGeometricRepresentation( bool includeChildren = true ) const
	{
		if( includeChildren )
		{
			for( size_t ii = 0; ii < m_vec_children.size(); ++ii )
			{
				const shared_ptr<ProductShapeData>& child = m_vec_children[ii];
				bool childHasGeom = child->hasGeometricRepresentation(includeChildren);
				if( childHasGeom )
				{
					return true;
				}
			}
		}

		for (size_t ii = 0; ii < m_geometric_items.size(); ++ii)
		{
			const shared_ptr<ItemShapeData>& child = m_geometric_items[ii];
			bool childHasGeom = child->hasGeometricRepresentation(includeChildren);
			if (childHasGeom)
			{
				return true;
			}
		}

		return false;
	}
};

inline bool isEqual(const shared_ptr<carve::mesh::MeshSet<3> >& existingMesh, const shared_ptr<carve::mesh::MeshSet<3> >& checkMesh)
{
	if( existingMesh->meshes.size() != checkMesh->meshes.size() ) { return false; }
	if( existingMesh->vertex_storage.size() != checkMesh->vertex_storage.size() ) { return false; }

	for( size_t ii = 0; ii < existingMesh->meshes.size(); ++ii )
	{
		const carve::mesh::Mesh<3>* mesh1 = existingMesh->meshes[ii];
		const carve::mesh::Mesh<3>* mesh2 = checkMesh->meshes[ii];
		if( mesh1->closed_edges.size() != mesh2->closed_edges.size() ) { return false; }
		if( mesh1->open_edges.size() != mesh2->open_edges.size() ) { return false; }
		if( mesh1->faces.size() != mesh2->faces.size() ) { return false; }

		for( size_t jj = 0; jj < mesh1->faces.size(); ++jj )
		{
			const carve::mesh::Face<3>* face1 = mesh1->faces[jj];
			const carve::mesh::Face<3>* face2 = mesh2->faces[jj];
			if( face1->n_edges != face2->n_edges ) { return false; }

			const carve::mesh::Edge<3>* edge1 = face1->edge;
			const carve::mesh::Edge<3>* edge2 = face2->edge;

			for( size_t kk = 0; kk < face1->n_edges; ++kk )
			{
				const carve::mesh::Vertex<3>* vertex1 = edge1->vert;
				const carve::mesh::Vertex<3>* vertex2 = edge2->vert;
				vec3 delt = vertex1->v - vertex2->v;
				if( delt.x > 0.0001 ) { return false; }
				if( delt.x < -0.0001 ) { return false; }
				if( delt.y > 0.0001 ) { return false; }
				if( delt.y < -0.0001 ) { return false; }
				if( delt.z > 0.0001 ) { return false; }
				if( delt.z < -0.0001 ) { return false; }
				edge1 = edge1->next;
				edge2 = edge2->next;
			}
		}
	}
	return true;
}

inline bool isEqual(const shared_ptr<ItemShapeData>& existingItem, const shared_ptr<ItemShapeData>& checkItem)
{
	if( existingItem->m_meshsets.size() != checkItem->m_meshsets.size() ) { return false; }
	if( existingItem->m_meshsets_open.size() != checkItem->m_meshsets_open.size() ) { return false; }

	for( size_t ii = 0; ii < existingItem->m_meshsets.size(); ++ii )
	{
		shared_ptr<carve::mesh::MeshSet<3> >& meshset1 = existingItem->m_meshsets[ii];
		shared_ptr<carve::mesh::MeshSet<3> >& meshset2 = checkItem->m_meshsets[ii];
		if( !isEqual(meshset1, meshset2) )
		{
			return false;
		}
	}

	for( size_t ii = 0; ii < existingItem->m_meshsets_open.size(); ++ii )
	{
		shared_ptr<carve::mesh::MeshSet<3> >& meshset1 = existingItem->m_meshsets_open[ii];
		shared_ptr<carve::mesh::MeshSet<3> >& meshset2 = checkItem->m_meshsets_open[ii];
		if( !isEqual(meshset1, meshset2) )
		{
			return false;
		}
	}
	return true;
}

static carve::geom::aabb<3> computeBoundingBoxLocalCoords( const shared_ptr<ProductShapeData>& productData, bool includeChildren )
{
	carve::geom::aabb<3> bbox;
	std::set<ItemShapeData*> setVisited;
	for( auto item : productData->m_vec_representations)
	{
		carve::geom::aabb<3> repBBox;
		item->computeBoundingBox(repBBox);
		if( bbox.isEmpty() )
		{
			bbox = repBBox;
		}
		else
		{
			if (!repBBox.isEmpty())
			{
				bbox.unionAABB(repBBox);
			}
		}
	}

	if( includeChildren )
	{
		for( auto child : productData->m_vec_children )
		{
			carve::geom::aabb<3> childBBox = computeBoundingBoxLocalCoords(child, true);
			if( bbox.isEmpty() )
			{
				bbox = childBBox;
			}
			else
			{
				if (!childBBox.isEmpty())
				{
					bbox.unionAABB(childBBox);
				}
			}
		}
	}
	return bbox;
}

static carve::geom::aabb<3> computeBoundingBox(shared_ptr<ProductShapeData>& productData, bool includeChildren, bool applyTransformToGlobalCoordinates)
{
	carve::geom::aabb<3> bbox;
	if( !productData )
	{
		return bbox;
	}
	
	carve::math::Matrix transform;
	if( applyTransformToGlobalCoordinates )
	{
		transform = productData->getTransform();
	}

	std::set<ItemShapeData*> setVisited;
	for( auto item : productData->m_vec_representations)
	{
		carve::geom::aabb<3> repBBox;
		item->computeBoundingBox(repBBox);

		if( repBBox.isEmpty() )
		{
			continue;
		}

		if( bbox.isEmpty() )
		{
			if( applyTransformToGlobalCoordinates )
			{
				repBBox.pos = transform * repBBox.pos;
			}
			bbox = repBBox;
		}
		else
		{
			bbox.unionAABB(repBBox);
		}
	}

	if( includeChildren )
	{
		for( shared_ptr<ProductShapeData>& child : productData->m_vec_children )
		{
			carve::geom::aabb<3> childBBox = computeBoundingBox(child, true, applyTransformToGlobalCoordinates);

			if( childBBox.isEmpty() )
			{
				continue;
			}

			if( bbox.isEmpty() )
			{
				bbox = childBBox;
			}
			else
			{
				bbox.unionAABB(childBBox);
			}
		}
	}

	return bbox;
}
