#pragma once

#include <exception>
#include <vector>
#include <glm/glm.hpp>

#include "../lib/json.hpp"

#include "../world/component/component.hpp"
#include "../world/world.hpp"
#include "../world/entity.hpp"

using json = nlohmann::json;

class JSONLoader;
struct ComponentValues;

struct MapInformation {
	JSONLoader& jsonLoader;

	std::string name;
	std::string map;
	json entities;

	MapInformation(JSONLoader& jsonLoader, std::string name, std::string map, json entities) : jsonLoader(jsonLoader), name(name), map(map), entities(entities) {}

	std::vector<Entity*> constructEntities(World& world);
};

class JSONLoader {
public:
	JSONLoader();
	virtual ~JSONLoader();

	std::shared_ptr<MapInformation> loadMap(const std::string& map);
	Entity* constructEntity(World& world, const std::string& entityTemplate, const json& mapEntityComponents);

private:
	typedef void (*constructComponent_f)(Entity* entity, const ComponentValues& value);

	std::map<std::string, constructComponent_f> _constructors;
};

struct ComponentValues {
	json& mapObj;
	json& templateObj;
	// XXX: HACK
	Entity* entity;

	bool getBool(const std::string& name, bool defaultValue) const;
	int getInt(const std::string& name, int defaultValue) const;
	float getFloat(const std::string& name, float defaultValue) const;
	glm::vec2 getVec2(const std::string& name, const glm::vec2& defaultValue) const;
	glm::vec3 getVec3(const std::string& name, const glm::vec3& defaultValue) const;
	std::string getString(const std::string& name, const std::string& defaultValue) const;
};

#include <string>
class JSONParseException : public std::exception {
public:
	JSONParseException(const std::string msg, const std::string name, const char* func, int line) {
		_what = std::string(func) + ":" + std::to_string(line) + ": [" + name + "] " + msg;
	}

	virtual const char* what() const throw() { return _what.c_str(); }

private:
	std::string _what;
};
