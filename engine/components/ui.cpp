// Source file for User Interface Elements.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, April 2020
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
	// Add Properties (Collapse)
	{
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

		this->AddProperty("OnLeftClick", [=](StringVector v) {
			onLeftClickReturnVector = v;
			AddDelegateLambda(this->OnLeftClickDelegate, v);
		}, [&]() -> StringVector {
			StringVector v;
			for (size_t i = 0; i < onLeftClickReturnVector.size(); i++) {
				v.push_back("\"" + onLeftClickReturnVector[i] + "\"");
			}
			return v;
		});

		this->AddProperty("OnMiddleClick", [=](StringVector v) {
			onMiddleClickReturnVector = v;
			AddDelegateLambda(this->OnMiddleClickDelegate, v);
		}, [&]() -> StringVector {
			StringVector v;
			for (size_t i = 0; i < onMiddleClickReturnVector.size(); i++) {
				v.push_back("\"" + onMiddleClickReturnVector[i] + "\"");
			}
			return v;
		});

		this->AddProperty("OnRightClick", [=](StringVector v) {
			onRightClickReturnVector = v;
			AddDelegateLambda(this->OnRightClickDelegate, v);
		}, [&]() -> StringVector {
			StringVector v;
			for (size_t i = 0; i < onRightClickReturnVector.size(); i++) {
				v.push_back("\"" + onRightClickReturnVector[i] + "\"");
			}
			return v;
		});
	}
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

			if (Input::GetButtonDown(BUTTONCODE_LEFT)) OnClick(BUTTONCODE_LEFT);
			if (Input::GetButtonDown(BUTTONCODE_MIDDLE)) OnClick(BUTTONCODE_MIDDLE);
			if (Input::GetButtonDown(BUTTONCODE_RIGHT)) OnClick(BUTTONCODE_RIGHT);
		}
	}
	else {
		if (entered) {
			OnLeave();
			entered = false;
		}
	}
}

void UIComponent::OnClick(int mouseIndex) {
	switch (mouseIndex)
	{
	case BUTTONCODE_LEFT:
		OnLeftClickDelegate.Execute();
		break;
	case BUTTONCODE_MIDDLE:
		OnMiddleClickDelegate.Execute();
		break;
	case BUTTONCODE_RIGHT:
		OnRightClickDelegate.Execute();
		break;
	default:
		break;
	}
}