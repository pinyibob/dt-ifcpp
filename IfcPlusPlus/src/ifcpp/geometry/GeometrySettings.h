/* -*-c++-*- IfcQuery www.ifcquery.com
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

#define _USE_MATH_DEFINES 
#include <cmath>
#include <functional>
#include <set>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BuildingObject.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

#define GEOM_TOLERANCE  0.0000001
#define HALF_SPACE_BOX_SIZE 100

class StatusCallback;

//\brief Central class to hold settings that influence geometry processing.
class GeometrySettings
{
public:
	GeometrySettings()
	{
		m_render_object_filter.insert(1287392070);  // IfcFeatureElementSubtraction
	}
	GeometrySettings(const shared_ptr<GeometrySettings>& other)
	{
		m_render_object_filter = other->m_render_object_filter;
		m_maxNumFaceEdges = other->m_maxNumFaceEdges;
		m_num_vertices_per_circle = other->m_num_vertices_per_circle;
		m_num_vertices_per_circle_default = other->m_num_vertices_per_circle_default;
		m_min_num_vertices_per_arc = other->m_min_num_vertices_per_arc;
		m_num_vertices_per_control_point = other->m_num_vertices_per_control_point;
		m_num_vertices_per_control_point_default = other->m_num_vertices_per_control_point_default;
		m_show_text_literals = other->m_show_text_literals;
		m_ignore_profile_radius = other->m_ignore_profile_radius;
		m_handle_styled_items = other->m_handle_styled_items;
		m_handle_layer_assignments = other->m_handle_layer_assignments;
		m_render_bounding_box = other->m_render_bounding_box;
		m_min_triangle_area = other->m_min_triangle_area;
		m_epsilonMergePoints = other->m_epsilonMergePoints;
		m_epsCoplanarAngle = other->m_epsCoplanarAngle;
	}

	// Number of discretization points per circle
	int getNumVerticesPerCircleWithRadius(double radius) { return m_num_vertices_per_circle_given_radius(radius); }
	int getNumVerticesPerCircle() { return m_num_vertices_per_circle; }
	void setNumVerticesPerCircle(int num) { m_num_vertices_per_circle = num; }
	void resetNumVerticesPerCircle() { m_num_vertices_per_circle = m_num_vertices_per_circle_default; }
	void setNumVerticesPerCircleGivenRadius(std::function<int(double)> f) { m_num_vertices_per_circle_given_radius = f; }

	// Minimum number of discretization points per arc
	int getMinNumVerticesPerArc() { return m_min_num_vertices_per_arc; }
	void resetMinNumVerticesPerArc() { m_min_num_vertices_per_arc = 6; }
	void setMinNumVerticesPerArc(int num) { m_min_num_vertices_per_arc = num; }

	int getNumVerticesPerControlPoint() { return m_num_vertices_per_control_point; }
	void setNumVerticesPerControlPoint(int num) { m_num_vertices_per_control_point = num; }
	void resetNumVerticesPerControlPoint() { m_num_vertices_per_control_point = m_num_vertices_per_control_point_default; }

	void setHandleLayerAssignments(bool handle) { m_handle_layer_assignments = handle; }
	bool handleLayerAssignments() { return m_handle_layer_assignments; }

	void setHandleStyledItems(bool handle) { m_handle_styled_items = handle; }
	bool handleStyledItems() { return m_handle_styled_items; }

	bool isShowTextLiterals() { return m_show_text_literals; }
	bool isIgnoreProfileRadius() { return m_ignore_profile_radius; }
	void setIgnoreProfileRadius(bool ignore_radius) { m_ignore_profile_radius = ignore_radius; }

	/**\brief setMinTriangleArea: if a triangle is smaller than this value, it is still in the carve meshset, but skipped for rendering.
	That reduces the number of triangles on the GPU, not visible unless you zoom in to a very small area */
	void setMinTriangleArea(double min_area) { m_min_triangle_area = min_area; }
	double getMinTriangleArea() { return m_min_triangle_area; }

	/**\brief Render bounding box for each object */
	bool getRenderBoundingBoxes() { return m_render_bounding_box; }
	void setRenderBoundingBoxes(bool render_bbox) { m_render_bounding_box = render_bbox; }

	/**\brief Render filter decides if a IfcObjectDefinition should be rendered.
	  The default filter will render all objects except objects based on IfcFeatureElementSubtraction.*/
	bool skipRenderObject(uint32_t classID)
	{
		if( m_render_object_filter.find(classID) != m_render_object_filter.end() )
		{
			return true;
		}
		return false;
	}

	void setEpsilonMergePoints(double eps)
	{
		m_epsilonMergePoints = eps;
	}

	double getEpsilonMergePoints()
	{
		return m_epsilonMergePoints;
	}

	void setEpsilonCoplanarAngle(double eps)
	{
		m_epsCoplanarAngle = eps;
	}

	double getEpsilonCoplanarAngle()
	{
		return m_epsCoplanarAngle;
	}

	void setEpsilonCoplanarDistance(double eps)
	{
		m_epsCoplanarDistance = eps;
	}

	double getEpsilonCoplanarDistance()
	{
		return m_epsCoplanarDistance;
	}

	std::set<uint32_t> m_render_object_filter;
	size_t m_maxNumFaceEdges = 10000;
	bool m_mergeAlignedEdges = true;

protected:
	int	m_num_vertices_per_circle = 14;
	int m_num_vertices_per_circle_default = 14;
	int m_min_num_vertices_per_arc = 5;
	int m_num_vertices_per_control_point = 1;
	int m_num_vertices_per_control_point_default = 1;
	bool m_show_text_literals = false;
	bool m_ignore_profile_radius = false;
	bool m_handle_styled_items = true;
	bool m_handle_layer_assignments = true;
	bool m_render_bounding_box = false;
	double m_min_triangle_area = 1e-9;
	double m_epsilonMergePoints = 1.5e-8;
	double m_epsCoplanarDistance = 1.5e-8;
	double m_epsCoplanarAngle = 1e-10;

	std::function<int(double)> m_num_vertices_per_circle_given_radius = [&](double radius)
	{
		if (radius > 0.5) return int(m_num_vertices_per_circle*1.5);
		return m_num_vertices_per_circle;
	};
};

class CarveMeshNormalizer;
struct GeomProcessingParams
{
	GeomProcessingParams( shared_ptr<GeometrySettings>& generalSettings )
	{
		epsMergePoints = generalSettings->getEpsilonMergePoints();
		epsMergeAlignedEdgesAngle = generalSettings->getEpsilonCoplanarAngle();
		minFaceArea = generalSettings->getMinTriangleArea();
		mergeAlignedEdges = generalSettings->m_mergeAlignedEdges;
		this->generalSettings = generalSettings;
	}
	GeomProcessingParams(shared_ptr<GeometrySettings>& generalSettings, bool dumpMeshes) : GeomProcessingParams(generalSettings)
	{
		this->debugDump = dumpMeshes;
	}
	GeomProcessingParams( shared_ptr<GeometrySettings>& generalSettings, BuildingEntity* ifc_entity, StatusCallback* callbackFunc) : GeomProcessingParams(generalSettings)
	{
		this->ifc_entity = ifc_entity;
		this->callbackFunc = callbackFunc;
	}
	GeomProcessingParams(const GeomProcessingParams& other) //= default; // TODO check this default copy constructor on Win/Linux
	{
		this->generalSettings = other.generalSettings;
		this->ifc_entity = other.ifc_entity;
		this->callbackFunc = other.callbackFunc;
		this->epsMergePoints = other.epsMergePoints;
		this->epsMergeAlignedEdgesAngle = other.epsMergeAlignedEdgesAngle;
		this->minFaceArea = other.minFaceArea;
		this->mergeAlignedEdges = other.mergeAlignedEdges;
		this->allowFinEdges = other.allowFinEdges;
		this->allowDegenerateEdges = other.allowDegenerateEdges;
		this->checkZeroAreaFaces = other.checkZeroAreaFaces;
		this->allowZeroAreaFaces = other.allowZeroAreaFaces;
		this->debugDump = other.debugDump;
		this->normalizer = other.normalizer;
	}
	shared_ptr<GeometrySettings> generalSettings;
	BuildingEntity* ifc_entity = nullptr;
	StatusCallback* callbackFunc = nullptr;

	double epsMergePoints = 1e-9;
	double epsMergeAlignedEdgesAngle = 1e-6;
	double minFaceArea = 1e-12;
	bool mergeAlignedEdges = true;
	bool allowFinEdges = false;
	bool allowDegenerateEdges = false;
	bool checkZeroAreaFaces = true;
	bool allowZeroAreaFaces = false;
	bool debugDump = false;
	CarveMeshNormalizer* normalizer = nullptr;

	// config
	bool treatLongThinFaceAsDegenerate = false;
};
