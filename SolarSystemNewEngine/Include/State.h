#include "Quartenion.h"


struct State
{
	State() = default;
	State(mat4 pos, qtrn rot) {
		this->pos = pos;
		this->rot = rot;
	}
	mat4 pos = matFactory::createIdentityMat4();
	qtrn rot = qtrn(1.0f,0.0f,0.0f,0.0f);
};
