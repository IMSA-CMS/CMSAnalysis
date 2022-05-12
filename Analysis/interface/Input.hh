#ifndef INPUT_HH
#define INPUT_HH

#include <string>

class TH1;

class Input
{
	public:
		virtual TH1* getInput(std::string name) const = 0;
<<<<<<< HEAD
=======
		virtual ~Input() {}
>>>>>>> ded437d466687e45b887622f5bbcba68c17918c1
};

#endif