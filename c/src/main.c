#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include <libsockclient/proto.h>
#include <libsockclient/client.h>

const char* const CONN_HOST = "0.0.0.0";
const uint16_t    CONN_PORT = 9000;

const char* const CRED_RA = "1424";
const char* const CRED_PASSWORD = "abrobrinha";

int ask_ip(const client_Client* const client);
int reg_user(const client_Client* const client);
int ask_secret(const client_Client* const client);
int check_response(const proto_Response response);

int main()
{
    client_Client client = { 0 };
    int err = client_new(&client, CONN_HOST, CONN_PORT);
    if (err)
    {
        printf("Failed to create client\n");
        return EXIT_FAILURE;
    }

    printf("Client created!\n");

    err = client_connect(&client);
    if (err)
    {
        printf("Failed to connect to server\n");
        return EXIT_FAILURE;
    }

    printf("Connected to server!\n");

    err = ask_ip(&client);
    if (err)
    {
        printf("Failed to ask for client's IP\n");
        return EXIT_FAILURE;
    }

    err = reg_user(&client);
    if (err)
    {
        printf("Failed to register user\n");
        return EXIT_FAILURE;
    }

    err = ask_secret(&client);
    if (err)
    {
        printf("Failed to ask for secret\n");
        return EXIT_FAILURE;
    }

    client_close(&client);

    return EXIT_SUCCESS;
}

int ask_ip(const client_Client* const client)
{
    int err;

    proto_Message message = {
        .method = proto_METHOD_ASK,
        .endpoint = "IP",
        .argument = proto_ARGUMENT_BLANK
    };
    proto_Response response;

    err = client_send_message(client, &response, message);
    if (err)
    {
        proto_free_response(response);
        return -1;
    }

    err = check_response(response);
    if (err)
    {
        proto_free_response(response);
        return -2;
    }

    printf("Client's IP: %s\n", response.message);

    proto_free_response(response);
    return 0;
}

int reg_user(const client_Client* const client)
{
    int err;

    proto_Message message = {
        .method = proto_METHOD_REG,
        .endpoint = "USER",
        .argument = {
            .name = CRED_RA,
            .value = CRED_PASSWORD
        }
    };
    proto_Response response;

    err = client_send_message(client, &response, message);
    if (err)
    {
        proto_free_response(response);
        return -1;
    }

    err = check_response(response);
    if (err)
    {
        proto_free_response(response);
        return -2;
    }

    printf("Registration: SUCCESS\n");

    proto_free_response(response);
    return 0;
}

int ask_secret(const client_Client* const client)
{
    int err;

    proto_Message message = {
        .method = proto_METHOD_ASK,
        .endpoint = "SECRET",
        .argument = {
            .name = CRED_RA,
            .value = CRED_PASSWORD
        }
    };
    proto_Response response;

    err = client_send_message(client, &response, message);
    if (err)
    {
        proto_free_response(response);
        return -1;
    }

    err = check_response(response);
    if (err)
    {
        proto_free_response(response);
        return -2;
    }

    printf("Secret: %s\n", response.message);

    proto_free_response(response);
    return 0;
}

int check_response(const proto_Response response)
{
    if (response.status_code == 200)
        return 0;

    printf("--- Status %d ---\n", response.status_code);
    printf("Response Message: %s\n", response.message);
    printf("------------------\n");

    return -1;
}