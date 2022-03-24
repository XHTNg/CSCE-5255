/// \file ObjectManager.h
/// \brief Interface for the Object manager class CObjectManager.

#ifndef __L4RC_GAME_OBJECTMANAGER_H__
#define __L4RC_GAME_OBJECTMANAGER_H__

#include "GameDefines.h"

#include "BaseObjectManager.h"
#include "Object.h"
#include "Common.h"

/// \brief The object manager.
///
/// A collection of all of the game objects.

class CObjectManager: 
  public LBaseObjectManager<CObject>,
  public CCommon
{
  private:
    LParticleDesc2D m_cPDesc0; ///< Particle descriptor for balls in step mode.
    LParticleDesc2D m_cPDesc1; ///< Particle descriptor for collisions in step mode.
    LParticleDesc2D m_cPDesc2; ///< Particle descriptor for collisions in real-time mode.

    CObject* m_pCueBall = nullptr; ///< Cue ball object pointer.
    CObject* m_p8Ball = nullptr; ///< 8 ball object pointer.

    float m_fCueAngle = 0; ///< Cue ball impulse angle.
    bool m_bDrawImpulseVector = true; ///< Whether to draw the impulse vector.


  private:
    void BroadPhase(); ///< Ball, rail, and pocket collision response for all balls.
    void NarrowPhase(CObject*, CObject*); ///< Ball collision response for two balls.

    void DrawLineInDirection(Vector2 start, Vector2 direction, float distance);

    void DrawLineInDirectionButStayInsidePoolTable(Vector2 start, Vector2 direction, float distance);

    bool IsBallWithinPlayingArea(Vector2 ballPosition);

    bool IsPositionWithinPlayingArea(Vector2 ballPosition);

    Vector2 GetPointOfImpact();

    Vector2 GetPrecisePointOfImpact();

    bool ImpactedWith8Ball();

    Vector2 GetPointOfImpactTo8BallDirection();

    Vector2 GetOnHitCueBallDirection();

    Vector2 ReflectVector(const Vector2& a, const Vector2& reflectingSurface);

    Vector2 BallTravelingDirection();

    bool BallCollide(CObject*, CObject*, float&); ///< Ball collision response for two balls.
    void RailCollide(CObject*); ///< Collision response for ball with rail.
    void PocketCollide(CObject*); ///< Collision response for ball with pocket.



  public:
    CObjectManager(); ///< Constructor.
    ~CObjectManager(); ///< Destructor.

    void create(eSprite, const Vector2&); ///< Create new object.

    void clear(); ///< Reset to initial conditions.
    void move(); ///< Move all objects.
    
    void Draw(); ///< Draw all objects.

    float DistanceOfVector(const Vector2& v1);

    Vector2 PerpendicularOfVector(const Vector2& v1);

    Vector2 NormalizeVector(Vector2 v1);

    
    void ResetImpulseVector(); ///< Reset the Impulse Vector.
    void AdjustImpulseVector(float); ///< Adjust the Impulse Vector.
    void AdjustCueBall(float); ///< Move cue-ball up or down.
    void Shoot(); ///< Shoot the cue ball.

    bool BallDown(); ///< Is a ball down in a pocket?
    bool CueBallDown(); ///< Is the cue ball down in a pocket?
    bool AllStopped(); ///< Have all balls stopped moving?



}; //CObjectManager

#endif //__L4RC_GAME_OBJECTMANAGER_H__
