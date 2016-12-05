#ifndef COMMON_1345325_HEADER
#define COMMON_1345325_HEADER

#include <memory>
#include <vector>
#include <cassert>
#include "../Elements/Element.h"

class Parser;
class SimMethod;
class Viewer;
class Simulation;

using elements_t = std::vector<Element>;
using parser_p = std::unique_ptr<Parser>; 
using simMethod_p = std::unique_ptr<SimMethod>;
using viewer_p = std::unique_ptr<Viewer>;


#endif