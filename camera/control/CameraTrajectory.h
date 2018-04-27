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

#ifndef SOFACV_CAM_CONTROL_ROTATEAROUNDENGINE_H
#define SOFACV_CAM_CONTROL_ROTATEAROUNDENGINE_H

#include "ImageProcessingPlugin.h"
#include "camera/common/CameraSettings.h"

#include <SofaCV/SofaCV.h>
#include <sofa/helper/OptionsGroup.h>

namespace sofacv
{
namespace cam
{
namespace control
{
class SOFA_IMAGEPROCESSING_API CameraTrajectory : public ImplicitDataEngine
{
  typedef sofa::core::objectmodel::SingleLink<
      CameraTrajectory, cam::CameraSettings,
      sofa::BaseLink::FLAG_STOREPATH | sofa::BaseLink::FLAG_STRONGLINK>
      CamSettings;
  typedef typename sofa::defaulttype::Vector3 Vector3;
  enum Plane
  {
    PLANE_XY = 0,
    PLANE_XZ = 1,
    PLANE_YZ = 2,
    PLANE_OTHER = 3
  };

 public:
  SOFA_CLASS(CameraTrajectory, ImplicitDataEngine);

  CameraTrajectory();

  virtual ~CameraTrajectory() override {}
  virtual void init() override;

  virtual void Update() override;

  virtual void handleEvent(sofa::core::objectmodel::Event* e) override;

  CamSettings l_cam;
  sofa::Data<Vector3> d_center;
  sofa::Data<double> d_angle;
  sofa::Data<sofa::helper::OptionsGroup> d_plane;

  double m_radius;

 private:
  void centerChanged() {}
  void angleChanged() {}
  void planeChanged() {}
};

SOFA_DECL_CLASS(CameraTrajectory)

int CameraTrajectoryClass = sofa::core::RegisterObject(
                                "Component to rotate a camera following a "
                                "trajectory on a plane, around a point")
                                .add<CameraTrajectory>();

}  // namespace control
}  // namespace cam
}  // namespace sofacv
#endif  // SOFACV_CAM_CONTROL_ROTATEAROUNDENGINE_H
