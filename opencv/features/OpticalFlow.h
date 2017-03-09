#ifndef SOFA_OR_PROCESSOR_OPTICALFLOW_H
#define SOFA_OR_PROCESSOR_OPTICALFLOW_H

#include "core/ImageFilter.h"

#include <sofa/defaulttype/Vec.h>
#include <sofa/helper/vector.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/video.hpp>

namespace sofa
{
namespace OR
{
namespace processor
{
class OpticalFlow : public ImageFilter
{
 public:
  SOFA_CLASS(OpticalFlow, ImageFilter);

  Data<defaulttype::Vec2i> d_winSize;
  Data<int> d_maxLevel;
  Data<int> d_criteria_type;
  Data<int> d_maxCount;
  Data<double> d_epsilon;
  Data<int> d_flags;
  Data<double> d_minEigThresh;
  Data<helper::vector<defaulttype::Vec2f> > d_points_in;
  Data<helper::vector<defaulttype::Vec2f> > d_points_out;
  Data<helper::vector<uchar> > d_status_out;
  Data<helper::vector<float> > d_error_out;
	Data<bool> d_startTracking;

  OpticalFlow()
      : d_winSize(
						initData(&d_winSize, defaulttype::Vec2i(51, 51), "win_size", "")),
				d_maxLevel(initData(&d_maxLevel, 0, "max_level", "")),
				d_criteria_type(initData(&d_criteria_type,
																 CV_TERMCRIT_ITER + CV_TERMCRIT_EPS,
																 "crit_type", "")),
				d_maxCount(initData(&d_maxCount, 20, "max_count", "")),
				d_epsilon(initData(&d_epsilon, 0.03, "epsilon", "")),
        d_flags(initData(&d_flags, 0, "flags", "")),
				d_minEigThresh(initData(&d_minEigThresh, 0.05, "eigen_threshold", "")),
				d_points_in(
						initData(&d_points_in, "points", "set of input points to track")),
        d_points_out(initData(&d_points_out, "points_out",
															"set of output points to track")),
				d_startTracking(initData(&d_startTracking, false, "start",
																 "set to true to stop reading from input "
																 "vector 'points' and start performing the "
																 "optical flow"))
	{
  }

  void init()
  {
    registerData(&d_maxLevel, 0, 10, 1);
    registerData(&d_maxCount, 0, 100, 1);
		registerData(&d_epsilon, 0.0, 0.2, 0.001);
		registerData(&d_minEigThresh, 0.001, 0.1, 0.001);
    ImageFilter::init();
  }

  void applyFilter(const cv::Mat& in, cv::Mat& out, bool)
  {
    cv::Mat gray;
    if (in.empty()) return;

		if (!d_startTracking.getValue())
		{
			addInput(&d_points_in);
			in.copyTo(out);
			return;
		}
		else
			removeInput(&d_points_in);  // stop reading from input, start looping on
																	// self-extracted points

		if (in.type() != CV_8UC1)
			cv::cvtColor(in, gray, CV_BGRA2GRAY);
		else
			gray = in;

		if (m_prev.empty())
		{
			m_prev = gray.clone();
			return;
		}

		if (d_points_in.getValue().size() < 2) return;
		std::vector<cv::Vec2f> pts_in;
		pts_in.reserve(d_points_in.getValue().size());
		std::vector<cv::Vec2f> pts_out;
		pts_out.reserve(d_points_out.getValue().size());
		std::vector<uchar> status;
		status.reserve(pts_in.capacity());
		std::vector<float> error;
		error.reserve(pts_in.capacity());

		for (const defaulttype::Vec2f& pt : d_points_in.getValue())
			pts_in.push_back(cv::Point2f(pt.x(), pt.y()));
		//		for (const defaulttype::Vec2f& pt : d_points_out.getValue())
		//			pts_out.push_back(cv::Point2f(pt.x(), pt.y()));

		cv::TermCriteria tc =
				cv::TermCriteria(d_criteria_type.getValue(), d_maxCount.getValue(),
												 d_epsilon.getValue());
		cv::imshow("LEFT", m_prev);
		cv::imshow("RIGHT", gray);
		cv::waitKey(1);

		cv::calcOpticalFlowPyrLK(
				m_prev, gray, pts_in, pts_out, status, error,
				cv::Size(d_winSize.getValue().x(), d_winSize.getValue().y()),
				d_maxLevel.getValue(), tc, d_flags.getValue(),
				d_minEigThresh.getValue());

		helper::vector<defaulttype::Vec2f>* points_out = d_points_out.beginEdit();
		helper::vector<defaulttype::Vec2f>* points_in = d_points_in.beginEdit();
		points_out->clear();
		points_in->clear();
		for (const cv::Point2f& pt : pts_out)
		{
			points_out->push_back(defaulttype::Vec2f(pt.x, pt.y));
			points_in->push_back(defaulttype::Vec2f(pt.x, pt.y));
		}
		helper::vector<uchar>& status_out = *d_status_out.beginEdit();
		status_out.assign(status.begin(), status.end());
		helper::vector<float>& error_out = *d_error_out.beginEdit();
		error_out.assign(error.begin(), error.end());

		d_points_out.endEdit();
		d_points_in.endEdit();
		d_status_out.endEdit();
		d_error_out.endEdit();
		m_prev.release();
		m_prev = gray.clone();
		if (d_displayDebugWindow.getValue())
		{
			in.copyTo(out);
      for (const cv::Point2f& pt : pts_out)
				cv::circle(out, pt, 2, cv::Scalar(0, 255, 0), 1);
		}
	}

 private:
	cv::Mat m_prev;
};

SOFA_DECL_CLASS(OpticalFlow)

int OpticalFlowClass =
		core::RegisterObject("Optical flow filters from OpenCV").add<OpticalFlow>();

}  // namespace processor

}  // namespace OR

}  // namespace sofa

#endif  // SOFA_OR_PROCESSOR_OPTICALFLOW_H
