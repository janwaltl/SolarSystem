#ifndef ELEMENTS_ELEMENT_HEADER
#define ELEMENTS_ELEMENT_HEADER

//Basic element of simulated data - planet, star, 
class Element
{
public:
	Element();
private:
	double x, y;
	double mass;
	double vx, vy;
};


#endif