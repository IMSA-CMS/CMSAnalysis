#ifndef INPUT_HH
#define INPUT_HH

#include <string>

class TH1;

class Input
{
	public:
		virtual TH1* getHist() const = 0;
		virtual ~Input() {}
		virtual int getTotalEvents() const = 0;
		virtual std::string getName() const = 0;
};

#endif