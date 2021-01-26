#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <vector>

class Parser {
private:
	std::string destination; /**< Destination to writefile */
	std::string _curLine; /**< Current line index */

	std::fstream _file; /**< File instance for stream*/

	bool _read;

	bool debug;
public:
	/**
	* Constructor, if read is true we can read from file, if not we can write
	*/
	Parser(std::string destination, bool read, bool debug = false);

	/**
	* Returns true if file is open
	*/
	bool fileIsOpen();

	/**
	* Read next line
	*/
	std::string ReadLine();

	/**
	* Write next line
	*/
	void WriteLine(std::string line);

	/**
	* Splits line at a character, and returns vector of strings
	*/
	std::vector<std::string> SplitLine(std::string line, char c);


	/**
	* Splits current line at a character, and returns vector of strings
	*/
	std::vector<std::string> SplitLine(char c);

	/**
	* Destructor
	*/
	~Parser();
};


#endif // !PARSER_H