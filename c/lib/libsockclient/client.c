#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#include "proto.h"
#include "client.h"

int _client_send_line(const client_Client* const client, const char* const line);
int _client_read_line(const client_Client* const client, char** dest);

#define client_READ_LINE_BUFFER_SIZE 1024

// Create a new client.
//
// ip argument will not be freed.
//
// Error Codes:
// -1: Already connected
// -2: Failed to create socket
// -3: Failed to parse address
int client_new(client_Client* const client, const char* const ip, uint16_t port)
{
    if (client_is_connected(client))
        return -1; // Already connected

    client->_conn_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client->_conn_fd < 0)
        return -2; // Failed to create socket

    client->_addr.sin_family = AF_INET;
    client->_addr.sin_port = htons(port);

    int err = inet_pton(AF_INET, ip, &client->_addr.sin_addr);
    if (err <= 0)
        return -3; // Failed to parse address

    return 0;
}

// Start the connection to the server.
int client_connect(client_Client* const client)
{
    if (client_is_connected(client))
        return -1; // Already connected

    client->_status = connect(
        client->_conn_fd,
        (struct sockaddr*)&client->_addr,
        sizeof(client->_addr)
    );
    if (client->_status)
        return -2; // Failed to connect

    client->_connected = 1;

    return 0;
}

// Check if the client is connected.
int client_is_connected(const client_Client* const client)
{
    return client->_connected;
}

// Close the connection.
void client_close(client_Client* const client)
{
    close(client->_conn_fd);
    client->_connected = 0;
}

// Sends a message to the server, returning the response.
//
// message argument will not be freed.
//
// Error Codes:
// -1: Not connected
// -2: Failed to encode message
// -3: Failed to send message
// -4: Failed to read response
// -5: Failed to decode response
int client_send_message(const client_Client* const client, proto_Response* response, proto_Message message)
{
    int err;

    if (!client_is_connected(client))
        return -1; // Not connected

    char* msg_str;
    err = proto_encode_message(&msg_str, message); // WARN: msg_str needs to be freed
    if (err)
        return -2; // Failed to encode message

    err = _client_send_line(client, msg_str);
    if (err)
    {
        free(msg_str); // NOTE: freeing here
        return -3; // Failed to send message
    }
    free(msg_str); // NOTE: freeing here

    char* response_line;
    err = _client_read_line(client, &response_line); // WARN: response_line needs to be freed
    if (err < 0)
        return -4; // Failed to read response

    err = proto_decode_response(response, response_line);
    if (err)
    {
        free(response_line); // NOTE: freeing here
        return -5; // Failed to decode response
    }
    free(response_line); // NOTE: freeing here

    return 0;
}

// Send text line to the server.
//
// line argument will not be freed.
//
// Error Codes:
// -1: Interrupted
// -2: Connection reset
int _client_send_line(const client_Client* const client, const char* const line)
{
    ssize_t size_or_err = send(client->_conn_fd, line, strlen(line), 0);

    if (size_or_err == EINTR)
        return -1; // Interrupted

    if (size_or_err == ECONNRESET)
        return -2; // Connection reset

    return 0;
}

// Read text line from the server.
//
// dest argument MUST BE FREED after use.
//
// Error Codes:
// -1: Interrupted
// -2: Buffer overflow
int _client_read_line(const client_Client* const client, char** dest)
{
    char buffer[client_READ_LINE_BUFFER_SIZE + 1] = { '\0' };
    ssize_t size_or_err = 0;

    size_or_err = read(client->_conn_fd, &buffer, client_READ_LINE_BUFFER_SIZE);

    if (size_or_err == EINTR)
        return -1; // Interrupted

    if (size_or_err >= client_READ_LINE_BUFFER_SIZE)
        return -2; // Buffer overflow

    *dest = (char*)malloc(sizeof(char) * (size_or_err + 1)); // WARN: *dest needs to be freed
    memcpy(*dest, buffer, size_or_err + 1);

    return 0;
}