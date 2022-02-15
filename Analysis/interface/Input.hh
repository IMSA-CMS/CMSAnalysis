#ifndef INPUT_HH
#define INPUT_HH

#include <string>

class TH1;

class Input
{
	public:
		virtual TH1* GetInput(std::string name) = 0;
};

#endif