#ifndef _PARSER_HH_
#define _PARSER_HH_

#include <istream>
#include "Scene.hh"

class Parser {
protected:
	std::istream &stream;
public:
	explicit Parser(std::istream &_stream) : stream(_stream) {};
	virtual bool parse(struct Scene &scene) = 0;
};

#endif
