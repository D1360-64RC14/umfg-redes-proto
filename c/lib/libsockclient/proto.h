#ifndef LIBSOCKCLIENT_PROTO_H
#define LIBSOCKCLIENT_PROTO_H

#define proto_ARGUMENT_BLANK { .name = "", .value = "" }

typedef const char* const proto_Method;

extern const proto_Method proto_METHOD_ASK;
extern const proto_Method proto_METHOD_REG;

typedef struct Argument
{
    char* name;
    char* value;
} proto_Argument;

typedef struct Message
{
    proto_Method method;
    char* endpoint;
    proto_Argument argument;
} proto_Message;

typedef struct Response
{
    int status_code;
    char* message;
} proto_Response;

int proto_encode_message(char** dst, proto_Message message);
int proto_decode_response(proto_Response* dst, char* response);

void proto_free_response(proto_Response response);

#endif