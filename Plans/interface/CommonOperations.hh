#ifndef COMMONOPERATIONS__HH
#define COMMONOPERATIONS__HH
#include <memory>
class EventModule;

namespace CommonOperations
{
	void addHiggsScaleFactors(std::shared_ptr<EventModule> module);
}
#endif