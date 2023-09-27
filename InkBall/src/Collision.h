#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include "Identifiers.h"
#include <iostream>
#include "Util.h"
#include <list>
#include <unordered_set>
namespace Collsion {

	using Vec2 = sf::Vector2f;
	using Vec2u = sf::Vector2u;
	static float dot(const Vec2& vc1, const Vec2& vc2) {
		return float(vc1.x * (double)vc2.x + vc1.y * (double)vc2.y);
	}
	static float length(const Vec2& vec) {
		return float(sqrt((vec.x * (double)vec.x) + (vec.y * (double)vec.y)));
	}
	static std::vector<sf::Vector2u> getCollsionQuadrant(const Vec2u& pos) {

		return {
		sf::Vector2u(pos.x , pos.y),sf::Vector2u(pos.x + 1, pos.y), sf::Vector2u(pos.x - 1, pos.y), sf::Vector2u(pos.x + 1, pos.y - 1), sf::Vector2u(pos.x - 1, pos.y - 1), sf::Vector2u(pos.x, pos.y + 1), sf::Vector2u(pos.x, pos.y - 1) ,sf::Vector2u(pos.x - 1 ,pos.y + 1), sf::Vector2u(pos.x + 1, pos.y + 1),
		
		};

	}

	static std::tuple<bool, Inkball::HitDir > collsionEnt(sf::Sprite& ball, const sf::FloatRect& EntBounds, bool recoil) { // since the block is imvable const &

		auto ballBounds = ball.getGlobalBounds();
		Vec2 ballPos = Vec2(ballBounds.left + ballBounds.width / 2.0f, ballBounds.top + ballBounds.height / 2.0f);
		float Nx = std::clamp(ballPos.x, EntBounds.left, EntBounds.left + EntBounds.width);
		float Ny = std::clamp(ballPos.y, EntBounds.top, EntBounds.top + EntBounds.height);


		Vec2 D = Vec2(Nx, Ny) - ballPos;
		if (D.x == 0 && D.y == 0)
			return { false,Inkball::HitDir::REV};

		float overlap = ballBounds.width / 2.0f - length(D);
		

		
		if(overlap <= 0.0f)
			return  { false , Inkball::HitDir::NOHIT };

		if (!recoil)
			return { true , Inkball::HitDir::HIT };

		auto amount = -D / length(D) * overlap;
		ball.setPosition(amount + ballPos + Vec2(-ballBounds.width / 2.0f, -ballBounds.height / 2.0f));

		Vec2 dirs[4] = {
			{0.0f, 1.0f},
			{1.0f, 0.0f},
			{0.0f, -1.0f},
			{-1.0f, 0.0f},
#if 0
			{-0.707106f,0.707106f},
			{0.707106f,0.707106f},
			{-0.707106f,-0.707106f},
			{0.707106f,-0.707106f},
#endif // 0

		};

		double max = 0.0f;
		int match = -1;

		for (std::size_t i = 0; i < 4; i++) {

			double dotProd = dot(D / length(D), dirs[i]);
			if (dotProd > max) {
				max = dotProd;
				match = i;
			}

		}

		return { true , (Inkball::HitDir)match };

	}// collsion with a entity
	static bool collsionBall(sf::Sprite& ball1, sf::Sprite& ball2) {

		auto ball1Bounds = ball1.getLocalBounds();
		auto ball2Bounds = ball2.getLocalBounds();
		Vec2 ball1Pos = ball1.getPosition();
		Vec2 ball2Pos = ball2.getPosition();
		Vec2 diff = ball2Pos - ball1Pos;

		if (length(diff) > ball1Bounds.width || (diff.x == 0 && diff.y == 0)) // balls have same width
			return false;


		float amount = ball1Bounds.width - length(diff);
		Vec2  offsetVec = -amount * (diff / length(diff));
		ball1.setPosition(ball1.getPosition() + offsetVec);
		return true;
	}
	static std::tuple <bool, Vec2, std::tuple<sf::VertexArray, bool, bool>* > collsionLineSegment(sf::Sprite& ball, const std::unordered_set< std::tuple<sf::VertexArray, bool, bool> * >& lineSet) {

		auto ballBounds = ball.getGlobalBounds();
		Vec2 ballCenter(ballBounds.left + ballBounds.width / 2.0f, ballBounds.top + ballBounds.height / 2.0f);
		Vec2 pt1;
		//std::tuple<sf::vertex,bool,bool>
		for (auto line = lineSet.begin(); line != lineSet.end(); ++line) {

		    auto& [vertex ,_,remove] = (*(*line));
			std::size_t count = vertex.getVertexCount();

			for (std::size_t i = 0; i < count; i++) {


				//std::cout << (*line)[i].position.x << "   :    " << (*line)[i].position.y << std::endl;
				if (ballBounds.contains( vertex[i].position)) {

					pt1 = vertex[i].position;
					vertex.clear();
					remove = true;
					return { true, (ballCenter - pt1) / length(ballCenter - pt1) , *line };

				}

			}


		}

		return { false,Vec2(),nullptr };
	}

}