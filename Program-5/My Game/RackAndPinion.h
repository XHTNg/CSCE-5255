/// \file RackAndPinion.h
/// \brief Interface for the rack and pinion class CRackAndPinion.

#ifndef __L4RC_GAME_RACKANDPINION_H__
#define __L4RC_GAME_RACKANDPINION_H__

#include "Object.h"
#include "Box2D\Box2D.h"

#include "Component.h"
#include "Common.h"

/// \brief Rack and pinion.
///
/// A rack and pinion connected by a prismatic joint in Physics World.
/// A rack and pinion is a type of linear actuator that comprises a pair of
/// gears which convert rotational motion into linear motion. A circular gear
/// called the pinion engages teeth on a linear gear bar called the rack.
/// Rotational motion applied to the pinion causes the rack to move relative to 
/// the pinion, thereby translating the rotational motion of the pinion into
/// linear motion.
/// @image html screenshot3.png

class CRackAndPinion:
  public LComponent,
  public CCommon
{
  private:
    b2RevoluteJoint* m_pRevoluteJoint = nullptr; ///< Pointer to revolute joint for pinion.
    b2PrismaticJoint* m_pPrismaticJoint = nullptr; ///< Pointer to prismatic joint for rack.
    float m_fLimit = 0; ///< Translation limit for prismatic joint.

    b2Body* CreateRack(float, float); ///< Create a rack.
    b2Body* CreatePinion(float, float); ///< Create a pinion.
    b2Body* CreateBase(float, float); ///< Create an invisible gentry.

  public:
    CRackAndPinion(float, float); ///< Constructor.

    void move(); ///< Reverse the rack when it hits the limits.
    void reverse(); ///< Reverse the motor.
}; //CRackAndPinion

#endif //__L4RC_GAME_RACKANDPINION_H__
