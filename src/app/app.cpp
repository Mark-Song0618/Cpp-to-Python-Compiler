#include "../lexer/lexerPublic.h"
#include "../parser/parserPublic.h"
#include "../generator/generatorPublic.h"
#include "app.h"
#include <iostream>
#include <sstream>

int main(int argc, const char** argv)
{
	// check param
	if (argc != 2) {
		// error out
		std::stringstream usage;
		usage << "Usage:" << "\r\n";
		usage << "		C2P srcPath";
		std::cout << usage.str() << std::endl;
		return 1;
	}

	// lexical analyze
	Token  tokens[MAX_TOKEN_LEN] = { {INVALID, {'\0'}}};
	//if (!ParseFile(argv[1], tokens, MAX_TOKEN_LEN)) {
	if (!ParseFile(argv[1], tokens, MAX_TOKEN_LEN)) {
		//error out
		std::cout << "lex exception" << std::endl;
		return 2;
	}


	// parse tree
	TreeNode* root = nullptr;
	Parser	  parser;
	if (!parser.parse(tokens, root)) {
		// error out
		std::cout << "parse exception" << std::endl;
		return 3;
	}
	else {
		std::cout << "AST parsed successfully!" << std::endl;
		Dumper dumper;
		dumper.dump(parser.getProgram());
	}

	std::string Splitter(120, '*');
	std::string Lable("Target code:");
	std::string Indent((120 - Lable.length()) / 2, ' ');
	std::cout << Splitter << std::endl;
	std::cout << Indent << Lable << std::endl;
	std::cout << Splitter << std::endl;

	PyGenerator pg;
	if (!pg.generate(root)) {
		// error out
		std::cout << "python generate exception" << std::endl;
		return 4;
	}

	std::cout << "\r\nEverything done" << std::endl;
	return 0;
}