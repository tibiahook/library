#ifndef CONCURRENTPROXY_H
#define CONCURRENTPROXY_H

#include <QtConcurrentRun>

#include <PacketReader.h>
#include <ProxyInterface.h>

class ConcurrentProxy: public ProxyInterface {
public:
    virtual ~ConcurrentProxy() {}

    virtual void handlePacketConcurrent(PacketReader& reader) = 0;

    bool handlePacket(PacketReader& reader) {
        QtConcurrent::run(this, &ConcurrentProxy::handlePacketConcurrent, reader);

        return true;
    }
};

#endif
