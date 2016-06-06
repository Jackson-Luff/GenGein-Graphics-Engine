#include <vld.h>
#include "../Projects/Test/TesterOfBaseApp.h"

int main(int argc, char** argv[])
{
	TesterOfBaseApp* test = new TesterOfBaseApp();
	test->StartUp(1280, 720, "Test");
	test->Run();
	delete test;
	return 0;
}