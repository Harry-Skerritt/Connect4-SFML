//
// Created by Harry Skerritt on 19/05/2025.
//

#ifndef FADETEXT_H
#define FADETEXT_H

#include <SFML/Graphics.hpp>

class FadeText : public sf::Text {
public:
    FadeText(const sf::Font& font, const std::string& string= "", float fade_duration = 1.f, float hold_duration = 3.f, sf::Color colour = sf::Color::White);

    void update();
    void fade();


private:

    enum class State {
        Idle,
        FadingIn,
        Holding,
        FadingOut
    };

    State m_state;
    sf::Clock m_clock;
    sf::Color m_base_colour;

    float m_fade_duration;
    float m_hold_duration;



};



#endif //FADETEXT_H
