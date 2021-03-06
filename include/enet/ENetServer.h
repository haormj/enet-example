#pragma once

#include "net/DeliveryType.h"
#include "net/Message.h"
#include "net/Server.h"

#include <enet/enet.h>

#include <iostream>
#include <map>
#include <memory>
#include <vector>

class ENetServer : public Server {

public:
    typedef std::shared_ptr<ENetServer> Shared;
    static Shared alloc();

    ENetServer();
    ~ENetServer();

    bool start(uint32_t);
    bool stop();
    bool isRunning() const;

    uint32_t numClients() const;

    void send(uint32_t, DeliveryType, StreamBuffer::Shared) const;
    void broadcast(DeliveryType, StreamBuffer::Shared) const;
    std::vector<Message::Shared> poll();

    void on(uint32_t, RequestHandler);

private:
    ENetPeer* getClient(uint32_t) const;
    void sendMessage(uint32_t id, DeliveryType type, Message::Shared msg) const;
    void broadcastMessage(DeliveryType type, Message::Shared msg) const;
    void sendResponse(uint32_t, uint32_t requestId, StreamBuffer::Shared stream) const;
    void handleRequest(uint32_t, uint32_t, StreamBuffer::Shared stream) const;

    // prevent copy-construction
    ENetServer(const ENetServer&);
    // prevent assignment
    ENetServer& operator=(const ENetServer&);

    ENetHost* host_;
    // NOTE: ENet allocates all peers at once and doesn't shuffle them,
    // which leads to non-contiguous connected peers. This map
    // will make it easier to manage them by id
    std::map<uint32_t, ENetPeer*> clients_;
    std::vector<Message::Shared> queue_;
    std::map<uint32_t, RequestHandler> handlers_;
    mutable uint32_t currentMsgId_;
};

std::string addressToString(const ENetAddress* address);
