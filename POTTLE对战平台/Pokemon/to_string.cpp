#include"header.h"

std::string to_string(Poke_type a)//精灵种类转字符串
{
	if (a == Poke_type::strength_type)
		return "strength_type";
	if (a == Poke_type::agility_type)
		return "agility_type";
	if (a == Poke_type::defense_type)
		return "defense_type";
	if (a == Poke_type::health_type)
		return "health_type";
	return NULL;
}

std::string to_string(Strength_poke_race a)//精灵种族转字符串
{
	if (a == Strength_poke_race::Charizard)
		return "Charizard";
	if (a == Strength_poke_race::Charmeleon)
		return "Charmeleon";
	return NULL;
}

std::string to_string(Health_poke_race a)//精灵种族转字符串
{
	if (a == Health_poke_race::Geodude)
		return "Geodude";
	if (a == Health_poke_race::Graveler)
		return "Graveler";
	return NULL;
}

std::string to_string(Defense_poke_race a)//精灵种族转字符串
{
	if (a == Defense_poke_race::Cloyster)
		return "Cloyster";
	if (a == Defense_poke_race::Shellder)
		return "Shellder";
	return NULL;
}

std::string to_string(Agility_poke_race a)//精灵种族转字符串
{
	if (a == Agility_poke_race::Pikachu)
		return "Pikachu";
	if (a == Agility_poke_race::Raichu)
		return "Raichu";
	return NULL;
}

std::string to_string(bool i)//布尔值转字符串
{
	if (i == true)
		return "true";
	else
		return "false";
}