/// \file Game.cpp
/// \brief Code for the game class CGame.

#include "Game.h"

#include "GameDefines.h"
#include "Renderer.h"
#include "ComponentIncludes.h"

/// Call renderer's Release function to do the required
/// Direct3D cleanup, then delete renderer and object manager.
/// Also delete Physics World, which MUST be deleted after
/// object manager because of way things are set up.


CGame::~CGame(){
  delete m_pObjectManager;
  delete m_pPhysicsWorld;
  delete m_pParticleEngine;
} //destructor

/// Initialize renderer and object manager, load 
/// images and sounds, start the timer, and begin the game.

void CGame::Initialize(){
  m_pRenderer = new CRenderer; 
  m_pRenderer->Initialize(eSprite::Size);
  m_pRenderer->LoadImages(); //load images from xml file list

  //set up object manager and Physics World
  m_pObjectManager = new CObjectManager; //set up object manager
  m_pPhysicsWorld = new b2World(RW2PW(0, -2000)); //set up Physics World with gravity
  m_pObjectManager->CreateWorldEdges(); //create world edges at edges of window
  m_pPhysicsWorld->SetContactListener(&m_cContactListener); //load up my contact listener

  LoadSounds(); //load the sounds for this game
  
  m_pParticleEngine = new LParticleEngine2D(m_pRenderer);

  //now start the game
  BeginGame();
} //Initialize

/// Initialize the audio player and load game sounds.

void CGame::LoadSounds(){
  m_pAudio->Initialize(eSound::Size);
  
  m_pAudio->Load(eSound::Whoosh, "whoosh");
  m_pAudio->Load(eSound::Yay, "yay");
  m_pAudio->Load(eSound::Bonk, "bonk");
  m_pAudio->Load(eSound::Buzz, "buzz");
  m_pAudio->Load(eSound::Restart, "restart");
} //LoadSounds

/// Release all of the DirectX12 objects by deleting the renderer.

void CGame::Release(){
  delete m_pRenderer;
  m_pRenderer = nullptr; //for safety
} //Release

/// Clear any old objects out of object manager and get ready
/// to start again.

void CGame::BeginGame(){  
  m_pParticleEngine->clear();
  m_pAudio->stop();

  m_pObjectManager->clear(); //clear old objects
  
  float w, h; //width and height of red button sprite
  m_pRenderer->GetSize(eSprite::RedButton, w, h); //get them
  CreateButton(10+ RW2PW(w)/2.0f, RW2PW(h)/2.0f +7); //create red button
  CreateMap();
} //BeginGame
/*
void CGame::CreateMap() {
    for (int a = 0; a < 5; a++) {
       // m_pObjectManager->CreateDynamicObject(eSprite::Box, 18+4*a, 55, 0.04f, 1.0f);
    }
    m_pObjectManager->CreateStaticObject(eSprite::Floor, 64, 67, 0.0f);
    m_pObjectManager->CreateStaticObject(eSprite::Wall, 50, 60, 0.0f);

    b2Body* base = m_pObjectManager->CreateStaticObject(eSprite::FloorBlue, 0, 53, 0.7f);

   // b2Body* base = m_pObjectManager->CreateStaticObject(eSprite::FloorBlue, 30, 22.2f, 0.0f);
    //b2RevoluteJoint* motors[35];
    for (int a = 0; a < 30; a++) {
        b2Body* wheel = m_pObjectManager->CreateCircleOfType(eSprite::GearSmall, (-3)+a*1.3, 37-a*0.15, 1000.0f, -60, b2_dynamicBody);
        b2RevoluteJointDef wd;
        wd.Initialize(base, wheel, wheel->GetPosition());
        wd.motorSpeed = 9.0f;
        wd.maxMotorTorque = 4000000.0f;
        wd.enableMotor = true;
        wd.collideConnected = false;
        (b2RevoluteJoint*)m_pPhysicsWorld->CreateJoint(&wd);
    }
    for (int a = 0; a < 100; a++) {
        // b2Body* wheel = m_pObjectManager->CreateCircleOfType(eSprite::GearSmall, -6.5 + a * 0.65, 6 - a * 0, 1000.0f, 0.95f, b2_dynamicBody);
         //b2Body* wheel = m_pObjectManager->CreateCircleOfType(eSprite::GearSmall, -10.5 + a * 0.65, 5 + exp(a * 0.01f)* 3, 1000.0f, 0.5f, b2_dynamicBody);
        b2Body* wheel = m_pObjectManager->CreateCircleOfType(eSprite::GearSmall, -10.5 + (a * 0.65), 10, 1000.0f, -60, b2_dynamicBody);
        b2RevoluteJointDef wd;
        wd.Initialize(base, wheel, wheel->GetPosition());
        wd.motorSpeed = -1000.0f;
        wd.maxMotorTorque = 400000000.0f;
        wd.enableMotor = true;
        wd.collideConnected = false;
        (b2RevoluteJoint*)m_pPhysicsWorld->CreateJoint(&wd);
    }
    for (int a = 0; a < 40; a++) {
       // b2Body* wheel = m_pObjectManager->CreateCircleOfType(eSprite::GearSmall, -6.5 + a * 0.65, 6 - a * 0, 1000.0f, 0.95f, b2_dynamicBody);
        //b2Body* wheel = m_pObjectManager->CreateCircleOfType(eSprite::GearSmall, -10.5 + a * 0.65, 5 + exp(a * 0.01f)* 3, 1000.0f, 0.5f, b2_dynamicBody);
        float lerp = a / 200.0f;
        b2Body* wheel = m_pObjectManager->CreateCircleOfType(eSprite::GearSmall, 50 + (a * 0.65) * (1.0f-lerp), 10 + lerp*lerp * 50, 1000.0f, 0.5f, b2_dynamicBody);
        b2RevoluteJointDef wd;
        wd.Initialize(base, wheel, wheel->GetPosition());
        wd.motorSpeed = -1000.0f;
        wd.maxMotorTorque = 400000000.0f;
        wd.enableMotor = true;
        wd.collideConnected = false;
        (b2RevoluteJoint*)m_pPhysicsWorld->CreateJoint(&wd);
    }





    b2Vec2 points[4];
    points[0] = b2Vec2(0, 0);
    points[1] = b2Vec2(0, 27);
    points[2] = b2Vec2(127, 97);
    points[3] = b2Vec2(127, 68);
    b2Body* body = m_pObjectManager->CreateObjectOfType(eSprite::DownRamp, 5, 61, 0.0f, 0.0f, b2_staticBody, points, 4, -0.1f);
   // body.
    b2Vec2 points2[4];
    points2[0] = b2Vec2(0, 0);
    points2[1] = b2Vec2(0, 27);
    points2[2] = b2Vec2(127, 97);
    points2[3] = b2Vec2(127, 68);
   // m_pObjectManager->CreateObjectOfType(eSprite::DownRamp, 15, 54, 0.0f, 0.0f, b2_staticBody, points2, 4, -0.1f);
    b2Vec2 points3[4];
    points3[0] = b2Vec2(127, 27);
    points3[1] = b2Vec2(127, 0);
    points3[2] = b2Vec2(0, 68);
    points3[3] = b2Vec2(0, 97);
    m_pObjectManager->CreateObjectOfType(eSprite::UpRamp, 45, 46, 0.0f, 0.5f, b2_staticBody, points3, 4,0.0f);
    b2Vec2 points6[4];
    points6[0] = b2Vec2(127, 27);
    points6[1] = b2Vec2(127, 0);
    points6[2] = b2Vec2(0, 68);
    points6[3] = b2Vec2(0, 97);
  //  m_pObjectManager->CreateObjectOfType(eSprite::UpRamp, 43, 43.5, 0.0f, 0.5f, b2_staticBody, points6, 4, 0.0f);
    b2Vec2 points4[4];
    points4[0] = b2Vec2(127, 27);
    points4[1] = b2Vec2(127, 0);
    points4[2] = b2Vec2(0, 68);
    points4[3] = b2Vec2(0, 97);
    m_pObjectManager->CreateObjectOfType(eSprite::UpRamp, 45, 24, 0.0f, 0.2f, b2_staticBody, points4, 4, -0.2f);
    b2Vec2 points5[4];
    points5[0] = b2Vec2(127, 27);
    points5[1] = b2Vec2(127, 0);
    points5[2] = b2Vec2(0, 68);
    points5[3] = b2Vec2(0, 97);
   // m_pObjectManager->CreateObjectOfType(eSprite::UpRamp, 34, 20, 0.0f, 0.2f, b2_staticBody, points5, 4, -0.2f);
} //BeginGame
*/

/*
void CGame::CreateMap() {
    for (int a = 0; a < 11; a++) {
        m_pObjectManager->CreateDynamicObject(eSprite::Box, 48+4*a, 55, 0.04f, 1.0f);
    }
    m_pObjectManager->CreateStaticObject(eSprite::Floor, 64, 67, 0.0f);

   // m_pObjectManager->CreateStaticObject(eSprite::FloorBlue, 70, 56.5f, 1.0f);
    b2Body* base = m_pObjectManager->CreateStaticObject(eSprite::FloorBlue, 52.5, 53, 0.7f);
     m_pObjectManager->CreateStaticObject(eSprite::FloorBlue, 0, 53, 0.7f);

   // b2Body* base = m_pObjectManager->CreateStaticObject(eSprite::FloorBlue, 30, 22.2f, 0.0f);
    //b2RevoluteJoint* motors[35];
    for (int a = 0; a < 53; a++) {
        b2Body* wheel = m_pObjectManager->CreateCircleOfType(eSprite::Gear0, 10+a*1.3, 38.5-a*0.15, 1000.0f, 0.6f, b2_dynamicBody);
        b2RevoluteJointDef wd;
        wd.Initialize(base, wheel, wheel->GetPosition());
        wd.motorSpeed = 9.0f;
        wd.maxMotorTorque = 4000000.0f;
        wd.enableMotor = true;
        wd.collideConnected = false;
        (b2RevoluteJoint*)m_pPhysicsWorld->CreateJoint(&wd);
    }
    for (int a = 0; a < 134; a++) {
        b2Body* wheel = m_pObjectManager->CreateCircleOfType(eSprite::Gear0, -6.5 + a * 0.65, 6 - a * 0, 1000.0f, 0.95f, b2_dynamicBody);
        b2RevoluteJointDef wd;
        wd.Initialize(base, wheel, wheel->GetPosition());
        wd.motorSpeed = -1.0f;
        wd.maxMotorTorque = 4000000.0f;
        wd.enableMotor = true;
        wd.collideConnected = false;
        (b2RevoluteJoint*)m_pPhysicsWorld->CreateJoint(&wd);
    }

    b2Vec2 points[4];
    points[0] = b2Vec2(0, 0);
    points[1] = b2Vec2(0, 27);
    points[2] = b2Vec2(127, 97);
    points[3] = b2Vec2(127, 68);
    b2Body* body = m_pObjectManager->CreateObjectOfType(eSprite::DownRamp, 5, 61, 0.0f, 0.0f, b2_staticBody, points, 4, -0.1f);
    // body.
    b2Vec2 points2[4];
    points2[0] = b2Vec2(0, 0);
    points2[1] = b2Vec2(0, 27);
    points2[2] = b2Vec2(127, 97);
    points2[3] = b2Vec2(127, 68);
    m_pObjectManager->CreateObjectOfType(eSprite::DownRamp, 15, 54, 0.0f, 0.0f, b2_staticBody, points2, 4, -0.1f);
    b2Vec2 points3[4];
    points3[0] = b2Vec2(127, 27);
    points3[1] = b2Vec2(127, 0);
    points3[2] = b2Vec2(0, 68);
    points3[3] = b2Vec2(0, 97);
    m_pObjectManager->CreateObjectOfType(eSprite::UpRamp, 97, 46, 0.0f, 0.5f, b2_staticBody, points3, 4, 0.0f);
    b2Vec2 points6[4];
    points6[0] = b2Vec2(127, 27);
    points6[1] = b2Vec2(127, 0);
    points6[2] = b2Vec2(0, 68);
    points6[3] = b2Vec2(0, 97);
    m_pObjectManager->CreateObjectOfType(eSprite::UpRamp, 93, 43.5, 0.0f, 0.5f, b2_staticBody, points6, 4, 0.0f);
    b2Vec2 points4[4];
    points4[0] = b2Vec2(127, 27);
    points4[1] = b2Vec2(127, 0);
    points4[2] = b2Vec2(0, 68);
    points4[3] = b2Vec2(0, 97);
    m_pObjectManager->CreateObjectOfType(eSprite::UpRamp, 97, 24, 0.0f, 0.2f, b2_staticBody, points4, 4, -0.2f);
    b2Vec2 points5[4];
    points5[0] = b2Vec2(127, 27);
    points5[1] = b2Vec2(127, 0);
    points5[2] = b2Vec2(0, 68);
    points5[3] = b2Vec2(0, 97);
    m_pObjectManager->CreateObjectOfType(eSprite::UpRamp, 84, 20, 0.0f, 0.2f, b2_staticBody, points5, 4, -0.2f);
}
*/

void CGame::CreateMap() {
    float y_ = 10.0f;
    for (int a = 0; a < 11; a++) {
        m_pObjectManager->CreateDynamicObject(eSprite::Box, 48 + 4 * a, 55 + y_, 0.04f, 1.0f);
    }
    b2Body* base = m_pObjectManager->CreateStaticObject(eSprite::FloorBlue, 52.5, 53 + y_, 0.7f);
    m_pObjectManager->CreateStaticObject(eSprite::FloorBlue, 0, 53 + y_, 0.7f);

    for (int a = 0; a < 53; a++) {
        b2Body* wheel = m_pObjectManager->CreateCircleOfType(eSprite::Gear0, 10 + a * 1.3, 38.5 - a * 0.15 + y_, 1000.0f, 0.6f, b2_dynamicBody);
        b2RevoluteJointDef wd;
        wd.Initialize(base, wheel, wheel->GetPosition());
        wd.motorSpeed = 9.0f;
        wd.maxMotorTorque = 4000000.0f;
        wd.enableMotor = true;
        wd.collideConnected = false;
        (b2RevoluteJoint*)m_pPhysicsWorld->CreateJoint(&wd);
    }
    for (int a = 0; a < 134; a++) {
        b2Body* wheel = m_pObjectManager->CreateCircleOfType(eSprite::Gear0, -6.5 + a * 0.65, 6 - a * 0 + y_, 1000.0f, 0.95f, b2_dynamicBody);
        b2RevoluteJointDef wd;
        wd.Initialize(base, wheel, wheel->GetPosition());
        wd.motorSpeed = -1.0f;
        wd.maxMotorTorque = 4000000.0f;
        wd.enableMotor = true;
        wd.collideConnected = false;
        (b2RevoluteJoint*)m_pPhysicsWorld->CreateJoint(&wd);
    }

    b2Vec2 points[4];
    points[0] = b2Vec2(0, 0);
    points[1] = b2Vec2(0, 27);
    points[2] = b2Vec2(127, 97);
    points[3] = b2Vec2(127, 68);
    b2Body* body = m_pObjectManager->CreateObjectOfType(eSprite::DownRamp, 5, 61 + y_, 0.0f, 0.0f, b2_staticBody, points, 4, -0.1f);
    // body.
    b2Vec2 points2[4];
    points2[0] = b2Vec2(0, 0);
    points2[1] = b2Vec2(0, 27);
    points2[2] = b2Vec2(127, 97);
    points2[3] = b2Vec2(127, 68);
    m_pObjectManager->CreateObjectOfType(eSprite::DownRamp, 15, 54 + y_, 0.0f, 0.0f, b2_staticBody, points2, 4, -0.1f);
    b2Vec2 points3[4];
    points3[0] = b2Vec2(127, 27);
    points3[1] = b2Vec2(127, 0);
    points3[2] = b2Vec2(0, 68);
    points3[3] = b2Vec2(0, 97);
    m_pObjectManager->CreateObjectOfType(eSprite::UpRamp, 97, 46 + y_, 0.0f, 0.5f, b2_staticBody, points3, 4, 0.0f);
    b2Vec2 points6[4];
    points6[0] = b2Vec2(127, 27);
    points6[1] = b2Vec2(127, 0);
    points6[2] = b2Vec2(0, 68);
    points6[3] = b2Vec2(0, 97);
    m_pObjectManager->CreateObjectOfType(eSprite::UpRamp, 93, 43.5 + y_, 0.0f, 0.5f, b2_staticBody, points6, 4, 0.0f);
    b2Vec2 points4[4];
    points4[0] = b2Vec2(127, 27);
    points4[1] = b2Vec2(127, 0);
    points4[2] = b2Vec2(0, 68);
    points4[3] = b2Vec2(0, 97);
    m_pObjectManager->CreateObjectOfType(eSprite::UpRamp, 97, 24 + y_, 0.0f, 0.2f, b2_staticBody, points4, 4, -0.2f);
    b2Vec2 points5[4];
    points5[0] = b2Vec2(127, 27);
    points5[1] = b2Vec2(127, 0);
    points5[2] = b2Vec2(0, 68);
    points5[3] = b2Vec2(0, 97);
    m_pObjectManager->CreateObjectOfType(eSprite::UpRamp, 84, 20 + y_, 0.0f, 0.2f, b2_staticBody, points5, 4, -0.2f);


    b2Vec2 points10[4];
    points10[0] = b2Vec2(0, 63);
    points10[1] = b2Vec2(249, 63);
    points10[2] = b2Vec2(249, 0);
    points10[3] = b2Vec2(0, 0);
    /* 
    b2Vec2 points10[5];
    points10[0] = b2Vec2(238, 4);
    points10[1] = b2Vec2(238, 63);
    points10[2] = b2Vec2(0, 63);
    points10[3] = b2Vec2(0, 99);
    points10[4] = b2Vec2(249, 99);*/
   
   
    rack = m_pObjectManager->CreateObjectOfType(eSprite::RackPurple, 60, 7, 10, 0.0f, b2_dynamicBody, points10, 4, 0);

    m_pObjectManager->CreateDynamicObject(eSprite::Box, 51.0f, 10, 15.0f, 1.0f);
    pusher = m_pObjectManager->CreateDynamicObject(eSprite::Box, 69.2f, 10, 15.0f, 1.0f);
    
}


void CGame::CreateButton(float x, float y){
  //Physics World
  b2BodyDef bd; 
	bd.type = b2_staticBody;
  bd.position.Set(x, y);

  //shape
  b2PolygonShape s;
  float w, h; //width and height of sprite
  m_pRenderer->GetSize(eSprite::RedButton, w, h);
  s.SetAsBox(RW2PW(w)/2.0f, RW2PW(h)/2.0f);

  //fixture
  b2FixtureDef fd;
	fd.shape = &s;
	fd.density = 1.0f;
	fd.restitution = 0.2f;

  //body
  b2Body* p = m_pPhysicsWorld->CreateBody(&bd);
  p->CreateFixture(&fd);

  //object manager
  m_pObjectManager->CreateObject(eSprite::RedButton, p);
} //CreateButton

/// Place a ball in Physics World and object manager.
/// \param x Horizontal coordinate in Physics World units.
/// \param y Vertical coordinate in Physics World units.
/// \param xv Horizontal component of velocity.
/// \param yv Vectical component of velocity.

void CGame::CreateBall(float x, float y, float xv, float yv){ 
  //Physics World
  b2BodyDef bd;
	bd.type = b2_dynamicBody;
  bd.position.Set(x, y);
  bd.linearVelocity.Set(xv, yv);

  //shape
  b2CircleShape s;
	s.m_radius = RW2PW(m_pRenderer->GetWidth(eSprite::Ball))/2.0f;

  //fixture
	b2FixtureDef fd;
	fd.shape = &s;
	fd.density = 1.0f;
	fd.restitution = 0.8f;

  //body
  b2Body* p = m_pPhysicsWorld->CreateBody(&bd);
  p->CreateFixture(&fd);
  //object manager
 // m_pObjectManager->CreateObject(eSprite::Ball, p);

  m_pObjectManager->CreateObject(eSprite::Ball, p);
  ball = p;

} //CreateBall

/// Poll the keyboard state and respond to the
/// key presses that happened since the last frame.

void CGame::KeyboardHandler(){
  m_pKeyboard->GetState(); //get current keyboard state 

  if(m_pKeyboard->TriggerDown(VK_F2)){ //change draw mode
    m_eDrawMode = eDrawMode((UINT)m_eDrawMode + 1);
    if(m_eDrawMode == eDrawMode::Size)m_eDrawMode = eDrawMode(0);
  } //if

  if(m_pKeyboard->TriggerDown(VK_SPACE)){
    switch(m_eGameState){
      case eGameState::Initial:
          //RW2PW(m_nWinWidth - 32)
        CreateBall(5, RW2PW(m_nWinHeight), -0, -100.0f);
        m_eGameState = eGameState::Running;
        m_pAudio->play(eSound::Whoosh);
        m_fStartTime = m_pTimer->GetTime();
      break;

      case eGameState::Finished:
        BeginGame(); //begin again
        m_eGameState = eGameState::Initial;
        m_pAudio->play(eSound::Restart); //must be after BeginGame(), which stops all sounds
      break;

      default: m_pAudio->play(eSound::Buzz);
    } //switch
  } //if
} //KeyboardHandler

/// Ask object manager to draw the game objects. RenderWorld
/// is notified of the start and end of the frame so
/// that it can let Direct3D do its pipelining jiggery-pokery.

void CGame::RenderFrame(){ 
  m_pRenderer->BeginFrame();
    m_pObjectManager->draw(); //draw the objects
    m_pParticleEngine->Draw(); //draw particles
    DrawClock(); //draw the timer
    if(m_eGameState == eGameState::Initial)
      m_pRenderer->DrawCenteredText("Hit space to begin.");
    else if(m_eGameState == eGameState::Finished)
      m_pRenderer->DrawCenteredText("Hit space to reset.");
  m_pRenderer->EndFrame();
} //RenderFrame

/// Draw a digital clock at the top right of the window, even if the
/// camera is panned left or right.

void CGame::DrawClock(){ 
  const float dx = 68.0f; //distance from right of screen
  const float dy = 48.0f; //distance from top of screen
  const float x = m_pRenderer->GetCameraPos().x; //ensure we are in screen space

  const Vector2 pos = Vector2(x + m_nWinWidth/2.0f - dx, m_nWinHeight - dy);
  m_pRenderer->Draw(eSprite::ClockFace, pos); //clock background

  float t = 0.0f; //for the time

  switch(m_eGameState){ //set t depending on game state
    case eGameState::Initial:  t = 0.0f; break; //clock reads zero
    case eGameState::Running:  t = m_pTimer->GetTime() - m_fStartTime; break;
    case eGameState::Finished: t = m_fTotalTime; break; //clock is stopped
    default: t = 0.0f;
  } //switch
  
  const UINT min = (UINT)floorf(t/60.0f); //minutes
  const UINT sec = (UINT)floorf(t - 60.0f*min); //seconds

  const std::string str = std::to_string(min) + ":" + //string to be drawn
    std::to_string(sec/10) + std::to_string(sec%10);

  const Vector2 pos2 = Vector2(m_nWinWidth - 112.0f, 18.0f); //text position
  m_pRenderer->DrawScreenText(str.c_str(), pos2, Colors::White); //draw in white
} //DrawClock

/// Handle keyboard input, move the game objects and render 
/// them in their new positions and orientations. Notify 
/// the timer of the start and end of the
/// frame so that it can calculate frame time. 

void CGame::ProcessFrame(){
  KeyboardHandler(); //handle keyboard input
  m_pAudio->BeginFrame(); //notify sound manager that frame has begun

  m_pTimer->Tick([&](){ 
    m_pPhysicsWorld->Step(m_pTimer->GetFrameTime(), 6, 2); //move all objects 
    m_pParticleEngine->step(); //move particles in particle effects
  });
  /*
  if (rack->GetPosition().x < 10) {
      isReverse = true;
  }
  if (rack->GetPosition().x > 90) {
      isReverse = false;
  }
  if (isReverse) {
      rack->SetLinearVelocity(b2Vec2(-5, 0));
  }
  else {
      rack->SetLinearVelocity(b2Vec2(5, 0));
  }*/
  if (ball != NULL) {
      if (rack->GetPosition().x > 30) {
          if (ball->GetPosition().y < 10) {
              rack->SetLinearVelocity(b2Vec2(-5, 0));
          }
          else {
              rack->SetLinearVelocity(b2Vec2(5, 0));
          }
      }
      else {
          pusher->SetLinearVelocity(b2Vec2(-5, 0));
      }
  }

  RenderFrame(); //render a frame of animation 
} //ProcessFrame


