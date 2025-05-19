//
// Created by Harry Skerritt on 19/05/2025.
//

#include "FadeText.h"

FadeText::FadeText(const sf::Font &font, const std::string &string, float fade_duration, float hold_duration, sf::Color colour)
    : Text(font, string), m_fade_duration(fade_duration), m_hold_duration(hold_duration), m_state(State::Idle), m_base_colour(colour) {
    setFillColor(sf::Color(m_base_colour.r, m_base_colour.g, m_base_colour.b, 0));
}


void FadeText::fade() {
    if (m_state == State::Idle) {
        m_state = State::FadingIn;
        m_clock.restart();
    }
}


void FadeText::update() {
    float time = m_clock.getElapsedTime().asSeconds();

    switch (m_state) {
        case State::FadingIn: {
            float alpha = std::min(255.f, (time / m_fade_duration) * 255.f);
            setFillColor(sf::Color(m_base_colour.r, m_base_colour.g, m_base_colour.b, static_cast<std::uint8_t>(alpha)));

            if (time >= m_fade_duration) {
                m_state = State::Holding;
                m_clock.restart();
            }
            break;
        }
        case State::Holding: {
            setFillColor(sf::Color(m_base_colour.r, m_base_colour.g, m_base_colour.b, 255));
            if (time >= m_hold_duration) {
                m_state = State::FadingOut;
                m_clock.restart();
            }
            break;
        }
        case State::FadingOut: {
            float alpha = std::max(0.f, 255.f - (time / m_fade_duration) * 255.f);
            setFillColor(sf::Color(m_base_colour.r, m_base_colour.g, m_base_colour.b, static_cast<std::uint8_t>(alpha)));

            if (time >= m_fade_duration) {
                m_state = State::Idle;
                setFillColor(sf::Color(m_base_colour.r, m_base_colour.g, m_base_colour.b, 0)); // fully transparent
            }
            break;
        }
        case State::Idle: {
            break;
        }
    }

}





