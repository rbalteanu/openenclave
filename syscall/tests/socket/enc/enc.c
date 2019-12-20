/* Copyright (c) Open Enclave SDK contributors. */
/* Licensed under the MIT License. */

#include <openenclave/enclave.h>
#include <openenclave/internal/time.h>
#include <syscall/module.h>

// enclave.h must come before socket.h
#include <arpa/inet.h>
#include <netinet/in.h>
#include <openenclave/internal/tests.h>
#include <sys/socket.h>
#include <unistd.h>

#include <socket_test_t.h>
#include <stdio.h>
#include <string.h>

static void _initialize()
{
    OE_TEST(oe_load_module_host_socket_interface() == OE_OK);

    {
        char buf[1024] = {0};
        OE_TEST(gethostname(buf, sizeof(buf)) == 0);
        printf("hostname=%s\n", buf);
        OE_TEST(strlen(buf) > 0);
    }

    {
        char buf[1024] = {0};
        OE_TEST(getdomainname(buf, sizeof(buf)) == 0);
        printf("domainname=%s\n", buf);
        //  OE_TEST(strlen(buf) > 0); Always fails on jenkins
    }
}

/* This client connects to an echo server, sends a text message,
 * and outputs the text reply.
 */
int ecall_run_client(char* recv_buff, ssize_t* recv_buff_len)
{
    _initialize();
    int sockfd = 0;
    ssize_t n = 0;
    size_t buff_len = (size_t)*recv_buff_len;
    struct sockaddr_in serv_addr = {0};

    memset(recv_buff, '0', buff_len);
    printf("create socket\n");
    if ((sockfd = oe_socket(OE_AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return OE_FAILURE;
    }
    serv_addr.sin_family = OE_AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    serv_addr.sin_port = htons(1492);

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
            close(sockfd);
            return OE_FAILURE;
        }
        else
        {
            printf("Connect Failed. Retrying \n");
        }
    }

    int sockdup = dup(sockfd);

    printf("reading...\n");
    n = read(sockdup, recv_buff, buff_len);

    *recv_buff_len = n;
    if (n > 0)
    {
        printf("finished reading: %ld bytes...\n", n);
    }
    else
    {
        printf("Read error, Fail\n");
        close(sockfd);
        oe_host_printf("fail close\n");
        return OE_FAILURE;
    }

    oe_host_printf("success close\n");
    close(sockfd);
    close(sockdup);
    return OE_OK;
}

/* This server acts as an echo server.  It accepts a connection,
 * receives messages, and echoes them back.
 */
int ecall_run_server()
{
    _initialize();
    int status = OE_FAILURE;
    static const char TESTDATA[] = "This is TEST DATA\n";
    int listenfd = oe_socket(OE_AF_INET, SOCK_STREAM, 0);
    int connfd = 0;
    struct sockaddr_in serv_addr = {0};

    const int optVal = 1;
    const oe_socklen_t optLen = sizeof(optVal);
    int rtn = oe_setsockopt(
        listenfd, OE_SOL_SOCKET, OE_SO_REUSEADDR, (void*)&optVal, optLen);
    if (rtn > 0)
    {
        printf("oe_setsockopt failed errno = %d\n", errno);
    }

    serv_addr.sin_family = OE_AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    serv_addr.sin_port = htons(1493);

    printf("enclave: binding\n");
    rtn = oe_bind(listenfd, (struct oe_sockaddr*)&serv_addr, sizeof(serv_addr));
    if (rtn < 0)
    {
        printf("bind error errno = %d\n", errno);
    }
    oe_host_printf("enclave: listening\n");
    rtn = oe_listen(listenfd, 10);
    if (rtn < 0)
    {
        printf("listen error errno = %d\n", errno);
    }

    while (1)
    {
        oe_sleep_msec(1);
        printf("enc: accepting\n");

        struct sockaddr_in peer_addr = {0};
        oe_socklen_t peer_addr_len = sizeof(peer_addr);
        connfd = oe_accept(
            listenfd, (struct oe_sockaddr*)&peer_addr, &peer_addr_len);
        OE_TEST(peer_addr_len == sizeof(peer_addr));
        OE_TEST(peer_addr.sin_family == OE_AF_INET);
        OE_TEST(ntohs(peer_addr.sin_port) >= 1024);
        OE_TEST(ntohl(peer_addr.sin_addr.s_addr) == INADDR_LOOPBACK);

        if (connfd >= 0)
        {
            printf("enc: accepted fd = %d\n", connfd);
            do
            {
                oe_host_printf("enclave: accepted\n");
                ssize_t n = write(connfd, TESTDATA, strlen(TESTDATA));
                if (n > 0)
                {
                    printf("write test data n = %ld\n", n);
                    close(connfd);
                    break;
                }
                else
                {
                    printf("write test data n = %ld errno = %d\n", n, errno);
                }
                oe_sleep_msec(3);
            } while (1);

            break;
        }
        else
        {
            printf("enc: accept failed errno = %d \n", errno);
        }
    }

    close(listenfd);
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
