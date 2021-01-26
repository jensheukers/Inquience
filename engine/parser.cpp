#include "parser.h"
#include "debug.h"

#include <sstream>

Parser::Parser(std::string destination, bool read, bool debug) {
	this->destination = destination;

	this->_file.open(destination);
	this->_read = read;
	this->debug = debug;
}

bool Parser::fileIsOpen() {
	return this->_file.is_open();
}

std::string Parser::ReadLine() {
	if (!this->_read) {
		Debug::Log("Parser : File is write only");
		return "";
	}

	if (this->fileIsOpen()) {
		std::getline(this->_file, this->_curLine);

		Debug::Log("Parser [READ] : " + this->_curLine);
		return this->_curLine;
	}
	Debug::Log("Parser : File is not open");
	return "";
}

void Parser::WriteLine(std::string line) {
	if (this->_read) {
		Debug::Log("Parser : File is read only");
		return;
	}

	if (this->fileIsOpen()) {
		this->_file << line + "\n";

		Debug::Log("Parser [WRITE] : " + line);
	}
	else {
		Debug::Log("Parser : File is not open");
	}
}

std::vector<std::string> Parser::SplitLine(std::string line, char c) {
	std::stringstream ss(line);
	std::string segment;
	std::vector<std::string> segments;

	while (std::getline(ss, segment, c)) {
		segments.push_back(segment);
	}
	return segments;
}

std::vector<std::string> Parser::SplitLine(char c) {
	return this->SplitLine(this->_curLine, c);
}

Parser::~Parser() {
	if (fileIsOpen()) {
		this->_file.close();
	}
}