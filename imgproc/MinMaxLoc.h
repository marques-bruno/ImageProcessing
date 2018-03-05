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

#ifndef SOFA_OR_PROCESSOR_MINMAXLOC_H
#define SOFA_OR_PROCESSOR_MINMAXLOC_H

#include "common/ImageFilter.h"

namespace sofaor
{
namespace processor
{
namespace imgproc
{
class MinMaxLoc : public ImageFilter
{

 public:
  SOFA_CLASS(MinMaxLoc, ImageFilter);

  sofa::Data<double> d_min;
  sofa::Data<double> d_max;
  sofa::Data<sofa::defaulttype::Vec2i> d_minLoc;
  sofa::Data<sofa::defaulttype::Vec2i> d_maxLoc;
  sofa::Data<common::cvMat> d_mask;

  MinMaxLoc();

  void init();

  void applyFilter(const cv::Mat& in, cv::Mat& /*out*/, bool);
};

SOFA_DECL_CLASS(MinMaxLoc)

int MinMaxLocClass =
    sofa::core::RegisterObject("OpenCV's implementation of cv::MinMaxLoc")
        .add<MinMaxLoc>();

}  // namespace imgproc
}  // namespace processor
}  // namespace sofaor
#endif  // SOFA_OR_PROCESSOR_MINMAXLOC_H
