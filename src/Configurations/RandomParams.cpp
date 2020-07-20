#include "RandomParams.h"

#include "Helper/Random.h"

RandomParams::RandomParams() 
	: seed(10.0f * MyRand::_m1to1()),
	  xySeed(glm::vec2(
		   10.0f * MyRand::_m1to1(),
		   10.0f * MyRand::_m1to1()
	  ))
{}