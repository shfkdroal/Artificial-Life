#pragma once
#include "Enemy.h"

Enemy enemy[10];

class Core : public Cell {
private:
	float l_speed;
	float r_speed;
	float speed;
	float b_speed;
	float weight;

	vector<int> collisionBody;
	vector<int> weapon;
	/*
	1: up
	2: down
	3: left
	4: right
	5: back
	*/
	int Leaf[6];

public:
	Core() {
		memset(Leaf, 0, sizeof(Leaf));
	}

	void initializeCore() {
		initializeCell();

		memset(Leaf, 0, sizeof(Leaf));
	}

	
	// 공격처리 관련함수
	//------------------------------------------------------------------------------------

	void travelLeaf(int bodyIndex) {
		if (body[bodyIndex].is_weapon()) {
			weapon.push_back(bodyIndex);
		}
		for (int i = 1; i < 6; i++) {
			if (body[bodyIndex].get_Index(i) == 0) continue;	
			travelLeaf(body[bodyIndex].get_Index(i));
		}
	}

	void attack() {
		CollisionRay ray;
		float fDist, minDistance;
		int index;

		for (int i = 1; i < 6; i++) {
			if (Leaf[i] == 0) continue;
			travelLeaf(Leaf[i]);
		}

		for (int i = 0; i < weapon.size(); i++) {
			isEffectRunning = true;//이펙트 소리 적용 플래그
			minDistance = 10;
			index = -1;
			body[weapon[i]].set_range(minDistance-1);

			// 적공격판정
			for (int n = 0; n < 10; n++) {
				if (enemy[n].get_isExist() && (abs(x - enemy[n].get_x())+ abs(z - enemy[n].get_z()) < 15 )) {
					ray = body[weapon[i]].attackRay();
					if (enemy[n].get_collisionBox()->obox.Intersects(ray.origin, ray.direction, fDist)) {
						if (fDist < 10) {
							if (minDistance > fDist) {
								minDistance = fDist;
								body[weapon[i]].set_range(minDistance);
								index = n + 200;
							}
						}
					}
				}
			}
			//body공격판정
			for (int n = 1; n < 200; n++) {
				if (body[n].get_isExist() && (abs(x - body[n].get_x()) + abs(z - body[n].get_z()) < 15) && (weapon[i] != n)) {
					ray = body[weapon[i]].attackRay();
					if (body[n].get_collisionBox()->obox.Intersects(ray.origin, ray.direction, fDist)) {
						if (fDist < 10) {
							if (minDistance > fDist) {
								minDistance = fDist;
								body[weapon[i]].set_range(minDistance);
								index = n;
							}
						}
					}
				}
			}
			body[weapon[i]].setFire();
			if (index > 0) {
				if (index >= 200) {
					enemy[index - 200].decreaseHP(2);
					enemyLocation[index - 200].decreseHP(2);  //6.20 윤승희 추가 
					if (enemy[index - 200].get_HP() <= 0) {
						enemyLocation[index - 200].Exist = false;
						enemy[index - 200].detachBody(1);
						enemy[index - 200].detachBody(2);
						enemy[index - 200].detachBody(3);
						enemy[index - 200].detachBody(4);
						enemy[index - 200].detachBody(5);
					}
				}
				else {
					body[index].decreaseHP(2);
				}
			}

		}
		weapon.clear();
	}


	// 이동 관련함수
	//------------------------------------------------------------------------------------

	void turn_left() {
		setCollisionBox();
		setLeaves();
		if (Collide()) {
			rotate -= 0.0001;
			return;
		}
		rotate -= l_speed / weight;
	}

	void turn_right() {
		setCollisionBox();
		setLeaves();
		if (Collide()) {
			rotate += 0.0001;
			return;
		}
		rotate += r_speed / weight;		
	}

	void go_forward() {
		setCollisionBox();
		setLeaves();
		if (Collide()) {
			x += 0.0002f * sin(rotate);
			z += 0.0002f * cos(rotate);
			return;
		}
		x += speed * sin(rotate) / weight;
		z += speed * cos(rotate) / weight;
	}

	void go_backward() {		
		setCollisionBox();
		setLeaves();
		if (Collide()) {
			x -= 0.0002f * sin(rotate);
			z -= 0.0002f * cos(rotate);
			return;
		}
		x -= b_speed * sin(rotate) / weight;
		z -= b_speed * cos(rotate) / weight;
	}


	// 블록분리 관련함수
	//------------------------------------------------------------------------------------

	void detachBody(int pos) {
		if (Leaf[pos] == 0) return;
		setDetachLeaves(Leaf[pos], pos, x, y, z, rotate);
		Leaf[pos] = 0;
	}

	void detachBodyBody(int bodyIndex, int pos) {
		if (body[bodyIndex].get_Index(pos) == 0) return;
		setDetachLeaves(body[bodyIndex].get_Index(pos), pos, body[bodyIndex].get_x(), body[bodyIndex].get_y(), body[bodyIndex].get_z(), body[bodyIndex].get_rotate());
		body[bodyIndex].detachBody(pos);
	}

	void setDetachLeaves(int bodyIndex, int pos, float _x, float _y, float _z, float _rotate) {

		switch (pos) {
		case 1:
			body[bodyIndex].setWorldCoord(_x, _y + 2.5f, _z);
			break;
		case 2:
			body[bodyIndex].setWorldCoord(_x, _y - 2.5f, _z);
			break;
		case 3:
			body[bodyIndex].setWorldCoord(_x - 2.5f * cos(_rotate), _y, _z + 2.5f * sin(_rotate));
			break;
		case 4:
			body[bodyIndex].setWorldCoord(_x + 2.5f * cos(_rotate), _y, _z - 2.5f * sin(_rotate));
			break;
		case 5:
			body[bodyIndex].setWorldCoord(_x - 2.5f * sin(_rotate), _y, _z - 2.5f * cos(_rotate));
			break;
		}
		body[bodyIndex].setCollisionBox();
		body[bodyIndex].set_isConnect(false);

		for (int i = 1; i < 6; i++) {
			if (body[bodyIndex].get_Index(i) == 0) continue;
			setDetachLeaves(body[bodyIndex].get_Index(i), i, body[bodyIndex].get_x(), body[bodyIndex].get_y(), body[bodyIndex].get_z(), body[bodyIndex].get_rotate());
		}
		body[bodyIndex].detachAllBodies();
	}


	// 블록결합 관련함수
	//------------------------------------------------------------------------------------

	void setLeaves() {
		r_speed = 0.005f;
		l_speed = 0.005f;
		speed = 0.02f;
		b_speed = 0.02f;
		weight = 1.0f;

		for (int i = 1; i < 6; i++) {
			if (Leaf[i] == 0) continue;
			if (!body[Leaf[i]].get_isExist()) {
				detachBody(i);
				continue;
			}
			setAttachLeaves(Leaf[i], i, x, y, z, rotate);
		}
	}

	void insertBody(int pos, int bodyIndex) {
		if (Leaf[pos] != 0) return;
		if (body[bodyIndex].get_isConnect()) return;

		/*if (body[bodyIndex].color)
		{
			HP += 5;
		}
		else if (!(body[bodyIndex].is_tail() || body[bodyIndex].is_weapon()))
		{
			HP += 2;
		}*/

		Leaf[pos] = bodyIndex;
		body[bodyIndex].core_id = id;
		bodyInform[bodyIndex].core_id = id; 		//bodyInform에 관한 처리. (코어 id와 parent id를 기록)
		bodyInform[bodyIndex].parantBodyId = id;
		setLeaves();
	}

	void setAttachLeaves(int bodyIndex, int pos, float _x, float _y, float _z, float _rotate) {
		switch (pos) {
		case 1:
			body[bodyIndex].setWorldCoord(_x, _y + 2, _z);
			body[bodyIndex].setRotate(_rotate);
			body[bodyIndex].setRotateX(XM_PI / 2);
			break;
		case 2:
			body[bodyIndex].setWorldCoord(_x, _y - 2, _z);
			body[bodyIndex].setRotate(_rotate);
			body[bodyIndex].setRotateX(-XM_PI / 2);
			break;
		case 3:
			body[bodyIndex].setWorldCoord(_x - 2 * cos(_rotate), _y, _z + 2 * sin(_rotate));
			body[bodyIndex].setRotate(_rotate + XM_PI / 2);
			body[bodyIndex].setRotateX(0);			
			break;
		case 4:
			body[bodyIndex].setWorldCoord(_x + 2 * cos(_rotate), _y, _z - 2 * sin(_rotate));
			body[bodyIndex].setRotate(_rotate - XM_PI / 2);
			body[bodyIndex].setRotateX(0);
			break;
		case 5:
			body[bodyIndex].setWorldCoord(_x - 2 * sin(_rotate), _y, _z - 2 * cos(_rotate));
			body[bodyIndex].setRotate(_rotate);
			body[bodyIndex].setRotateX(0);
			
			break;
		}
		body[bodyIndex].setCollisionBox();
		body[bodyIndex].set_isConnect(true);
		collisionBody.push_back(bodyIndex);

		weight += 0.1f;
		if (body[bodyIndex].is_tail()) {
			float boost = rotate - body[bodyIndex].get_rotate();
			float cos_boost = cos(boost);
			float sin_boost = sin(boost);

			if (cos_boost > 0) {
				speed += cos_boost * 0.003f;
			}
			else {
				b_speed += -cos_boost * 0.003f;
			}

			if (sin_boost > 0) {
				l_speed += sin_boost * 0.002f;
			}
			else {
				r_speed += -sin_boost * 0.002f;
			}	
		}

		for (int i = 1; i < 6; i++) {
			if (body[bodyIndex].get_Index(i) == 0) continue;
			if (!body[body[bodyIndex].get_Index(i)].get_isExist()) {
				detachBodyBody(bodyIndex, i);
				continue;
			}
			setAttachLeaves(body[bodyIndex].get_Index(i), i, body[bodyIndex].get_x(), body[bodyIndex].get_y(), body[bodyIndex].get_z(), body[bodyIndex].get_rotate());
		}
	}

	// 충돌 관련함수
	//------------------------------------------------------------------------------------

	bool Collide() {
		for (int i = 0; i < 10; i++) {
			if (enemy[i].get_isExist() && (abs(x - enemy[i].get_x()) + abs(z - enemy[i].get_z()) < 15)) {
				if (enemy[i].get_collisionBox()->obox.Contains(collisionBox.obox)) {
					collisionBody.clear();
					return true;
				}
			}
		}
		for (int i = 0; i < collisionBody.size(); i++) {
			for (int n = 0; n < 10; n++) {	
				if (enemy[n].get_isExist() && (abs(body[collisionBody[i]].get_x() - enemy[n].get_x()) + abs(body[collisionBody[i]].get_z() - enemy[n].get_z()) < 15)) {
					if (body[collisionBody[i]].get_collisionBox()->obox.Contains(enemy[n].get_collisionBox()->obox)) {
						collisionBody.clear();
						return true;
					}
				}
			}
		}
		for (int i = 0; i < collisionBody.size(); i++) {
			for (int n = 0; n < 200; n++) {
				if (body[n].get_isExist() && collisionBody[i] != n && (abs(body[collisionBody[i]].get_x() - body[n].get_x()) + abs(body[collisionBody[i]].get_z() - body[n].get_z()) < 15)) {
					if (body[collisionBody[i]].get_collisionBox()->obox.Contains(body[n].get_collisionBox()->obox)) {
						collisionBody.clear();
						return true;
					}
				}
			}
		}
		for (int n = 0; n < 200; n++) {
			if (body[n].get_isExist() && (abs(x - body[n].get_x()) + abs(z - body[n].get_z()) < 15)) {
				if (collisionBox.obox.Contains(body[n].get_collisionBox()->obox)) {
					collisionBody.clear();
					return true;
				}
			}
		}
		collisionBody.clear();
		return false;
	}

	//먹이와 코어간, 맵 경계간 충돌방치 관련 함수 
	//___________________________________________________________________________________________________________
	void interact_core_food()
	{
		for (int i = 1; i < foodLocation.size(); i++)
		{
			if (foodLocation[i].Exist && 10 >= HP + 2)
			{
				if (abs(foodLocation[i].x - x) < 3 && abs(foodLocation[i].z - z) < 6)
				{
					foodLocation[i].Exist = false;
					food[i].modif_Exist(false);
					HP += 2;
				}
			}
		}
	}

};