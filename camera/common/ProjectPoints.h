/******************************************************************************
*       SOFAOR, SOFA plugin for the Operating Room, development version       *
*                        (c) 2017 INRIA, MIMESIS Team                         *
*                                                                             *
* This program is a free software; you can redistribute it and/or modify it   *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 1.0 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: Bruno Marques and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact-mimesis@inria.fr                               *
******************************************************************************/

#ifndef SOFA_OR_PROCESSOR_PROJECTPOINTS_H
#define SOFA_OR_PROCESSOR_PROJECTPOINTS_H

#include "initPlugin.h"

#include <SofaORCommon/ImplicitDataEngine.h>
#include "CameraSettings.h"

#include <opencv2/opencv.hpp>

namespace sofaor
{
namespace processor
{
namespace cam
{
/**
 * @brief The ProjectPoints class
 *
 * Projects a 2D point cloud in 3D or vice-versa using a linked CameraSettings
 * component
 */
class ProjectPoints : public common::ImplicitDataEngine
{
	typedef sofa::core::objectmodel::SingleLink<
			ProjectPoints, CameraSettings,
			sofa::BaseLink::FLAG_STOREPATH | sofa::BaseLink::FLAG_STRONGLINK>
			CamSettings;

	typedef typename sofa::defaulttype::Vector3 Vector3;
	typedef typename sofa::defaulttype::Vector2 Vector2;

 public:
	SOFA_CLASS(ProjectPoints, common::ImplicitDataEngine);

	ProjectPoints()
			: l_cam(initLink("cam",
											 "link to CameraSettings component containing and "
											 "maintaining the camera's parameters")),
				d_2Dto3D(initData(&d_2Dto3D, true, "to3D", "if false, 3D to 2D")),
				d_depth(
						initData(&d_depth, -1.0, "depth",
										 "default is -1 (retrieves depth from fz in camSettings)")),
				d_Pts3D(initData(&d_Pts3D, "points3D", "3D points")),
				d_Pts2D(initData(&d_Pts2D, "points2D", "2D points"))
	{
	}

	~ProjectPoints() {}
	void init()
	{
		if (!l_cam.get())
			msg_error(getName() + "::init()") << "Error: No camera link set. "
																					 "Please use attribute 'cam' "
																					 "to define one";
		if (d_2Dto3D.getValue())
		{
			addInput(&d_Pts2D);
			addOutput(&d_Pts3D);
		}
		else
		{
			addInput(&d_Pts3D);
			addOutput(&d_Pts2D);
		}
		update();
	}

	void update()
	{
		if (d_2Dto3D.getValue())
		{
			sofa::helper::vector<Vector3>& pts3d = *d_Pts3D.beginEdit();
			pts3d.clear();
			for (auto pt : d_Pts2D.getValue())
				pts3d.push_back(l_cam->get3DFrom2DPosition(pt, d_depth.getValue()));
		}
		else
		{
			sofa::helper::vector<Vector2>& pts2d = *d_Pts2D.beginEdit();
			pts2d.clear();
			for (auto pt : d_Pts3D.getValue())
				pts2d.push_back(l_cam->get2DFrom3DPosition(pt));
		}
	}

	CamSettings l_cam;           ///< Linked CameraSettings
	sofa::Data<bool> d_2Dto3D;   ///< projection direction (2Dto3D or 3Dto2D)
	sofa::Data<double> d_depth;  ///< focal distance for 2Dto3D projection (-1 to
															 /// take CameraSettings focal distance)
	sofa::Data<sofa::helper::vector<Vector3> >
			d_Pts3D;  ///< [INPUT / OUTPUT] set of 3D points
	sofa::Data<sofa::helper::vector<Vector2> >
			d_Pts2D;  ///< [INPUT / OUTPUT] set of 2D points
};

}  // namespace cam
}  // namespace processor
}  // namespace sofaor
#endif  // SOFA_OR_PROCESSOR_PROJECTPOINTS_H
