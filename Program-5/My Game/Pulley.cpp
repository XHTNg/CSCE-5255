/// \file Pulley.cpp
/// \brief Code for the pulley class CPulley.

#include "Pulley.h"

#include "GameDefines.h"
#include "ObjectManager.h"
#include "Renderer.h"
#include "ComponentIncludes.h"

void b2SetAngle(b2Body*, const float);

/// \param x X coordinate in Physics World units.
/// \param y Y coordinate in Physics World units.
/// \param w Pulley wheel horizontal separation in Physics World units.

CPulley::CPulley(float x, float y, float w){
  //crate calculations
  const float fCrateWidth = m_pRenderer->GetWidth(eSprite::Crate); //crate width in Render World
  const float fCrateHt = m_pRenderer->GetHeight(eSprite::Crate); //crate height in Render World
  const float fCrateWidth2 = RW2PW(fCrateWidth)/2.0f; //crate half width in Physics World
  const float fCrateHt2 = RW2PW(fCrateHt)/2.0f; //crate half height in Physics World

  //safe calculations

  const float fSafeWidth = m_pRenderer->GetWidth(eSprite::Safe); //safe width in Render World
  const float fSafeHt = m_pRenderer->GetHeight(eSprite::Safe); //safe height in Render World
  const float fSafeWidth2 = RW2PW(fSafeWidth)/2.0f; //safe half width in Physics World
  const float fSafeHt2 = RW2PW(fSafeHt)/2.0f; //safe half height in Physics World

  //pulley wheel calculations

  const float fWheelDiam = m_pRenderer->GetWidth(eSprite::Pulley); //pulley wheel diameter in Render World
  m_fWheelRad = RW2PW(fWheelDiam)/2.0f - RW2PW(4); //pulley wheel radius in Physics World
  const float fWheelSep2 = w/2.0f; //half pulley wheel separation in Physics World
  const float fWheelAlt = 2.0f*(y - 1.2f*m_fWheelRad); //wheel altitude on screen

   //calculate positions
  
  const b2Vec2 vCratePos = b2Vec2(x - fWheelSep2 - m_fWheelRad, fSafeHt2);
  const b2Vec2 vSafePos = b2Vec2(x + fWheelSep2 + m_fWheelRad, fWheelAlt - 2.0f*m_fWheelRad - fCrateHt2);

  const b2Vec2 vWheelPos0 = b2Vec2(x - fWheelSep2, fWheelAlt);
  const b2Vec2 vWheelPos1 = b2Vec2(x + fWheelSep2, fWheelAlt);

  //create bodies

  b2Body* pCrate = CreateCrate(vCratePos.x, vCratePos.y);
  b2Body* pSafe = CreateObject(vSafePos, 0.5f, eSprite::Safe);

  m_pWheel0 = CreateWheel(vWheelPos0.x, vWheelPos0.y);
  m_pWheel1 = CreateWheel(vWheelPos1.x, vWheelPos1.y);

  //calculate anchor points

  const b2Vec2 vCrateAnchor = vCratePos + b2Vec2(0.0f, fCrateHt2);
  const b2Vec2 vSafeAnchor = vSafePos + b2Vec2(0.0f, fSafeHt2);

  const b2Vec2 vWheelAnchor0 = vWheelPos0 - b2Vec2(m_fWheelRad, 0.0f);
  const b2Vec2 vWheelAnchor1 = vWheelPos1 + b2Vec2(m_fWheelRad, 0.0f);

  //create pulley joint

  b2PulleyJointDef jd;
  jd.Initialize(pCrate, pSafe, //bodies
    vWheelAnchor0, vWheelAnchor1, //anchors on wheels
    vCrateAnchor, vSafeAnchor, //anchors on bodies
    1.0f); 

  m_pJoint = (b2PulleyJoint*)m_pPhysicsWorld->CreateJoint(&jd);
  m_fJointLenA = m_pJoint->GetCurrentLengthA();

  //create lines to represent the rope

  //rope from left wheel to crate
  b2Vec2 offset0 = b2Vec2(-m_fWheelRad, 0.0f);
  b2Vec2 offset1 = b2Vec2(0.0f, fCrateHt2 - RW2PW(4));
  m_pObjectManager->CreateLine(m_pWheel0, offset0, false, pCrate, offset1, true);

  //rope from right wheel to safe
  offset0 = b2Vec2(m_fWheelRad, 0.0f);
  offset1 = b2Vec2(0.0f, 0.0f);
  m_pObjectManager->CreateLine(m_pWheel1, offset0, false, pSafe, offset1, true);

  //rope from one pulley wheel across to the other
  offset0 = b2Vec2(0.0f, m_fWheelRad);
  m_pObjectManager->CreateLine(m_pWheel0, offset0, false, m_pWheel1, offset0, false);

  //elephant spawn point
  const float h = RW2PW(m_pRenderer->GetHeight(eSprite::Elephant)); //elephant height in Physics World
  m_vElephantSpawnPt = b2Vec2(vCratePos.x, 2.0f*y + h/2.0f);
} //constructor

/// The pulley wheels are going to be static objects that we
/// rotate ourselves depending on the positions of the objects
/// that the pulley is attached to.
/// \param x X coordinate in Physics World units.
/// \param y Y coordinate in Physics World units.
/// \return Pointer to physics body.

b2Body* CPulley::CreateWheel(float x, float y){ 
  b2BodyDef bd;
  bd.type = b2_staticBody;
  bd.position.Set(x, y);

  //body
  b2Body* p = m_pPhysicsWorld->CreateBody(&bd);
  m_pObjectManager->create(eSprite::Pulley, p); 

  return p;
} //CreateWheel

/// The bucket is the thing on the left that the elephant falls into.
/// It consists of a left side, a right side, and a bottom, all
/// of which are rectangles.
/// \param x X coordinate in Physics World units.
/// \param y Y coordinate in Physics World units.
/// \return Pointer to physics body.

b2Body* CPulley::CreateCrate(float x, float y){  
  b2BodyDef bd; 
  bd.type = b2_dynamicBody;
  bd.position.Set(x, y);

  b2Body* p = m_pPhysicsWorld->CreateBody(&bd);
  m_pObjectManager->create(eSprite::Crate, p); 

  float cw = RW2PW(m_pRenderer->GetWidth(eSprite::Crate)/2.0f); //crate half width 
  float ch = RW2PW(m_pRenderer->GetHeight(eSprite::Crate)/2.0f); //crate half height 
  const float sh = RW2PW(17)/2.0f; //half height of crate floor

  b2PolygonShape s; //shape for crate floor and walls
  b2FixtureDef fd; //fixture definition
  fd.shape = &s;
  fd.density = 0.3f; 
  fd.restitution = 0.0f;

  s.SetAsBox(cw, sh, b2Vec2(0.0f, sh - ch), 0.0f); //bottom of crate
  p->CreateFixture(&fd);

  s.SetAsBox(sh, ch, b2Vec2(-cw + sh, 0.0f), 0.0f); //left wall
  p->CreateFixture(&fd);
  
  s.SetAsBox(sh, ch, b2Vec2(cw - sh, 0.0f), 0.0f); //right wall
  p->CreateFixture(&fd);
  p->SetLinearDamping(0.2f);
  p->SetAngularDamping(0.1f);

  return p;
} //CreateCrate

/// Create an object, used for the safe and the elephant.
/// \param v Position in Physics World.
/// \param d Object density.
/// \param t Type of object to be created.
/// \return Pointer to physics body.

b2Body* CPulley::CreateObject(const b2Vec2& v, float d, eSprite t){  
  float w, h; //width and height of object in Render World
  m_pRenderer->GetSize(t, w, h);

  //Physics World
  b2BodyDef bd; 
  bd.type = b2_dynamicBody;
  bd.position.Set(v.x, v.y);

  //shape
  b2PolygonShape shape;
  shape.SetAsBox(RW2PW(w)/2.0f, RW2PW(h)/2.0f);

  //fixture
  b2FixtureDef fd;
  fd.shape = &shape;
  fd.density = d;
  fd.restitution = 0.0f;

  //body
  b2Body* p = m_pPhysicsWorld->CreateBody(&bd);
  m_pObjectManager->create(t, p); 

  p->CreateFixture(&fd);
  p->SetAngularDamping(4.0f);
  p->SetLinearDamping(0.9f);

  return p;
} //CreateObject

/// Create an elephant at its spawn point offset by a
/// small pseudorandom amount to make the crate swing a bot.

void CPulley::CreateElephant(){
  const float dx = RW2PW(20.0f*m_pRandom->randf() - 10.0f); //small random offset
  const b2Vec2 spawnPt = m_vElephantSpawnPt + b2Vec2(dx, 0.0f); //spawn point
  CreateObject(spawnPt, 0.3f, eSprite::Elephant); //make elephant there
} //CreateElephant

/// Delete the first elephant, if there is one.
/// \return true if there was an elephant.

bool CPulley::DeleteElephant(){
  return m_pObjectManager->DeleteObject(eSprite::Elephant);
} //DeleteElephant

/// Animate the pulley wheels by setting their orientations
/// to reflect the amount of rope that has gone over them.

void CPulley::move(){ 
  const float fLenA = m_pJoint->GetCurrentLengthA(); //current length of left rope
  const float theta = (fLenA - m_fJointLenA)/m_fWheelRad; //new wheel orientation

  b2SetAngle(m_pWheel0, theta); //left wheel
  b2SetAngle(m_pWheel1, theta + 2.4f); //right wheel is offset slightly so it looks different
} //move
