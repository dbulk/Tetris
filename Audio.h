#pragma once
#include <SFML/Audio.hpp>

enum class SOUND{ CLEAR, DROP, TETRIS,GAMEOVER};

class Audio
{
public:
    Audio() {
        soundbuffers[0].loadFromFile("clearsound.wav");
        soundbuffers[1].loadFromFile("dropsound.wav");
        soundbuffers[2].loadFromFile("tetris.wav");
        soundbuffers[3].loadFromFile("gameover.wav");
    }

    void play(SOUND s) {
        sound.setBuffer(soundbuffers[static_cast<size_t>(s)]);
        sound.play();
    }
    bool isPlaying() {
        return sound.getStatus() == sf::Sound::Status::Playing;
    }

    void playloop(std::string filename,float start, float end ) {
        music.openFromFile(filename);
        music.setLoopPoints(sf::Music::TimeSpan(sf::seconds(start), sf::seconds(end)));
        music.setLoop(true);
        music.setVolume(20);
        music.play();
    }

    void pause() {
        music.pause();
    }
    void stop() {
        music.stop();
    }
    void play() {
        music.play();
    }

private:
    std::vector<sf::SoundBuffer> soundbuffers{ 4 };
    sf::Sound sound;
    sf::Music music;
};

