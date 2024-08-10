#pragma once


#include <memory>
#include <string>
#include <vector>
#include <utility>
#include "Date.h"
#include "RaylibCpp.h"
#include "External/tinyxml2.h"


enum LoadXmlError {
	Success = 0,
	FileNotFound,

};

enum CurrentList {
	SystemListSelect,
	GameListSelect,
};

struct GameList
{

	int mapIndex;
	std::string path;
	std::string name;
	std::string desc;
	std::string rating;
	std::string developer;
	std::string publisher;
	std::string genre;
	std::string players;
	std::string hash;
	std::string image;
	std::string thumbnail;
	std::string video;
	std::string genreId;
	bool bFavorite;
	int playCount;
	std::string executable;
	std::string arguments;
	DateTime releaseDate;
	DateTime lastPlayed;
	Texture2D texture;
	Texture2D textureMini;

	GameList()
		: mapIndex(-1)
		, players("1")
		, bFavorite(false)
		, playCount(0)
		, texture()
		, textureMini()
	{
	}

	GameList(const GameList&) = default;  	// Copy constructor

	GameList& operator=(const GameList&) = default;

	GameList(GameList&& other) noexcept       // Move constructor
		: mapIndex(other.mapIndex)
		, path(std::move(other.path))
		, name(std::move(other.name))
		, desc(std::move(other.desc))
		, rating(std::move(other.rating))
		, developer(std::move(other.developer))
		, publisher(std::move(other.publisher))
		, genre(std::move(other.genre))
		, players(std::move(other.players))
		, hash(std::move(other.hash))
		, image(std::move(other.image))
		, thumbnail(std::move(other.thumbnail))
		, video(std::move(other.video))
		, genreId(std::move(other.genreId))
		, bFavorite(other.bFavorite)
		, playCount(other.playCount)
		, executable(std::move(other.executable))
		, arguments(std::move(other.arguments))
		, releaseDate(other.releaseDate)
		, lastPlayed(other.lastPlayed)
		, texture(other.texture)
		, textureMini(other.textureMini)
	{
		// Reset the textures in the source object to prevent double deletion
		other.texture.id = 0;
		other.textureMini.id = 0;
	}

	GameList& operator=(GameList&& other) noexcept  // Move assignment operator
	{
		if (this != &other)
		{
			// Clean up current resources
			UnloadTexture(texture);
			UnloadTexture(textureMini);

			// Move data
			mapIndex = other.mapIndex;
			path = std::move(other.path);
			name = std::move(other.name);
			desc = std::move(other.desc);
			rating = std::move(other.rating);
			developer = std::move(other.developer);
			publisher = std::move(other.publisher);
			genre = std::move(other.genre);
			players = std::move(other.players);
			hash = std::move(other.hash);
			image = std::move(other.image);
			thumbnail = std::move(other.thumbnail);
			video = std::move(other.video);
			genreId = std::move(other.genreId);
			bFavorite = other.bFavorite;
			playCount = other.playCount;
			executable = std::move(other.executable);
			arguments = std::move(other.arguments);
			releaseDate = other.releaseDate;
			lastPlayed = other.lastPlayed;
			texture = other.texture;
			textureMini = other.textureMini;

			// Reset the textures in the source object to prevent double deletion
			other.texture.id = 0;
			other.textureMini.id = 0;
		}
		return *this;
	}

	~GameList()
	{
		UnloadTexture(texture);
		//UnloadTexture(textureMini);
	}

	bool operator==(const GameList& a) const
	{
		return (mapIndex == a.mapIndex);
	}
	bool operator>(const GameList& a) const
	{
		return (mapIndex > a.mapIndex);
	}
	bool operator<(const GameList& a) const
	{
		return (mapIndex < a.mapIndex);
	}
};


struct SystemList
{
	int mapIndex;
	std::string executable;
	std::string arguments;
	std::string romPath;
	std::string systemName;
	std::string systemLabel;
	std::string image;
	std::string screenshot;
	std::string video;
	std::string desc;
	Texture2D texture;
	Texture2D textureMini;

	SystemList()
		: mapIndex(-1)
		, texture()
		, textureMini()
	{
	}

	~SystemList()
	{
		UnloadTexture(texture);
		UnloadTexture(textureMini);
	}

};



class GameListManager
{

private:

	GameListManager()
		: currentList(SystemListSelect)
		, idSystemList(0)
		, idGameList(0)
	{
	}

	friend class ImageLoader;

public:

	static GameListManager* GetInstance();

private:

	CurrentList currentList;

	int idSystemList;
	int idGameList;

	tinyxml2::XMLDocument documentGameListXml;
	tinyxml2::XMLDocument documentSystemListXml;
	std::vector< std::shared_ptr<GameList> > gameList;
	std::vector< std::shared_ptr<SystemList> > systemList;

public:

	void LoadGameList();
	void LoadSystemList();

	void AddId(const int newId);
	void ChangeId(const int newId);
	int GetId() const;
	int GetGameListSize() const;
	std::vector<std::shared_ptr<GameList>>& GetAllGameList();
	GameList* GetCurrentGameList();
	SystemList* GetCurrentSystemList();
	void ClearSystemList();
	void ClearGameList();

private:

	void ReplaceCurrentPath(GameList* game) const;
	static bool IsValidElement(const tinyxml2::XMLElement* pElement, const char* name = "");


};
