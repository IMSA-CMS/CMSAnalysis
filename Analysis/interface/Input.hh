#ifndef INPUT_HH
#define INPUT_HH

#include <string>
#include <memory>
#include <vector>
#include "HistVariable.hh"


class TH1;

class Input
{
	public:
		virtual TH1* getHist(HistVariable histType) const = 0;
		virtual TH1* get2DHist(HistVariable histType) const = 0;
		virtual ~Input() {}
		virtual int getTotalEvents() const = 0;
};

#endif
