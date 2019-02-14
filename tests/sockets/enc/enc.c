/* Copyright (c) Microsoft Corporation. All rights reserved. */
/* Licensed under the MIT License. */

#include <openenclave/enclave.h>
#include <openenclave/internal/time.h>

// enclave.h must come before socket.h
#include <openenclave/bits/socket.h>
#include <openenclave/internal/device.h>
#include <openenclave/internal/host_socket.h>

#include <socket_test_t.h>
#include <stdio.h>
#include <string.h>

int ecall_device_init()
{
    (void)oe_allocate_devid(OE_DEVID_HOST_SOCKET);
    (void)oe_set_devid_device(OE_DEVID_HOST_SOCKET, oe_socket_get_hostsock());
    return 0;
}

/* This client connects to an echo server, sends a text message,
 * and outputs the text reply.
 */
int ecall_run_client(char* recv_buff, ssize_t* recv_buff_len)
{
    int sockfd = 0;
    ssize_t n = 0;
    size_t buff_len = (size_t)*recv_buff_len;
    struct oe_sockaddr_in serv_addr = {0};

    memset(recv_buff, '0', buff_len);
    printf("create socket\n");
    if ((sockfd = oe_socket(OE_AF_INET, OE_SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return OE_FAILURE;
    }
    serv_addr.sin_family = OE_AF_INET;
    serv_addr.sin_addr.s_addr = oe_htonl(OE_INADDR_LOOPBACK);
    serv_addr.sin_port = oe_htons(1492);

    printf("socket fd = %d\n", sockfd);
    printf("Connecting...\n");
    int retries = 0;
    static const int max_retries = 4;
    while (oe_connect(
               sockfd, (struct oe_sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        if (retries++ > max_retries)
        {
            printf("\n Error : Connect Failed \n");
            oe_close(sockfd);
            return OE_FAILURE;
        }
        else
        {
            printf("Connect Failed. Retrying \n");
        }
    }

    printf("reading...\n");
    n = oe_read(sockfd, recv_buff, buff_len);
    *recv_buff_len = n;
    if (n > 0)
    {
        printf("finished reading: %ld bytes...\n", n);
    }
    else
    {
        printf("Read error, Fail\n");
        oe_close(sockfd);
        return OE_FAILURE;
    }

    oe_close(sockfd);
    return OE_OK;
}

/* This server acts as an echo server.  It accepts a connection,
 * receives messages, and echoes them back.
 */
int ecall_run_server()
{
    int status = OE_FAILURE;
    const static char TESTDATA[] = "This is TEST DATA\n";
    int listenfd = oe_socket(OE_AF_INET, OE_SOCK_STREAM, 0);
    int connfd = 0;
    struct oe_sockaddr_in serv_addr = {0};

    const int optVal = 1;
    const oe_socklen_t optLen = sizeof(optVal);
    int rtn = oe_setsockopt(
        listenfd, OE_SOL_SOCKET, OE_SO_REUSEADDR, (void*)&optVal, optLen);
    if (rtn > 0)
    {
        printf("oe_setsockopt failed errno = %d\n", oe_errno);
    }

    serv_addr.sin_family = OE_AF_INET;
    serv_addr.sin_addr.s_addr = oe_htonl(OE_INADDR_LOOPBACK);
    serv_addr.sin_port = oe_htons(1493);

    printf("accepting\n");
    oe_bind(listenfd, (struct oe_sockaddr*)&serv_addr, sizeof(serv_addr));
    oe_listen(listenfd, 10);

    while (1)
    {
        oe_sleep(1);
        printf("accepting\n");
        connfd = oe_accept(listenfd, (struct oe_sockaddr*)NULL, NULL);
        if (connfd >= 0)
        {
            printf("accepted fd = %d\n", connfd);
            do
            {
                ssize_t n = oe_write(connfd, TESTDATA, strlen(TESTDATA));
                if (n > 0)
                {
                    printf("write test data n = %ld\n", n);
                    oe_close(connfd);
                    break;
                }
                else
                {
                    printf("write test data n = %ld errno = %d\n", n, oe_errno);
                }
                oe_sleep(3);
            } while (1);
            break;
        }
        else
        {
            printf("accept failed errno = %d \n", oe_errno);
        }
    }

    oe_close(listenfd);
    printf("exit from server thread\n");
    return status;
}

OE_SET_ENCLAVE_SGX(
    1,    /* ProductID */
    1,    /* SecurityVersion */
    true, /* AllowDebug */
    256,  /* HeapPageCount */
    256,  /* StackPageCount */
    1);   /* TCSCount */
