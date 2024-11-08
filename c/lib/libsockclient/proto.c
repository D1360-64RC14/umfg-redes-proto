#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "proto.h"

const proto_Method proto_METHOD_ASK = "ASK";
const proto_Method proto_METHOD_REG = "REG";

// Encode given message to a string.
//
// dest parameter MUST BE FREED after use.
//
// Error Codes:
// -1: Method is required
// -2: Endpoint is required
int proto_encode_message(char** dest, proto_Message message)
{
    char* data;
    size_t data_len = 0;

    if (message.method == NULL)
        return -1; // Method is required
    if (message.endpoint == NULL)
        return -2; // Endpoint is required

    size_t method_len = strlen(message.method);
    size_t endpoint_len = strlen(message.endpoint);
    size_t arg_name_len = strlen(message.argument.name);
    size_t arg_value_len = strlen(message.argument.value);

    data_len += method_len;    // <METHOD>
    data_len += 1;             // SPACE
    data_len += endpoint_len;  // <ENDPOINT>
    data_len += 1;             // SPACE
    data_len += arg_name_len;  // <ARG NAME>
    data_len += 1;             // SPACE
    data_len += arg_value_len; // <ARG VALUE>
    data_len += 1;             // NEWLINE
    data_len += 1;             // NULL

    data = (char*)malloc(sizeof(char) * data_len); // WARN: data needs to be freed
    memset(data, '\0', data_len);

    data = strcat(data, message.method);
    data = strcat(data, " ");
    data = strcat(data, message.endpoint);
    data = strcat(data, " ");
    data = strcat(data, message.argument.name);
    data = strcat(data, " ");
    data = strcat(data, message.argument.value);

    *dest = data;
    return 0;
}

// Decode given response from a string.
//
// dest parameter will not be freed.
//
// Error Codes:
// -1: Response is required
// -2: Invalid response
// -3: Status code exceeds buffer size
int proto_decode_response(proto_Response* dest, char* response)
{
    if (response == NULL)
        return -1; // Response is required

    char* response_cur = response; // cursor is at response start

    if (!strcmp(response_cur, "RESPONSE "))
        return -2; // Invalid response

    response_cur += 9; // cursor is at status start

    size_t status_len = strcspn(response_cur, " \n");

    if (status_len >= 16)
        return -3; // Status code exceeds buffer size

    char status_buf[16] = { '\0' };
    strncpy(status_buf, response_cur, status_len);
    dest->status_code = atoi(status_buf);

    response_cur += status_len + 1; // cursor is at content start

    size_t content_len = strcspn(response_cur, " \n");
    dest->message = malloc(sizeof(char) * content_len + 1); // WARN: dest->message needs to be freed
    strncpy(dest->message, response_cur, content_len + 1);

    return 0;
}

// Free given response content.
void proto_free_response(proto_Response response)
{
    free(response.message);
}
