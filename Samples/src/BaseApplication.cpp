/*
-----------------------------------------------------------------------------
Filename:    BaseApplication.cpp
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
#include "BaseApplication.h"
#include "Ogre.h"

//-------------------------------------------------------------------------------------
BaseApplication::BaseApplication(void)
    : OgreBites::ApplicationContext("OgreCrowd"),
    mCamera(0),
    mSceneMgr(0),
    mTrayMgr(0),
    mCameraMan(0),
    mControls(0),
    mCursorWasVisible(false),
    mShutDown(false)
{
}
//-------------------------------------------------------------------------------------
BaseApplication::~BaseApplication(void)
{
}

//-------------------------------------------------------------------------------------
void BaseApplication::chooseSceneManager(void)
{
    // Get the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager();
    mSceneMgr->addRenderQueueListener(getOverlaySystem());
    // register our scene with the RTSS
    auto shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(mSceneMgr);
}
//-------------------------------------------------------------------------------------
void BaseApplication::createCamera(void)
{
    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");

    mCameraNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    mCameraNode->attachObject(mCamera);
    // Position it at 500 in Z direction
    mCameraNode->setPosition(Ogre::Vector3(0,0,80));
    // Look back along -Z
    mCameraNode->lookAt(Ogre::Vector3(0,0,-300), Ogre::Node::TS_PARENT);
    mCamera->setNearClipDistance(0.1);

    mCameraMan = new OgreBites::CameraMan(mCameraNode);   // create a default camera controller
    mCameraMan->setStyle(OgreBites::CS_ORBIT);
    addInputListener(mCameraMan);
}
//-------------------------------------------------------------------------------------
void BaseApplication::createFrameListener(void)
{
    mTrayMgr = new OgreBites::TrayManager("InterfaceName", getRenderWindow());
    mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    mTrayMgr->hideCursor();
    addInputListener(mTrayMgr);

    mControls = new OgreBites::AdvancedRenderControls(mTrayMgr, mCamera);
    addInputListener(mControls);
}
//-------------------------------------------------------------------------------------
void BaseApplication::destroyScene(void)
{
    if (mControls) delete mControls;
    if (mTrayMgr) delete mTrayMgr;
    if (mCameraMan) delete mCameraMan;
}
//-------------------------------------------------------------------------------------
void BaseApplication::createViewports(void)
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = getRenderWindow()->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0.2,0.2,0.2));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}
//-------------------------------------------------------------------------------------
void BaseApplication::go(void)
{
    initApp();
    mRoot->startRendering();
    // clean up
    destroyScene();
    closeApp();
}
//-------------------------------------------------------------------------------------
void BaseApplication::setup(void)
{
    OgreBites::ApplicationContext::setup();

    addInputListener(this);
    chooseSceneManager();
    createCamera();
    createViewports();

    // Create the scene
    createScene();

    createFrameListener();
}

bool BaseApplication::keyPressed(const OgreBites::KeyboardEvent &evt)
{
    if(evt.keysym.sym == 27)
        getRoot()->queueEndRendering();

    return false;
}