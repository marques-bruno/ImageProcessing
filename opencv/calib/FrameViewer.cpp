#include "FrameViewer.h"

#include <sofa/core/ObjectFactory.h>
#include <sofa/core/visual/DrawToolGL.h>

#include <sofa/helper/gl/RAII.h>
#include <sofa/helper/system/gl.h>
#include <sofa/helper/system/glut.h>
#include <sofa/simulation/AnimateBeginEvent.h>

namespace sofa
{
namespace OR
{
namespace processor
{
SOFA_DECL_CLASS(FrameViewer)

int FrameViewerClass = core::RegisterObject(
													 "debug component to project images in OpenGL using "
													 "the a projection matrix")
													 .add<FrameViewer>();

FrameViewer::FrameViewer()
		: l_cam(initLink("cam",
										 "link to CameraSettings component containing and "
										 "maintaining the camera's parameters")),
			d_frame(initData(&d_frame, "img", "frame to display in opencv window")),
			d_depth(initData(&d_depth, -1.0f, "depth",
											 "[Optional] distance to the camera at which to render "
											 "the image (default is CamSettings' fz value")),
			d_mode(
					initData(&d_mode, "mode", "viewer mode (PERSPECTIVE, ORTHO, HIDDEN)"))
{
	sofa::helper::OptionsGroup *t = d_mode.beginEdit();
	t->setNames(3, "PERSPECTIVE", "ORTHO", "HIDDEN");
	t->setSelectedItem(0);
	d_mode.endEdit();
}

FrameViewer::~FrameViewer() {}
void FrameViewer::init()
{
	addInput(&d_frame);
	addInput(&d_depth);

	if (!l_cam.get())
		msg_error(getName() + "::init()") << "Error: No camera link set. "
																				 "Please use attribute 'cam' "
																				 "to define one";
	update();
}

void FrameViewer::update() {}
// Render from the viewpoint of the opengl's context
void FrameViewer::perspectiveDraw()
{
	std::stringstream imageString;
	imageString.write((const char *)d_frame.getValue().data,
										d_frame.getValue().total() * d_frame.getValue().elemSize());

	glEnable(GL_TEXTURE_2D);  // enable the texture
	glDisable(GL_LIGHTING);   // disable the light

	glBindTexture(GL_TEXTURE_2D, 0);  // texture bind
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, d_frame.getValue().cols,
							 d_frame.getValue().rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE,
							 imageString.str().c_str());
	// glTexImage2D (GL_TEXTURE_2D, 0, GL_LUMINANCE, m_imageWidth,
	// m_imageHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, m_imgData.c_str() );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
									GL_LINEAR);  // Linear Filtering

	//		float eps = 0.0;
	//		float z0 = 0.0;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	defaulttype::Vector3 p1;
	defaulttype::Vector3 p2;
	defaulttype::Vector3 p3;
	defaulttype::Vector3 p4;
	l_cam->getCornersPosition(p1, p2, p3, p4, d_depth.getValue());

	glBegin(GL_QUADS);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glTexCoord2f(1, 0);
	glVertex3f(p4[0], p4[1], p4[2]);
	glTexCoord2f(0, 0);
	glVertex3f(p1[0], p1[1], p1[2]);
	glTexCoord2f(0, 1);
	glVertex3f(p2[0], p2[1], p2[2]);
	glTexCoord2f(1, 1);
	glVertex3f(p3[0], p3[1], p3[2]);
	glEnd();

	// glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);     // enable light
	glDisable(GL_TEXTURE_2D);  // disable texture 2D
														 // glDepthMask (GL_TRUE);		// enable zBuffer
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}

void FrameViewer::orthoDraw()
{
	std::stringstream imageString;
	imageString.write((const char *)d_frame.getValue().data,
										d_frame.getValue().total() * d_frame.getValue().elemSize());

	glEnable(GL_TEXTURE_2D);  // enable the texture
	glDisable(GL_LIGHTING);   // disable the light

	glBindTexture(GL_TEXTURE_2D, 0);  // texture bind
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, d_frame.getValue().cols,
							 d_frame.getValue().rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE,
							 imageString.str().c_str());
	// glTexImage2D (GL_TEXTURE_2D, 0, GL_LUMINANCE, m_imageWidth,
	// m_imageHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, m_imgData.c_str() );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
									GL_LINEAR);  // Linear Filtering

	//		float eps = 0.0;
	//		float z0 = 0.0;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);



	glMatrixMode(GL_PROJECTION);  // init the projection matrix
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, -1, 1);  // orthogonal view
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glBegin(GL_QUADS);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	glTexCoord2f(0, 1);
	glVertex2f(0, 0);
	glTexCoord2f(1, 1);
	glVertex2f(1, 0);
	glTexCoord2f(1, 0);
	glVertex2f(1, 1);
	glTexCoord2f(0, 0);
	glVertex2f(0, 1);
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();


	// glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);     // enable light
	glDisable(GL_TEXTURE_2D);  // disable texture 2D
														 // glDepthMask (GL_TRUE);		// enable zBuffer
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}

void FrameViewer::draw(const core::visual::VisualParams *)
{
	if (d_frame.getValue().data)
	{
		switch (d_mode.getValue().getSelectedId())
		{
			case 0:
				perspectiveDraw();
				break;
			case 1:
				orthoDraw();
				break;
			default:
				break;
		}
	}
}

void FrameViewer::computeBBox(const core::ExecParams *params, bool)
{
	if (d_mode.getValue().getSelectedId() != 0) return;

	helper::vector<defaulttype::Vec3f> x;
	defaulttype::Vector3 p1;
	defaulttype::Vector3 p2;
	defaulttype::Vector3 p3;
	defaulttype::Vector3 p4;
	l_cam->getCornersPosition(p1, p2, p3, p4, d_depth.getValue());
	x.push_back(p1);
	x.push_back(p2);
	x.push_back(p3);
	x.push_back(p4);

	double minBBox[3] = {std::numeric_limits<double>::max(),
											 std::numeric_limits<double>::max(),
											 std::numeric_limits<double>::max()};
	double maxBBox[3] = {-std::numeric_limits<double>::max(),
											 -std::numeric_limits<double>::max(),
											 -std::numeric_limits<double>::max()};

	for (unsigned int i = 0; i < x.size(); i++)
	{
		const defaulttype::Vec3f &p = x[i];
		for (int c = 0; c < 3; c++)
		{
			if (p[c] > maxBBox[c]) maxBBox[c] = p[c];
			if (p[c] < minBBox[c]) minBBox[c] = p[c];
		}
	}
	this->f_bbox.setValue(
			params, sofa::defaulttype::TBoundingBox<double>(minBBox, maxBBox));
}

}  // namespace processor
}  // namespace OR
}  // namespace sofa
