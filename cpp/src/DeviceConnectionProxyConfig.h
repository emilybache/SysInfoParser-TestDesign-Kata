#ifndef DEVICECONNECTIONPROXYCONFIG_H
#define DEVICECONNECTIONPROXYCONFIG_H

class DeviceConnectionProxyConfig {
public:
    virtual ~DeviceConnectionProxyConfig() = default;
    virtual bool is_master() const = 0;
};

#endif // DEVICECONNECTIONPROXYCONFIG_H
