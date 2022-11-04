#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include "Identifiers.h"
#include <iostream>
class Animate   //simple animation that manages fram changes of animated blocks
{
	
public:
	enum class State {
		PLAY,
		STOP,
	};
public:
	Animate(sf::Sprite *sprite,unsigned short frames,bool loop,float ticks):m_sprite(sprite),m_frames(frames),m_animState(State::PLAY),m_loop(loop),m_frameNo(1),m_ticks(ticks),m_tickNo(0.0f){
	    
		m_sprite->setTextureRect(sf::IntRect((frames-1)*Inkball::CELL_SIZE,0,Inkball::CELL_SIZE,Inkball::CELL_SIZE));

	}
	void updateState(float dt) {
		if (m_animState != State::PLAY)
			return;
		m_tickNo+=dt;
		if (m_tickNo >= m_ticks) {
			std::cout << m_frameNo<<"   "<<"animationg" << std::endl;
			m_frameNo++;
			m_tickNo = 0;
			m_sprite->setTextureRect(sf::IntRect((m_frames - m_frameNo) *Inkball::CELL_SIZE, 0,Inkball::CELL_SIZE, Inkball::CELL_SIZE));
			if (m_frameNo == m_frames && m_loop == false)
				m_animState = State::STOP;
			else if (m_frameNo == m_frames && m_loop == true)
				m_frameNo = 0;
		}
    }
	State getState() const  {
		
		return m_animState;

	}
private:
	const unsigned short m_frames;
    float m_ticks;
	unsigned short m_frameNo;
	float m_tickNo;
	sf::Sprite* m_sprite;
	State m_animState;
	bool m_loop;
};

