#ifndef ZMQ_WORKER_HPP
#define ZMQ_WORKER_HPP
#include <string>
#include <memory>
#include <CpperoMQ/All.hpp>
#include <CpperoMQ/OutgoingMessage.hpp>
#include <CpperoMQ/IncomingMessage.hpp>
#include "worker.hpp"
#include "utils.hpp"

class ZMQ_Worker : public Worker
{
public:
    ZMQ_Worker(std::string filepath, Transport _transport=Transport::TCP);
    ZMQ_Worker(std::string filepath, CpperoMQ::Context &context1);
    void run() override;

private:
    uint sent_tiles,received_tiles;
    const Transport _transport;
    CpperoMQ::Context context;
    CpperoMQ::PullSocket pull_socket;
    CpperoMQ::PushSocket push_socket;
    CpperoMQ::RouterSocket ctrl_socket;
    CpperoMQ::SubscribeSocket subscriber_socket;
    bool isDone;
    void connect();
    //void map(TileData* tileData);
    void info() override;
    void send(int) override;
};

#endif // ZMQ_WORKER_HPP
