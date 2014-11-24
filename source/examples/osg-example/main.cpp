
#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable : 4100) // ignore unreferenced parameter warning introduced in osg
#endif

#include <osg/Geode>
#include <osg/Group>

#include <osg/ShapeDrawable>
#include <osg/Shape>

#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osg/GraphicsContext>

#include <osg/Texture2D>

#include <osgViewer/Viewer>

#include <osgGA/TrackballManipulator>

#ifdef _MSC_VER
    #pragma warning(pop)
#endif


#include <iostream>

#include "impl.h"


GlobjectsInterface globjectsInterface;

class EventHandler : public osgGA::GUIEventHandler
{
public:
    virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter&) override;
};

bool EventHandler::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter&)
{
    switch (ea.getEventType())
    {
        case osgGA::GUIEventAdapter::KEYDOWN:
        {
            if (ea.getKey()==osgGA::GUIEventAdapter::KEY_Right)
            {
                std::cout << "Got key input" << std::endl;
                return true;
            }
            return false;
        }

        case  osgGA::GUIEventAdapter::RESIZE:
        {
            int x = ea.getWindowWidth();
            int y = ea.getWindowHeight();

            globjectsInterface.resize(x, y);

            return false;
        }

        default:
        {
            return false;
        }
    }
}

//void EventHandler::getUsage(osg::ApplicationUsage& usage) const
//{
//    usage.addKeyboardMouseBinding("Left Arrow","Advance to next equation");
//    usage.addKeyboardMouseBinding("Right Array","Move to previous equation");
//}


osg::GraphicsContext * createContext()
{
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;

    traits->x = 100;
    traits->y = 100;
    traits->width = 800;
    traits->height = 600;
    traits->windowDecoration = true;
    traits->doubleBuffer = true;
    traits->samples = 4;
    traits->sampleBuffers = 1;

    return osg::GraphicsContext::createGraphicsContext(traits.get());
}

int main(int argc, char* argv[])
{
    osg::ArgumentParser arguments(&argc, argv);

    arguments.getApplicationUsage()->setDescription(arguments.getApplicationName()+" is the example which demonstrates osg together with globjects.");
    arguments.getApplicationUsage()->setCommandLineUsage(arguments.getApplicationName()+" [options] filename ...");
    arguments.getApplicationUsage()->addCommandLineOption("-h or --help","Display this information");

    if (arguments.readHelpType() != osg::ApplicationUsage::NO_HELP)
    {
        arguments.getApplicationUsage()->write(std::cout);
        return 0;
    }

    osg::Node * model = osgDB::readNodeFiles(arguments);

    if (!model)
    {
        //osg::ShapeDrawable * shape = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(), 2));
        osg::ShapeDrawable * shape = new osg::ShapeDrawable(new osg::Box(osg::Vec3(), 10));
        shape->setColor(osg::Vec4f(1.0f, 1.0f, 0.0f, 1.0f));
        osg::Geode * geode = new osg::Geode;
        geode->addDrawable(shape);

        model = geode;
    }

    osg::Group * root = new osg::Group;
    root->addChild(model);

    osgViewer::Viewer viewer;

    osg::ref_ptr<osg::GraphicsContext> context = createContext();

    viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);

    viewer.getCamera()->setGraphicsContext(context);
    viewer.getCamera()->setViewport(new osg::Viewport(0, 0, 800, 600));

    viewer.addEventHandler(new EventHandler);
    viewer.setCameraManipulator(new osgGA::TrackballManipulator);
    viewer.setSceneData(root);

    osg::Camera * camera = viewer.getCamera();
    //camera->setRenderOrder(osg::Camera::PRE_RENDER);
    camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);

    osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;

    texture->setTextureSize(800, 600);
    texture->setInternalFormat(GL_RGBA);
    texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
    texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
    texture->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
    texture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);

    camera->attach(osg::Camera::COLOR_BUFFER, texture);
    camera->attach(osg::Camera::DEPTH_BUFFER, GL_DEPTH_COMPONENT);

    /*viewer.getCamera()->setClearColor(osg::Vec4f(0.2f,0.3f,0.4f,1.0f));
    viewer.getCamera()->setProjectionMatrixAsPerspective(45.0, 4.0/3.0, 0.001, 10.0);
    //viewer.getCamera()->setViewMatrixAsLookAt(osg::Vec3(-1,-1,-1), osg::Vec3(), osg::Vec3(0,1,0));
    viewer.getCamera()->setViewMatrix(osg::Matrix::lookAt(osg::Vec3(-1,-1,-1), osg::Vec3(0,1,0), osg::Vec3(0,1,0.3f)));*/

    if (!viewer.isRealized())
        viewer.realize();

    viewer.init();

    bool initialized = false;

    while (!viewer.done())
    {
        viewer.frame();
        /*viewer.advance();
        viewer.eventTraversal();
        viewer.updateTraversal();
        viewer.renderingTraversals();*/

        if (context->valid())
        {
            context->makeCurrent();

            if (!initialized)
            {

                unsigned int contextID = context->getState()->getContextID();
                auto t = texture->getTextureObject(contextID);

                globjectsInterface.initialize();
                globjectsInterface.resize(800, 600);

                globjectsInterface.setupTexture(t->id(), t->target());

                initialized = true;
            }

            globjectsInterface.paint();
        }
    }

    return 0;
}
