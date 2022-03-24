/// \file Gear.cpp
/// \brief Code for the gear class CGear.

#include "Gear.h"

#include "GameDefines.h"
#include "ObjectManager.h"
#include "Renderer.h"
#include "ComponentIncludes.h"

/// \param x X coordinate in Physics World units.
/// \param y Y coordinate in Physics World units.

CGear::CGear(float x, float y){
  b2Body* pBase = CreateBase(x, y);

  CreateRoundGears(x - RW2PW(55), y);
  CreateSquareGears(x + RW2PW(117), y);

  //revolute joint for gears

  b2RevoluteJointDef rd;
  rd.Initialize(pBase, m_stdGear[0], m_stdGear[0]->GetPosition());
  rd.motorSpeed = -1.0f;
  rd.maxMotorTorque = 10.0f;
  rd.enableMotor = true;
  rd.collideConnected = false;

  b2Joint* pJoint = m_pPhysicsWorld->CreateJoint(&rd);
  m_stdJoint.push_back((b2RevoluteJoint*)pJoint);
  
  rd.enableMotor = false;

  const size_t n = m_stdGear.size();

  for(size_t i=1; i<n; i++){
    rd.Initialize(pBase, m_stdGear[i], m_stdGear[i]->GetPosition());
    pJoint = m_pPhysicsWorld->CreateJoint(&rd);
    m_stdJoint.push_back((b2RevoluteJoint*)pJoint);
  } //for
  
  //create round gear joints - the 3rd parameter is ratio of number of teeth

  CreateGearJoint(0, 1, 18.0f/33.0f); 
  CreateGearJoint(1, 2, 12.0f/18.0f);
  CreateGearJoint(1, 3, 12.0f/18.0f);
  CreateGearJoint(3, 4, 18.0f/12.0f);

  //create round-to-square gear joints

  CreateGearJoint(0, 5, 18.0f/33.0f);

  //create square gear joints
  CreateGearJoint(5, 6, 1.0f);
  CreateGearJoint(5, 7, 1.0f);
  CreateGearJoint(7, 8, 1.0f);
} //constructor


/// Create the square gears and push them onto the gear list.
/// It is assumed that this function is called first and that
/// the driving gear (the one with the motor on its
/// revolute joint) is to be the first one on the gear list.
/// \param x X coordinate in Physics World units.
/// \param y Y coordinate in Physics World units.

void CGear::CreateRoundGears(float x, float y){
  m_stdGear.push_back(CreateGear(x, y, eSprite::Gear0));

  x -= RW2PW(145); y += RW2PW(94);
  m_stdGear.push_back(CreateGear(x, y, eSprite::Gear2));

  m_stdGear.push_back(CreateGear(x - RW2PW(66), y + RW2PW(77), eSprite::Gear1));
  m_stdGear.push_back(CreateGear(x - RW2PW(100), y - RW2PW(14), eSprite::Gear1));
  m_stdGear.push_back(CreateGear(x - RW2PW(92), y - RW2PW(115), eSprite::Gear2));
} //CreateRoundGears

/// Create the square gears and push them onto the gear list.
/// \param x X coordinate in Physics World units.
/// \param y Y coordinate in Physics World units.

void CGear::CreateSquareGears(float x, float y){
  const float delta = RW2PW(172);

  m_stdGear.push_back(CreateGear(x, y, eSprite::Gear4));
  m_stdGear.push_back(CreateGear(x + delta, y, eSprite::Gear3));
  m_stdGear.push_back(CreateGear(x, y - delta, eSprite::Gear3));
  m_stdGear.push_back(CreateGear(x + delta, y - delta, eSprite::Gear5));
} //CreateSquareGears

/// Create a gear wheel at a given position in Physics World.
/// \param x X coordinate in Physics World units.
/// \param y Y coordinate in Physics World units.
/// \param t Sprite type of the gear wheel.
/// \return Pointer to the cog wheel body in Physics World.

b2Body* CGear::CreateGear(float x, float y, eSprite t){
  const float r = RW2PW(m_pRenderer->GetWidth(t))/2.0f; //radius in Physics World

  //Physics World
  b2BodyDef bd;
  bd.type = b2_dynamicBody;
  bd.position.Set(x, y);

  b2CircleShape s;
  s.m_radius = RW2PW(r);

  b2FixtureDef fd;
  fd.shape = &s;
  fd.density = 0.7f;
  fd.restitution = 0.0f;

  b2Body* p = m_pPhysicsWorld->CreateBody(&bd);
  m_pObjectManager->create(t, p); 
  p->CreateFixture(&fd);

  return p;
} //CreateGear

/// Create an invisible base to attach the largest gear wheel to. The base has
/// no sprite and no shape in Physics World. The latter means it doesn't
/// collide with anything.
/// \param x X coordinate in Physics World units.
/// \param y Y coordinate in Physics World units.
/// \return Pointer to the cog wheel body in Physics World.

b2Body* CGear::CreateBase(float x, float y){
  b2BodyDef bd; 
  bd.type = b2_staticBody;
  bd.position.Set(x, y);

  b2Body* p = m_pPhysicsWorld->CreateBody(&bd);
  m_pObjectManager->create(eSprite::Size, p);

  return p;
} //CreateBase

/// Create a gear joint between two of hthe revolute joints in m_pJoint.
/// \param i Index of first revolute joint and body attached to it.
/// \param j Index of second revolute joint and body attached to it.
/// \param r Gear ratio.

void CGear::CreateGearJoint(UINT i, UINT j, float r){
  b2GearJointDef gd;

  gd.ratio = r;
  gd.joint1 = m_stdJoint[i];
  gd.joint2 = m_stdJoint[j];
  gd.bodyA = m_stdGear[i];
  gd.bodyB = m_stdGear[j];

  m_pPhysicsWorld->CreateJoint(&gd);
} //CreateGearJoint

/// Reverse the gears by setting the motor speed on the
/// revolute joint for the first gear.

void CGear::reverse(){
  m_stdJoint[0]->SetMotorSpeed(-m_stdJoint[0]->GetMotorSpeed());
} //reverse