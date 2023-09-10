#include "GameState.h"
#include "Identifiers.h"
#include "Collision.h"
#include <iostream>
#include <SFML/Graphics/RectangleShape.hpp>

constexpr int SCORE = 0;
constexpr int HIGH_SCORE = 1;
constexpr int LEVEL = 2;
constexpr int TIME = 3;
 
GameState::GameState(StateStack& stack, sf::RenderWindow* window, std::vector<std::string>&& level,size_t currLevel ): State(stack, window),
m_score(0), m_vertices(), m_lastMousePos(0, 0), m_pressed(false), m_set(false), m_remove(),m_levelInfo(level),m_levelPointer(currLevel)
{
	m_level.loadLevel(m_levelInfo[m_levelPointer]);
	if (m_level.m_balls.size() - 1 >= 0)
		m_counter = m_level.m_balls.size() - 1;
}

 void GameState::draw()
{
	 // draw hud
	 
     
	 for (const auto& I : m_level.m_ballsDisplay)
		 m_window->draw(I);

	 for (const auto& hud : m_level.m_levelHud) 
		 m_window->draw(hud);
	 
	 for (const auto& hudStr : m_level.m_levelStr)
		 m_window->draw(hudStr);


	 for (std::size_t i = 0; i < Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE; i++)
		 for (int j = 0; j < Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE; j++)
			 if (!m_level.m_levelmap[i][j].empty())
				 m_window->draw(*m_level.m_levelmap[i][j].back());

#if 0
	 for (auto cell : coll) {
		 sf::RectangleShape shape(sf::Vector2f(32, 32));
		 shape.setPosition(float(cell.x * Inkball::CELL_SIZE), float(cell.y * Inkball::CELL_SIZE));
		 shape.setFillColor(sf::Color::Red);
		 m_window->draw(shape);
	 }
#endif // 0 checking collion quadrants



	 //draw lines
	 //---------------------------------------
	// draw balls

	 for(const auto & ball : m_level.m_balls)
		 if (!ball.m_supdate && !ball.m_remove)
			 m_window->draw(*ball.s_ball);

     //draw lines 
	 for (auto it = m_vertices.begin(); it != m_vertices.end(); it++) {
		 const auto& [arr,_, rem] = *it;
		 if (rem) {
			 m_remove = it;
			 m_set = true;
		 }
		 else
			 m_window->draw(arr);
	 }
	 //remove any lines
	 if (m_set == true) {
		 m_vertices.erase(m_remove);
		 m_set = false;
	 }

		 
	
}
 

bool GameState::update(sf::Time dt)
{
	
	if (m_pressed)
	{
		sf::Vector2f pos(sf::Mouse::getPosition().x - m_window->getPosition().x + mOffset.x, sf::Mouse::getPosition().y - m_window->getPosition().y + mOffset.y);
		if (m_lastMousePos != pos)
		{


			size_t locx = size_t(pos.x) / Inkball::CELL_SIZE;
			size_t locy = size_t(pos.y) / Inkball::CELL_SIZE;
			///)

			if (locx < 17 && locy < 17) {

				if (m_vertices.size() == 0 || !std::get<1>(m_vertices.front())) {// { ,working  ,  remove}
					m_vertices.push_front({ sf::VertexArray(),true,false });
					std::get<0>(m_vertices.front()).setPrimitiveType(sf::LinesStrip);
				}
				float length = Collsion::length(m_lastMousePos - pos);
	
				
				if (length > mSeparationCoff) {

					sf::Vector2f unit = (pos - m_lastMousePos) / Collsion::length(pos - m_lastMousePos);

					//std::cout << length << std::endl;
					
					for (float i = 1;; i+=1) {

						if ( i > length)
							break;

						std::get<0>(m_vertices.front()).append(sf::Vertex(m_lastMousePos + sf::Vector2f(unit.x * i, unit.y * i), sf::Color::Black));
					}


				}
				std::cout << pos.x << "   " << pos.y << std::endl;
				std::get<0>(m_vertices.front()).append(sf::Vertex(pos, sf::Color::Black));
				m_lineSegments[locx][locy].emplace(&(*m_vertices.begin()));

			}

		}
		m_lastMousePos = pos;
	}
	else
		m_lastMousePos= sf::Vector2f(sf::Mouse::getPosition().x - m_window->getPosition().x + mOffset.x, sf::Mouse::getPosition().y - m_window->getPosition().y + mOffset.y);


	//display 




	//

	// update balls and resolve any collsion
	size_t i = 0; //could have used normal for loop
	for (auto&ball : m_level.m_balls) {
	   
		ball.status(dt);
		
		if (ball.m_supdate && ball.m_spawnAfter >= sf::Time::Zero && ball.m_spawnAfter < dt * 5.0f) {
			
			if (i != 0 ) {
				
				if (!ball.m_threadCreated && m_level.m_balls[i - 1].m_supdate == false) {

				    std::thread	m_uiThread = std::thread(&GameState::updateUiAnim, this, std::ref(ball));
					m_uiThread.detach();
					ball.m_threadCreated = true;

				}
				
			}
			else {
				if (!ball.m_threadCreated) {
					std::thread	m_uiThread = std::thread(&GameState::updateUiAnim, this, std::ref(ball));
					m_uiThread.detach();
					ball.m_threadCreated = true;
				}	
			
			}
			
		}
	
	
	

	    // since the balls are sorted in increasing order of spawn time
		
		if (!ball.m_supdate & !ball.m_remove) {
			ball.s_ball->update(dt);
			auto pos = ball.s_ball->getPosition();

			// use velocity vector to check for neighbouring entitys 
			auto vel = ball.s_ball->getVelocity();
			auto cells = Collsion::getCollsionQuadrant(pos);
			coll = cells; // collsion boundary  to be   removed later
			//std::cout << pos.x <<"     " << pos.y << std::endl;
			for (auto& cell : cells) {//cells to 
				

				if ((cell.x >= 0 && cell.x <= 16) && (cell.y >= 0 && cell.y <= 16) ){ // due to speed of the ball it might be near to the edge
					
					if (!m_level.m_levelmap[cell.x][cell.y].empty()) {

						if (m_level.m_levelmap[cell.x][cell.y].back()->getID() == (std::size_t)Inkball::Textures::EntityType::TILE)
						{
							Tile* tile = (static_cast<Tile*>(&*m_level.m_levelmap[cell.x][cell.y].back()));
							//check for precise collsions  cell.x,cell.y,Inkball::CELL_SIZE, Inkball::CELL_SIZE
							auto [hit, dir] = Collsion::collsionEnt(ball.s_ball->getSprite(),tile->getBounds(), false); // no recoil needed for tile entity
							if (hit) {
								tile->onContact(*ball.s_ball, ball.m_remove, ball.m_match, m_score);
								if (ball.m_match == false) {

									//game over condition

								}

								if (ball.m_remove & ball.m_match) {

									m_level.m_levelStr[SCORE].setString(std::to_string(m_score));

								}
							}
					   	}
						else if (m_level.m_levelmap[cell.x][cell.y].back()->getID() == (std::size_t)Inkball::Textures::EntityType::BLOCK)
						{
							Block* block = (static_cast<Block*>(&*m_level.m_levelmap[cell.x][cell.y].back()));

						    Collsion::collsionEnt(ball.s_ball->getSprite(), block->getBounds(), false); //first with no recoil if vel vector is allowed add recoil
							bool allow = block->allowCollision(vel, ball.s_ball->getColor());
							if (allow) {
								auto [hit, dir] = Collsion::collsionEnt(ball.s_ball->getSprite(), block->getBounds(), true); // block has diffrent float rects not uniform ones as tiles
								if (hit) {
									//resolve collsion here 
								   // std::cout << "HIT" << (std::size_t)dir << std::endl;
									if (dir == Inkball::HitDir::LEFT || dir == Inkball::HitDir::RIGHT)
										ball.s_ball->setVelocity(-vel.x, vel.y);
									else if (dir == Inkball::HitDir::DOWN || dir == Inkball::HitDir::UP)
										ball.s_ball->setVelocity(vel.x, -vel.y);

									block->onCollision(*ball.s_ball,m_score);

								}
								else if (dir == Inkball::HitDir::REV)
									ball.s_ball->setVelocity(-vel.x, -vel.y);
							}
						}

					}
					else {
						    //  resolve any ball collsion ball - to line collsion
					       //	std::cout  << m_vertices.size()<< std::endl;
						    sf::Vector2f inScale = ball.s_ball->getSprite().getScale();
						    ball.s_ball->getSprite().setScale( std::clamp(   float(inScale.x + 0.02) , 0.f ,0.8f ) , std::clamp(float(inScale.y + 0.02), 0.f, 0.8f)  );

						    if (m_lineSegments[cell.x][cell.y].size() > 0) {
								
								auto [hit,dir,rem] = Collsion::collsionLineSegment(ball.s_ball->getSprite(),m_lineSegments[cell.x][cell.y]);
								if (hit) {
								
								     m_level.m_balls[i].s_ball->setVelocity(dir.x*100,dir.y*100);

									 for(size_t l = 0; l < Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE; l++)
										 for (size_t j = 0; j < Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE; j++) {
											 if (m_lineSegments[l][j].find(rem) != m_lineSegments[l][j].end())
												 m_lineSegments[l][j].erase(rem);
											
										 }
									 

								}

							}
						    
					
						    ///ball dynamic resoulution
							for (size_t j = 0; j < m_level.m_balls.size(); j++) {
								if (j != i && m_level.m_balls[j].m_remove == false && m_level.m_balls[j].m_supdate==false && m_level.m_balls[j].s_ball->getPosition().x == cell.x && m_level.m_balls[j].s_ball->getPosition().y == cell.y) {
									auto& sp1 = ball.s_ball->getSprite();
									auto& sp2 = m_level.m_balls[j].s_ball->getSprite();
									auto vel2 = m_level.m_balls[j].s_ball->getVelocity();
									if (Collsion::collsionBall(sp1,sp2)) {
										sf::Vector2f normal  = (sp2.getPosition() - sp1.getPosition()) / float(Inkball::CELL_SIZE);
										sf::Vector2f tangent(-normal.y,normal.x);
										
										float tangResp1 = Collsion::dot(vel,tangent);
										float tangResp2 = Collsion::dot(vel2, tangent);

										float normalResp1 = Collsion::dot(vel, normal);
										float normalResp2 = Collsion::dot(vel2, normal);

										ball.s_ball->setVelocity(tangent.x * tangResp1 + normal.x * normalResp2, tangent.y * tangResp1 + normal.y * normalResp2);
										m_level.m_balls[j].s_ball->setVelocity(tangent.x * tangResp2 + normal.x * normalResp1, tangent.y * tangResp2 + normal.y * normalResp1);

									}
								}

							}

					}

			    }
				
			
			}
			
		}
		i++;
	}

	//finaly update level
	m_level.updateLevel(dt);

	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed ) {

		if (event.key.code == sf::Keyboard::Escape) {
			
			sf::Clock clock;
			while (clock.getElapsedTime().asSeconds() < 10.0f);
			requestStackPop();
			requestStackPush(Inkball::States::Id::TITLE);
		}
    }
	else if (event.type == sf::Event::MouseButtonPressed) {
		m_pressed = true;
	}
	else if (event.type == sf::Event::MouseButtonReleased)
	{
		if (m_vertices.size() > 0)
			std::get<1>(m_vertices.front()) = false;
		m_pressed = false; // Reset line

	}
	
	return false;
}

GameState::~GameState()
{
	m_vertices.clear();
}

void GameState::updateUiAnim(Level::ballInfo& ball)
{


	sf::Clock clock;

	sf::Color col = m_level.m_ballsDisplay[m_counter].getColor();


	for (int j = 3; j < 9; j++) {

		m_level.m_ballsDisplay[m_counter].setColor(sf::Color::Transparent);

		while (clock.getElapsedTime().asSeconds() < 1.0f - j * 0.09);

		clock.restart();

		m_level.m_ballsDisplay[m_counter].setColor(col);

		while (clock.getElapsedTime().asSeconds() < 1.0f - j * 0.09);

		clock.restart();
		
	}
	m_level.m_ballsDisplay[m_counter].setColor(sf::Color::Transparent);

	

	for (int j = 0; j < m_level.m_ballsDisplay.size() - 1; j++)
		m_level.m_ballsDisplay[j].setPosition(m_level.m_ballsDisplay[j + 1].getPosition());


    m_level.m_ballsDisplay[m_counter].setColor(sf::Color::Transparent);


	--m_counter;

	ball.m_supdate = false;
	
}




