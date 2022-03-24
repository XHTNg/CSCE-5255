/// \file ObjectManager.cpp
/// \brief Code for the object manager CObjectManager.

#include "ObjectManager.h"
#include "ComponentIncludes.h"
#include "Renderer.h"

CObjectManager::CObjectManager(){
} //constructor

/// The destructor clears the object list, which destructs
/// all of the objects in it.

CObjectManager::~CObjectManager(){
  clear();
} //destructor

/// Delete all of the entities managed by object manager. 
/// This involves deleting all of the CObject instances pointed
/// to by the object list, then clearing the object list itself.

void CObjectManager::clear(){
  for(auto const& p: m_stdList) //for each object
    delete p; //delete object

  m_stdList.clear(); //clear the object list
} //clear

/// Draw the game objects using Painter's Algorithm.
/// The background is drawn first, then the game
/// objects are asked to draw themselves in the
/// order that they are in the object list. That is,
/// they are drawn from back to front.

void CObjectManager::draw(){  
  if(m_eDrawMode == eDrawMode::Sprites || m_eDrawMode == eDrawMode::Both)
    m_pRenderer->Draw(eSprite::Background, m_vWinCenter); //draw background

  for(auto const& p: m_stdList) //for each object
    p->draw(); //draw it in renderer
} //draw

/// Create world edges in Physics World.
/// Place Box2D edge shapes in the Physics World in places that correspond to the
/// bottom, right, and left edges of the screen in renderer. The left and
/// right edges continue upwards for a distance. There is no top to the world.

void CObjectManager::CreateWorldEdges(){
  const float w = RW2PW(m_nWinWidth); //window width in Physics World units
  const float h = RW2PW(m_nWinHeight); //window height in Physics World units

  //corners of the window
  const b2Vec2 vBLeft  = b2Vec2(0, 0); //bottom left
  const b2Vec2 vBRight = b2Vec2(w, 0); //bottom right
  const b2Vec2 vTLeft  = b2Vec2(0, h); //top left
  const b2Vec2 vTRight = b2Vec2(w, h); //top right

  //Box2D ground
  b2BodyDef bd; //body definition
  b2Body* pBody = m_pPhysicsWorld->CreateBody(&bd); //body
  b2EdgeShape shape; //shape

  //bottom of window
  shape.m_vertex1 = vBLeft;
  shape.m_vertex2 = vBRight;
  pBody->CreateFixture(&shape, 0);

  //left edge of window
  shape.m_vertex1 = vBLeft;
  shape.m_vertex2 = vTLeft;
  pBody->CreateFixture(&shape, 0);

  //right edge of window
  shape.m_vertex1 = vBRight;
  shape.m_vertex2 = vTRight;
  pBody->CreateFixture(&shape, 0); 
} //CreateWorldEdges

void RemapPoints(b2Vec2 p[], int numbersOfCorners, int w, int h) {
    for (int i = 0; i < numbersOfCorners; i++) {
        p[i].x = RW2PW(p[i].x - w / 2.0f);
        p[i].y = RW2PW(-p[i].y + h / 2.0f);
    } //for
} //RemapPoints

/// Create an object in object manager and link its Physics World
/// body to it.
/// \param t Sprite type.
/// \param p Pointer to Box2D body.

void CObjectManager::CreateObject(eSprite t, b2Body* p){
  CObject* pObj = new CObject(t, p);
  m_stdList.push_back(pObj);
  p->GetUserData().pointer = (uintptr_t)pObj;
} //CreateObject





b2Body* CObjectManager::CreateDynamicObject(eSprite object, float x, float y, float density, float elasticity) {
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position.Set(x, y);
    float w, h;
    m_pRenderer->GetSize(object, w, h);
    b2PolygonShape s;
    s.SetAsBox(RW2PW(w) / 2.0f, RW2PW(h) / 2.0f); 
    b2FixtureDef fd;
    fd.shape = &s;
    fd.density = density;
    fd.restitution = 0.3f;
    b2Body* pCrate = m_pPhysicsWorld->CreateBody(&bd);
    pCrate->CreateFixture(&fd);
    m_stdList.push_back(new CObject(object, pCrate));
    return pCrate;
} //CreateCrate

b2Body* CObjectManager::CreateStaticObject(eSprite object, float x, float y, float elasticity) {
    return CreateBoxOfType(object, x, y, 1.0f, elasticity, b2_staticBody);
} //CreateCrate


b2Body* CObjectManager::CreateBoxOfType(eSprite object, float x, float y,  float density, float elasticity, b2BodyType type) {
    b2BodyDef bd;
    bd.type = type;
    bd.position.Set(x, y);
    float w, h;
    m_pRenderer->GetSize(object, w, h);
    b2PolygonShape s;
    s.SetAsBox(RW2PW(w) / 2.0f, RW2PW(h) / 2.0f);
    b2FixtureDef fd;
    fd.shape = &s;
    fd.density = density;
    fd.restitution = elasticity;
    b2Body* pCrate = m_pPhysicsWorld->CreateBody(&bd);
    pCrate->CreateFixture(&fd);
    m_stdList.push_back(new CObject(object, pCrate));
    return pCrate;
} //CreateCrate

b2Body* CObjectManager::CreateObjectOfType(eSprite object, float x, float y, float density, float elasticity, b2BodyType type, b2Vec2 corners[], int numbersOfCorners, float angle) {
    b2BodyDef bd;
    bd.type = type;
    bd.position.Set(x, y);
    bd.angle = angle;
    float w, h;
    m_pRenderer->GetSize(object, w, h);
    RemapPoints(corners, numbersOfCorners,w, h);
    b2PolygonShape s;
    s.Set(corners, numbersOfCorners);
    b2FixtureDef fd;
    fd.shape = &s;
    fd.density = density;
    fd.restitution = elasticity;
    b2Body* pCrate = m_pPhysicsWorld->CreateBody(&bd);
    pCrate->CreateFixture(&fd);
    m_stdList.push_back(new CObject(object, pCrate));
    return pCrate;
} //CreateCrate

b2Body* CObjectManager::CreateCircleOfType(eSprite object, float x, float y, float density, float elasticity, b2BodyType type) {
    b2BodyDef bd;
    bd.type = type;
    bd.position.Set(x, y);
    float w, h;
    m_pRenderer->GetSize(object, w, h);
    b2CircleShape s;
    s.m_radius = RW2PW(m_pRenderer->GetWidth(object) / 2.0f);
    b2FixtureDef fd;
    fd.shape = &s;
    fd.density = density;
    fd.restitution = elasticity;
    fd.friction = 2.5f;
    b2Body* pCrate = m_pPhysicsWorld->CreateBody(&bd);
    pCrate->CreateFixture(&fd);
    m_stdList.push_back(new CObject(object, pCrate));
    return pCrate;
} //CreateCrate



