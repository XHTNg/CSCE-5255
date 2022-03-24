/// \file ObjectManager.h
/// \brief Interface for the object manager CObjectManager.

#ifndef __L4RC_GAME_OBJECTMANAGER_H__
#define __L4RC_GAME_OBJECTMANAGER_H__

#include <vector>

#include "Object.h"

#include "Component.h"
#include "Common.h"
#include "Settings.h"

/// \brief object manager.
///
/// object manager is an abstract representation of all of
/// the objects in the game.

class CObjectManager: 
  public LComponent, 
  public LSettings,
  public CCommon 
{
  private:
    std::vector<CObject*> m_stdList; ///< Object list.

  public:
    CObjectManager(); ///< Constructor.
    ~CObjectManager(); ///< Destructor.

    void CreateObject(eSprite t, b2Body* p); ///< Create object.

    b2Body* CreateDynamicObject(eSprite object, float x, float y, float density, float elasticity);

    b2Body* CreateStaticObject(eSprite object, float x, float y, float elasticity);

    b2Body* CreateBoxOfType(eSprite object, float x, float y, float density, float elasticity, b2BodyType type);

    b2Body* CreateObjectOfType(eSprite object, float x, float y, float density, float elasticity, b2BodyType type, b2Vec2 corners[], int numbersOfCorners, float angle);

    b2Body* CreateCircleOfType(eSprite object, float x, float y, float density, float elasticity, b2BodyType type);

    void clear(); ///< Reset to initial conditions.
    void draw(); ///< Draw all objects.

    void CreateWorldEdges(); ///< Create the edges of the world.
}; //CObjectManager

#endif //__L4RC_GAME_OBJECTMANAGER_H__