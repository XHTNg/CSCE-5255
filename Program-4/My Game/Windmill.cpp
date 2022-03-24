/// \file Windmill.cpp
/// \brief Code for the windmill class CWindmill.

#include "Windmill.h"

#include "GameDefines.h"
#include "ObjectManager.h"
#include "Renderer.h"
#include "ComponentIncludes.h"

/// Create physics bodies for the windmill's blades and base,
/// and a rotational joint between them.
/// \param x X coordinate in Physics World units.
/// \param y Y coordinate in Physics World units.

CWindmill::CWindmill(float x, float y){
  b2Body* pBlade = CreateBlade(x, y);
  b2Body* pBase = CreateBase(x, y);

  //revolute joint definition
  b2RevoluteJointDef wd;

  wd.Initialize(pBase, pBlade, pBlade->GetPosition());
  wd.motorSpeed = -3.0f;
  wd.maxMotorTorque = 4000.0f;
  wd.enableMotor = true;
  wd.collideConnected = false;

  //create revolute joint 
  m_pJoint = (b2RevoluteJoint*)m_pPhysicsWorld->CreateJoint(&wd);
} //constructor

/// Create a physics body for the windmill blades in Physics World.
/// \param x X coordinate in Physics World units.
/// \param y Y coordinate in Physics World units.
/// \return Pointer to physics body for the windmill blades.

b2Body* CWindmill::CreateBlade(float x, float y){ 
  b2BodyDef bd;
  bd.type = b2_dynamicBody;
  bd.position.Set(x + RW2PW(60), y + RW2PW(200));

  //create shape
  b2CircleShape s;
  s.m_radius = RW2PW(m_pRenderer->GetWidth(eSprite::WindmillBlade)/2.0f);

  //create fixture definition
  b2FixtureDef fd;
  fd.shape = &s;
  fd.density = 0.1f;

  //create physics body
  b2Body* pBody = m_pPhysicsWorld->CreateBody(&bd);
  m_pObjectManager->create(eSprite::WindmillBlade, pBody);

  pBody->CreateFixture(&fd); //attach fixture

  return pBody;
} //CreateBlade

/// Create a physics body for the windmill base in Physics World.
/// \param x X coordinate in Physics World units.
/// \param y Y coordinate in Physics World units.
/// \return Pointer to physics body for the windmill base.

b2Body* CWindmill::CreateBase(float x, float y){ 
  b2BodyDef bd;
  bd.type = b2_staticBody;
  bd.position.Set(x, y);

  float x_base = 15.5;// 360;
  float y_base = 0;// 270;

  //create physics body
  b2Body* pBody = m_pPhysicsWorld->CreateBody(&bd);
  m_pObjectManager->create(eSprite::WindmillBase, pBody);
  //pBody->CreateFixture(&bd);


  //create triangle shape
  {
	  b2PolygonShape triangleShape;
	  b2Vec2 vertices[3];
	  vertices[0].Set(RW2PW(235) - x_base,y -RW2PW(35));
	  vertices[1].Set(RW2PW(174) - x_base, y-RW2PW(539));
	  vertices[2].Set(RW2PW(297) - x_base, y-RW2PW(539));
	  triangleShape.Set(vertices,3);
	 // triangleShape.m_radius = RW2PW(m_pRenderer->GetWidth(eSprite::WindmillBlade) / 2.0f);
	  b2FixtureDef triangleObject;
	  triangleObject.shape = &triangleShape;
	  triangleObject.density = 0.1f;
	  pBody->CreateFixture(&triangleObject);
  }



  //create wind shape
  {
	  b2PolygonShape wingShape;
	  b2Vec2 vertices[5];
	  vertices[0].Set(RW2PW(4) - x_base, y - RW2PW(0));
	  vertices[1].Set(RW2PW(0) - x_base, y - RW2PW(101));
	  vertices[2].Set(RW2PW(117) - x_base, y - RW2PW(87));
	  vertices[3].Set(RW2PW(130) - x_base, y - RW2PW(58));
	  vertices[4].Set(RW2PW(118) - x_base, y - RW2PW(29));
	  wingShape.Set(vertices, 5);
	  b2FixtureDef wingObject;
	  wingObject.shape = &wingShape;
	  wingObject.density = 0.1f;
	  pBody->CreateFixture(&wingObject);
  }

  //create wind shape
  {
	  b2PolygonShape crosspieceShape;
	  b2Vec2 vertices[7];
	  vertices[0].Set(RW2PW(199) - x_base, y - RW2PW(228));
	  vertices[1].Set(RW2PW(161) - x_base, y - RW2PW(248));
	  vertices[2].Set(RW2PW(160) - x_base, y - RW2PW(251));
	  vertices[3].Set(RW2PW(177) - x_base, y - RW2PW(247));
	  vertices[4].Set(RW2PW(228) - x_base, y - RW2PW(246));
	  vertices[5].Set(RW2PW(311) - x_base, y - RW2PW(260));
	  vertices[6].Set(RW2PW(314) - x_base, y - RW2PW(250));
	  crosspieceShape.Set(vertices, 7);
	  b2FixtureDef crosspieceObject;
	  crosspieceObject.shape = &crosspieceShape;
	  crosspieceObject.density = 0.1f;
	  pBody->CreateFixture(&crosspieceObject);
  }

  return pBody;
} //CreateBase

/// Reverse the windmill by setting the motor speed on the
/// revolute joint.

void CWindmill::reverse(){
  m_pJoint->SetMotorSpeed(-m_pJoint->GetMotorSpeed());
} //reverse