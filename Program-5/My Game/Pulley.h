/// \file Pulley.h
/// \brief Interface for the pulley class CPulley.

#ifndef __L4RC_GAME_PULLEY_H__
#define __L4RC_GAME_PULLEY_H__

#include "Object.h"
#include "Box2D\Box2D.h"

#include "Common.h"
#include "Component.h"

/// \brief The pulley class.
///
/// A pair of objects connected by a pulley joint, with
/// a pair of pulley wheels that are animated in code.
/// @image html screenshot6.png

class CPulley: 
  public CCommon,
  public LComponent{

  private:
    b2PulleyJoint* m_pJoint = nullptr; ///< Pointer to the pulley joint.
    float m_fJointLenA = 0; ///< Length of left side of pulley.

    b2Body* m_pWheel0 = nullptr; ///< Pointer to the left pulley wheel.
    b2Body* m_pWheel1 = nullptr; ///< Pointer to the right pulley wheel.
    float m_fWheelRad = 1; ///< Pulley wheel radius.

    b2Vec2 m_vElephantSpawnPt; ///< Elephant spawn point.

    b2Body* CreateWheel(float, float); ///< Create a pulley wheel.
    b2Body* CreateCrate(float, float); ///< Create the elephant's crate.
    b2Body* CreateObject(const b2Vec2&, float, eSprite); ///< Create an object.

  public:
    CPulley(float, float, float); ///< Constructor.

    void move(); ///< Rotate the pulley wheels.

    void CreateElephant(); ///< Create an elephant.
    bool DeleteElephant(); ///< Delete an elephant.
}; //CPulley

#endif //__L4RC_GAME_PULLEY_H__
