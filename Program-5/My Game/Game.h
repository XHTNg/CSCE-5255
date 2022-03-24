/// \file Game.h
/// \brief Interface for the game class CGame.

#ifndef __L4RC_GAME_GAME_H__
#define __L4RC_GAME_GAME_H__

#include "Component.h"
#include "Common.h"
#include "ObjectManager.h"
#include "Settings.h"

#include "ContactListener.h"

/// \brief The game class.

class CGame: 
  public LComponent, //game components from the Engine
  public LSettings, //game settings from gamesettings.xml, via the Engine
  public CCommon{ //common parameters for this game

  private: 
    CMyListener m_cContactListener; ///< Contact listener.
    b2Body* rack;
    b2Body* pusher;
    b2Body* ball;
    bool isReverse = false;
    void LoadSounds(); ///< Load sounds. 

    void BeginGame(); ///< Begin playing the game.
    void CreateMap();
    void KeyboardHandler(); ///< The keyboard handler.
    void DrawClock(); ///< Draw a timer.
    void RenderFrame(); ///< Render an animation frame.

    void CreateButton(float x, float y); ///< Create final button.
    void CreateBall(float x, float y, float xv=0.0f, float yv=0.0f); ///< Create and launch ball.

  public:
    ~CGame(); ///< Destructor.

    void Initialize(); ///< Initialize the game.
    void ProcessFrame(); ///< Process an animation frame.
    void Release(); ///< Release the renderer.
}; //CGame

#endif //__L4RC_GAME_GAME_H__
