/*
-----------------------------------------------------------------------------
Filename:    BaseApplication.h
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _ 
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/                              
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#ifndef __BaseApplication_h_
#define __BaseApplication_h_

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

#include <OISEvents.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <OgreApplicationContext.h>
#include <OgreTrays.h>
#include <OgreCameraMan.h>
#include <OgreAdvancedRenderControls.h>

class BaseApplication : public OgreBites::ApplicationContext, public OgreBites::InputListener
{
public:
    BaseApplication(void);
    virtual ~BaseApplication(void);

    void go(void);

protected:
    void setup() override;
    virtual void chooseSceneManager(void);
    virtual void createCamera(void);
    virtual void createFrameListener(void);
    virtual void createScene(void) = 0; // Override me!
    virtual void destroyScene(void);
    virtual void createViewports(void);

    bool keyPressed(const OgreBites::KeyboardEvent &evt) override;

    Ogre::Camera* mCamera;
    Ogre::SceneNode* mCameraNode;
    Ogre::SceneManager* mSceneMgr;

    // OgreBites
    OgreBites::TrayManager* mTrayMgr;
    OgreBites::CameraMan* mCameraMan;       // basic camera controller
    OgreBites::AdvancedRenderControls* mControls;   
    bool mCursorWasVisible;                    // was cursor visible before dialog appeared
    bool mShutDown;
};

#endif // #ifndef __BaseApplication_h_
