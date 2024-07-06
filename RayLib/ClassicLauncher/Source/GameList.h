#pragma once

#include <string>

struct GameList
{
	// map index

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

	std::string screenshot;

	std::string video;

	std::string genreId;

	bool bFavorite;

	int playCount;

	std::string executable;

	std::string arguments;

	//
	//formatted 
	//

	std::string pathFormatted;

	std::string nameFormatted;

	std::string DescFormatted;

	std::string imageFormatted;

	std::string screenshotFormatted;

	std::string videoFormatted;

	//
	//datetime
	//
	
	std::string releaseDate;

	
	std::string lastPlayed;

	//constructor
	GameList()
		: mapIndex(-1) //map index
		, players("1")
		, bFavorite(false)
		, playCount(0)
	{}

	bool operator==(const GameList& A) const
	{
		return (mapIndex == A.mapIndex);
	}
	bool operator>(const GameList& A) const
	{
		return (mapIndex > A.mapIndex);
	}
	bool operator<(const GameList& A) const
	{
		return (mapIndex < A.mapIndex);
	}
};