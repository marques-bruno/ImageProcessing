#ifndef IMAGEFILTERDISPLAY_H
#define IMAGEFILTERDISPLAY_H

#include "ProcessOR/initPlugin.h"
#include "common/ImageFilter.h"
#include "gui/ImageFilterModel.h"

#include <SofaQtQuickGUI/SofaComponent.h>
#include <SofaQtQuickGUI/SofaQtQuickGUI.h>

#include <QObject>
#include <QtQuick/QQuickFramebufferObject>

#include <math.h>

namespace sofaor
{
namespace processor
{
class ImageFilterDisplay;

class SOFA_PROCESSOR_API FilterRenderer
    : public QQuickFramebufferObject::Renderer
{
 public:
  FilterRenderer(
      const sofaor::processor::ImageFilterDisplay* imageFilterDisplay);
  void render();
  QOpenGLFramebufferObject* createFramebufferObject(const QSize& size);

 private:
  const sofaor::processor::ImageFilterDisplay* m_imageFilterDisplay;
};

class SOFA_PROCESSOR_API ImageFilterDisplay : public QQuickFramebufferObject
{
  Q_OBJECT

  Q_PROPERTY(ImageFilterModel* imageFilterModel READ imageFilterModel WRITE
                 setImageFilterModel)  // NOTIFY imageFilterModelChanged)
 public:
  Renderer* createRenderer() const;

  ImageFilterModel* imageFilterModel() const;

 public slots:
  void sendMouseEvent(int eventType, int buttonFlags, int modifierFlags,
                      int mousex, int mousey);

 protected:
  void setImageFilterModel(ImageFilterModel* imageFilterModel);

 private:
  ImageFilterModel* m_imageFilterModel;
};

}  // namespace processor
}  // namespace sofaor

#endif  // IMAGEFILTERDISPLAY_H
