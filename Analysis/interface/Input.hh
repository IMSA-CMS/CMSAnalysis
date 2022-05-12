#ifndef INPUT_HH
#define INPUT_HH

#include <string>

class TH1;

class Input
{
	public:
		virtual TH1* getInput(std::string name) const = 0;
};

#endif