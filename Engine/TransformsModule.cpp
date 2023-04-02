#include "pch.h"

#include "TransformsModule.h"

#include "Logger.h"
#include "TransformComp.h"

using namespace BlahEngine;

TransformsModule::TransformsModule():
	_ecs(nullptr),
	_isSortingRequired(false)
{
	
}

TransformsModule::~TransformsModule()
{
	Logger::Debug("transforms module", "destroyed");
}

void TransformsModule::Init(entt::registry* ecs)
{
	_ecs = ecs;

	_ecs->on_construct<TransformComp>().connect<&TransformsModule::OnTransformAddedOrRemoved>(this);
	_ecs->on_destroy<TransformComp>().connect<&TransformsModule::OnTransformAddedOrRemoved>(this);

	Logger::Debug("transforms module", "initialized");
}

void TransformsModule::AdjustGlobalPositions()
{
	auto view = _ecs->view<TransformComp>();
	for (auto ent : view)
	{
		auto& tf = _ecs->get<TransformComp>(ent);
		if (tf.Parent != tf.PrevParent)
		{
			tf.PrevParent = tf.Parent;
			_isSortingRequired = true;
		}
	}

	if (_isSortingRequired)
	{
		auto capEcs = _ecs;
		_ecs->sort<TransformComp>([capEcs](const entt::entity a, const entt::entity b)
			{
				return a == capEcs->get<TransformComp>(b).Parent;
			});
		_isSortingRequired = false;
	}

	view = _ecs->view<TransformComp>();
	for (auto ent : view)
	{
		auto& tf = view.get<TransformComp>(ent);
		if (tf.Parent != entt::null)
			tf.GlobalPos = view.get<TransformComp>(tf.Parent).GlobalPos + tf.Pos;
		else
			tf.GlobalPos = tf.Pos;
	}
}

void TransformsModule::ResetTransformsIsChanged()
{
	auto view = _ecs->view<TransformComp>();
	for (auto ent : view)
	{
		auto& tf = view.get<TransformComp>(ent);
		tf.PrevPos = tf.Pos;
		tf.PrevRot = tf.Rot;
		tf.PrevScale = tf.Scale;
		tf.PrevGlobalPos = tf.GlobalPos;
	}
}

void TransformsModule::OnTransformAddedOrRemoved(entt::registry& reg, entt::entity ent)
{
	_isSortingRequired = true;
}
