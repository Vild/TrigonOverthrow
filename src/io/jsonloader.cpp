#include "jsonloader.hpp"

#include <cstdlib>
#include <cstdio>
#include <fstream>

#include "../world/component/cameracomponent.hpp"
#include "../world/component/kbmouseinputcomponent.hpp"
#include "../world/component/lookatcomponent.hpp"
#include "../world/component/modelcomponent.hpp"
#include "../world/component/particlecomponent.hpp"
#include "../world/component/textcomponent.hpp"
#include "../world/component/dynamicmodelcomponent.hpp"
#include "../world/component/guncomponent.hpp"
#include "../world/component/pointlightcomponent.hpp"
#include "../world/component/suncomponent.hpp"
#include "../world/component/instancedsimplemeshcomponent.hpp"
#include "../world/component/transformcomponent.hpp"
#include "../world/component/lifecomponent.hpp"
#include "../world/component/projectilecomponent.hpp"
#include "../world/component/floortilecomponent.hpp"
#include "../world/component/hovercomponent.hpp"
#include "../world/component/rigidbodycomponent.hpp"
#include "../world/component/experiencecomponent.hpp"
#include "../world/component/experienceorbcomponent.hpp"
#include "../world/component/aicomponent.hpp"
#include "../world/component/sfxcomponent.hpp"
#include "../world/component/upgradecomponent.hpp"
#include "../world/component/bossaicomponent.hpp"

#ifdef WIN32
#define __PRETTY_FUNCTION__ __func__
#endif

std::vector<Entity*> MapInformation::constructEntities(World& world) {
	std::vector<Entity*> result;

	for (auto it = entities.begin(); it != entities.end(); ++it) {
		json entity = *it;
		std::string template_ = entity.at("template").get<std::string>();
		json components = entity.at("components");

		result.push_back(jsonLoader.constructEntity(world, "assets/entities/" + template_ + ".json", components));
	}

	return result;
}

template <typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
static void constructComponent(Entity* entity, const ComponentValues& value) {
	entity->addComponent<T>(value);
}

JSONLoader::JSONLoader() {
	_constructors["CameraComponent"] = &constructComponent<CameraComponent>;
	_constructors["KBMouseInputComponent"] = &constructComponent<KBMouseInputComponent>;
	_constructors["LookAtComponent"] = &constructComponent<LookAtComponent>;
	_constructors["ModelComponent"] = &constructComponent<ModelComponent>;
	_constructors["ParticleComponent"] = &constructComponent<ParticleComponent>;
	_constructors["TextComponent"] = &constructComponent<TextComponent>;
	_constructors["DynamicModelComponent"] = &constructComponent<DynamicModelComponent>;
	_constructors["GunComponent"] = &constructComponent<GunComponent>;
	_constructors["PointLightComponent"] = &constructComponent<PointLightComponent>;
	_constructors["SunComponent"] = &constructComponent<SunComponent>;
	_constructors["InstancedSimpleMeshComponent"] = &constructComponent<InstancedSimpleMeshComponent>;
	_constructors["TransformComponent"] = &constructComponent<TransformComponent>;
	_constructors["LifeComponent"] = &constructComponent<LifeComponent>;
	_constructors["ProjectileComponent"] = &constructComponent<ProjectileComponent>;
	_constructors["FloorTileComponent"] = &constructComponent<FloorTileComponent>;
	_constructors["HoverComponent"] = &constructComponent<HoverComponent>;
	_constructors["RigidBodyComponent"] = &constructComponent<RigidBodyComponent>;
	_constructors["ExperienceComponent"] = &constructComponent<ExperienceComponent>;
	_constructors["ExperienceOrbComponent"] = &constructComponent<ExperienceOrbComponent>;
	_constructors["AIComponent"] = &constructComponent<AIComponent>;
	_constructors["SFXComponent"] = &constructComponent<SFXComponent>;
	_constructors["UpgradeComponent"] = &constructComponent<UpgradeComponent>;
	_constructors["BossAIComponent"] = &constructComponent<BossAIComponent>;
}

JSONLoader::~JSONLoader() {}

std::shared_ptr<MapInformation> JSONLoader::loadMap(const std::string& map) {
	json root;
	try {
		printf("Loading map: %s\n", map.c_str());
		std::ifstream input(map);
		input >> root;
	} catch (const std::invalid_argument& e) {
		fprintf(stderr, "Failed to load map: Map file path is invalid!: %s\n", e.what());
		return std::shared_ptr<MapInformation>();
	} catch (const std::exception& e) {
		fprintf(stderr, "Failed to load map: %s\n", e.what());
		return std::shared_ptr<MapInformation>();
	} catch (const char* msg) {
		fprintf(stderr, "Failed to load map: %s\n", msg);
		return std::shared_ptr<MapInformation>();
	}

	return std::make_shared<MapInformation>(*this, root.at("name").get<std::string>(), root.at("map").get<std::string>(), root.at("entities"));
}

Entity* JSONLoader::constructEntity(World& world, const std::string& entityTemplate, const json& mapEntityComponents) {
	json root;
	try {
		std::ifstream input(entityTemplate);
		input >> root;
	} catch (const std::invalid_argument& e) {
		fprintf(stderr, "Failed to load entity: Entity template file path is invalid!: %s\n", e.what());
		return nullptr;
	} catch (const std::exception& e) {
		fprintf(stderr, "Failed to load entity: %s\n", e.what());
		return nullptr;
	} catch (const char* msg) {
		fprintf(stderr, "Failed to load entity: %s\n", msg);
		return nullptr;
	}

	std::string name = root.at("name").get<std::string>();

	json components = root.at("components");

	Entity* entity = world.addEntity(name);

	for (auto it = components.begin(); it != components.end(); ++it) {
		std::string name = it.key();

		auto f = _constructors[name];
		if (!f)
			throw JSONParseException("Missing component constructor!", name, __PRETTY_FUNCTION__, __LINE__);

		json mapObj;
		auto mapIT = mapEntityComponents.find(name);
		if (mapIT != mapEntityComponents.end())
			mapObj = *mapIT;

		f(entity, ComponentValues{mapObj, it.value(), entity});
	}

	return entity;
}

bool ComponentValues::getBool(const std::string& name, bool defaultValue) const {
	json tmp;
	auto mapIT = mapObj.find(name);
	if (mapIT == mapObj.end()) {
		auto templateIT = templateObj.find(name);
		if (templateIT == templateObj.end())
			return defaultValue;
		tmp = *templateIT;
	} else
		tmp = *mapIT;

	if (tmp.type() == json::value_t::boolean)
		return tmp.get<bool>();
	else if (tmp.type() != json::value_t::object)
		throw JSONParseException("Expected another type!", name, __PRETTY_FUNCTION__, __LINE__);

	return !!(rand() % 2);
}

int ComponentValues::getInt(const std::string& name, int defaultValue) const {
	json tmp;
	auto mapIT = mapObj.find(name);
	if (mapIT == mapObj.end()) {
		auto templateIT = templateObj.find(name);
		if (templateIT == templateObj.end())
			return defaultValue;
		tmp = *templateIT;
	} else
		tmp = *mapIT;

	if (tmp.type() == json::value_t::number_integer)
		return tmp.get<int>();
	else if (tmp.type() == json::value_t::number_unsigned)
		return tmp.get<unsigned int>();
	else if (tmp.type() != json::value_t::object)
		throw JSONParseException("Expected another type!", name, __PRETTY_FUNCTION__, __LINE__);

	int upper = tmp.at("upper").get<int>();
	int lower = tmp.at("lower").get<int>();

	return lower + (rand() % (upper - lower));
}

float ComponentValues::getFloat(const std::string& name, float defaultValue) const {
	json tmp;
	auto mapIT = mapObj.find(name);
	if (mapIT == mapObj.end()) {
		auto templateIT = templateObj.find(name);
		if (templateIT == templateObj.end())
			return defaultValue;
		tmp = *templateIT;
	} else
		tmp = *mapIT;

	if (tmp.type() == json::value_t::number_float)
		return tmp.get<float>();
	else if (tmp.type() == json::value_t::number_integer)
		return tmp.get<int>();
	else if (tmp.type() == json::value_t::number_unsigned)
		return tmp.get<unsigned int>();
	else if (tmp.type() != json::value_t::object)
		throw JSONParseException("Expected another type!", name, __PRETTY_FUNCTION__, __LINE__);

	float upper;
	float lower;

	{
		auto val = tmp.at("upper");
		if (val.type() == json::value_t::number_float)
			upper = val.get<float>();
		else if (val.type() == json::value_t::number_integer)
			upper = val.get<int>();
		else if (val.type() == json::value_t::number_unsigned)
			upper = val.get<unsigned int>();
		else
			throw JSONParseException("Expected another type!", name, __PRETTY_FUNCTION__, __LINE__);
	}

	{
		auto val = tmp.at("lower");
		if (val.type() == json::value_t::number_float)
			lower = val.get<float>();
		else if (val.type() == json::value_t::number_unsigned)
			lower = val.get<unsigned int>();
		else if (val.type() == json::value_t::number_integer)
			lower = val.get<int>();
		else
			throw JSONParseException("Expected another type!", name, __PRETTY_FUNCTION__, __LINE__);
	}

	return lower + (float(rand()) / float(RAND_MAX)) * (upper - lower);
}

glm::vec2 ComponentValues::getVec2(const std::string& name, const glm::vec2& defaultValue) const {
	json tmp;
	auto mapIT = mapObj.find(name);
	if (mapIT == mapObj.end()) {
		auto templateIT = templateObj.find(name);
		if (templateIT == templateObj.end())
			return defaultValue;
		tmp = *templateIT;
	} else
		tmp = *mapIT;

	if (tmp.type() != json::value_t::array)
		throw JSONParseException("Expected another type!", name, __PRETTY_FUNCTION__, __LINE__);

	glm::vec2 result;

	for (int i = 0; i < 2; i++) {
		auto entry = tmp.at(i);
		if (entry.type() == json::value_t::number_float) {
			result[i] = entry.get<float>();
			continue;
		} else if (entry.type() == json::value_t::number_integer) {
			result[i] = entry.get<int>();
			continue;
		} else if (entry.type() == json::value_t::number_unsigned) {
			result[i] = entry.get<unsigned int>();
			continue;
		} else if (entry.type() != json::value_t::object)
			throw JSONParseException("Expected another type!", name, __PRETTY_FUNCTION__, __LINE__);

		float upper;
		float lower;

		{
			auto val = entry.at("upper");
			if (val.type() == json::value_t::number_float)
				upper = val.get<float>();
			else if (val.type() == json::value_t::number_integer)
				upper = val.get<int>();
			else if (val.type() == json::value_t::number_unsigned)
				upper = val.get<unsigned int>();
			else
				throw JSONParseException("Expected another type!", name, __PRETTY_FUNCTION__, __LINE__);
		}

		{
			auto val = entry.at("lower");
			if (val.type() == json::value_t::number_float)
				lower = val.get<float>();
			else if (val.type() == json::value_t::number_integer)
				lower = val.get<int>();
			else if (val.type() == json::value_t::number_unsigned)
				lower = val.get<unsigned int>();
			else
				throw JSONParseException("Expected another type!", name, __PRETTY_FUNCTION__, __LINE__);
		}

		result[i] = lower + (rand() / float(RAND_MAX)) * (upper - lower);
	}

	return result;
}

glm::vec3 ComponentValues::getVec3(const std::string& name, const glm::vec3& defaultValue) const {
	json tmp;
	auto mapIT = mapObj.find(name);
	if (mapIT == mapObj.end()) {
		auto templateIT = templateObj.find(name);
		if (templateIT == templateObj.end())
			return defaultValue;
		tmp = *templateIT;
	} else
		tmp = *mapIT;

	if (tmp.type() != json::value_t::array)
		throw JSONParseException("Expected another type!", name, __PRETTY_FUNCTION__, __LINE__);

	glm::vec3 result;

	for (int i = 0; i < 3; i++) {
		auto entry = tmp.at(i);
		if (entry.type() == json::value_t::number_float) {
			result[i] = entry.get<float>();
			continue;
		} else if (entry.type() == json::value_t::number_integer) {
			result[i] = entry.get<int>();
			continue;
		} else if (entry.type() == json::value_t::number_unsigned) {
			result[i] = entry.get<unsigned int>();
			continue;
		} else if (entry.type() != json::value_t::object)
			throw JSONParseException("Expected another type!", name, __PRETTY_FUNCTION__, __LINE__);

		float upper;
		float lower;

		{
			auto val = entry.at("upper");
			if (val.type() == json::value_t::number_float)
				upper = val.get<float>();
			else if (val.type() == json::value_t::number_integer)
				upper = val.get<int>();
			else if (val.type() == json::value_t::number_unsigned)
				upper = val.get<unsigned int>();
			else
				throw JSONParseException("Expected another type!", name, __PRETTY_FUNCTION__, __LINE__);
		}

		{
			auto val = entry.at("lower");
			if (val.type() == json::value_t::number_float)
				lower = val.get<float>();
			else if (val.type() == json::value_t::number_integer)
				lower = val.get<int>();
			else if (val.type() == json::value_t::number_unsigned)
				lower = val.get<unsigned int>();
			else
				throw JSONParseException("Expected another type!", name, __PRETTY_FUNCTION__, __LINE__);
		}

		result[i] = lower + (rand() / float(RAND_MAX)) * (upper - lower);
	}

	return result;
}

std::string ComponentValues::getString(const std::string& name, const std::string& defaultValue) const {
	json tmp;
	auto mapIT = mapObj.find(name);
	if (mapIT == mapObj.end()) {
		auto templateIT = templateObj.find(name);
		if (templateIT == templateObj.end())
			return defaultValue;
		tmp = *templateIT;
	} else
		tmp = *mapIT;

	if (tmp.type() != json::value_t::string)
		throw JSONParseException("Expected another type!", name, __PRETTY_FUNCTION__, __LINE__);

	return tmp.get<std::string>();
}
