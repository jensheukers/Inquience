// Main entry point.
#include "../engine/core.h"
#include "../engine/input.h"

//Define default tilemap scale
#define DEFAULT_TILEMAP_TILE_SCALE 32 

int main(int argc, char * argv[]) {
	Core::Initialize(argc, argv);

	while (Core::IsActive()) {
		Core::Update(); // Handle updates for engine
	}

	return 0; // return succes
}