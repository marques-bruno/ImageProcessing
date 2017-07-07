#ifndef SOFA_OR_PROCESSOR_SOLVEPNP_H
#define SOFA_OR_PROCESSOR_SOLVEPNP_H

#include "initPlugin.h"

#include <SofaORCommon/ImplicitDataEngine.h>
#include "camera/common/CameraSettings.h"

#include <sofa/core/objectmodel/Link.h>
#include <sofa/helper/OptionsGroup.h>

#include <opencv2/opencv.hpp>

namespace sofaor
{
namespace processor
{
namespace cam
{
namespace calib
{
/**
 * @brief The SolvePnP class
 *
 * Estimates the pose of a camera relative to a 3D object by using a set of
 * points on the object, and their 2D correspondances in the camera's image
 *
 * (see SolvePnP in http://docs.opencv.org/3.2.0/d9/d0c/group__calib3d.html
 * for details)
 */
class SolvePnP : public common::ImplicitDataEngine
{
	typedef sofa::core::objectmodel::SingleLink<
			SolvePnP, CameraSettings,
			sofa::BaseLink::FLAG_STOREPATH | sofa::BaseLink::FLAG_STRONGLINK>
			CamSettings;

 public:
	SOFA_CLASS(SolvePnP, common::ImplicitDataEngine);

	SolvePnP()
			: l_cam(initLink("cam",
											 "link to CameraSettings component containing and "
											 "maintaining the camera's parameters")),
				d_imagePoints(
						initData(&d_imagePoints, "imagePoints",
										 "a vector of 2D projections of the 3D "
										 "object's points in the image. imagePoints.size() and "
										 "objectPoints.size() must "
										 "be equal")),
				d_objectPoints(
						initData(&d_objectPoints, "objectPoints",
										 "a vector of calibration pattern "
										 "points in the calibration pattern's coordinate "
										 "space")),
				d_imgSize(initData(&d_imgSize, "imageSize",
													 "[Optional] size in px of the image. If not "
													 "provided, recovered from CameraSettings")),
				d_K(initData(&d_K, "K",
										 "[Optional] Intrinsic matrix. If not provided, intrinsics "
										 "will be recovered from CameraSettings, or approximated "
										 "from the imageSize")),
				d_distCoefs(
						initData(&d_distCoefs, "distCoefs",
										 "[Optional] distortion coefficients initial guess")),
				d_pnpFlags(initData(
						&d_pnpFlags, 4, "pnpFlags",
						"One or a combination of the following flags:\n"
						"ITERATIVE (1): Iterative method based on Levenberg-Marquardt "
						"optimization.\n"
						"P3P (2): Method based on the paper of X.S. Gao, X.-R. Hou, J. "
						"Tang, H.-F. Chang “Complete Solution Classification for the "
						"Perspective-Three-Point Problem”. In this case the function "
						"requires exactly four object and image points.\n"
						"EPNP (4): Method introduced by F.Moreno-Noguer, V.Lepetit and "
						"P.Fua in the paper “EPnP: Efficient Perspective-n-Point Camera "
						"Pose Estimation\n"))
	{
	}

	~SolvePnP() {}
	void init()
	{
		addInput(&d_imagePoints);
		addInput(&d_objectPoints);
		addInput(&d_imgSize);
		addInput(&d_K);
		addInput(&d_distCoefs);

		if (!(l_cam.get()))
			msg_error(this->getName() + "::init()") << "Error: No camera link set. "
																								 "Please use attribute 'cam' "
																								 "to define one";
		update();
	}

	void update();

	CamSettings l_cam;  ///< Camera settings to update

	// INPUTS
	sofa::Data<sofa::helper::vector<sofa::defaulttype::Vector2> >
			d_imagePoints;  ///< [INPUT] 2D points in the image
	sofa::Data<sofa::helper::vector<sofa::defaulttype::Vector3> >
			d_objectPoints;  ///< [INPUT] 3D points on the object

	// OPTIONAL INPUT
	sofa::Data<sofa::defaulttype::Vec2i> d_imgSize;  ///< [INPUT] image size to estimate K
	sofa::Data<sofa::defaulttype::Matrix3> d_K;      ///< [INPUT] Intrinsic Guess
	sofa::Data<sofa::helper::vector<double> >
			d_distCoefs;             ///< [INPUT] Distortion coefficients guess
	sofa::Data<int> d_pnpFlags;  ///< OpenCV's PNP flags
};

}  // namespace calib
}  // namespace cam
}  // namespace processor
}  // namespace sofaor

#endif  // SOFA_OR_PROCESSOR_SOLVEPNP_H
