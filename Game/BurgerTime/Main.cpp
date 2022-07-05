#include "MiniginPCH.h"
#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "BurgerTime.h"

int main(int, char*[]) {

	dae::BurgerTime engine;
	std::cout << "Press A and Z to play sounds (they both do one each)\n";
	engine.Run();

    return 0;
}