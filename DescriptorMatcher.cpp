#include "DescriptorMatcher.h"

#include <sofa/core/ObjectFactory.h>
#include <sofa/simulation/AnimateBeginEvent.h>

namespace sofa
{
namespace OR
{
namespace processor
{
SOFA_DECL_CLASS(DescriptorMatcher)

int DescriptorMatcherClass =
    core::RegisterObject("OpenCV component matching descriptors")
        .add<DescriptorMatcher>();

DescriptorMatcher::DescriptorMatcher()
    : d_matcherType(initData(&d_matcherType, "matcher",
                             "type of matcher to use (BRUTEFORCE or FLANN).")),
      d_matchingAlgo(initData(
          &d_matchingAlgo, "algo",
          "matching algorithm to use (STANDARD, KNN_MATCH, RADIUS_MATCH).")),
      d_k(initData(&d_k, 2, "k",
                   "k Count of best matches found per each query descriptor or "
                   "less if a query descriptor has less than k possible "
                   "matches in total.")),
      d_maxDistance(initData(
          &d_maxDistance, .0f, "maxDistance",
          "maxDistance Threshold for the distance between matched descriptors. "
          "Distance means here metric distance (e.g. Hamming distance), not "
          "the distance between coordinates (which is measured in Pixels)!")),

      d_queryDescriptors(initData(&d_queryDescriptors, "queryDescriptors",
                                  "Query set of descriptors")),
      d_trainDescriptors(initData(&d_trainDescriptors, "trainDescriptors",
                                  "Train set of descriptors")),
      d_matches(initData(&d_matches, "matches", "output array of matches", true,
                         true))
{
  f_listening.setValue(true);

  sofa::helper::OptionsGroup* t = d_matcherType.beginEdit();
  t->setNames(MatcherType_COUNT, "FLANN", "BRUTEFORCE");
  t->setSelectedItem(0);
  d_matcherType.endEdit();

  std::cout << "MatcherType: " << d_matcherType.getValue().getSelectedItem()
            << std::endl;

  if (d_matcherType.getValue().getSelectedId() == FLANN)
    m_matcher = new cv::FlannBasedMatcher();
  else
    m_matcher = new cv::BFMatcher();

  t = d_matchingAlgo.beginEdit();
  t->setNames(MatchingAlgo_COUNT, "STANDARD", "KNN_MATCH", "RADIUS_MATCH");
  t->setSelectedItem(0);
  d_matchingAlgo.endEdit();
}

DescriptorMatcher::~DescriptorMatcher() {}
void DescriptorMatcher::init()
{
  addInput(&d_queryDescriptors);
  addInput(&d_trainDescriptors);
  addOutput(&d_matches);
  setDirtyValue();
}
void DescriptorMatcher::update()
{
  std::vector<std::vector<cv::DMatch> > matches;
  if (d_matchingAlgo.getValue().getSelectedId() == STANDARD_MATCH)
    m_matcher->match(d_queryDescriptors.getValue(),
                     d_trainDescriptors.getValue(), matches[0]);
  if (d_matchingAlgo.getValue().getSelectedId() == KNN_MATCH)
    m_matcher->knnMatch(d_queryDescriptors.getValue(),
                        d_trainDescriptors.getValue(), matches, d_k.getValue());
  if (d_matchingAlgo.getValue().getSelectedId() == RADIUS_MATCH)
    m_matcher->radiusMatch(d_queryDescriptors.getValue(),
                           d_trainDescriptors.getValue(), matches,
                           d_maxDistance.getValue());

  cleanDirty();

  sofa::helper::vector<sofa::helper::vector<common::cvDMatch> >* vec =
      d_matches.beginEdit();
  vec->clear();
  for (std::vector<cv::DMatch>& matchVec : matches)
  {
    vec->push_back(helper::vector<common::cvDMatch>());
    for (cv::DMatch& match : matchVec)
        vec->back().push_back(common::cvDMatch(match));
  }
  d_matches.endEdit();
}

void DescriptorMatcher::reinit() {}
void DescriptorMatcher::handleEvent(sofa::core::objectmodel::Event* e)
{
  if (sofa::simulation::AnimateBeginEvent::checkEventType(e)) this->update();
}

}  // namespace processor
}  // namespace OR
}  // namespace sofa
