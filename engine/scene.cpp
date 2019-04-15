// Source file for scene class.
//
// Version: 3/4/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, March 2019
#include "scene.h"

void Scene::Update() {
	this->UpdateChildren(); 
}