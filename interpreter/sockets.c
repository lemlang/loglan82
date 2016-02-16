#include "sockets.h"

void socket_setup() {
    #if defined WIN32
    WSADATA wsa_data;

	int iResult;

// Initialize Winsock
iResult = WSAStartup(MAKEWORD(2,2), &wsa_data);
if (iResult != 0) {
    printf("WSAStartup failed: %d\n", iResult);
}
    #endif
}

SOCKET socket_connect()
{
    struct sockaddr_in addr;  // local address variable
    SOCKET   s;               // local socket
    int      r;               // will hold return values
    struct hostent* h;               // server host entry (holds IPs, etc)

    // get the server host entry
        memset((void *)&addr, 0, sizeof(addr));
    //addr.sin_addr.s_addr = inet_addr(listen_host);
    //if(INADDR_NONE == addr.sin_addr.s_addr) {
        h = gethostbyname(listen_host);
        if(NULL == h) {

            perror("Could not get host by name");
            perror(listen_host);
            return -1;
        }
    /*} else {
        h = gethostbyaddr((const char*)&addr.sin_addr, sizeof(struct sockaddr_in), AF_INET);
        if(NULL == h) {
            perror("Could not get host by address");
            return -1;
        }
    }*/

    // create the local socket
    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(INVALID_SOCKET == s) {
        perror("Could not create socket\n");
        return INVALID_SOCKET;
    } else {
		DEBUG_PRINT("Socket created %d\n", s);
		perror("Could not create socket");
        
	}

    // setup the rest of our local address
    addr.sin_family = AF_INET;
    addr.sin_addr   = *((struct in_addr*)*h->h_addr_list);
    addr.sin_port   = htons(3600);

    // a little user interaction... ;)
    DEBUG_PRINT("Connecting... %s\n",listen_host);
    //fflush(stdout);

    // connect to the server
    r = connect(s, (struct sockaddr*)&addr, sizeof(struct sockaddr));
    if(SOCKET_ERROR == r) {
        perror("Cannot connect to server\n");
        closesocket(s);
        return -1;
    }
    DEBUG_PRINT("connected. %d\n", r);
    return s;
}


void socket_disconnect(SOCKET socket){
    closesocket(socket);
}
void socket_teardown(){
    #if defined WIN32
    WSACleanup();
    #endif
}
int send_and_select_response(SOCKET connection_socket, MESSAGE* output, MESSAGE* input){
    fd_set rset, wset;
    int communication_result;

    struct timeval tv;
    tv.tv_sec = 30;
    tv.tv_usec = 0;

    FD_ZERO(&wset);
    FD_SET(connection_socket, &wset);

    if ( (communication_result = select(connection_socket+1, NULL, &wset, NULL, &tv) ) > 0) {
        DEBUG_PRINT("send_and_select_response: select returned: %d\n", communication_result);
        communication_result = write ( connection_socket, output, sizeof ( MESSAGE ) );
        DEBUG_PRINT("send_and_select_response: write %d\n",  communication_result );
    } else {
        DEBUG_PRINT("Cannot send_and_select_response: select timeout? %d %d %d\n", tv.tv_sec, tv.tv_usec, communication_result);
        return (-1);
    }

    FD_ZERO(&rset);
    FD_SET(connection_socket, &rset);

    bzero( input, sizeof(MESSAGE ) );



    if ( (communication_result =select(connection_socket+1, &rset, NULL, NULL, &tv) ) > 0)
    {
        DEBUG_PRINT("send_and_select_response: select returned: %d\n",  communication_result );
        communication_result = recv(connection_socket, input, sizeof( MESSAGE ), 0 );
        DEBUG_PRINT("send_and_select_response: recv %d\n",  communication_result );
    } else {
        DEBUG_PRINT("Cannot send_and_select_response: select timeout? %d %d %d\n", tv.tv_sec, tv.tv_usec, communication_result);
        return (-1);
    }
    return 0;
}

int receive_message(SOCKET connection_socket, MESSAGE* input){
    fd_set rset;
    int communication_result;

    struct timeval tv;
    tv.tv_sec = 30;
    tv.tv_usec = 0;

    FD_ZERO(&rset);
    FD_SET(connection_socket, &rset);

    bzero( input, sizeof(MESSAGE ) );



    if ( (communication_result =select(connection_socket+1, &rset, NULL, NULL, &tv) ) > 0)
    {
        DEBUG_PRINT("receive_message: select returned: %d\n",  communication_result );
        communication_result = recv(connection_socket, input, sizeof( MESSAGE ), 0 );
        DEBUG_PRINT("receive_message: recv %d\n",  communication_result );
    } else {
        DEBUG_PRINT("Cannot receive_message: select timeout? %d %d %d\n", tv.tv_sec, tv.tv_usec, communication_result);
        return (-1);
    }
    return 0;
}
int send_message(SOCKET connection_socket, MESSAGE* output){
    fd_set rset, wset;
    int communication_result;

    struct timeval tv;
    tv.tv_sec = 30;
    tv.tv_usec = 0;

    FD_ZERO(&wset);
    FD_SET(connection_socket, &wset);

    if ( (communication_result = select(connection_socket+1, NULL, &wset, NULL, &tv) ) > 0) {
        DEBUG_PRINT("send_message: select returned: %d\n", communication_result);
        communication_result = write ( connection_socket, output, sizeof ( MESSAGE ) );
        DEBUG_PRINT("send_message: write %d\n",  communication_result );
    } else {
        DEBUG_PRINT("Cannot send_message: select timeout? %d %d %d\n", tv.tv_sec, tv.tv_usec, communication_result);
        return (-1);
    }
}
