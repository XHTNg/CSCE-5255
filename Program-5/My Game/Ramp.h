/// \file Ramp.h
/// \brief Interface for the ramp class CRamp.

#ifndef __L4RC_GAME_RAMP_H__
#define __L4RC_GAME_RAMP_H__

#include "Object.h"
#include "Box2D\Box2D.h"

#include "Component.h"
#include "Common.h"

/// \brief Some platforms and ramps.
///
/// Three platforms and two ramps intended to be driven on by the car in Level 5.
/// @image html screenshot4.png

class CRamp: 
  public LComponent,
  public CCommon
{
  private:
    void CreatePlatform(float, float); ///< Create a platform.
    void CreateRampDown(float, float); ///< Create a down ramp.
    void CreateRampUp(float, float); ///< Create an up ramp.

  public:
    CRamp(float, float); ///< Constructor.
}; //CRamp

#endif //__L4RC_GAME_RAMP_H__
