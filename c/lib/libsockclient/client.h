#ifndef LIBSOCKCLIENT_CLIENT_H
#define LIBSOCKCLIENT_CLIENT_H

#include <arpa/inet.h>

typedef struct
{
    int read_timeout_ms;
} client_Config;

typedef struct
{
    client_Config config;
    uint8_t _connected;
    int _status;
    int _conn_fd;
    struct sockaddr_in _addr;
} client_Client;

int client_new(client_Client* const client, const char* const ip, uint16_t port);
int client_connect(client_Client* const client);
int client_is_connected(const client_Client* const client);

int client_send_message(const client_Client* const client, proto_Response* response, proto_Message message);

void client_close(client_Client* const client);

#endif
