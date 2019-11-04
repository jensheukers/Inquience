// Source file for User Interface Elements.
//
// Version: 2/7/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, July 2019
#include "ui.h"

//Include input for position checks, and physics for checks
#include "../input.h"
#include "../math/physics.h"

#include "../entity.h"

#include "../luascript.h"

UIComponent::UIComponent() {
	std::function<void(Delegate& d, StringVector v)> AddDelegateLambda = 
		[](Delegate& d, StringVector v) { {
			d.AddLambda([=]() {
				std::string fileName = v[0];
				std::string funcName = v[1];

				StringVector a;
				for (size_t i = 2; i < v.size(); i++) {
					a.push_back(v[i]);
				}

				LuaScript::RunFunction(fileName, funcName, a);
			});
		}
	};

	this->AddProperty("OnEnter", [=](StringVector v) {
		onEnterReturnVector = v;
		AddDelegateLambda(this->OnEnterDelegate, v);
	}, [&]() -> StringVector {
		StringVector v;
		for (size_t i = 0; i < onEnterReturnVector.size(); i++) {
			v.push_back("\"" + onEnterReturnVector[i] + "\"");
		}
		return v;
	});

	this->AddProperty("OnStay", [=](StringVector v) {
		onStayReturnVector = v;
		AddDelegateLambda(this->OnStayDelegate, v);
	}, [&]() -> StringVector {
		StringVector v;
		for (size_t i = 0; i < onStayReturnVector.size(); i++) {
			v.push_back("\"" + onStayReturnVector[i] + "\"");
		}
		return v;
	});

	this->AddProperty("OnLeave", [=](StringVector v) {
		onLeaveReturnVector = v;
		AddDelegateLambda(this->OnLeaveDelegate, v);
	}, [&]() -> StringVector {
		StringVector v;
		for (size_t i = 0; i < onLeaveReturnVector.size(); i++) {
			v.push_back("\"" + onLeaveReturnVector[i] + "\"");
		}
		return v;
	});
}

void UIComponent::Update() {
	if (!GetOwner()->HasComponent<Sprite>()) return;
	if (!GetOwner()->GetComponent<Sprite>()->GetTexture()) return;

	if (Physics::InBounds(Input::GetMousePosition(), GetOwner()->GetPosition(), GetOwner()->GetPosition() + GetOwner()->GetScale())) {
		if (!entered) {
			OnEnter();
			entered = true;
		}
		else {
			OnStay();
		}
	}
	else {
		if (entered) {
			OnLeave();
			entered = false;
		}
	}
}