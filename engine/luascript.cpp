// Luascript imported from aquarite3d, modified in FoundryFactoryEngine
// Copy Version: 7/2/2019
// Modify Version: 2019
//
//	© 2019, Jens Heukers
#include "luascript.h"
#include "debug.h"
#include "core.h"

LuaParsableLine::LuaParsableLine(std::string line, unsigned tabs) {
	this->line = line;
	this->tabs = tabs;
}

void LuaScriptFile::ParseLine(std::string line, unsigned tabs) {
	if (!this->stream.is_open()) {
		this->stream = std::ofstream(this->path);
	}
	for (unsigned i = 0; i < tabs; i++) {
		this->stream << "	";
	}

	this->stream << line << std::endl;
}

LuaScriptFile::LuaScriptFile(std::string path) {
	this->path = Core::GetExecutableDirectoryPath() + path;

	//Create new file
	this->stream = std::ofstream(this->path);
}

void LuaScriptFile::ParseFunction(std::string name, std::vector<std::string> args, std::vector<LuaParsableLine> lines) {
	this->stream << "function " << name << "(";

	for (size_t i = 0; i < args.size(); i++) {
		this->stream << args[i]; 
		
		if(i + 1 != args.size()) {
			this->stream << ",";
		}
	}
	this->stream << ")" << std::endl;

	for (size_t i = 0; i < lines.size(); i++) {
		this->ParseLine(lines[i].line, lines[i].tabs);
	}

	this->stream << "end" << std::endl;
}

std::string LuaScriptFile::CreateLuaFunctionCallString(std::string name, std::vector<std::string> args) {
	std::string functionCallString = name + "(";
	for (size_t i = 0; i < args.size(); i++) {
		functionCallString += args[i];

		if (i + 1 != args.size()) {
			functionCallString += ",";
		}
	}
	functionCallString += ")";
	return functionCallString;
}

LuaScriptFile::~LuaScriptFile() {
	if (this->stream.is_open()) {
		this->stream.close();
	}
}

LuaScript::LuaScript() {
	this->state = luaL_newstate(); // Create a new lua state
	luaopen_base(this->state); // Open base functions
}

int LuaScript::Run(std::string script) {
	return luaL_dostring(this->state, script.c_str());
}

std::string LuaScript::RunFunction(std::string file, std::string function, std::vector<std::string> arguments) {
	std::string absolutePath = Core::GetExecutableDirectoryPath();
	absolutePath.append(file);

	if (luaL_dofile(this->state, absolutePath.c_str()) != LUA_OK) {
		return "Lua: Error opening file";
	}

	lua_getglobal(this->state, function.c_str());

	if (lua_isfunction(this->state, -1)) {
		for (size_t i = 0; i < arguments.size(); i++) {
			lua_pushstring(this->state, arguments[i].c_str());
		}
		size_t num_args = arguments.size();
		lua_pcall(this->state, num_args, 1, 0); // We expect 1 return
		if (lua_tostring(this->state, -1)) {
			std::string luaReturn = "Lua: ";
			luaReturn.append(lua_tostring(this->state, -1)); // Get return value from lua
			return luaReturn;
		}
		else {
			return "LUASCRIPT::EMPTY::RETURN::VALUE";
		}
	}
	else {
		return "Lua: " + function + " Is not a function";
	}
}

void LuaScript::AddNativeFunction(std::string name, int(*func)(lua_State*)) {
	lua_pushcfunction(this->state, func);
	lua_setglobal(this->state, name.c_str());
}

int LuaScript::GetType(std::string variableName) {
	//Push to top of stack
	lua_getglobal(this->state, variableName.c_str());

	//Return type
	return lua_type(this->state, -1);
}

int LuaScript::GetNumber(std::string variableName) {
	//Push to top of stack
	lua_getglobal(this->state, variableName.c_str());
		
	//Determine type of last pushed value
	int type = lua_type(this->state, -1);
	
	if (type == LUA_TNUMBER) {
		return (int)lua_tonumber(this->state, -1);
	}
	else {
		Debug::Log("Lua: Variable is not a number!");
		return 0;
	}
}

std::string LuaScript::GetString(std::string variableName) {
	//Push to top of stack
	lua_getglobal(this->state, variableName.c_str());

	//Determine type of last pushed value
	int type = lua_type(this->state, -1);

	if (type == LUA_TSTRING) {
		return lua_tostring(this->state, -1);
	}
	else {
		Debug::Log("Lua: Variable is not a string!");
		return "";
	}
}

LuaScript::~LuaScript() {
	lua_close(this->state); // Destroy the lua state
}