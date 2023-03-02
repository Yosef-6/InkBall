#include "GameState.h"
#include "Identifiers.h"
#include "Collision.h"
#include <iostream>
#include <SFML/Graphics/RectangleShape.hpp>

GameState::GameState(StateStack& stack, sf::RenderWindow* window):State(stack,window),m_level(1),m_score(0),m_vertices(),m_lastMousePos(0,0),m_pressed(false), mOffset(-5, -25)
{
	m_vertices.push_back(sf::VertexArray());
	m_vertices.back().setPrimitiveType(sf::LinesStrip);
	
}

 void GameState::draw()
{
	 // draw hud


	 for (const auto& hud : m_level.m_levelHud) 
		 m_window->draw(*hud);
	 


	 for (std::size_t i = 0; i < Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE; i++)
		 for (int j = 0; j < Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE; j++)
			 if (!m_level.m_levelmap[i][j].empty())
				 m_window->draw(*m_level.m_levelmap[i][j].back());

	 for (auto cell : coll) {
		 sf::RectangleShape shape(sf::Vector2f(32,32));
		 shape.setPosition( float(cell.x* Inkball::CELL_SIZE),float(cell.y*Inkball::CELL_SIZE));
		 shape.setFillColor(sf::Color::Red);
		 m_window->draw(shape);
	 }


	 //draw lines
	 //---------------------------------------
	// draw balls

	 for(const auto & ball : m_level.m_balls)
		 if (ball.s_ready)
			 m_window->draw(*ball.s_ball);

	
     //draw lines 
	 for (auto& i : m_vertices) 
		 m_window->draw(i);
	
}
 

bool GameState::update(sf::Time dt)
{
	
	
	if (m_pressed)
	{
		sf::Vector2f pos(sf::Mouse::getPosition().x - m_window->getPosition().x + mOffset.x, sf::Mouse::getPosition().y - m_window->getPosition().y + mOffset.y);
		if (m_lastMousePos != sf::Mouse::getPosition()) 
		{ 
			
		 
			size_t locx = size_t(pos.x) / Inkball::CELL_SIZE;
			size_t locy = size_t(pos.y) / Inkball::CELL_SIZE -1;
			
			if (locx < 17 && locy < 17) {
				m_vertices.front().append(sf::Vertex(pos, sf::Color::Black));
		        m_lineSegments[locx][locy].emplace(&(*m_vertices.begin()));
				//std::cout <<locx << "  ,  " << locy<< " |||| " << m_vertices.size() << "    :      " << m_lineSegments[locx][locy].size() << std::endl;
			}
			
			
			m_lastMousePos = sf::Mouse::getPosition();

		}
	}
	


	// update balls and resolve any collsion
	size_t i = 0; //could have used normal for loop
	for (auto&ball : m_level.m_balls) {
		ball.status(dt);
		if (ball.s_ready) {
			ball.s_ball->update(dt);
			auto pos = ball.s_ball->getPosition();

			// use velocity vector to check for neighbouring entitys 
			auto vel = ball.s_ball->getVelocity();
			auto cells = Collsion::getCollsionQuadrant(pos);
			coll = cells; // collsion boundary  to be   removed later
			//std::cout << pos.x <<"     " << pos.y << std::endl;
			for (auto& cell : cells) {//cells to 

				if ((cell.x >= 0 && cell.x <= 16) && (cell.y >= 0 && cell.y <= 16)){ // due to speed of the ball it might be near to the edge
					if (!m_level.m_levelmap[cell.x][cell.y].empty()) {

						if (m_level.m_levelmap[cell.x][cell.y].back()->getID() == (std::size_t)Inkball::Textures::EntityType::TILE)
						{
							Tile* tile = (static_cast<Tile*>(&*m_level.m_levelmap[cell.x][cell.y].back()));
							//check for precise collsions  cell.x,cell.y,Inkball::CELL_SIZE, Inkball::CELL_SIZE
							auto [hit, dir] = Collsion::collsionEnt(ball.s_ball->getSprite(), sf::FloatRect((float)cell.x, (float)cell.y, (float)Inkball::CELL_SIZE, (float)Inkball::CELL_SIZE), true); // no recoil needed for tile entity
							if (hit)
								tile->onContact(*ball.s_ball);
						}
						else if (m_level.m_levelmap[cell.x][cell.y].back()->getID() == (std::size_t)Inkball::Textures::EntityType::BLOCK)
						{
							Block* block = (static_cast<Block*>(&*m_level.m_levelmap[cell.x][cell.y].back()));

							auto [hit, dir] = Collsion::collsionEnt(ball.s_ball->getSprite(), block->getBounds(), true); // block has diffrent float rects not uniform ones as tiles
							if (hit) {
								//resolve collsion here 
							   // std::cout << "HIT" << (std::size_t)dir << std::endl;
								if (dir == Collsion::HitDir::LEFT || dir == Collsion::HitDir::RIGHT)
									ball.s_ball->setVelocity(-vel.x, vel.y);
								else if (dir == Collsion::HitDir::DOWN || dir == Collsion::HitDir::UP)
									ball.s_ball->setVelocity(vel.x, -vel.y);

                                 block->onCollision(*ball.s_ball);

							}
							else if (dir == Collsion::HitDir::REV)
								ball.s_ball->setVelocity(-vel.x, -vel.y);
						}

					}
					else {
						    //resolve any ball collsion ball - to line collsion
						    
						    if (m_lineSegments[cell.x][cell.y].size() > 0) {
								
								auto [hit,dir] = Collsion::collsionLineSegment(ball.s_ball->getSprite(),m_lineSegments[cell.x][cell.y]);
								if (hit) {
									std::cout << "HIT REGISTERED" << std::endl;
									float length = Collsion::length(vel);
									float cosAngle = Collsion::dot(-(vel/length),dir);
								
			
									ball.s_ball->setVelocity( sqrt((length * length) - pow(cosAngle * length,2.0f) ) , -(cosAngle * length) );
								}

							}
						    
						    
						    
						    ///ball dynamic resoulution
							for (size_t j = 0; j < m_level.m_balls.size(); j++) {
								if (j != i && m_level.m_balls[j].s_ball->getPosition().x == cell.x && m_level.m_balls[j].s_ball->getPosition().y == cell.y) {
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


	for (std::size_t i = 0; i < Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE; i++)
		for (int j = 0; j < Inkball::SCREEN_WIDTH / Inkball::CELL_SIZE; j++)
			if (!m_level.m_levelmap[i][j].empty())
				  (m_level.m_levelmap[i][j].back())->update(dt);



	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed ) {

		if (event.key.code == sf::Keyboard::Escape) {
			requestStackPop();
			requestStackPush(Inkball::States::Id::TITLE);
		}
    }
	else if (event.type == sf::Event::MouseButtonPressed) {
		m_pressed = true;
	}
	else if (event.type == sf::Event::MouseButtonReleased)
	{

		m_vertices.push_front(sf::VertexArray());
		m_vertices.front().setPrimitiveType(sf::LinesStrip);
		m_pressed = false; // Reset line

	}
	return false;
}

GameState::~GameState()
{
	m_vertices.clear();
}
