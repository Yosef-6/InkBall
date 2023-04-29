#pragma once
namespace Inkball{
    
	const int SCREEN_WIDTH  = 544;
	const int SCREEN_HEIGHT = 576;
	const int CELL_SIZE     = 32;
	
	enum class HitDir {
		UP,
		RIGHT,
		DOWN,
		LEFT,
		HIT,
		REV,
		NOHIT,
	};



	enum class ActionType {
		DELETE_SELF =0,
		DELETE,
		NONE,
		//...
	};

	struct Action {
		ActionType mtype;
		size_t cellx, celly;
		//...
	};
	enum class Direction {
		DEFAULT = 0,
		UP,
		LEFT,
		DOWN,
		RIGHT
	};
	enum class Fonts {

		TEXT1,

	};
	namespace Textures {
		enum class Color {
			WHITE = 0, // default color for items that does not have color
			BLUE,
			ORANGE,
			GREEN,
			YELLOW,
			END,
		};
		enum class BlockType {
			COLORED = 0,  // blocks that change colors of the ball
			BROKEN,
			DIRECTIONAL,
			DIRECTIONAL2,
			KEY,  // after hole logic is implemented
			TIMER,
			END,
		};
		enum class TileType {
			HOLE = 0,
			TRACK,
			SPAWN,
			END,
		};
		enum class EntityType {
			BLOCK = 0,
			BALL,
			TILE,
		};
		enum class OtherType {
			FLOOR = -1,
			S_FLOOR = -2,
			LOGO  = -3,
			MENU_FLOOR = -4,
			ERROR_PREVIEW = -5,
			MENU_RIGHT = -6,
			MENU_LEFT  = -7,
			//...
		};
	}
	namespace States {
		enum class Id {
			NONE = 0,
			GAME,
			MAIN_MENU,
			TITLE,
			LEVEL_MANAGER,
			//...
		};
	}

	namespace Sound {
		enum class SoundEffects{
            HOVER,
			BALL_COLLIDE,
			MENU_CLICK,
			LEVEL_SELECT
		};
	}
	
}
