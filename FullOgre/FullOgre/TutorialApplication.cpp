#include "TutorialApplication.h"

#include <OgreException.h>
#include <OgreConfigFile.h>
#include <OgreRenderWindow.h>
#include <OgreCamera.h> 
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include<OgreMeshManager.h>

using namespace Ogre;
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 
#define WIN32_LEAN_AND_MEAN 
#include "windows.h"
#endif

#ifdef __cplusplus 
extern "C" 
{ 
#endif
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 
	INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT ) 

#else int main(int argc, char *argv[]) 
#endif 
{ 
	TutorialApplication app;
	try {
	app.goOgre();
	}
	catch(Ogre::Exception& e) 
	{ 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 
	MessageBox( 
		NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL); 
#else 
		std::cerr << "An exception has occured: " << e.getFullDescription().c_str() << std::endl; 
#endif 
	}
	return 0;
} 

#ifdef __cplusplus 
} 
#endif





TutorialApplication::TutorialApplication(void) : mRoot(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),mTrayMgr(0)

{
}



TutorialApplication::~TutorialApplication(void)
{
	delete mRoot;
}

void TutorialApplication::createFrameListener()
{
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputManager = OIS::InputManager::createInputSystem(pl);

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));

    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);

    // Set initial mouse clipping size
    windowResized(mWindow);

    // Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

    mInputContext.mKeyboard = mKeyboard;
    mInputContext.mMouse = mMouse;
    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mInputContext, this);
    mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    mTrayMgr->hideCursor();

    // Create a params panel for displaying sample details
    Ogre::StringVector items;
    items.push_back("cam.pX");
    items.push_back("cam.pY");
    items.push_back("cam.pZ");
    items.push_back("");
    items.push_back("cam.oW");
    items.push_back("cam.oX");
    items.push_back("cam.oY");
    items.push_back("cam.oZ");
    items.push_back("");
    items.push_back("Filtering");
    items.push_back("Poly Mode");

    mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
    mDetailsPanel->setParamValue(9, "Bilinear");
    mDetailsPanel->setParamValue(10, "Solid");
    mDetailsPanel->hide();

    mRoot->addFrameListener(this);
}

bool TutorialApplication::goOgre()
{
		#ifdef _DEBUG
	  mResourcesCfg = "resources_d.cfg";
	  mPluginsCfg = "plugins_d.cfg";
	#else
	  mResourcesCfg = "resources.cfg";
	  mPluginsCfg = "plugins.cfg";
	#endif

	  mRoot = new Ogre::Root(mPluginsCfg);
	  Ogre::ConfigFile cf; 
	  cf.load(mResourcesCfg);
	Ogre::String name, locType; 
	Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();
while (secIt.hasMoreElements()) 
{ 
	Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext(); 
	Ogre::ConfigFile::SettingsMultiMap::iterator it;
	for (it = settings->begin(); it != settings->end(); ++it) 
	{ 
		locType = it->first; name = it->second;
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, locType);
	}
}
if(!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
		return false;

mWindow = mRoot->initialise(true, "TutorialApplication Render Window");

Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();



CreateScene();
createFrameListener();

// Start rendering loop 
mRoot->startRendering();


    return true;
}

void TutorialApplication::CreateScene()
{
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);




mCamera = mSceneMgr->createCamera("MainCam");
mCamera->setPosition(Ogre::Vector3(300, 300, 400));
mCamera->lookAt(Ogre::Vector3(0, 0, 0));
mCamera->setNearClipDistance(5);
Ogre::Viewport* vp = mWindow->addViewport(mCamera);
vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
mSceneMgr->setAmbientLight(ColourValue(1, 1, 1));

Ogre::Plane plane(Vector3::UNIT_Y, -10);
Ogre::MeshManager::getSingleton().createPlane("plane",
ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
1500,1500,20,20,true,1,5,5,Vector3::UNIT_Z);
Ogre::Entity* ent = mSceneMgr->createEntity("LightPlaneEntity", "plane");
mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);
ent->setMaterialName("Examples/BeachStones");
Ogre::SceneNode* node = mSceneMgr->createSceneNode("Node1");
mSceneMgr->getRootSceneNode()->addChild(node);

Ogre::Light* light1 = mSceneMgr->createLight("Light1");
light1->setType(Ogre::Light::LT_POINT);
light1->setPosition(0, 20, 0);
light1->setDiffuseColour(1.0f, 1.0f, 1.0f);

Ogre::Entity* LightEnt = mSceneMgr->createEntity("MyEntity","sphere.mesh");
Ogre::SceneNode* node2 = node->createChildSceneNode("node2");
node2->setScale(0.1f, 0.1f, 0.1f);
node2->setPosition(0, 20, 0);
node2->attachObject(LightEnt);

// Create the second point light
Ogre::Light* light2 = mSceneMgr->createLight("Light2");
light2->setType(Ogre::Light::LT_POINT);
light2->setPosition(20, 20, 200);
light2->setDiffuseColour(1.0f, 0.0f, 0.0f);

Ogre::Entity* LightEnt2 = mSceneMgr->createEntity("MyEntity2","sphere.mesh");
Ogre::SceneNode* node3 = node->createChildSceneNode("node3");
node3->setScale(0.1f, 0.1f, 0.1f);
node3->setPosition(20, 20, 200);
node3->attachObject(LightEnt2);

mSceneMgr->setAmbientLight(Ogre::ColourValue(.2, .2, .2));

Ogre::Entity* tudorEntity = mSceneMgr->createEntity("tudorhouse.mesh");
Ogre::SceneNode* node10 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "Node10"); 
node10->attachObject(tudorEntity);

Ogre::Light* light10 = mSceneMgr->createLight("Light10"); 
light10->setType(Ogre::Light::LT_POINT); 
light10->setPosition(Ogre::Vector3(250, 150, 250)); 
light10->setDiffuseColour(Ogre::ColourValue::White); 
light10->setSpecularColour(Ogre::ColourValue::White);

Ogre::Entity* ninjaEntity = mSceneMgr->createEntity("Ninja", "ninja.mesh");
Ogre::SceneNode *node11 = mSceneMgr->getRootSceneNode() ->createChildSceneNode("NinjaNode"); 
node11->setPosition(300,300,700);
node11->yaw(Degree(180));
node11->attachObject(ninjaEntity);

/*Ogre::Entity* cubeEntity = mSceneMgr->createEntity("cube", "cube.mesh");
Ogre::SceneNode *node12 = mSceneMgr->getRootSceneNode() ->createChildSceneNode("cubeNode"); 
node12->setPosition(500,300,500);
//node12->yaw(Degree(180));
node12->attachObject(cubeEntity);

Ogre::Entity* cubeEntity2 = mSceneMgr->createEntity("cube2", "cube.mesh");
Ogre::SceneNode *node13 = mSceneMgr->getRootSceneNode() ->createChildSceneNode("cubeNode2"); 
node13->setPosition(-200,200,500);
//node12->yaw(Degree(180));
node13->attachObject(cubeEntity2);
mCamera->setPosition(600, 800, 1000);

Ogre::Entity* cubeEntity3 = mSceneMgr->createEntity("cube3", "cube.mesh");
Ogre::SceneNode *node15 = mSceneMgr->getRootSceneNode() ->createChildSceneNode("cubeNode3"); 
node15->setPosition(500,300,300);
//node12->yaw(Degree(180));
node15->attachObject(cubeEntity3);

Ogre::Entity* cubeEntity4 = mSceneMgr->createEntity("cube4", "cube.mesh");
Ogre::SceneNode *node14 = mSceneMgr->getRootSceneNode() ->createChildSceneNode("cubeNode4"); 
node14->setPosition(500,300,100);
//node12->yaw(Degree(180));
node14->attachObject(cubeEntity4);*/

Ogre::Entity* BarrelEntity = mSceneMgr->createEntity("Barrel", "sphere.mesh");
Ogre::SceneNode *node16 = mSceneMgr->getRootSceneNode() ->createChildSceneNode("BarrelNode"); 
node16->setPosition(500,300,400);
//node12->yaw(Degree(180));
node16->attachObject(BarrelEntity);

mCamera->setPosition(600, 800, 1000);

return;
}


void TutorialApplication::windowResized(Ogre::RenderWindow* rw) 
{ 
	int left, top; 
	unsigned int width, height, depth;
	rw->getMetrics(width, height, depth, left, top);
	const OIS::MouseState& ms = mMouse->getMouseState();
	ms.width = width; ms.height = height;
}

void TutorialApplication::windowClosed(Ogre::RenderWindow* rw) 
{ 
	if(rw == mWindow) 
	{ 
		if(mInputManager) 
		{ 
			mInputManager->destroyInputObject(mMouse); mInputManager->destroyInputObject(mKeyboard);
			OIS::InputManager::destroyInputSystem(mInputManager); mInputManager = 0;
		}
	}
}

bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& fe) 
{ 
	if (mWindow->isClosed()) 
		return false;

	mKeyboard->capture();
	mMouse->capture();

	    mTrayMgr->frameRenderingQueued(fe);

	if (mKeyboard->isKeyDown(OIS::KC_ESCAPE)) return false;
		return true;
}

bool TutorialApplication::mouseMoved(const OIS::MouseEvent &arg)
{
	if(mTrayMgr->injectMouseMove(arg)) return true;

	mCameraMan->injectMouseMove(arg);
	return true;
}

bool TutorialApplication::keyPressed(const OIS::KeyEvent &arg)
{
	return true;
}

bool TutorialApplication::keyReleased(const OIS::KeyEvent &arg)
{
	return true;
}

bool TutorialApplication::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	return true;
}

bool TutorialApplication::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return true;
}