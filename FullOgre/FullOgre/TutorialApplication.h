#pragma once
#include <OgreRoot.h>
#include <OgreWindowEventUtilities.h>
#include <OgreFrameListener.h>

#include <OISEvents.h> 
#include <OISInputManager.h> 
#include <OISKeyboard.h> 
#include <OISMouse.h>

#include <SdkCameraMan.h>
#  include <SdkTrays.h>
#include<InputContext.h>
class TutorialApplication  : public Ogre::WindowEventListener,
							 public Ogre::FrameListener,public OIS::KeyListener,
							 public OIS::MouseListener,OgreBites::SdkTrayListener

{
private:
	Ogre::Root* mRoot;
	Ogre::String mResourcesCfg;
	Ogre::String mPluginsCfg;
	Ogre::RenderWindow* mWindow;
	Ogre::SceneManager* mSceneMgr; 
	Ogre::Camera* mCamera;
	OIS::InputManager* mInputManager; 
	OIS::Mouse* mMouse; 
	OIS::Keyboard* mKeyboard;
	OgreBites::SdkCameraMan* mCameraMan;
	OgreBites::SdkTrayManager* mTrayMgr;
	OgreBites::ParamsPanel*     mDetailsPanel;
	OgreBites::InputContext     mInputContext;
	virtual void windowResized( Ogre::RenderWindow* rw);
	virtual void windowClosed(Ogre::RenderWindow* rw);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);

public:
	TutorialApplication(void);
	virtual ~TutorialApplication(void);

	virtual bool keyPressed(const OIS::KeyEvent &arg);
	virtual bool keyReleased(const OIS::KeyEvent &arg);	
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	 //virtual bool keyPressed(const OIS::KeyEvent &arg);
    //virtual bool keyReleased(const OIS::KeyEvent &arg);
    virtual bool mouseMoved(const OIS::MouseEvent &arg);
	virtual void createFrameListener();
	void CreateScene();
	bool goOgre();
};

