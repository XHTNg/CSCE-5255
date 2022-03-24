/// \file Gear.h
/// \brief Interface for the gear class CGear.

#ifndef __L4RC_GAME_GEAR_H__
#define __L4RC_GAME_GEAR_H__

#include "Object.h"
#include "Box2D\Box2D.h"
#include <vector>

#include "Component.h"
#include "Common.h"

/// \brief A system of interlocking gears.
///
/// The number and positions of the gears are hard-coded 
/// but there is enough storage for any number of them.
/// @image html screenshot1.png

class CGear: 
  public LComponent,
  public CCommon
{
  private:
    std::vector<b2Body*> m_stdGear; ///< Gear body pointers.
    std::vector<b2RevoluteJoint*> m_stdJoint; ///< Revolute joint pointers.

    b2Body* CreateBase(float, float); ///< Create invisible base in Physics World.
    void CreateRoundGears(float, float); ///< Create round gears in Physics World.
    void CreateSquareGears(float, float); ///< Create square gears in Physics World.
    b2Body* CreateGear(float, float, eSprite); ///< Create gear wheel in Physics World.
    void CreateGearJoint(UINT, UINT, float); ///< Create gear joint in Physics World.

  public:
    CGear(float, float); ///< Constructor.
    void reverse(); ///< Reverse gears.
}; //CGear

#endif //__L4RC_GAME_GEAR_H__
