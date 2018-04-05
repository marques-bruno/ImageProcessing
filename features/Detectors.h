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

#ifndef SOFA_OR_PROCESSOR_DETECTORS_H
#define SOFA_OR_PROCESSOR_DETECTORS_H

#include "common/ImageFilter.h"

#include <SofaORCommon/cvMat.h>

#include <sofa/helper/OptionsGroup.h>
#include <sofa/helper/vector.h>

#include <opencv2/xfeatures2d.hpp>

namespace sofaor
{
namespace processor
{
namespace features
{
struct BaseDetector
{
  BaseDetector(sofa::core::DataEngine* c)
  {
    m_obj = c;
  }

  virtual ~BaseDetector();

  virtual void enable(bool) = 0;

  virtual void init() = 0;

  virtual void registerData(ImageFilter* parent) = 0;

  virtual void detect(const common::cvMat&, const common::cvMat&,
                      std::vector<cv::KeyPoint>&);
  virtual void compute(const common::cvMat&, std::vector<cv::KeyPoint>&,
                       common::cvMat&);
  virtual void detectAndCompute(const common::cvMat&, const common::cvMat&,
                                std::vector<cv::KeyPoint>&, common::cvMat&);

 protected:
  cv::Ptr<cv::Feature2D> m_detector;
  sofa::core::DataEngine* m_obj;
};

struct SimpleBlobDetector : BaseDetector
{
  SimpleBlobDetector(sofa::core::DataEngine* c);
  void enable(bool);
  void init();

  virtual void registerData(ImageFilter* parent);

  virtual void detect(const common::cvMat& img, const common::cvMat& mask,
                      std::vector<cv::KeyPoint>& keypoints);

  virtual void compute(const common::cvMat&, std::vector<cv::KeyPoint>&,
                       common::cvMat&);
  virtual void detectAndCompute(const common::cvMat& img,
                                const common::cvMat& mask,
                                std::vector<cv::KeyPoint>& kpts,
                                common::cvMat&);

  sofa::Data<int> minThreshold;
  sofa::Data<int> maxThreshold;
  sofa::Data<bool> filterByArea;
  sofa::Data<int> minArea;
  sofa::Data<bool> filterByCircularity;
  sofa::Data<double> minCircularity;
  sofa::Data<bool> filterByConvexity;
  sofa::Data<double> minConvexity;
  sofa::Data<bool> filterByInertia;
  sofa::Data<double> minInertiaRatio;
};

struct ShiTomasiDetector : BaseDetector
{
  ShiTomasiDetector(sofa::core::DataEngine* c);
  void enable(bool);
  void init();
  virtual void registerData(ImageFilter* parent);

  virtual void detect(const common::cvMat&, const common::cvMat&,
                      std::vector<cv::KeyPoint>&);

  virtual void compute(const common::cvMat&, std::vector<cv::KeyPoint>&,
                       common::cvMat&);
  virtual void detectAndCompute(const common::cvMat& img,
                                const common::cvMat& mask,
                                std::vector<cv::KeyPoint>& kpts,
                                common::cvMat&);

  sofa::Data<int> maxCorners;
  sofa::Data<double> qualityLevel;
  sofa::Data<int> minDistance;
  sofa::Data<int> blockSize;
};

struct FASTDetector : BaseDetector
{
  FASTDetector(sofa::core::DataEngine* c);
  void enable(bool);
  void init();
  virtual void registerData(ImageFilter* parent);
  virtual void compute(const common::cvMat&, std::vector<cv::KeyPoint>&,
                       common::cvMat&);
  virtual void detectAndCompute(const common::cvMat& img,
                                const common::cvMat& mask,
                                std::vector<cv::KeyPoint>& kpts,
                                common::cvMat&);

  sofa::Data<int> threshold;
  sofa::Data<bool> nonmaxsuppression;
  sofa::Data<sofa::helper::OptionsGroup> type;
};
struct MSERDetector : BaseDetector
{
  MSERDetector(sofa::core::DataEngine* c);
  void enable(bool);
  void init();
  virtual void registerData(ImageFilter* parent);

  virtual void compute(const common::cvMat&, std::vector<cv::KeyPoint>&,
                       common::cvMat&);
  virtual void detectAndCompute(const common::cvMat& img,
                                const common::cvMat& mask,
                                std::vector<cv::KeyPoint>& kpts,
                                common::cvMat&);

  sofa::Data<int> delta;
  sofa::Data<int> minArea;
  sofa::Data<int> maxArea;
  sofa::Data<float> maxVariation;
  sofa::Data<float> minDiversity;
  sofa::Data<int> maxEvolution;
  sofa::Data<double> areaThreshold;
  sofa::Data<double> minMargin;
  sofa::Data<int> edgeBlurSize;
};
struct ORBDetector : BaseDetector
{
  ORBDetector(sofa::core::DataEngine* c);
  void enable(bool);
  void init();

  virtual void registerData(ImageFilter*);

  sofa::Data<int> nFeatures;
  sofa::Data<float> scaleFactor;
  sofa::Data<int> nLevels;
  sofa::Data<int> edgeThreshold;
  sofa::Data<int> firstLevel;
  sofa::Data<int> WTA_K;
  sofa::Data<sofa::helper::OptionsGroup> scoreType;
  sofa::Data<int> patchSize;
  sofa::Data<int> fastThreshold;
};
struct BRISKDetector : BaseDetector
{
  BRISKDetector(sofa::core::DataEngine* c);
  void enable(bool);
  void init();
  virtual void registerData(ImageFilter*);

  sofa::Data<int> threshold;
  sofa::Data<int> octaves;
  sofa::Data<float> npatternScale;
};
struct KAZEDetector : BaseDetector
{
  KAZEDetector(sofa::core::DataEngine* c);
  void enable(bool);
  void init();
  virtual void registerData(ImageFilter*);

  sofa::Data<bool> extended;
  sofa::Data<bool> upright;
  sofa::Data<float> threshold;
  sofa::Data<int> octaves;
  sofa::Data<int> sublevels;
  sofa::Data<sofa::helper::OptionsGroup> diffusivity;
};
struct AKAZEDetector : BaseDetector
{
  AKAZEDetector(sofa::core::DataEngine* c);
  void enable(bool);
  void init();
  virtual void registerData(ImageFilter*);

  sofa::Data<sofa::helper::OptionsGroup> descriptorType;
  sofa::Data<int> descriptorSize;
  sofa::Data<int> descriptorChannels;
  sofa::Data<float> threshold;
  sofa::Data<int> octaves;
  sofa::Data<int> sublevels;
  sofa::Data<sofa::helper::OptionsGroup> diffusivity;
};

#ifdef SOFAOR_OPENCV_CONTRIB_ENABLED

struct BRIEFDetector : BaseDetector
{
  BRIEFDetector(sofa::core::DataEngine* c);
  void enable(bool);
  void init();
  virtual void registerData(ImageFilter*);

  void detect(const common::cvMat&, const common::cvMat&,
              std::vector<cv::KeyPoint>&);
  virtual void detectAndCompute(const common::cvMat&, const common::cvMat&,
                                std::vector<cv::KeyPoint>&, common::cvMat&);

  sofa::Data<int> bytes;
  sofa::Data<bool> use_orientation;
};

struct SIFTDetector : BaseDetector
{
  SIFTDetector(sofa::core::DataEngine* c);
  void enable(bool);
  void init();
  virtual void registerData(ImageFilter*);

  sofa::Data<int> nFeatures;
  sofa::Data<int> nOctaveLayers;
  sofa::Data<double> contrastThreshold;
  sofa::Data<double> edgeThreshold;
  sofa::Data<double> sigma;
};

struct SURFDetector : BaseDetector
{
  SURFDetector(sofa::core::DataEngine* c);
  void enable(bool);
  void init();
  virtual void registerData(ImageFilter*);

  sofa::Data<double> threshold;
  sofa::Data<int> nOctaves;
  sofa::Data<int> nOctaveLayers;
  sofa::Data<bool> extended;
  sofa::Data<bool> upright;
};

struct DAISYDetector : BaseDetector
{
  DAISYDetector(sofa::core::DataEngine* c);
  void enable(bool);
  void init();
  virtual void registerData(ImageFilter*);

  void detect(const common::cvMat&, const common::cvMat&,
              std::vector<cv::KeyPoint>&);
  virtual void detectAndCompute(const common::cvMat&, const common::cvMat&,
                                std::vector<cv::KeyPoint>&, common::cvMat&);

  sofa::Data<float> radius;
  sofa::Data<int> q_radius;
  sofa::Data<int> q_theta;
  sofa::Data<int> q_hist;
  sofa::Data<sofa::helper::OptionsGroup> norm;
  sofa::Data<common::cvMat> H;
  sofa::Data<bool> interpolation;
  sofa::Data<bool> use_orientation;
};

#endif  // SOFAOR_OPENCV_CONTRIB_ENABLED

}  // namespace features
}  // namespace processor
}  // namespace sofaor

#endif  // SOFA_OR_PROCESSOR_DETECTORS_H
