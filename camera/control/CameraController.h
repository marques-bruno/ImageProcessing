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

#ifndef SOFA_OR_PROCESSOR_CONTROLLERENGINE_H
#define SOFA_OR_PROCESSOR_CONTROLLERENGINE_H

#include "camera/common/CameraSettings.h"
#include "initPlugin.h"
#include <fstream>

#include <SofaORCommon/ImplicitDataEngine.h>
#include <sofa/helper/OptionsGroup.h>

#include <zmq.hpp>

namespace sofaor
{
namespace processor
{
class CameraController : public common::ImplicitDataEngine
{
  typedef sofa::core::objectmodel::SingleLink<
      CameraController, cam::CameraSettings,
      sofa::BaseLink::FLAG_STOREPATH | sofa::BaseLink::FLAG_STRONGLINK>
      CamSettings;
  typedef typename sofa::defaulttype::Matrix3 Matrix3;
  typedef typename sofa::defaulttype::Vector3 Vector3;
  typedef typename sofa::defaulttype::Quat Quat;

  SOFAOR_CALLBACK_SYSTEM(CameraController);

 public:
  SOFA_CLASS(CameraController, common::ImplicitDataEngine);

  CameraController()
      : l_cam(initLink("cam", "camera to control")),
      d_tx(initData(&d_tx, "tx",
                   "optical center position along x in the world reference frame")),
      d_ty(initData(&d_ty, "ty",
                   "optical center position along y in the world reference frame")),
      d_tz(initData(&d_tz, "tz",
                   "optical center position along z in the world reference frame")),
      d_R00(initData(&d_R00, "R00",
                   "Rotation matrix 00")),
      d_R01(initData(&d_R01, "R01",
                   "Rotation matrix 01")),
      d_R02(initData(&d_R02, "R02",
                   "Rotation matrix 02")),
      d_R10(initData(&d_R10, "R10",
                   "Rotation matrix 10")),
      d_R11(initData(&d_R11, "R11",
                   "Rotation matrix 11")),
      d_R12(initData(&d_R12, "R12",
                   "Rotation matrix 12")),
      d_R20(initData(&d_R20, "R20",
                   "Rotation matrix 20")),
      d_R21(initData(&d_R21, "R21",
                   "Rotation matrix 21")),
      d_R22(initData(&d_R22, "R22",
                   "Rotation matrix 22")),
      d_imgS(initData(&d_imgS, "d_img",
                   "serialized image"))
  {
  }

  ~CameraController() {}
  void init()
  {
    SOFAOR_ADD_CALLBACK(&d_R00, &CameraController::RotationMatrixDataChanged);
    SOFAOR_ADD_CALLBACK(&d_R01, &CameraController::RotationMatrixDataChanged);
    SOFAOR_ADD_CALLBACK(&d_R02, &CameraController::RotationMatrixDataChanged);
    SOFAOR_ADD_CALLBACK(&d_R10, &CameraController::RotationMatrixDataChanged);
    SOFAOR_ADD_CALLBACK(&d_R11, &CameraController::RotationMatrixDataChanged);
    SOFAOR_ADD_CALLBACK(&d_R12, &CameraController::RotationMatrixDataChanged);
    SOFAOR_ADD_CALLBACK(&d_R20, &CameraController::RotationMatrixDataChanged);
    SOFAOR_ADD_CALLBACK(&d_R21, &CameraController::RotationMatrixDataChanged);
    SOFAOR_ADD_CALLBACK(&d_R22, &CameraController::RotationMatrixDataChanged);
    SOFAOR_ADD_CALLBACK(&d_tx, &CameraController::TranslationVectorDataChanged);
    SOFAOR_ADD_CALLBACK(&d_ty, &CameraController::TranslationVectorDataChanged);
    SOFAOR_ADD_CALLBACK(&d_tz, &CameraController::TranslationVectorDataChanged);

    addInput(&d_tx);
    addInput(&d_ty);
    addInput(&d_tz);

    addInput(&d_R00);
    addInput(&d_R01);
    addInput(&d_R02);

    addInput(&d_R10);
    addInput(&d_R11);
    addInput(&d_R12);

    addInput(&d_R20);
    addInput(&d_R21);
    addInput(&d_R22);

    if (!l_cam.get())
      msg_error(getName() + "::init()") << "Error: No camera link set. ";

    update();
  }

  void update()
  {
    Matrix3 R;
      R[0][0] = d_R00.getValue();
      R[0][1] = d_R01.getValue();
      R[0][2] = d_R02.getValue();
      R[1][0] = d_R10.getValue();
      R[1][1] = d_R11.getValue();
      R[1][2] = d_R12.getValue();
      R[2][0] = d_R20.getValue();
      R[2][1] = d_R21.getValue();
      R[2][2] = d_R22.getValue();

      Vector3 t;
      t[0]=d_tx.getValue();
      t[1]=d_ty.getValue();
      t[2]=d_tz.getValue();

    l_cam->setPosition(t, false);
    l_cam->setRotationMatrix(R, false);
    l_cam->buildFromKRT();
  }

  virtual void handleEvent(sofa::core::objectmodel::Event* e)
  {
    if (sofa::simulation::AnimateBeginEvent::checkEventType(e))
    {
      cleanInputs();
      update();
    }
  }

  CamSettings l_cam;

  Vector3 m_pole;
  sofa::Data<double> d_thetaInit;
  sofa::Data<double> d_phiInit;
  sofa::Data<double> d_rhoInit;

  double m_theta;
  double m_phi;
  double m_rho;

  sofa::Data<double>
      d_tx;  ///< Position along x in world coordinates of the camera's optical center
  sofa::Data<double>
      d_ty;  ///< Position along y in world coordinates of the camera's optical center
  sofa::Data<double>
      d_tz;  ///< Position along z in world coordinates of the camera's optical center

  sofa::Data<double> d_R00;  ///< 3x3 rotation matrix 00
  sofa::Data<double> d_R01;  ///< 3x3 rotation matrix 01
  sofa::Data<double> d_R02;  ///< 3x3 rotation matrix 02
  sofa::Data<double> d_R10;  ///< 3x3 rotation matrix 10
  sofa::Data<double> d_R11;  ///< 3x3 rotation matrix 10
  sofa::Data<double> d_R12;  ///< 3x3 rotation matrix 10
  sofa::Data<double> d_R20;  ///< 3x3 rotation matrix 20
  sofa::Data<double> d_R21;  ///< 3x3 rotation matrix 21
  sofa::Data<double> d_R22;  ///< 3x3 rotation matrix 22

  sofa::Data<std::string> d_imgS;  ///< serialized image


 private:
  void TranslationVectorDataChanged(sofa::core::objectmodel::BaseData*)
  {
    //setPositionData(d_tx.getValue(),d_ty.getValue(),d_tz.getValue());
  }
  void RotationMatrixDataChanged(sofa::core::objectmodel::BaseData*)
  {
    //setRotationMatrixData(d_R00.getValue(),d_R01.getValue(),d_R02.getValue(),d_R10.getValue(),d_R11.getValue(),d_R12.getValue(),d_R20.getValue(),d_R21.getValue(),d_R22.getValue());
  }
};

SOFA_DECL_CLASS(CameraController)

int CameraControllerClass =
    sofa::core::RegisterObject(
        "Component to rotate a camera following any trajectory on a sphere, "
        "around a point. Init sets the correct camera orientation if necessary")
        .add<CameraController>();

}  // namespace processor
}  // namespace sofaor
#endif  // SOFA_OR_PROCESSOR_ROTATEAROUNDENGINE_H
