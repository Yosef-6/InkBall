#include "LevelEditor.h"
#include "Resource.h"
#include "SFML/Graphics/Image.hpp"

const Resource& res = Resource::getResourceHandle();

std::string generateCode(const sf::Event& event) {
	size_t posx = event.mouseMove.x / Inkball::CELL_SIZE;
	size_t posy = event.mouseMove.y / Inkball::CELL_SIZE;
	return std::to_string(posx) + " | " + std::to_string(posy);
}
std::string generateCode(int x , int y) {
	size_t posx = x / Inkball::CELL_SIZE;
	size_t posy = y / Inkball::CELL_SIZE;
	return std::to_string(posx) + " | " + std::to_string(posy);
}


LevelEditor::LevelEditor(StateStack& stack, sf::RenderWindow* window):State(stack, window)
{
	
	auto pos = window->getPosition();
	window->close();
	window->create(sf::VideoMode(544, 576 + 96), "Inkball v1.0 Level Editor", sf::Style::Close);
	window->setPosition(sf::Vector2i(pos.x,pos.y-96));
	window->setMouseCursorGrabbed(true);

	m_backg.setTexture(res.getTexture(std::tuple<int, int, int>(static_cast<int>(Inkball::Textures::OtherType::S_FLOOR), 0, 0)));
	m_hud.setTexture(res.getTexture(std::tuple<int, int, int>(static_cast<int>(Inkball::Textures::OtherType::EDITER_HUD), 0, 0)));
	m_items.setTexture(res.getTexture(std::tuple<int, int, int>(static_cast<int>(Inkball::Textures::OtherType::ITEMS), 0, 0)));
	m_select.setTexture(res.getTexture(std::tuple<int, int, int>(static_cast<int>(Inkball::Textures::OtherType::ITEM_SELECT), 0, 0)));
	m_select.setPosition(0,576);
	m_items.setPosition(0,576);
	m_hover.setBuffer(res.getSoundBuffer(Inkball::Sound::SoundEffects::HOVER));
	m_click.setBuffer(res.getSoundBuffer(Inkball::Sound::SoundEffects::MENU_CLICK));
	for (int i = 0; i < 5; i++) {
         
		std::size_t posx = i * 64 / Inkball::CELL_SIZE;
		std::size_t posy = 576 / Inkball::CELL_SIZE ;
		m_itemsMapSize[std::to_string(posx) + " | " + std::to_string(posy)] = new sf::IntRect(i * 64, 576, 64, 64);
		m_itemsMapSize[std::to_string(posx + 1) + " | " + std::to_string(posy)] = m_itemsMapSize[std::to_string(posx) + " | " + std::to_string(posy)];
		m_itemsMapSize[std::to_string(posx + 1) + " | " + std::to_string(posy + 1)] = m_itemsMapSize[std::to_string(posx) + " | " + std::to_string(posy)];
		m_itemsMapSize[std::to_string(posx) + " | " + std::to_string(posy + 1)] = m_itemsMapSize[std::to_string(posx) + " | " + std::to_string(posy)];

	}

	for (int i = 5; i < 12; i++) {
		m_itemsMapSize[std::to_string(((i - 5) * 32 + 320) / Inkball::CELL_SIZE ) + " | " + std::to_string(576 / Inkball::CELL_SIZE) ] = new sf::IntRect((i - 5) * 32 + 320, 576, 32, 32);
		m_itemsMapSize[std::to_string(((i - 5) * 32 + 320) / Inkball::CELL_SIZE) + " | " + std::to_string(608 / Inkball::CELL_SIZE)] = new sf::IntRect((i - 5) * 32 + 320, 608, 32, 32);
	}
	for (int i = 19; i < 36; i++) 
		m_itemsMapSize[std::to_string(((i - 19) * 32) / Inkball::CELL_SIZE) + " | " + std::to_string( 640 / Inkball::CELL_SIZE)] = new sf::IntRect((i - 19) * 32, 640, 32, 32);
	
	for (int i = 0; i < 17;i++) { // lazy way of adding core blocks
		for (int j = 0; j < 17; j++) {
			if ((i > 0 && i < 16) && (j > 0 && j < 16))
				continue;
			Item whiteBlock;
			whiteBlock.m_cellPos    = std::make_tuple(i,j);
			whiteBlock.m_entityType = std::make_tuple(0,0,0);
			whiteBlock.m_itemSprite.setTexture(res.getTexture(std::tuple<int, int, int>(0,0,0)));
			whiteBlock.m_itemSprite.setPosition(j*32, i * 32 + 32); //x grows to the right 
			whiteBlock.m_lastItem = true;
			m_board[i][j].push_back(whiteBlock);
		}
    }

	if (!m_overlay.create(Inkball::SCREEN_WIDTH, Inkball::SCREEN_HEIGHT - 32)) 
		throw std::runtime_error("LevelEditor::Constructor - unable to create Render Texture ");
	 
	m_overlay.display();
	m_overlaySprite.setTexture(m_overlay.getTexture());
	m_overlaySprite.setPosition(0.0f,32.0f);

	
	//holes
	m_itemsMapEntityCode["0 | 18"] = std::make_tuple(2, 0, 0);
	m_itemsMapEntityCode["0 | 19"] = std::make_tuple(2, 0, 0);
	m_itemsMapEntityCode["1 | 18"] = std::make_tuple(2, 0, 0);
	m_itemsMapEntityCode["1 | 19"] = std::make_tuple(2, 0, 0);

	m_itemsMapEntityCode["2 | 18"] = std::make_tuple(2, 0, 2);
	m_itemsMapEntityCode["2 | 19"] = std::make_tuple(2, 0, 2);
	m_itemsMapEntityCode["3 | 18"] = std::make_tuple(2, 0, 2);
	m_itemsMapEntityCode["3 | 19"] = std::make_tuple(2, 0, 2);

	m_itemsMapEntityCode["4 | 18"] = std::make_tuple(2, 0, 3);
	m_itemsMapEntityCode["4 | 19"] = std::make_tuple(2, 0, 3);
	m_itemsMapEntityCode["5 | 18"] = std::make_tuple(2, 0, 3);
	m_itemsMapEntityCode["5 | 19"] = std::make_tuple(2, 0, 3);

	m_itemsMapEntityCode["6 | 18"] = std::make_tuple(2, 0, 4);
	m_itemsMapEntityCode["6 | 19"] = std::make_tuple(2, 0, 4);
	m_itemsMapEntityCode["7 | 18"] = std::make_tuple(2, 0, 4);
	m_itemsMapEntityCode["7 | 19"] = std::make_tuple(2, 0, 4);

	m_itemsMapEntityCode["8 | 18"] = std::make_tuple(2, 0, 1);
	m_itemsMapEntityCode["8 | 19"] = std::make_tuple(2, 0, 1);
	m_itemsMapEntityCode["9 | 18"] = std::make_tuple(2, 0, 1);
	m_itemsMapEntityCode["9 | 19"] = std::make_tuple(2, 0, 1);

	//colored blocks
	m_itemsMapEntityCode["10 | 18"] = std::make_tuple(0, 0, 0);
	m_itemsMapEntityCode["11 | 18"] = std::make_tuple(0, 0, 2);
	m_itemsMapEntityCode["12 | 18"] = std::make_tuple(0, 0, 3);
	m_itemsMapEntityCode["13 | 18"] = std::make_tuple(0, 0, 4);
	m_itemsMapEntityCode["14 | 18"] = std::make_tuple(0, 0, 1);
	//broken blocks
	m_itemsMapEntityCode["10 | 19"] = std::make_tuple(0, 1, 0);
	m_itemsMapEntityCode["11 | 19"] = std::make_tuple(0, 1, 2);
	m_itemsMapEntityCode["12 | 19"] = std::make_tuple(0, 1, 3);
	m_itemsMapEntityCode["13 | 19"] = std::make_tuple(0, 1, 4);
	m_itemsMapEntityCode["14 | 19"] = std::make_tuple(0, 1, 1);
	//balls
	m_itemsMapEntityCode["15 | 18"] = std::make_tuple(1, 0, 2);
	m_itemsMapEntityCode["16 | 18"] = std::make_tuple(1, 0, 0);
	m_itemsMapEntityCode["15 | 19"] = std::make_tuple(1, 0, 1);
	m_itemsMapEntityCode["16 | 19"] = std::make_tuple(1, 0, 3);
	m_itemsMapEntityCode["16 | 20"] = std::make_tuple(1, 0, 4);
	//directional 2
	m_itemsMapEntityCode["0 | 20"] = std::make_tuple(0, 3, 1);
	m_itemsMapEntityCode["1 | 20"] = std::make_tuple(0, 3, 3);
	m_itemsMapEntityCode["2 | 20"] = std::make_tuple(0, 3, 2);
	m_itemsMapEntityCode["3 | 20"] = std::make_tuple(0, 3, 4);
	//directional 
	m_itemsMapEntityCode["4 | 20"] = std::make_tuple(0, 2, 1);
	m_itemsMapEntityCode["5 | 20"] = std::make_tuple(0, 2, 3);
	m_itemsMapEntityCode["6 | 20"] = std::make_tuple(0, 2, 2);
	m_itemsMapEntityCode["7 | 20"] = std::make_tuple(0, 2, 4);
	m_itemsMapEntityCode["8 | 20"] = std::make_tuple(0, 2, 0);
	//others
	m_itemsMapEntityCode["9 | 20"] = std::make_tuple(0, 4, 1);
	m_itemsMapEntityCode["10 | 20"] = std::make_tuple(0, 4, 3);
	m_itemsMapEntityCode["11 | 20"] = std::make_tuple(0, 4, 2);
	m_itemsMapEntityCode["12 | 20"] = std::make_tuple(0, 4, 4);
	m_itemsMapEntityCode["13 | 20"] = std::make_tuple(0, 5, 0);
	m_itemsMapEntityCode["14 | 20"] = std::make_tuple(2, 1, 0);
	m_itemsMapEntityCode["15 | 20"] = std::make_tuple(2, 2, 0);

	setSelector("0 | 18"); // default selector position
}

bool LevelEditor::draw()
{
	
	m_window->draw(m_backg);
	m_window->draw(m_items);
	m_window->draw(m_hud);
	m_window->draw(m_select);
	for (int i = 0; i < 17; i++) { 
		for (int j = 0; j < 17; j++) 
			if (!m_board[i][j].empty())
				m_window->draw(m_board[i][j].back().m_itemSprite);
	}
	
	if (m_itemMove) {
		m_window->draw(m_overlaySprite);
		m_window->draw(m_selectedItemSprite);
	}

	
	return false;
}

bool LevelEditor::update(sf::Time dt)
{
	if (m_itemMove)
	{   
		//correction to set item in center of mouse pointer
		m_selectedItemSprite.setPosition(sf::Mouse::getPosition().x - m_window->getPosition().x - 8 -( m_itemsMapSize[m_selectedItemCode]->width / 2), sf::Mouse::getPosition().y - m_window->getPosition().y - 31 - m_itemsMapSize[m_selectedItemCode]->height / 2); 


	}
	return false;
}

bool LevelEditor::handleEvent(const sf::Event& event)
{   
	if (event.type == sf::Event::MouseMoved) {

		if (!m_select.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y) && !m_itemMove)
			setSelector(generateCode(event));

		if (m_itemMove) {
			m_overlay.clear(sf::Color(255U,255U,255U,0U));

			const std::vector<std::pair<int,int>> info = calculatePlacingCells();
			const std::size_t numberOfCells = info[0].first;
			const std::pair<int, int> cellNumber = info[1];

			int i = 0;
			bool isBlocked = false;
			while (i != numberOfCells) {

				
				auto xCell = cellNumber.first + info[i+2].first;
				auto yCell = cellNumber.second - 1 + info[i+2].second;
				if (xCell <= 16 && xCell >= 0 && yCell >= 0 && yCell <= 16)
					if (m_board[xCell][yCell].size() != 0 && m_board[xCell][yCell][0].m_lastItem) {
						isBlocked = true;
						break;
					}
				i++;
			}
			i = 0;
			while (i != numberOfCells) {

				auto xCell = cellNumber.first + info[i + 2].first;
				auto yCell = cellNumber.second - 1 + info[i + 2].second;
				sf::Sprite s;
				if(!isBlocked)
				s.setTexture(res.getTexture(std::tuple<int, int, int>(static_cast<int>(Inkball::Textures::OtherType::CORRECT), 0, 0)));
				else
				s.setTexture(res.getTexture(std::tuple<int, int, int>(static_cast<int>(Inkball::Textures::OtherType::ERROR), 0, 0)));

				s.setPosition(xCell * Inkball::CELL_SIZE, yCell * Inkball::CELL_SIZE);
				m_overlay.draw(s);
				m_overlay.display();
				i++;
			}

			

			
		}  
	}
	else if (event.type == sf::Event::MouseButtonPressed) {
       
		//constants are used to convert the global mouse postion to renderwindow points since the mouse moved event is not captured here
		sf::Vector2i pos(sf::Mouse::getPosition().x - m_window->getPosition().x - 8, sf::Mouse::getPosition().y - m_window->getPosition().y - 31);
		std::string code = generateCode(pos.x,pos.y);
		
		if (m_select.getGlobalBounds().contains(pos.x, pos.y)) {
			
			if (!m_itemMove) {
				m_selectedItemSprite = sf::Sprite(res.getTexture(m_itemsMapEntityCode[code]));
				if (code == "9 | 20" || code == "10 | 20" || code == "11 | 20" || code == "12 | 20" || code == "13 | 20")
					m_selectedItemSprite.setTextureRect(sf::IntRect(128, 0, 32, 32));

				m_selectedItemSprite.setColor(sf::Color(255U,255U,255U,40U));
				m_click.play();
				while (m_click.getStatus() == sf::Sound::Playing);
				m_itemMove = true;
			}
			
		}

		if (m_itemMove) {



		}
		
	}
	else if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Escape) {
			if (m_itemMove) {
				m_itemMove = false;
				m_overlay.clear(sf::Color(255U, 255U, 255U, 0U));
				m_overlay.display();
			}
		}
	}

	return false;
}

LevelEditor::~LevelEditor()
{
	
}

void LevelEditor::setSelector(const std::string& code)
{
	if (m_itemsMapSize.find(code) != m_itemsMapSize.end()) {
		const auto& rect = m_itemsMapSize.at(code);
		m_select.setScale(sf::Vector2f(float(rect->width)/Inkball::CELL_SIZE, float(rect->height)/Inkball::CELL_SIZE));
		m_select.setPosition(rect->left,rect->top);
		m_selectedItemCode = code;
		m_hover.play();
	}
}

std::vector<std::pair<int, int>> LevelEditor::calculatePlacingCells()
{
	const auto& size = m_itemsMapSize.at(m_selectedItemCode);
	std::size_t numberOfCells = (size->width * size->height) / (Inkball::CELL_SIZE * Inkball::CELL_SIZE);
	sf::Vector2i mousePos(sf::Mouse::getPosition().x - m_window->getPosition().x - 8, sf::Mouse::getPosition().y - m_window->getPosition().y - 31);
	sf::Vector2i cellNumber = sf::Vector2i(mousePos.x / Inkball::CELL_SIZE, mousePos.y / Inkball::CELL_SIZE);
	//std::cout << cellNumber.x << "  |  " << cellNumber.y << std::endl;
	sf::Vector2i cellPosCenter = (Inkball::CELL_SIZE * cellNumber) + sf::Vector2i(Inkball::CELL_SIZE / 2, Inkball::CELL_SIZE / 2);


	std::vector<std::pair<int, int>> ret;

	ret.push_back({ numberOfCells,0 });          // req information for further calc
	ret.push_back({cellNumber.x,cellNumber.y}); // req information for further calc

	ret.push_back({ 0,0 });

	if ((mousePos - cellPosCenter).x >= 0)
		ret.push_back({ 1,0 });
	else
		ret.push_back({ -1,0 });//

	if ((mousePos - cellPosCenter).y >= 0)
		ret.push_back({ 0,1 });
	else
		ret.push_back({ 0,-1 });//


	ret.push_back({ ret[3].first + ret[4].first , ret[3].second + ret[4].second });


	return ret;
}


