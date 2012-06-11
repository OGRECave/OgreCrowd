/*
-----------------------------------------------------------------------------
Filename:    OgreRecastApplication.h
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
#ifndef __OgreRecastApplication_h_
#define __OgreRecastApplication_h_

#include "BaseApplication.h"
#include "OgreRecastDefinitions.h"
#include "OgreRecast.h"
#include "OgreDetourCrowd.h"
#include "Character.h"
#include "OgreDetourTileCache.h"

class OgreRecastApplication : public BaseApplication
{
public:
    OgreRecastApplication(void);
    virtual ~OgreRecastApplication(void);

    /**
      * Calculate an intersection point of a ray with objects in the scene.
      **/
    bool rayQueryPointInScene(Ogre::Ray ray, unsigned long queryMask, Ogre::Vector3 &result, Ogre::MovableObject **foundMovable);

    /**
      * Retrieve a marker with specified name and optional material (to override default material).
      * Creates the marker if it does not exist yet.
      **/
    Ogre::SceneNode* getOrCreateMarker(Ogre::String name, Ogre::String materialName="");

    /**
      * Create a new character in the scene, backed with an agent in the crowd.
      * Character must have a unique name and will be positioned at specified position.
      **/
    Character* createCharacter(Ogre::String name, Ogre::Vector3 position);

    /**
      * Query flags used for ray intersection tests.
      * All scene entities but the navmesh should be in the default group.
      **/
    enum QueryFlags {
       DEFAULT_MASK = 0u,
       NAVMESH_MASK = 5u,
    };

    /**
      * Set to true to draw visual debug clues related to Recast/Detour navigation
      **/
    static const bool DEBUG_DRAW;

    /**
      * Set to true to use animated human characters, otherwise test cylinders will
      * be used to represent agents.
      **/
    static const bool HUMAN_CHARACTERS;

    /**
      * Place obstacles in the scene as separate meshes.
      **/
    static const bool OBSTACLES;

    /**
      * Determines whether demo app will build simple single navmesh,
      * or build a tiled navmesh using detourTileCache that supports temp obstacles.
      **/
    static const bool SINGLE_NAVMESH;

    /**
      * Determines whether also dungeon mesh will be queried when clicking to set
      * begin position or destination.
      * Nearest point to navmesh within certain bounds will be found.
      * Set to false to only query points exactly on the navmesh.
      **/
    static const bool RAYCAST_SCENE;

    /**
      * Determines whether agent steering mode of the demo will use a temp obstacle.
      * Set to false to steer an agent in the crowd instead.
      * Setting to true only has effect when SINGLE_NAVMESH is false.
      **/
    static const bool TEMP_OBSTACLE_STEERING;

    /**
      * Sets recast visual debugging geometry in the scene to visible (true) or hide
      * it (false).
      **/
    void setDebugVisibility(bool visible);


protected:
    /**
      * Initialise the scene and everything needed for pathfinding and steering.
      * Setup demo specific constructions.
      **/
    virtual void createScene(void);

    /**
      * Fix font loading.
      **/
    virtual void createFrameListener(void);

    /**
      * Handle mouse input.
      **/
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

    /**
      * Handle mouse movement.
      **/
    bool mouseMoved(const OIS::MouseEvent &arg);

    /**
      * Handle keyboard input when keys are pressed.
      **/
    virtual bool keyPressed( const OIS::KeyEvent &arg );

    /**
      * Handle keyboard input when keys are released.
      **/
    bool keyReleased(const OIS::KeyEvent &arg);

    /**
      * Update state for rendering a new frame.
      **/
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    /**
      * Update all agents to navigate to the new destination set by the end marker.
      * First agent well be repositioned at the begin marker.
      **/
    void UpdateAllAgentDestinations(void);

    /**
      * Find a new path between the markers and draw it as a line (only for debugging purposes).
      * Path should only be drawn if mDebug is true.
      * Also makes begin marker visible.
      * Calculated path is stored at slot pathNb, its target is identified with id targetId.
      **/
    void drawPathBetweenMarkers(int pathNb, int targetId);

    /**
      * Set path and begin marker visible or not.
      * Path will only be set visible if DRAW_DEBUG is true.
      **/
    void setPathAndBeginMarkerVisibility(bool visibility);

    /**
      * Give each agent in the scene (except the first one) a different random destination.
      **/
    void setRandomTargetsForCrowd(void);

    /**
      * Set all agents in the scene except the first to follow the first agent.
      **/
    void setFollowTargetForCrowd(Ogre::Vector3 targetDestination);

    /**
      * Set the same destination for all agents in the scene.
      **/
    void setDestinationForAllAgents(Ogre::Vector3 destination, bool adjustExistingPath= false);

    /**
      * Find a new path using Recast and draw it as a line in the scene. The calculated path is stored
      * in slot pathNb for possible future reuse and its target is identified by targetId (how to use this
      * id is up to the developer, it's not used by recast or the demo app).
      **/
    void calculateAndDrawPath(Ogre::Vector3 beginPos, Ogre::Vector3 endPos, int pathNb, int targetId);

    /**
      * Create an obstacle in the scene.
      **/
    Ogre::Entity* createObstacle(Ogre::String name, Ogre::Vector3 position, Ogre::Vector3 scale);

    /**
      * Current state the demo application is in.
      **/
    enum ApplicationState
    {
       SIMPLE_PATHFIND,     // All agents follow destination marker
       CROWD_WANDER,        // All agents wander off except first one
       FOLLOW_TARGET,       // All agents follow first one
       STEER_AGENT          // Manually steer first agent, others wander
    } mApplicationState;

private:
    /**
      * Ogre version of the code of the original Recast demo that comes with the recast distribution.
      **/
    OgreRecast* mRecast;

    /**
      * Ogre version of DetourTileCache.
      * Only used when SINGLE_NAVMESH is false.
      **/
    OgreDetourTileCache *mDetourTileCache;

    /**
      * Ray scene intersection query object.
      **/
    Ogre::RaySceneQuery* mRayScnQuery;

    /**
      * Ogre wrapper around the detourCrowd library
      **/
    OgreDetourCrowd *mDetourCrowd;

    /**
      * Overlay label drawn on the screen. Shows current application state.
      **/
    OgreBites::Label *mLabelOverlay;

    /**
      * Crosshair overlay shown in the center of the screen.
      **/
    Ogre::Overlay *mCrosshair;

    /**
      * All characters in the scene that represent an agent.
      **/
    std::vector<Character*> mCharacters;

    /**
      * The camera used in the agent steering demo mode
      **/
    Ogre::Camera *mChaseCam;

    /**
      * Buffers whether forward key was pressed this frame.
      **/
    bool mMoveForwardKeyPressed;

    /**
      * Buffers the horizontal mouse movement performed this frame.
      **/
    Ogre::Real mMouseMoveX;

    /**
      * Current visibility of recast visual debug structures.
      * True renders them in the scene, false hides them.
      **/
    bool mDebugDraw;

    /**
      * The scenenode containing the visual representation of the navmesh
      * and other static debug drawing geometry.
      **/
    Ogre::SceneNode *mNavMeshNode;

    std::vector<Ogre::Entity*> mDebugEntities;
};




#endif // #ifndef __OgreRecastApplication_h_
