#include <vector>
#include <iostream>

#include <glm/glm.hpp>

export module game;

using std::vector;
using std::cout;
using std::abs;

using glm::vec3;
using glm::mat4;

namespace game {


	struct Coord3D {
		int x, y, z;
		Coord3D(int x, int y, int z) : x(x), y(y), z(z) {}
		vec3 getTranslateVec3() {
			return -vec3((float)x * 2, (float)y * 2, (float)z * 2);
		}
	};

	struct FallingShape {
		int x, y, z;
		int r1, r2, r3;
		vector<Coord3D>* shape;

		FallingShape(
			int x, int y, int z,
			int r1, int r2, int r3,
			vector<Coord3D>* shape
		) : x(x), y(y), z(z),
			r1(r1), r2(r2), r3(r3),
			shape(shape) {}

		vector<Coord3D> getCoords() {
			vector<Coord3D> ret;
			for (auto coord : *shape) {
				int x = coord.x;
				int y = coord.y;
				int z = coord.z;

				switch (abs(r1)) {
				case 1: coord.x = x; break;
				case 2: coord.x = y; break;
				case 3: coord.x = z; break;
				default:
					cout << "bad rotation: r1=" << r1 << "\n";
				}

				switch (abs(r2)) {
				case 1: coord.y = x; break;
				case 2: coord.y = y; break;
				case 3: coord.y = z; break;
				default:
					cout << "bad rotation: r2=" << r1 << "\n";
				}

				switch (abs(r3)) {
				case 1: coord.z = x; break;
				case 2: coord.z = y; break;
				case 3: coord.z = z; break;
				default:
					cout << "bad rotation: r3=" << r1 << "\n";
				}


				if (r1 < 0) {
					coord.x = -coord.x;
				}
				if (r2 < 0) {
					coord.y = -coord.y;
				}
				if (r3 < 0) {
					coord.z = -coord.z;
				}

				coord.x += this->x;
				coord.y += this->y;
				coord.z += this->z;

				ret.push_back(coord);
			}
			return ret;
		}
	};

	export class Game
	{
	private:
		void initShapes() {
			shapes[0].push_back(Coord3D(0, 0, 0));
			shapes[0].push_back(Coord3D(0, 0, 1));
			shapes[0].push_back(Coord3D(0, 0, 2));
			shapes[0].push_back(Coord3D(0, 0, 3));

			shapes[1].push_back(Coord3D(0, 0, 0));
			shapes[1].push_back(Coord3D(0, 0, 1));
			shapes[1].push_back(Coord3D(1, 0, 0));
			shapes[1].push_back(Coord3D(2, 0, 0));

			shapes[2].push_back(Coord3D(0, 0, 0));
			shapes[2].push_back(Coord3D(0, 0, 1));
			shapes[2].push_back(Coord3D(1, 0, 0));
			shapes[2].push_back(Coord3D(1, 0, 1));

			shapes[3].push_back(Coord3D(0, 0, 0));
			shapes[3].push_back(Coord3D(0, 0, 1));
			shapes[3].push_back(Coord3D(1, 0, 1));
			shapes[3].push_back(Coord3D(1, 0, 2));

			shapes[4].push_back(Coord3D(0, 0, 0));
			shapes[4].push_back(Coord3D(0, 0, 1));
			shapes[4].push_back(Coord3D(0, 0, -1));
			shapes[4].push_back(Coord3D(1, 0, 0));

			shapes[5].push_back(Coord3D(0, 0, 0));
			shapes[5].push_back(Coord3D(0, 0, 1));
			shapes[5].push_back(Coord3D(0, -1, 0));
			shapes[5].push_back(Coord3D(1, 0, 0));

			shapes[6].push_back(Coord3D(0, 0, 0));
			shapes[6].push_back(Coord3D(0, 1, 0));
			shapes[6].push_back(Coord3D(0, 1, 1));
			shapes[6].push_back(Coord3D(1, 1, 1));
		};
	public:
		int*** gameGrid;
		FallingShape* falling;
		vector<Coord3D>* shapes;
		int score;

		bool moved;
		bool pushed;

		Game() : moved(false), score(0), pushed(false) {
			shapes = new std::vector<Coord3D>[7];
			initShapes();
			falling = new FallingShape(5, 0, 5, 1, 2, 3, shapes + std::rand() % 7);
			gameGrid = new int** [10];
			for (int i = 0; i < 10; i++) {
				gameGrid[i] = new int* [30];
				for (int j = 0; j < 30; j++) {
					gameGrid[i][j] = new int[10];
					for (int k = 0; k < 10; k++) {
						gameGrid[i][j][k] = 0;
					}
				}
			}
		}
		~Game() {
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 30; j++) {
					delete[] gameGrid[i][j];
				}
				delete[] gameGrid[i];
			}
			delete falling;
			delete[] shapes;
		}

		mat4 getModelMat() {
			return glm::mat4(1.0f);
		}

		int update() {
			auto coords = falling->getCoords();
			bool flag = false;
			for (auto coord : coords) {
				if (coord.y == 29 || gameGrid[coord.x][coord.y + 1][coord.z]) {
					flag = true;
					break;
				}
			}
			if (flag) {
				for (auto coord : coords) {
					gameGrid[coord.x][coord.y][coord.z] = 1;
				}
				for (int i2 = 29; i2 >= 0; i2--) {
					bool flag3 = true;
					for (int i1 = 0; i1 < 10 && flag3; i1++)
						for (int i3 = 0; i3 < 10 && flag3; i3++)
							if (!gameGrid[i1][i2][i3])
								flag3 = false;
					if (flag3) {
						score++;
						for (int i4 = i2; i4 > 0; i4--)
							for (int i1 = 0; i1 < 10; i1++)
								for (int i3 = 0; i3 < 10; i3++)
									gameGrid[i1][i4][i3] = gameGrid[i1][i4 - 1][i3];
						for (int i1 = 0; i1 < 10; i1++)
							for (int i3 = 0; i3 < 10; i3++)
								gameGrid[i1][0][i3] = 0;
					}

				}
				falling->y = 0;
				falling->x = 5;
				falling->z = 5;
				falling->shape = shapes + std::rand() % 7;
				bool flag2 = true;
				while (flag2) {
					flag2 = false;
					for (auto coord : falling->getCoords()) {
						if (coord.y < 0) {
							flag2 = true;
							falling->y++;
							break;
						}
					}
				}
				for (auto coord : falling->getCoords()) {
					if (gameGrid[coord.x][coord.y][coord.z]) {
						return 1;
					}
				}
				return 2;
			}
			else {
				falling->y++;
			}
			return 0;
		}

		void moveFalling(int x, bool d) {
			if (moved) return;
			bool flag = true;
			for (auto coord : falling->getCoords()) {
				int movedCoord;
				if (d) {
					coord.x += x;
					movedCoord = coord.x;
				}
				else {
					coord.z += x;
					movedCoord = coord.z;
				}
				if (movedCoord < 0 ||
					movedCoord>9 ||
					gameGrid[coord.x][coord.y][coord.z]
					) {
					flag = false;
					break;
				}
			}
			if (flag) {
				if (d) {
					falling->x += x;
				}
				else {
					falling->z += x;
				}
			}
		}

		void rotateFalling(int a, bool r) {
			if (moved) return;
			int temp = 0;
			int oldr1 = falling->r1;
			int oldr2 = falling->r2;
			int oldr3 = falling->r3;
			switch (a) {
			case 1:
				temp = falling->r2;
				falling->r2 = falling->r3;
				falling->r3 = temp;
				if (r) {
					falling->r2 = -falling->r2;
				}
				else {
					falling->r3 = -falling->r3;
				}
				break;
			case 2:
				temp = falling->r3;
				falling->r3 = falling->r1;
				falling->r1 = temp;
				if (r) {
					falling->r3 = -falling->r3;
				}
				else {
					falling->r1 = -falling->r1;
				}
				break;
			case 3:
				temp = falling->r1;
				falling->r1 = falling->r2;
				falling->r2 = temp;
				if (r) {
					falling->r1 = -falling->r1;
				}
				else {
					falling->r2 = -falling->r2;
				}
				break;
			default:
				break;
			}
			bool flag = false;
			for (auto coord : falling->getCoords()) {
				if (coord.x < 0 || coord.y < 0 || coord.z < 0 ||
					coord.x > 9 || coord.y > 29 || coord.z > 9
					) {
					flag = true;
					break;
				}
				if (gameGrid[coord.x][coord.y][coord.z]) {
					flag = true;
					break;
				}
			}
			if (flag)
			{
				falling->r1 = oldr1;
				falling->r2 = oldr2;
				falling->r3 = oldr3;
			}
		}
	};
}