#include"../../Header/Service/Application.h"
#include<exception>
#include<fstream>

std::string Application::_configUrl = "Config/config.json";

Application::Application() {
	this->_locationService = new LocationService();
	this->_mapService = new MapService();
	this->loadConfig();
}

Application::~Application() {
	this->savaConfig();
	delete this->_locationService;
	delete this->_mapService;
}

LocationService* Application::getLocationService() {
	return this->_locationService;
}

MapService* Application::getMapService() {
	return this->_mapService;
}

bool Application::savaConfig() {
	try {
		Json::Value ConfigValue;
		Json::Value LocationConfig;
		Json::Value MapConfig;
		std::ofstream outfile;
		outfile.open(Application::_configUrl, std::ios::out);

		//location config
		auto locRepo = this->_locationService->_locationRepo;
		for (auto itr = locRepo.begin();itr != locRepo.begin(); itr++) {
			Json::Value location;
			location["locationName"] = (*itr)->getLocationName();
			location["description"] = (*itr)->getDescription();
			Json::Value picContainer;
			for (auto itrPic=(*itr)->getPictureContainer().begin();
				itrPic != (*itr)->getPictureContainer().end(); itrPic++) {
				picContainer.append(*itrPic);
			}
			location["pictureContainer"] = picContainer;
			LocationConfig.append(location);
		}
		ConfigValue["LocationConfig"] = LocationConfig;

		//map config --- traverse the map tree with BFS
		Map* currentMap = nullptr;
		std::list<Map*> mapQueue;
		mapQueue.push_back(this->_mapService->_rootMap);
		do {
			currentMap = mapQueue.front();
			for (auto itr = currentMap->_subMap.begin(); itr != currentMap->_subMap.end(); itr++) {
				mapQueue.push_back(itr->second);
			}
			mapQueue.pop_front();

			//make map json value
			Json::Value map;
			//map::id
			map["id"] = currentMap->_Id;
			//map::coordinate
			Json::Value coordinate;
			coordinate["x"] = currentMap->_coordinate.x;
			coordinate["y"] = currentMap->_coordinate.y;
			map["coordinate"] = coordinate;
			//map::parentmap
			if (nullptr != currentMap->_parentMap) {
				map["parentmap"] = currentMap->_parentMap->_Id;
			}
			else {
				map["parentmap"] = -1;
			}
			//map::location
			map["location"] = currentMap->getLoc()->getLocationName();
			//map::url
			map["url"] = currentMap->getUrl();
			//map::pictureName
			map["pictureName"] = currentMap->getName();

			//append
			MapConfig.append(map);

		} while (0!=mapQueue.size());
		ConfigValue["MapConfig"] = MapConfig;
		
		//output file
		outfile << ConfigValue.toStyledString();
		outfile.close();
		return true;
	}
	catch (std::exception e) {
		return false;
	}
}

bool Application::loadConfig() {
	try {
		std::ifstream in(Application::_configUrl, std::ios::binary);
		std::filebuf* pbuf=in.rdbuf();
		//get size of file
		long long filesize = pbuf->pubseekoff(0, std::ios::end, std::ios::in);
		pbuf->pubseekpos(0,std::ios::in);
		//alloc memory & read file
		char* configChars = new char[filesize];
		pbuf->sgetn(configChars, filesize);
		//Parse json config
		Json::Reader configReader;
		Json::Value configValue;
		if (configReader.parse(*(new string(configChars)), configValue)) {
			//load location config
			Json::Value locationConfig = configValue["LocationConfig"];
			for (auto itr=locationConfig.begin(); itr != locationConfig.end(); itr++) {
				Location* locationItem = new Location();
				locationItem->_locationName = (*itr)["locationName"].asString();
				locationItem->_description = (*itr)["description"].asString();
				for (auto itrPic = (*itr)["pictureContainer"].begin(); itrPic != (*itr)["pictureContainer"].end(); itrPic++) {
					locationItem->_pictureContainer.push_back((*itrPic).asString());
				}
				this->_locationService->addNewLocation(locationItem);
			}
			//load map config

		}
		else {
			return false;
		}

	}catch (...) {
		return false;
	}
}