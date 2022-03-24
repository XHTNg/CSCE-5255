/// \file Ramp.cpp
/// \brief Code for the ramp class CRamp.

#include "Ramp.h"

#include "GameDefines.h"
#include "ObjectManager.h"
#include "Renderer.h"
#include "ComponentIncludes.h"

/// \brief Helper function to translate an array of points from Artist World
/// (measured in pixels from top left) to Physics WOrld (measured in
/// Physics World units from bottom left).
/// \param p Array of 2D points.
/// \param n Number of points in a.
/// \param w Double the distance to move the points in X direction.
/// \param h Double the distance to move the points in Y direction.

void RemapPoints(b2Vec2 p[], int n, int w, int h){
  for(int i=0; i<n; i++){
    p[i].x = RW2PW(p[i].x - w/2.0f);
    p[i].y = RW2PW(-p[i].y + h/2.0f);
  } //for
} //RemapPoints

/// Create a platform, a ramp down, a platform, a ramp up,
/// and a platform, form left to right.
/// \param x X coordinate in Physics World units.
/// \param y Y coordinate in Physics World units.

CRamp::CRamp(float x, float y){
  float pw, ph, rw, rh; //platform and ramp width and height in Render World
  m_pRenderer->GetSize(eSprite::Platform, pw, ph);
  m_pRenderer->GetSize(eSprite::DownRamp, rw, rh);

  const float PLAT_HT2 = RW2PW(ph)/2.0f; //half platform height
  const float PLAT_WIDTH2 = RW2PW(pw)/2.0f; //half platform width

  const float RAMP_HT2 = RW2PW(rh)/2.0f; //half ramp width
  const float RAMP_WIDTH2 = RW2PW(rw)/2.0f; //half ramp height

  const float xdelta = RAMP_WIDTH2 + PLAT_WIDTH2; //delta x from ramp to platform
  const float ydelta = RAMP_HT2 - PLAT_HT2; //delta y from ramp to platform

  x += PLAT_WIDTH2;
  y -= PLAT_HT2;
  CreatePlatform(x, y);

  x += xdelta; y -= ydelta;
  CreateRampDown(x, y);

  x += xdelta; y -= ydelta;
  CreatePlatform(x, y);

  x += xdelta; y += ydelta;
  CreateRampUp(x, y);

  x += xdelta; y += ydelta;
  CreatePlatform(x, y);
} //constructor

/// \param x X coordinate in Physics World units.
/// \param y Y coordinate in Physics World units.

void CRamp::CreatePlatform(float x, float y){
  float w, h; //width and height in Render World
  m_pRenderer->GetSize(eSprite::Platform, w, h);
  float w2 = RW2PW(w)/2.0f; //half width in Physics World
  float h2 = RW2PW(h)/2.0f; //half height in Physics World

  //body definition
  b2BodyDef bd; 
  bd.type = b2_staticBody;
  bd.position.Set(x, y);

  //shape
  b2PolygonShape s;
  s.SetAsBox(w2, h2);

  //fixture
  b2FixtureDef fd;
  fd.shape = &s;
  fd.density = 0.8f;
  fd.restitution = 0.2f;
  fd.friction = 0.9f;

  //body
  b2Body* pBody = m_pPhysicsWorld->CreateBody(&bd);
  m_pObjectManager->create(eSprite::Platform, pBody);
  pBody->CreateFixture(&fd);
} //CreatePlatform

/// \param x X coordinate in Physics World units.
/// \param y Y coordinate in Physics World units.

void CRamp::CreateRampDown(float x, float y){
  //body definition
  b2BodyDef bd; 
  bd.type = b2_staticBody;
  bd.position.Set(x, y);

  //shape
  b2Vec2 points[4];

  points[0] = b2Vec2(0, 0);
  points[1] = b2Vec2(0, 27);
  points[2] = b2Vec2(127, 97);
  points[3] = b2Vec2(127, 68);

  RemapPoints(points, 4, 128, 100);

  b2PolygonShape s;
  s.Set(points, 4);

  //fixture
  b2FixtureDef fd;
  fd.shape = &s;
  fd.density = 0.8f;
  fd.restitution = 0.2f;
  fd.friction = 0.9f;

  //body
  b2Body* pBody = m_pPhysicsWorld->CreateBody(&bd);
  m_pObjectManager->create(eSprite::DownRamp, pBody);
  pBody->CreateFixture(&fd);
} //CreateRampDown

/// \param x X coordinate in Physics World units.
/// \param y Y coordinate in Physics World units.

void CRamp::CreateRampUp(float x, float y){
  //body definition
  b2BodyDef bd; 
  bd.type = b2_staticBody;
  bd.position.Set(x, y);

  //shape
  b2Vec2 points[4];

  points[0] = b2Vec2(127, 27);
  points[1] = b2Vec2(127, 0);
  points[2] = b2Vec2(0, 68);
  points[3] = b2Vec2(0, 97);

  RemapPoints(points, 4, 128, 100);

  b2PolygonShape s;
  s.Set(points, 4);

  //fixture
  b2FixtureDef fd;
  fd.shape = &s;
  fd.density = 0.8f;
  fd.restitution = 0.2f;

  //body
  b2Body* pBody = m_pPhysicsWorld->CreateBody(&bd);
  m_pObjectManager->create(eSprite::UpRamp, pBody);
  pBody->CreateFixture(&fd);
} //CreateRampUp