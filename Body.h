#pragma once
#include "Cell.h"

Food food[50];
vector<identify> enemyLocation;
vector<identify> foodLocation;
vector<bodyCertification> bodyInform;

class Body : public Cell {
private:
	bool weapon;
	bool tail;
	float range;

	//weapon animation
	float partx;
	float party;
	float partz;
	float shotValue;
	bool fire;

	//tail animation
	float part1x;
	float part1y;
	float part1z;
	float part2x;
	float part2y;
	float part2z;
	bool go_back;
	float tailValue;

	/*
	1: up
	2: down
	3: left
	4: right
	5: back
	*/
	int bodyLeaf[6];
	bool isConnect;
public:
	int core_id;
	bool color;

	Body() {
		isConnect = false;
		weapon = false;
		tail = false;
		
		memset(bodyLeaf, 0, sizeof(bodyLeaf));

		go_back = true;
		tailValue = 0.001f;

		fire = false;
		shotValue = 0.0f;
		range = 0;
	}

	void initializeBody() {
		initializeCell();

		isConnect = false;
		weapon = false;
		tail = false;
		
		memset(bodyLeaf, 0, sizeof(bodyLeaf));

		go_back = true;
		tailValue = 0.001f;

		fire = false;
		shotValue = 0.0f;
		range = 0;
	}

	void set_weapon() {
		weapon = true;
	}

	void set_tail() {
		tail = true;
	}

	bool is_weapon() {
		return weapon;
	}

	bool is_tail() {
		return tail;
	}

	void set_isConnect(bool connect) {
		isConnect = connect;
	}

	bool get_isConnect() {
		return isConnect;
	}

	CollisionRay attackRay() {
		CollisionRay ray;
		if (weapon) {
			ray.origin = XMVectorSet(x, y, z, 1);
			if (rotate_x == 0)
				ray.direction = XMVectorSet(-sin(rotate), 0, -cos(rotate), 1.0f);
			else
				ray.direction = XMVectorSet(0, sin(rotate_x), 0, 1.0f);
		}
		return ray;
	}

	void insertBody(int pos, int bodyIndex) {
		if (!isConnect) return;
		if (weapon || tail) return;
		if (bodyLeaf[pos] != 0) return;

		//if (80 <= bodyIndex && bodyIndex <100)
		//{
		//	//core_id
		//	enemyLocation[core_id].hp += 5;

		//	/*
		//	if (core_id == 10)
		//	{
		//	string s = to_string(enemyLocation[core_id].hp);
		//	MessageBoxA(nullptr, s.c_str(), "", MB_OK);
		//	}*/
		//}
		//else if (!(bodyInform[bodyIndex].is_Tail || bodyInform[bodyIndex].is_weapone))
		//{
		//	enemyLocation[core_id].hp += 2;
		//}

		bodyLeaf[pos] = bodyIndex;

		//6.20 À±½ÂÈñ Ãß°¡ 
		bodyInform[bodyIndex].id_own = bodyIndex;
		bodyInform[bodyIndex].core_id = core_id;
		bodyInform[bodyIndex].parantBodyId = id;
		bodyInform[id].Leaf[pos] = bodyIndex;
	}

	void detachBody(int pos) {
		bodyInform[bodyLeaf[pos]].core_id = -1;
		bodyInform[bodyLeaf[pos]].parantBodyId = -1;
		bodyLeaf[pos] = 0;

	}

	void detachAllBodies() {
		for (int i = 0; i < 6; i++)
		{
			bodyInform[bodyLeaf[i]].core_id = -1;
			bodyInform[bodyLeaf[i]].parantBodyId = -1;
			bodyLeaf[i] = 0;
		}
	}

	int get_Index(int pos) {
		return bodyLeaf[pos];
	}


	// tail animation
	float getPart1x() {
		return part1x;
	}
	float getPart1y() {
		return part1y;
	}
	float getPart1z() {
		return part1z;
	}
	float getPart2x() {
		return part2x;
	}
	float getPart2y() {
		return part2y;
	}
	float getPart2z() {
		return part2z;
	}
	void MoveTailvalue() {	
		if (go_back) {
			part1x = tailValue * sin(rotate);
			part1y = 0;
			part1z = tailValue * cos(rotate);

			part2x = 1.8 * tailValue * sin(rotate);
			part2y = 0;
			part2z = 1.8 * tailValue * cos(rotate);

			tailValue += 0.0008f;

			if (tailValue > 0.9) {
				go_back = false;
			}
		}
		else {
			part1x = tailValue * sin(rotate);
			part1y = 0;
			part1z = tailValue * cos(rotate);

			part2x = 1.8 * tailValue * sin(rotate);
			part2y = 0;
			part2z = 1.8 * tailValue * cos(rotate);

			tailValue -= 0.0008f;

			if (tailValue < 0.1) {
				go_back = true;
			}
		}
	}


	// weapon animation
	bool isFire() {
		return fire;
	}
	void setFire() {
		fire = true;
	}
	void set_range(float r) {
		range = r;
	}

	float getPartx() {
		return partx;
	}
	float getParty() {
		return party;
	}
	float getPartz() {
		return partz;
	}

	void moveWeaponValue() {
		partx = - shotValue * sin(rotate);
		party = 0;
		partz = - shotValue * cos(rotate);

		if (rotate_x != 0.0f) {
			partx = 0;
			party = shotValue * sin(rotate_x);
			partz = 0;
		}

		shotValue += 0.1f;
		if (shotValue > range) {
			fire = false;
			shotValue = 0.0f;
		}
	}

};

