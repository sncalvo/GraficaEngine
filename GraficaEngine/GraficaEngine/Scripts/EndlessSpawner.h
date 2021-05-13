#pragma once

#include <string>
#include <map>
#include <vector>

#include "../Core/Behaviour.h"
#include "../Core/GameObject.h"
#include "../Renderer/Shader.h"
#include "../Utils/CircularBuffer.h"

constexpr float FIRST_ROW = -18.f;
constexpr float SPACE_BETWEEN_ROWS = 3.f;
constexpr float RADIUS_TO_PLAYER = 15.f;

using Environment = Engine::GameObject *;
using Obstacles = std::vector<Engine::GameObject *>;

class EndlessSpawner : public Engine::Behaviour
{
private:
	std::vector<Environment> _environments;
	CircularBuffer<Engine::GameObject*> *_rows;
	int _currentRow;
	Engine::Scene* _scene = nullptr;
	Engine::BaseGameObject* _player = nullptr;
	float _getZCoordinateRow() const;
	bool _nearPlayer() const;
public:
	EndlessSpawner(std::vector<Environment> environments);
	~EndlessSpawner();
	void update();
};
