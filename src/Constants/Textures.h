#pragma once

class Textures {
public:
	static inline unsigned int Play()      { return m_playID; }
	static inline unsigned int Pause()     { return m_pauseID; }
	static inline unsigned int Stop()      { return m_stopID; }
	static inline unsigned int Record()    { return m_recordID; }
	static inline unsigned int Folder()    { return m_folderID; }

	static void _LoadAll();
	static void _DestroyAll();

private:
	static unsigned int LoadTexture(const std::string& filepath);
	static void DestroyTexture(unsigned int texID);

private:
	static unsigned int m_playID;
	static unsigned int m_pauseID;
	static unsigned int m_stopID;
	static unsigned int m_recordID;
	static unsigned int m_folderID;
};