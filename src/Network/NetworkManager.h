//
// Created by Harry Skerritt on 19/05/2025.
//

#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <SFML/Network.hpp>
#include <thread>
#include <string>
#include <map>

namespace Network {

    // Represents a client
    struct ClientKey {
        sf::IpAddress address;
        unsigned short port;

        bool operator==(const ClientKey &other) const;
        bool operator<(const ClientKey& other) const;
    };

    class NetworkManager {
    public:
        NetworkManager();
        ~NetworkManager();

        // Control connections
        void startListening();
        void stopListening();

        [[nodiscard]] unsigned short getLocalPort() const;

        // Initialises a client
        void sendInitialCommunicationMessage(const sf::IpAddress& address, unsigned short port, int playerId);

        // Send message to client
        void broadcastMessage(const std::string& message);

        // Disconnect
        void disconnectClient(const ClientKey& key);

    private:
        sf::UdpSocket _m_socket;
        unsigned short _m_local_port{};
        std::thread _m_listening_thread;
        bool _m_is_listening{};
        std::map<ClientKey, int> _m_clients; // Client IPs and Ports
        int _m_next_player_id = 1;

        void handlePacket(sf::Packet& packet, const sf::IpAddress& sender, unsigned short port);

    };
}




#endif //NETWORKMANAGER_H
