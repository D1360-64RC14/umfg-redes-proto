#include <stdio.h>
#include <stdlib.h>
#include <proto.h>

#define TEST(fn)                             \
    printf("----------- Testing %s\n", #fn); \
    printf("----------- Ended: Code %d\n", fn());

int test_proto_encode_message()
{
    proto_Message message = {
        .method = proto_METHOD_ASK,
        .endpoint = "epoint",
        .argument = {
            .name = "key",
            .value = "value",
        },
    };

    char* msg;
    int err = proto_encode_message(&msg, message);
    if (err)
        return err;

    if (strcmp(msg, "ASK epoint key value\n"))
        return EXIT_FAILURE;

    free(msg);
    return EXIT_SUCCESS;
}

int test_proto_decode_response()
{
    proto_Response response;

    int err = proto_decode_response(&response, "RESPONSE 200 OK");
    if (err)
        return err;

    if (response.status_code != 200)
        return EXIT_FAILURE * 10;

    if (strcmp(response.message, "OK"))
        return EXIT_FAILURE * 100;

    free(response.message);
    return EXIT_SUCCESS;
}

int main()
{
    TEST(test_proto_encode_message);
    TEST(test_proto_decode_response);
}