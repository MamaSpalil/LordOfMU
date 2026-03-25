#pragma once

class cTexture
{
public:
	static void Models();
	static void Textures();
	void FenrirLoadModel(int index, char* folder, char* name, int value); // OK
	void FenrirLoadTexture(int index, char* folder, char* name, int value);
private:
	char _Buffer[30];
};

extern cTexture Texture;