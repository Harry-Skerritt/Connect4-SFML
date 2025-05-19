//
// Created by Harry Skerritt on 19/05/2025.
//

#include "NetworkManager.h"
#include <iostream>

namespace Network {

    bool ClientKey::operator==(const ClientKey &other) const {
        return (address == other.address) && (port == other.port);
    }

    bool ClientKey::operator<(const ClientKey& other) const {
        return std::tie(address, port) < std::tie(other.address, other.port);
    }

    NetworkManager::NetworkManager() : _m_socket(sf::UdpSocket()) {
        // Bind to any port - get the port thats bound
        if (_m_socket.bind(sf::Socket::AnyPort) != sf::Socket::Status::Done) {
            std::cerr << "Failed to bind socket" << std::endl;
            // Todo: Throw exception
        }

        _m_local_port = _m_socket.getLocalPort();
        std::cout << "Server listening on port " << _m_local_port << std::endl;
        _m_socket.setBlocking(false);
    }

    NetworkManager::~NetworkManager() {
        // Disconnect all
        for (auto& client : _m_clients) {
            disconnectClient(client.first);
        }
    }

    void NetworkManager::startListening() {
        // Lambda function for the listening thread.
        _m_listening_thread = std::thread([this]() {
            while (_m_is_listening) {
                // Receive packets.
                sf::Packet packet;
                std::optional<sf::IpAddress> sender;
                unsigned short port;

                sf::Socket::Status status = _m_socket.receive(packet, sender, port);
                if (status == sf::Socket::Status::Done) {
                    handlePacket(packet, sender.value(), port);
                }
                else if (status == sf::Socket::Status::NotReady) {
                    // No data was received, which is normal in non-blocking mode.
                    // We don't need to do anything here, but you could add a small sleep
                    // to prevent the loop from consuming too much CPU time if needed.
                    // std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
                else {
                    std::cerr << "Error receiving packet: " << static_cast<int>(status) << std::endl;
                    // Handle other errors (e.g., Disconnected, Error) as appropriate for your application.
                    // You might want to disconnect the client or take other corrective action.
                    if (status == sf::Socket::Status::Disconnected)
                    {
                         ClientKey clientKey = {sender.value(), port};
                         if(_m_clients.find(clientKey) != _m_clients.end())
                         {
                            disconnectClient(clientKey);
                         }
                    }
                }
            }
        });
    }

    void NetworkManager::stopListening() {
        _m_is_listening = false;
        if (_m_listening_thread.joinable()) {
            _m_listening_thread.join();
        }
    }

    unsigned short NetworkManager::getLocalPort() const {
        return _m_local_port;
    }

    void NetworkManager::sendInitialCommunicationMessage(const sf::IpAddress &address, unsigned short port, int playerId) {
        sf::Packet packet;
        packet << "connect_ok" << playerId << _m_local_port;
        if (_m_socket.send(packet, address, port) != sf::Socket::Status::Done) {
            std::cerr << "Failed to send connect confirmation" << std::endl;
        }
    }

    void NetworkManager::broadcastMessage(const std::string &message) {
        sf::Packet packet;
        packet << "message" << message;
        for (const auto &client : _m_clients) {
            if (_m_socket.send(packet, client.first.address, client.first.port) != sf::Socket::Status::Done) {
                std::cerr << "Failed to send message" << std::endl;
            }
        }
    }

    void NetworkManager::disconnectClient(const ClientKey &key) {
        sf::Packet packet;
        packet << "disconnect";
        if (_m_socket.send(packet, key.address, key.port) != sf::Socket::Status::Done) {
            std::cerr << "Failed to send disconnect message" << std::endl;
        }
        _m_clients.erase(key);
    }

    void NetworkManager::handlePacket(sf::Packet &packet, const sf::IpAddress &sender, unsigned short port) {
        std::string type;
        packet >> type;

        ClientKey client_key = { sender, port };

        // Check if the client is connected
        if (type == "connect") {
            std::cout << "Received connection request from " << sender << ":" << port << std::endl;

            if (_m_clients.find(client_key) == _m_clients.end()) {
                // Assign a new player id
                int player_id = _m_next_player_id++;
                _m_clients[client_key] = player_id;

                sendInitialCommunicationMessage(sender, port, player_id);
                std::cout << "Client " << player_id << " connected" << std::endl;
                broadcastMessage("Player " + std::to_string(player_id) + " connected");
            }
            else {
                std::cout << "Client already connected" << std::endl;
            }
        }
        else if (type == "disconnect") {
            // Handle disconnection
            std::cout << "Received disconnect message from " << sender << ":" << port << std::endl;
            if (_m_clients.find(client_key) == _m_clients.end()) {
                int player_id = _m_next_player_id++;
                _m_clients.erase(client_key);
                std::cout << "Client disconnected, ID was " << player_id << std::endl;
                broadcastMessage("Player " + std::to_string(player_id) + " disconnected");
            }
            else {
                std::cout << "Client was not connected" << std::endl;
            }
        }
        // Put other handles in here for handling other data
        /* EG:
        else if (type == "fire") {
                //  handling fire data
                int playerId;
                float angle;
                packet >> playerId >> angle;
                std::cout << "Received fire from player " << playerId << ", angle=" << angle << std::endl;
                //  create a new bullet in your game world.
                // world_.addBullet(playerId, angle);
            }

         */
        else if (type == "message") {
            std::string message;
            packet >> message;
            std::cout << "Received message: " << message << std::endl;
            broadcastMessage("Relayed Message: " + message);
        }
        else {
            // Handle unknown packet type.
            std::cout << "Received unknown packet type from " << sender << ":" << port << std::endl;
        }

    }

}
