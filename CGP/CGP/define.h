#pragma once
#define SINGLE(type) public: \
						 static type* GetInst()\
						{ \
							static type mgr;\
							 return &mgr;\
						 } \
						private: \
							type(); \
							~type();

#define MAPSIZE 15



enum class DIR
{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT
};

enum class GROUP_TYPE
{
	DEFAULT, // 0
	WALL,
	PLAYER,  
	ENEMY,
	BOMB,
	FIRE,

	END = 32,

};


enum class EFFECT_TYPE
{
	FIRE,
	FOG,
	WATER,

	END = 32
};

enum class PARTICLE_TYPE
{
	DEFAULT = 0,

	END = 32
};

enum class SCENE_TYPE
{
	TOOL,
	START,
	STAGE_01,
	STAGE_02,
	GAME_OVER,
	GAME_CLEAR,
	END,
};

enum class EVENT_TYPE
{
	CREATE_OBJECT,
	DELETE_OBJECT,
	SCENE_CHANGE,
	DEAD_OBJECT,
	DEAD_PLAYER,

	END,
};

enum class GAME_STATE
{
	GAME_PLAY = 0,
	GAME_OVER,
	GAME_CLEAR	
};