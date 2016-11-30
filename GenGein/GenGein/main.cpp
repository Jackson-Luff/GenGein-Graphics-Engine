#include <vld.h>
#include "Projects\Test\TestScene.h"

int main(int argc, char** argv[])
{
	TestScene* test = new TestScene();
	test->StartUp(1600, 900, "Test");
	test->Run();
	delete test;
	return 0;
}