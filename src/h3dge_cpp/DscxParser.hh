#ifndef _DSCXPARSER_HH_
#define _DSCXPARSER_HH_

#include "Parser.hh"

class DscxParser : public Parser {
public:
	explicit DscxParser(std::istream &_stream) : Parser(_stream) {};
	bool parse(struct Scene &scene);
};

#endif
