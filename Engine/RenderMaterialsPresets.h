#pragma once

#include "RenderComp.h"

namespace BlahEngine
{
	static const RenderComp::Material MatPresetBlackPlastic
	{
		{0.01, 0.01, 0.01},
		{0.0, 0.0, 0.0},
		{0.5, 0.5, 0.5},
		32
	};
	static const RenderComp::Material MatPresetBrass
	{
		{0.780392, 0.568627, 0.113725},
		{ 0.329412, 0.223529, 0.027451 },
		{0.992157, 0.941176, 0.807843},
		27.8974
	};
	static const RenderComp::Material MatPresetPolishedSilver
	{
		{0.2775, 0.2775, 0.2775},
		{0.23125, 0.23125, 0.23125},
		{0.773911, 0.773911, 0.773911},
		89.6
	};
	static const RenderComp::Material MatPresetChrome
	{
		{0.4, 0.4, 0.4},
		{0.25, 0.25, 0.25},
		{0.774597, 0.774597, 0.774597},
		76.8
	};
	static const RenderComp::Material MatPresetBlackRubber
	{
		{0.01, 0.01, 0.01},
		{0.02, 0.02, 0.02},
		{0.4, 0.4, 0.4},
		10.0
	};
	static const RenderComp::Material MatPreset
	{
		{0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0},
		{0.0, 0.0, 0.0},
		0.0
	};
}