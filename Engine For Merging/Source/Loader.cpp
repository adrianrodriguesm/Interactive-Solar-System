#include <iostream>
#include <fstream>

#include "Loader.h"

Loader::Loader(SceneGraph* scene)
{
	this->scene = scene;
}

void Loader::updateState()
{
	char text[200];
	
	file.open(".\\..\\..\\FILE\\State.txt", ios::out | ios::in);
	

	//cout << "Camera:" << endl;
	//cin.getline(text, sizeof(text));
	

	// Writing on file
	file <<  "View Matrix:" << endl;
	file << scene->getCamera()->ViewMatrix << endl;


	// Writing on file
	file << "Projection Matrix:" << endl;
	file << scene->getCamera()->ProjectionMatrix << endl;

	// Reding from file
	file >> text;
	cout << text << endl;

	//closing the file
	file.close();
}

void Loader::readState()
{
	file.open(".\\..\\..\\FILE\\State.txt", ios::in);

}


