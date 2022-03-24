/// \file RackAndPinion.cpp
/// \brief Code for the rack and pinion class CRackAndPinion.

#include "RackAndPinion.h"

#include "GameDefines.h"
#include "ObjectManager.h"
#include "Renderer.h"
#include "ComponentIncludes.h"

/// Create a rack and a pinion at a given position in Physics World,
/// with a revolute joint for the pinion and a prismatic joint for the rack.
/// The revolute joint has a motor.
/// \param x X coordinate in Physics World units.
/// \param y Y coordinate in Physics World units.

CRackAndPinion::CRackAndPinion(float x, float y){
  b2Body* pPinion = CreatePinion(x, y);
  b2Body* pRack = CreateRack(x, y + RW2PW(59));
  b2Body* pBase = CreateBase(x, y);

  //revolute joint for pinion
  b2RevoluteJointDef wd;

  wd.Initialize(pBase, pPinion, pPinion->GetPosition());
  wd.motorSpeed = 2.0f;
  wd.maxMotorTorque = 1000.0f;
  wd.enableMotor = true;

  m_pRevoluteJoint = (b2RevoluteJoint*)m_pPhysicsWorld->CreateJoint(&wd);

  //prismatic joint for the rack
  const float w = m_pRenderer->GetWidth(eSprite::Rack); //rack width in Render World
  const float w2 = RW2PW(w)/2.0f; //half rack width in Physics World
  m_fLimit = w2 - RW2PW(16); //rack translation limit

  b2PrismaticJointDef pjd;

  pjd.Initialize(pBase, pRack, pRack->GetPosition(), b2Vec2(1.0f, 0.0f));
  pjd.lowerTranslation = -m_fLimit;
  pjd.upperTranslation = m_fLimit;
  pjd.enableLimit = true;

  m_pPrismaticJoint = (b2PrismaticJoint*)m_pPhysicsWorld->CreateJoint(&pjd);

  //gear joint connecting revolute and prismatic joints
  b2GearJointDef gd;

  gd.ratio = b2_pi/w2;
  gd.joint1 = m_pRevoluteJoint;
  gd.joint2 = m_pPrismaticJoint;
  gd.bodyA = pPinion;
  gd.bodyB = pRack;

  m_pPhysicsWorld->CreateJoint(&gd);
} //constructor

/// Create a rack object at a given position in Physics World.
/// \param x X coordinate in Physics World units.
/// \param y Y coordinate in Physics World units.
/// \return Pointer to the rack body in Physics World.

b2Body* CRackAndPinion::CreateRack(float x, float y){
  float w, h; //width and height in Render World
  m_pRenderer->GetSize(eSprite::Rack, w, h);
  const float w2 = RW2PW(w)/2.0f; //half width in Physics World
  const float h2 = RW2PW(h)/2.0f; //half width in Physics World

  //Physics World
  b2BodyDef bd; 
  bd.type = b2_dynamicBody;
  bd.position.Set(x, y);

  //shape
  b2PolygonShape shape;
  shape.SetAsBox(w2, h2);

  //fixture
  b2FixtureDef fd;
  fd.shape = &shape;
  fd.density = 0.1f;
  fd.restitution = 0.0f;

  //body
  b2Body* p = m_pPhysicsWorld->CreateBody(&bd);
  m_pObjectManager->create(eSprite::Rack, p); 
  p->CreateFixture(&fd);

  return p;
} //CreateRack

/// Create a pinion at a given position in Physics World.
/// \param x X coordinate in Physics World units.
/// \param y Y coordinate in Physics World units.
/// \return Pointer to the pinion body in Physics World.

b2Body* CRackAndPinion::CreatePinion(float x, float y){
  const float w = m_pRenderer->GetWidth(eSprite::Pinion); //diameter in Render World

  //Physics World
  b2BodyDef bd;
  bd.type = b2_dynamicBody;
  bd.position.Set(x, y);

  b2CircleShape s;
  s.m_radius = RW2PW(w)/2.0f;

  b2FixtureDef fd;
  fd.shape = &s;
  fd.density = 0.1f;
  fd.restitution = 1.0f;

  b2Body* p = m_pPhysicsWorld->CreateBody(&bd);
  m_pObjectManager->create(eSprite::Pinion, p); 
  p->CreateFixture(&fd);

  return p;
} //CreatePinion

/// Create an invisible base to attach the pinion to. The base has
/// no sprite and no shape in Physics World. The latter means it doesn't
/// collide with anything.
/// \param x X coordinate in Physics World units.
/// \param y Y coordinate in Physics World units.
/// \return Pointer to the base body in Physics World.

b2Body* CRackAndPinion::CreateBase(float x, float y){
  b2BodyDef bd; 
  bd.type = b2_staticBody;
  bd.position.Set(x, y);

  //body
  b2Body* p = m_pPhysicsWorld->CreateBody(&bd);
  m_pObjectManager->create(eSprite::Size, p); 

  return p;
} //CreateBase

/// Monitor the pinion offset, and if it is p against the stops, 
/// then reverse the direction of the motor on the revolute joint.

void CRackAndPinion::move(){
  const float d = m_pPrismaticJoint->GetJointTranslation(); //rack displacement
  const float s = m_pRevoluteJoint->GetMotorSpeed(); //pinion motor speed

  if(s < 0.0f && d >= m_fLimit - 1.01f) //up against right stop and not reversed
    m_pRevoluteJoint->SetMotorSpeed(-s); //flip motor direction

  else if(s > 0.0f && d <= -m_fLimit + 1.01f) //up against left stop and not reversed
    m_pRevoluteJoint->SetMotorSpeed(-s);  //flip motor direction
} //move

/// Reverse the pinion by setting the motor speed on the
/// revolute joint.

void CRackAndPinion::reverse(){
  m_pRevoluteJoint->SetMotorSpeed(-m_pRevoluteJoint->GetMotorSpeed());
} //reverse