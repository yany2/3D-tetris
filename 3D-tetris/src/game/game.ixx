#include <glm.hpp>
#include <vector>

export module game;

struct Coord3D
{
	int x, y, z;
	Coord3D(int x, int y, int z);
	glm::vec3 getTranslateVec3();
};

struct FallingShape
{
	int x, y, z;
	int r1, r2, r3;
	std::vector<Coord3D>* shape;

	FallingShape(int x, int y, int z, int r1, int r2, int r3, std::vector<Coord3D>* shape);
	std::vector<Coord3D> getCoords();
};

export class Game
{
private:
	void initShapes();
public:
	int*** gameGrid;
	FallingShape* falling;
	std::vector<Coord3D>* shapes;
	int score;

	bool moved;
	bool pushed;

	Game();
	~Game();

	glm::mat4 getModelMat();
	int update();
	void moveFalling(int x, bool d);
	void rotateFalling(int a, bool r);
};