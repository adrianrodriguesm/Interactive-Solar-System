#include "Mesh.h"
#include "Camera.h"
#include "State.h"

using namespace EngineMesh;
using namespace EngineCamera;
using namespace engineState;


namespace EngineSceneNode {
	struct SceneNode {

	private :

		mat4 localMatrix;
		mat4 worldMatrix;
		std::vector<SceneNode*> childNodes;
		SceneNode* parent;
		Shader* shader;
		Mesh* mesh;
		vec4 color;
		State stateI;
		State state;
		State stateF;
		mat4 scale;
	

	public :

		SceneNode();
		SceneNode* createNode();
		
		void draw();
		void switchState();
		void playAnimation(float Duration, float curr_time);

		///Setters
		void setMesh(Mesh* mesh);
		void setParent(SceneNode* parent);
		void setLocalMatrix(const mat4& m);
		void setShaderProgram(Shader* s);
		void setColor(const vec4& color);
		
		void setTrans(const mat4& m);
		void setRot(const qtrn& q);
		void setScale(const mat4& s);
		void setFinalState(State& state);
		void setInitialState(State& state);
		void updateTrans(const mat4& m);

		Shader* getShader();


		
		




	};

}