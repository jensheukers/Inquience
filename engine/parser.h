#ifndef PARSER_H
#define PARSER_H

//Define filetype extensions
#define FILETYPE_SCENE ".scene"
#define FILETYPE_PREFAB ".prefab"
#define FILETYPE_ANIMATION ".animation"

#include <iostream>
#include <fstream>
#include <vector>

//Include json
#include "json.hpp"

class Parser {
private:
	std::string destination; /**< Destination to writefile */
	std::string _curLine; /**< Current line index */

	std::fstream _file; /**< File instance for stream*/

	bool _read;

	bool debug;

	/**
	* Writes a entity to a json array
	*/
	void BufferEntityToJsonArray(class Entity* e, nlohmann::json& _array);

	/**
	* Reads a entity entry in a json file and creates a instance
	*/
	class Entity* ReadEntityFromJsonData(nlohmann::json& jsonData);

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
	* Returns the file
	*/
	std::fstream& GetFile();

	/**
	* Writes a scene to a .scene file
	*/
	void WriteSceneToFile(class Scene* scene);

	/**
	* Read a scene from a .scene file
	* @return Scene*
	*/
	Scene* ReadSceneFromFile();


	/**
	* Writes a entity and its children to a .prefab file
	*/
	void WritePrefabToFile(Entity* e);

	/**
	* Reads a json prefab and returns the top parent as a Entity*
	* @return Entity*
	*/
	Entity* ReadPrefabFromFile();


	/**
	* Destructor
	*/
	~Parser();
};


#endif // !PARSER_H