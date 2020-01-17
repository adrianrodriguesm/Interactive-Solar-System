#include <iostream>
#include <fstream>

#include "Loader.h"

Loader::Loader()
{
}



void Loader::updateState(SceneGraph* scene, float camDistance, qtrn rot, mat4 camTargetRot, mat4 camTargetTrans, int stencilId)
{

	file.open(".\\..\\..\\FILE\\State.txt", ios::out);

	// Writing on file
	file << "View" << endl;
	file << scene->getCamera()->ViewMatrix << endl;


	// Writing on file
	file << "Projection" << endl;
	file << scene->getCamera()->ProjectionMatrix << endl;

	// Reding from file
	//file >> text;
	scene->getRoot()->writeNodeInFile(file, Matrices);

	file << "CamDist" << endl;
	file << camDistance << endl;

	file << "QtrnRot" << endl;
	file << rot.t << endl;
	file << rot.x << endl;
	file << rot.y << endl;
	file << rot.z << endl;

	file << "camTargetRot" << endl;
	file << camTargetRot << endl;

	file << "camTargetTrans" << endl;
	file << camTargetTrans << endl;

	file << "stencilId" << endl;
	file << stencilId << endl;


	//closing the file
	file.close();
}

void Loader::readState()
{

	file.open(".\\..\\..\\FILE\\State.txt", ios::in);
	std::string text;
	int i = 0;
	while (std::getline(file, text))
	{
		if (text == ("View")) {
			loadMatrices(text);
		}
		else if (text == ("Projection")) {
			loadMatrices(text);
		}
		else if (text == ("base")) {
			loadMatrices(text);
		}
		else if (text == ("skyboxNode")) {
			loadMatrices(text);
		}
		else if (text == ("sunNode")) {
			loadMatrices(text);
		}
		else if (text == ("earthNode")) {
			loadMatrices(text);
		}
		else if (text == ("mercuryNode")) {
			loadMatrices(text);
		}
		else if (text == ("venusNode")) {
			loadMatrices(text);
		}
		else if (text == ("marsNode")) {
			loadMatrices(text);
		}
		else if (text == ("jupiterNode")) {
			loadMatrices(text);
		}
		else if (text == ("saturnNode")) {
			loadMatrices(text);
		}
		/*else if (text == ("saturnRingNode")) {
			loadMatrices(text);
		}*/
		else if (text == ("uranusNode")) {
			loadMatrices(text);
		}
		else if (text == ("neptuneNode")) {
			loadMatrices(text);
		}
		else if (text == ("lensFlare")) {
			loadMatrices(text);
		}
		else if (text == ("QtrnRot")) {
			rot = qtrn(1, 0, 0, 0);
			std::getline(file, text);
			float t = strtof((text).c_str(), 0);
			std::getline(file, text);
			float x = strtof((text).c_str(), 0);
			std::getline(file, text);
			float y = strtof((text).c_str(), 0);
			std::getline(file, text);
			float z = strtof((text).c_str(), 0);

			rot = qtrn(t, x, y, z);
		}
		else if (text == ("CamDist")) {
			std::getline(file, text);
			camDist = strtof((text).c_str(), 0);

		}
		else if (text == ("camTargetRot")) {
			loadMatrices(text);
		}
		else if (text == ("camTargetTrans")) {
			loadMatrices(text);
		}
		else if (text == ("stencilId")) {
			std::getline(file, text);
			stencilId = strtof((text).c_str(), 0);
		}


	}
	//closing the file
	file.close();



}
float* Loader::insertMatrixValues(std::string text, float data[16], int& index)
{
	stringstream ss(text); // Turn the string into a stream.
	string tok;
	float val = 0.0f;
	while (getline(ss, tok, ' ')) {
		val = strtof((tok).c_str(), 0);
		data[index] = val;
		index++;
	}

	return data;
}


void Loader::loadMatrices(std::string name)
{

	int count = 0;
	float data[16];
	std::string text;
	while (std::getline(file, text) && count < 16) {
		insertMatrixValues(text, data, count);

	}
	mat4 matrix = mat4(data[0], data[1], data[2], data[3],
		data[4], data[5], data[6], data[7],
		data[8], data[9], data[10], data[11],
		data[12], data[13], data[14], data[15]);

	Matrices.insert(std::pair< std::string, mat4>(name, matrix));
}

bool Loader::is_empty()
{
	file.open(".\\..\\..\\FILE\\State.txt", ios::in);
	bool result = (file.peek() == std::ifstream::traits_type::eof());
	file.close();
	return result;
}


