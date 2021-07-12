#pragma once
#include <SFML/Audio.hpp>

enum class SOUND{ CLEAR, DROP, TETRIS,GAMEOVER};

class Audio
{
public:
    Audio(float width, float height) {
        soundbuffers[0].loadFromFile("clearsound.wav");
        soundbuffers[1].loadFromFile("dropsound.wav");
        soundbuffers[2].loadFromFile("tetris.wav");
        soundbuffers[3].loadFromFile("gameover.wav");

      
        icons.loadFromFile("icons.png");
        musicsprite.setTexture(icons);
        musicsprite.setTextureRect(sf::IntRect(0, 0, 256, 256));
        musicsprite.setScale({ .2f,.2f });
        musicsprite.setOrigin(musicsprite.getLocalBounds().width, musicsprite.getLocalBounds().height-5);
        musicsprite.setPosition({ width-10,height-10 });

        soundeffectssprite.setTexture(icons);
        soundeffectssprite.setTextureRect(sf::IntRect(256, 0, 256, 256));
        soundeffectssprite.setScale({ .2f,.2f });
        soundeffectssprite.setOrigin(musicsprite.getLocalBounds().width, musicsprite.getLocalBounds().height);
        soundeffectssprite.setPosition({ width-musicsprite.getGlobalBounds().width-20, height-10 });
    

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


    void draw(sf::RenderWindow &window) {
        musicsprite.setColor(musicmuted ? sf::Color{ 100,100,100 } : sf::Color::White);
        soundeffectssprite.setColor(soundmuted ? sf::Color{ 100,100,100 } : sf::Color::White);

        window.draw(musicsprite);
        window.draw(soundeffectssprite);

        music.setVolume(musicmuted ? 0.f : 20.f);
        sound.setVolume(soundmuted ? 0.f : 100.f);
    }

    void input(const sf::Event &event, const sf::RenderWindow &window) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left) {
            const auto coord{ window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y}) };
            if (musicsprite.getGlobalBounds().contains(coord)) {
                musicmuted = !musicmuted;
            }
            else if (soundeffectssprite.getGlobalBounds().contains(coord)) {
                soundmuted = !soundmuted;
            }
        }
    }

private:
    std::vector<sf::SoundBuffer> soundbuffers{ 4 };
    sf::Sound sound;
    sf::Music music;

    bool soundmuted{ false }, musicmuted{ false };
    
    sf::Texture icons;
    sf::Sprite musicsprite;
    sf::Sprite soundeffectssprite;
};

