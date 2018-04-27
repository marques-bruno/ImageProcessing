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

#ifndef SOFACV_IMGPROC_TEMPLATEMATCHER_H
#define SOFACV_IMGPROC_TEMPLATEMATCHER_H

#include "ImageProcessingPlugin.h"
#include "common/ImageFilter.h"

#include <sofa/helper/OptionsGroup.h>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

namespace sofacv
{
namespace imgproc
{
class SOFA_IMAGEPROCESSING_API TemplateMatcher : public common::ImageFilter
{
 public:
  SOFA_CLASS(TemplateMatcher, common::ImageFilter);

  sofa::Data<cvMat> d_template;
  sofa::Data<sofa::helper::OptionsGroup> d_method;

  TemplateMatcher();

  void init() override;

  void applyFilter(const cv::Mat& in, cv::Mat& out, bool) override;
};

}  // namespace imgproc
}  // namespace sofacv
#endif  // SOFACV_IMGPROC_TEMPLATEMATCHER_H
