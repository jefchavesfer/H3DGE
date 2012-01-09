#ifndef _OFFPARSER_HH_
#define _OFFPARSER_HH_

#include "Parser.hh"

class OffParser : public Parser {
public:
	explicit OffParser(std::istream &_stream) : Parser(_stream) {};
	bool parse(struct Scene &scene);
};

#endif
