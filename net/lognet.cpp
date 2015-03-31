#include "../head/genint1.h"
#include "../head/comm.h"

#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <string.h>
#include <qlist.h>
#include <qfile.h>
#include <qstring.h>
#include <qstringlist.h>
#include <unistd.h>

#define REMOTE_PATH "REMOTE"
#define MAXLINKS 30
#define LOGPORT 3600
#define CODEPORT 3700
#define CODEPORT1 3800
#define MAXINTERP 10
#define FILE_BUFFER_SIZE 2048

// ************** Interpreter slot *******************
class INTlink {
public:
    int sock,ID;
    bool connected;
    INTlink();
};

INTlink::INTlink() {
    connected=FALSE;
    sock=0;
}

// ********************  Network slot ********************
class NETlink {
public:
    int sock;
    bool connected,code_transmit;
    char addr[255];

    int node_number;
    int aliases[5];

    FILE *CodeFile;
    char CodeName[255];
    long CodeSize;

    NETlink();
};

NETlink::NETlink() {
    int i;
    for ( i=0; i<5; i++ ) aliases[i]=-1;
    connected=FALSE;
    sock=0;
    code_transmit=FALSE;
}

//********************** NET Module ****************************
class NETMOD {
public:
    int kernel_sock,listen_sock;
    bool all_connected,local_mode;
    int to_connect,MyNode;
    char kername[256];

    QList<INTlink> Interpreters; // List of the Interpeter slots
    QList<NETlink> Links;       //  List of the Network slots

    NETMOD ( char* );

    void load_config ( char* );
    void write_at_console ( char* );
    void send_to_kernel ( MESSAGE* );
    void sock_reopen ( NETlink* );
    void send_connect_info ( NETlink* );
    void send_accept_info ( NETlink* );
    void send_to_node ( NETlink*,MESSAGE* );
    void send_to_int ( MESSAGE* );
    void send_code_ack ( NETlink* );
    void send_to_all ( MESSAGE * );

    NETlink *findNETlink ( int node );
    INTlink *findINTlink ( int id );
    void transmit_file ( int ton, char *fname, int fromINT );
    void propagate_msg ( MESSAGE *msg );
    void check_node ( int,int );

    void run();
    void exit_sequence();
    void NET_EXIT();
    void connect_seq ( char* );
    void accept_connection();
    void remote_messages();
    void check_links();
    void get_message ( NETlink* );
    void conn_info ( int );

    void doitall(); // 2010
};

NETMOD::NETMOD ( char *kernel_name ) {
    int i,len,on;
    struct sockaddr_in svr;
    struct sockaddr_un svr1;
    MESSAGE m;
    char s[256];

    Links.clear();
    Interpreters.clear();

    bzero ( &svr, sizeof ( svr ) );
    listen_sock = socket ( AF_INET, SOCK_STREAM, 0 );
    svr.sin_family = AF_INET;
    svr.sin_addr.s_addr = INADDR_ANY;
    svr.sin_port = htons ( LOGPORT );
    bind ( listen_sock, ( struct sockaddr* ) &svr, sizeof ( svr ) );
    listen ( listen_sock,5 );
    fcntl ( listen_sock, F_SETFL,O_NONBLOCK | fcntl ( listen_sock, F_GETFL,0 ) );

    to_connect=0;
    all_connected=FALSE;
    load_config ( "vlp.cfg" );

    kernel_sock = socket ( AF_UNIX,SOCK_STREAM,0 );
    bzero ( &svr1,sizeof ( svr1 ) );
    svr1.sun_family = AF_UNIX;
    strcpy ( svr1.sun_path,kernel_name );
    strcpy ( kername,kernel_name );
    len = strlen ( svr1.sun_path ) +sizeof ( svr1.sun_family );
    i = connect ( kernel_sock, ( struct sockaddr* ) &svr1,len );
    if ( i==0 )
        fcntl ( kernel_sock,F_SETFL, O_NONBLOCK|fcntl ( kernel_sock,F_GETFL,0 ) );
    on=1;
    setsockopt ( kernel_sock,IPPROTO_TCP,TCP_NODELAY, ( char* ) &on,sizeof ( on ) );
    m.msg_type = MSG_NET;
    m.param.pword[0] = NET_NODE;
    m.param.pword[1] = MyNode;
    send_to_kernel ( &m );

// if (regme) regme_sequence();

    if ( to_connect > 0 ) {
        write_at_console ( "Connecting remote VLPs..." );
        while ( !all_connected ) check_links();
    }
    sprintf ( s,"Local node number %d",MyNode );
    write_at_console ( s );
}

// #####################  Load configuration ##########################
void NETMOD::load_config ( char *fname ) {
    QFile f ( fname );
    QString line;
    QString val;
    int br=0,on,k=0;
    NETlink *pomlink;

    if ( !f.exists() ) {
        write_at_console ( "Cannot load configuration file!" );
        sleep ( 2 );
        exit ( 3 );
    }
    f.open ( IO_ReadOnly );
    br = f.readLine ( line,256 );
    while ( br>0 ) {
        QStringList l = QStringList::split ( "=",line,FALSE );
        QStringList::Iterator it = l.begin();
        line = *it;
        ++it;
        val = *it;
        val = val.stripWhiteSpace();
        if ( line == "node_number" ) {
            MyNode = val.toInt();
        };
        if ( line == "host" ) {
            k++;
            pomlink = new NETlink;
            strcpy ( pomlink->addr,val.ascii() );
            pomlink->connected = FALSE;
            pomlink->sock = socket ( AF_INET, SOCK_STREAM, 0 );
            fcntl ( pomlink->sock, F_SETFL,O_NONBLOCK | fcntl ( pomlink->sock,F_GETFL,0 ) );
            on=1;
            setsockopt ( pomlink->sock,IPPROTO_TCP,TCP_NODELAY, ( char* ) &on,sizeof ( on ) );
            Links.append ( pomlink );
            to_connect++;
        };
        br = f.readLine ( line,256 );
    }
    f.close();
    if ( k==0 ) all_connected=TRUE;
    if ( MyNode==-1 ) {
        write_at_console ( "Node number must be specified" );
        exit ( 1 );
    };
}

void NETMOD::write_at_console ( char *s ) {
    MESSAGE msg;

    msg.msg_type = MSG_NET;
    msg.param.pword[0] = NET_CSWRITELN;
    strcpy ( msg.param.pstr,s );
    send_to_kernel ( &msg );
}

void NETMOD::send_to_kernel ( MESSAGE *msg ) {
    write ( kernel_sock,msg,sizeof ( MESSAGE ) );
}

void NETMOD::send_to_node ( NETlink *lnk, MESSAGE *msg ) {
    msg->msg_type = MSG_NET;
// msg2netmsg(msg);
    if ( lnk->sock )
        write ( lnk->sock,msg,sizeof ( MESSAGE ) );
}

void NETMOD::send_to_int ( MESSAGE *msg ) {
    INTlink *pomlink;

    pomlink = findINTlink ( msg->param.pword[5] );
    if ( pomlink!=NULL ) write ( pomlink->sock,msg,sizeof ( MESSAGE ) );
}

void NETMOD::accept_connection() {
    unsigned int sz;
    int nsock, on;
    struct sockaddr_in svr;
    fd_set rset,wset;
    struct timeval tout = {0,0};
    NETlink *pomlink;

    FD_ZERO ( &rset );
    FD_ZERO ( &wset );
    FD_SET ( listen_sock,&rset );

    if ( select ( listen_sock+1,&rset,&wset,0, ( struct timeval * ) &tout ) >0 )
        if ( FD_ISSET ( listen_sock,&rset ) ) {

            /* accept connection on listen socket */
            sz = sizeof ( svr );
            bzero ( &svr, sizeof ( svr ) );
            nsock = accept ( listen_sock, ( struct sockaddr* ) &svr, &sz );

            if ( nsock>0 ) {

                /* i<0 someone wants to connect us */

                pomlink = new NETlink;
                strcpy ( pomlink->addr,inet_ntoa ( svr.sin_addr ) );
                pomlink->sock = nsock;
                pomlink->connected = TRUE;
                fcntl ( pomlink->sock, F_SETFL,O_NONBLOCK | fcntl ( pomlink->sock,
                        F_GETFL,0 ) );
                on=1;
                setsockopt ( pomlink->sock,IPPROTO_TCP,TCP_NODELAY, ( char* ) &on,sizeof ( on ) );
                Links.append ( pomlink );
            } /* nsock > 0 */
        } /* ISSET */
}

void NETMOD::check_node ( int n, int sc ) {
    MESSAGE m;
    NETlink *pomlink;

    m.msg_type = MSG_NET;
    m.param.pword[0] = NET_NODE_EXIST;

    pomlink = Links.first();
    m.param.pword[1] = 0;
    while ( pomlink!=NULL ) {
        if ( pomlink->node_number==n ) {
            m.param.pword[1] = 1;
            strcpy ( m.param.pstr,pomlink->addr );
            break;
        }
        pomlink = Links.next();
    }
    write ( sc,&m,sizeof ( MESSAGE ) );
}

void NETMOD::get_message ( NETlink *lnk ) {
    int nr;
    MESSAGE msg;
    char pomstr[80];
    int rdbt,rd,sz,j,psock;
    struct sockaddr_in svr;
    unsigned char buffer[FILE_BUFFER_SIZE];
    protdescr proto;

    if ( lnk->connected ) {
        nr = read ( lnk->sock,&msg, sizeof ( MESSAGE ) );
        if ( nr>0 ) {
//     netmsg2msg(&msg);
            if ( msg.msg_type == MSG_NET ) {
                switch ( msg.param.pword[0] ) {
                case NET_CCD_START:

                    lnk->code_transmit = TRUE;
                    sprintf ( pomstr,"%s/%s",REMOTE_PATH,msg.param.pstr );
                    strcat ( pomstr,".ccd" );
                    lnk->CodeFile = fopen ( pomstr,"wb" );
                    if ( lnk->CodeFile == NULL ) {
                        write_at_console ( "Cannot open file\n" );
                        lnk->code_transmit=FALSE;
                    }
                    lnk->CodeSize=msg.param.pword[1];
                    psock = socket ( AF_INET, SOCK_STREAM, 0 );
                    bzero ( &svr, sizeof ( svr ) );
                    svr.sin_family = AF_INET;
                    svr.sin_port = htons ( CODEPORT );
                    svr.sin_addr.s_addr = inet_addr ( lnk->addr );
                    j = connect ( psock, ( struct sockaddr* ) &svr, sizeof ( svr ) );
                    if ( j==0 ) {
                        //fcntl(psock, F_SETFL,O_NONBLOCK | fcntl(psock, F_GETFL,0));
                        sz=0;
                        while ( sz<lnk->CodeSize ) {
                            rd = read ( psock,&buffer,sizeof ( buffer ) );
                            rdbt = fwrite ( &buffer,sizeof ( unsigned char ),rd,lnk->CodeFile );
                            sz=sz+rd;
                        }
                        close ( psock );
                        fclose ( lnk->CodeFile );
                    }

                    break;

                case NET_PCD_START:

                    lnk->code_transmit = TRUE;
                    sprintf ( pomstr,"%s/%s",REMOTE_PATH,msg.param.pstr );
                    strcat ( pomstr,".pcd" );
                    lnk->CodeFile = fopen ( pomstr,"wb" );
                    if ( lnk->CodeFile == NULL ) {
                        write_at_console ( "Cannot open file\n" );
                        lnk->code_transmit=FALSE;
                    }
                    lnk->CodeSize=msg.param.pword[1];
                    psock = socket ( AF_INET, SOCK_STREAM, 0 );
                    bzero ( &svr, sizeof ( svr ) );
                    svr.sin_family = AF_INET;
                    svr.sin_port = htons ( CODEPORT1 );
                    svr.sin_addr.s_addr = inet_addr ( lnk->addr );
                    j = connect ( psock, ( struct sockaddr* ) &svr, sizeof ( svr ) );
                    if ( j==0 ) {
                        //fcntl(psock, F_SETFL,O_NONBLOCK | fcntl(psock, F_GETFL,0));
                        sz=0;
                        while ( sz<lnk->CodeSize ) {
                            rd = read ( psock,&proto,sizeof ( proto ) );
                            rdbt = fwrite ( &proto,sizeof ( unsigned char ),rd,lnk->CodeFile );
                            sz=sz+rd;
                        }
                        close ( psock );
                        fclose ( lnk->CodeFile );
                    }
                    break;
                case NET_CONNECT:
                    sprintf ( pomstr,"Node: %d Addr: %s",msg.param.pword[1],
                              lnk->addr );
                    lnk->node_number = msg.param.pword[1];
                    write_at_console ( pomstr );
                    send_accept_info ( lnk );
                    break;
                case NET_ACCEPT:

                    sprintf ( pomstr,"Node: %d Addr: %s",msg.param.pword[1],
                              lnk->addr );
                    lnk->node_number = msg.param.pword[1];
                    write_at_console ( pomstr );
                    break;
                case NET_DISCONNECT:

                    sprintf ( pomstr,"Node: %d disconnected",msg.param.pword[1] );
                    write_at_console ( pomstr );
                    ::close ( lnk->sock );
                    Links.remove ( lnk );
                    delete ( lnk );
                    break;
                case NET_PROPAGATE:
                    if ( msg.param.pword[1] == MSG_VLP ) send_to_kernel ( &msg );
                    else if ( msg.param.pword[1] == MSG_INT ) send_to_int ( &msg );
                    break;
                } /* end switch */
            }
        } /* nr > 0 */
    } /* end if used */
}

void NETMOD::remote_messages() {
    int max;
    fd_set rset,wset;
    struct timeval tout= {0,0};
    NETlink *pomlink;

    FD_ZERO ( &rset );
    FD_ZERO ( &wset );
    max=0;

    pomlink = Links.first();
    while ( pomlink!=NULL ) {
        if ( pomlink->connected ) {
            FD_SET ( pomlink->sock,&rset );
            if ( max<pomlink->sock ) max=pomlink->sock;
        }
        pomlink=Links.next();
    }

    if ( select ( max+1,&rset,&wset,0, ( struct timeval * ) &tout ) >0 ) {
        pomlink=Links.first();
        while ( pomlink!=NULL ) {
            if ( FD_ISSET ( pomlink->sock,&rset ) ) get_message ( pomlink );
            pomlink=Links.next();
        }
    }
}

/******************************    2010 ********************************************/
void NETMOD::doitall() {
    unsigned int sz;
    int nsock,max=0,on, nr, si, sj;
    struct sockaddr_in svr;
    fd_set rset;
    NETlink *pomlink;
    INTlink *pomlink2;
    struct sockaddr_un svr2;
    MESSAGE msg;

    FD_ZERO ( &rset );
    FD_SET ( listen_sock,&rset );
    max = listen_sock;
    FD_SET ( kernel_sock,&rset );
    if ( max<kernel_sock ) {
        max=kernel_sock;
    }
    pomlink2 = Interpreters.first();
    while ( pomlink2!=NULL ) {
        FD_SET ( pomlink2->sock,&rset );
        if ( max<pomlink2->sock ) max=pomlink2->sock;
        pomlink2=Interpreters.next();
    }
    pomlink = Links.first();
    while ( pomlink!=NULL ) {
        if ( pomlink->connected ) {
            FD_SET ( pomlink->sock,&rset );
            if ( max<pomlink->sock ) max=pomlink->sock;
        }
        pomlink=Links.next();
    }

// odczyt
    if ( select ( max+1,&rset,0,0,NULL ) > 0 ) {
        // accept connection
        if ( FD_ISSET ( listen_sock,&rset ) ) {
            /* accept connection on listen socket */
            sz = sizeof ( svr );
            bzero ( &svr, sizeof ( svr ) );
            nsock = accept ( listen_sock, ( struct sockaddr* ) &svr, &sz );
            if ( nsock>0 ) {
                pomlink = new NETlink;
                strcpy ( pomlink->addr,inet_ntoa ( svr.sin_addr ) );
                pomlink->sock = nsock;
                pomlink->connected = TRUE;
                fcntl ( pomlink->sock, F_SETFL,O_NONBLOCK | fcntl ( pomlink->sock,F_GETFL,0 ) );
                on=1;
                setsockopt ( pomlink->sock,IPPROTO_TCP,TCP_NODELAY, ( char* ) &on,sizeof ( on ) );
                Links.append ( pomlink );
            } /* nsock > 0 */
        } /* ISSET */

        // get internal message
        /* Check request sockets */
        pomlink2 = Interpreters.first();
        while ( pomlink2!=NULL ) {
            if ( FD_ISSET ( pomlink2->sock,&rset ) ) {
                nr = read ( pomlink2->sock,&msg,sizeof ( MESSAGE ) );
                if ( nr>0 ) {
                    if ( msg.msg_type == MSG_NET )
                        switch ( msg.param.pword[0] ) {
                        case NET_PROPAGATE:
                            propagate_msg ( &msg );
                            break;
                        case NET_NODE_EXIST:
                            check_node ( msg.param.pword[1],pomlink2->sock );
                            break;
                        case NET_GET_INFO:
                            conn_info ( pomlink2->sock );
                            break;
                        case NET_NODES_NUM:
                            msg.param.pword[0]=NET_NODES_NUM_RESPONSE;
                            msg.param.pword[1]=Links.count();
                            write ( pomlink2->sock,&msg,sizeof ( MESSAGE ) );
                            break;
                        }/* switch */
                }
            } /* ISSET */
            pomlink2=Interpreters.next();
        } // while
        /* Check internal socket */
        if ( FD_ISSET ( kernel_sock,&rset ) ) {
            nr = read ( kernel_sock, &msg, sizeof ( MESSAGE ) );
            if ( nr>0 ) {
                if ( msg.msg_type == MSG_NET ) {
                    switch ( msg.param.pword[0] ) {
                    case NET_TRANSMIT_CODE:

                        transmit_file ( msg.param.pword[2],msg.param.pstr,msg.param.pword[1] );

                        break;
                    case NET_EXIT: {
                        disconnect_seq();
                        exit_sequence();
                    }
                    break;
                    case NET_GET_INFO:
                        conn_info ( kernel_sock );
                        break;
                    case NET_PROPAGATE:
                        propagate_msg ( &msg );
                        break;
                    case NET_DISCONNECT:
                        disconnect_seq();
                        break;
                    case NET_NODE_EXIST:
                        check_node ( msg.param.pword[1],kernel_sock );
                        break;
                    case NET_CONNECT_TO:
                        connect_seq ( msg.param.pstr );
                    } /* end switch */
                } /* MSG_NET */
                if ( msg.msg_type == MSG_VLP )
                    switch ( msg.param.pword[0] ) {
                    case VLP_REGINT: {

                        pomlink2 = new INTlink;
                        pomlink2->sock = socket ( AF_UNIX,SOCK_STREAM,0 );
                        bzero ( &svr2,sizeof ( svr2 ) );
                        svr2.sun_family = AF_UNIX;
                        strcpy ( svr2.sun_path,msg.param.pstr );
                        si = strlen ( svr2.sun_path ) +sizeof ( svr2.sun_family );
                        sj = connect ( pomlink2->sock, ( struct sockaddr* ) &svr2,si );
                        if ( sj==0 )
                            fcntl ( pomlink2->sock,F_SETFL, O_NONBLOCK|fcntl ( pomlink2->sock,F_GETFL,0 ) );
                        int on=1;
                        setsockopt ( pomlink2->sock,IPPROTO_TCP,TCP_NODELAY, ( char* ) &on,sizeof ( on ) );
                        pomlink2->ID = msg.param.pword[1];
                        pomlink2->connected=TRUE;
                        Interpreters.append ( pomlink2 );

                    };
                    break;
                    case VLP_INTERPRETER_DOWN: {

                        pomlink2 = findINTlink ( msg.param.pword[1] );
                        if ( pomlink2!=NULL ) {
                            close ( pomlink2->sock );
                            Interpreters.remove ( pomlink2 );
                        }
                    };
                    break;
                    } /* MSg_VLP */
            } // nr > 0
        }/* ISSET */

        // get remote message

        pomlink=Links.first();
        while ( pomlink!=NULL ) {
            if ( FD_ISSET ( pomlink->sock,&rset ) ) get_message ( pomlink );
            pomlink=Links.next();
        }
    } // select
}
/******************************    END 2010 ********************************************/

void NETMOD::propagate_msg ( MESSAGE *msg ) {
    char ss[255];
    NETlink *pomlink;

    pomlink = findNETlink ( msg->param.pword[4] );
    if ( ( pomlink!=NULL ) && ( pomlink->connected ) )
        send_to_node ( pomlink,msg );
    else {
        if ( msg->param.pword[1]==MSG_INT ) {
            send_to_int ( msg );
        } else {
            sprintf ( ss,"Not connected to Node %d",msg->param.pword[4] );
            write_at_console ( ss );
        }
    }
}

void NETMOD::connect_seq ( char *a ) {
    NETlink *pom;
    struct sockaddr_in svr;
    int j,on;

    pom=Links.first();
    while ( pom!=NULL ) {
        if ( strcmp ( pom->addr,a ) ==0 ) return;
        pom=Links.next();
    }
    pom = new NETlink;
    strcpy ( pom->addr,a );
    pom->connected = FALSE;
    pom->sock = socket ( AF_INET, SOCK_STREAM, 0 );
    bzero ( &svr, sizeof ( svr ) );
    svr.sin_family = AF_INET;
    svr.sin_port = htons ( LOGPORT );
    svr.sin_addr.s_addr = inet_addr ( pom->addr );
    j = connect ( pom->sock, ( struct sockaddr* ) &svr, sizeof ( svr ) );
    if ( j==0 ) {
        pom->connected = TRUE;
        fcntl ( pom->sock, F_SETFL,O_NONBLOCK | fcntl ( pom->sock,
                F_GETFL,0 ) );
        on=1;
        setsockopt ( pom->sock,IPPROTO_TCP,TCP_NODELAY, ( char* ) &on,sizeof ( on ) );
        send_connect_info ( pom );
        Links.append ( pom );
    } else write_at_console ( "Connection failed" );
}

void NETMOD::check_links() {

    int j=1;
    struct sockaddr_in svr;
    NETlink *pomlink;

    /* connect to all other nodes */
    if ( !all_connected ) {
        pomlink=Links.first();
        while ( pomlink!=NULL ) {
            if ( ! ( pomlink->connected ) ) {
                bzero ( &svr, sizeof ( svr ) );
                svr.sin_family = AF_INET;
                svr.sin_port = htons ( LOGPORT );
                svr.sin_addr.s_addr = inet_addr ( pomlink->addr );

                j = connect ( pomlink->sock, ( struct sockaddr* ) &svr, sizeof ( svr ) );
                if ( j==0 ) {
                    pomlink->connected = TRUE;
                    fcntl ( pomlink->sock, F_SETFL,O_NONBLOCK | fcntl ( pomlink->sock,
                            F_GETFL,0 ) );
                    send_connect_info ( pomlink );

                }
                if ( errno == ECONNREFUSED )
                    sock_reopen ( pomlink );
            } // not connected
            pomlink=Links.next();
        } //while

    }//if
    all_connected=TRUE;
    pomlink=Links.first();
    while ( pomlink!=NULL ) {
        if ( pomlink->connected==FALSE ) {
            all_connected=FALSE;
            break;
        }
        pomlink=Links.next();
    }
}

void NETMOD::sock_reopen ( NETlink *lnk ) {
    int on=1;

    close ( lnk->sock );
    lnk->sock = socket ( AF_INET, SOCK_STREAM, 0 );
    fcntl ( lnk->sock, F_SETFL,O_NONBLOCK | fcntl ( lnk->sock,
            F_GETFL,0 ) );
    setsockopt ( lnk->sock,IPPROTO_TCP,TCP_NODELAY, ( char* ) &on,sizeof ( on ) );
}

// **************** Acknowledges *************************
void NETMOD::send_connect_info ( NETlink *lnk ) {
    MESSAGE m;

    m.param.pword[0] = NET_CONNECT;
    m.param.pword[1] = MyNode;
    m.msg_type = MSG_NET;
// msg2netmsg(&m);
    if ( lnk->sock )
        write ( lnk->sock,&m,sizeof ( MESSAGE ) );
}

void NETMOD::send_accept_info ( NETlink *lnk ) {
    MESSAGE m;

    m.param.pword[0] = NET_ACCEPT;
    m.param.pword[1] = MyNode;
    m.msg_type = MSG_NET;
// msg2netmsg(&m);
    if ( lnk->sock )
        write ( lnk->sock,&m,sizeof ( MESSAGE ) );
}

void NETMOD::send_code_ack ( NETlink *lnk ) {
    MESSAGE m;

    m.param.pword[0] = NET_CODESTREAM_OK;
    m.msg_type = MSG_NET;
// msg2netmsg(&m);
    if ( lnk->sock )
        write ( lnk->sock,&m,sizeof ( MESSAGE ) );
}


void NETMOD::send_to_all ( MESSAGE *msg ) {
    NETlink *pomlink;
    pomlink=Links.first();
    while ( pomlink!=NULL ) {
        write ( pomlink->sock,msg,sizeof ( MESSAGE ) );
        pomlink=Links.next();
    }
}




void NETMOD::run() {
    while ( 1 ) {
        // 2010
        doitall();
    }
}

void NETMOD::exit_sequence() {
    NETlink *pomlink;

    ::close ( kernel_sock );
    ::close ( listen_sock );
    unlink ( kername );
    pomlink = Links.first();
    while ( pomlink!=NULL ) {
        ::close ( pomlink->sock );
        pomlink=Links.next();
    }
    exit ( 0 );
}

void NETMOD::disconnect_seq() {
    MESSAGE m;
    NETlink *p;

    bzero ( &m,sizeof ( MESSAGE ) );
    m.msg_type = MSG_NET;
    m.param.pword[0] = NET_DISCONNECT;
    m.param.pword[1] = MyNode;

    p=Links.first();
    while ( p!=NULL ) {
        send_to_node ( p,&m );
        p=Links.next();
    }
    p=Links.first();
    while ( p!=NULL ) {
        ::close ( p->sock );
        p=Links.next();
    }
    Links.clear();
}

NETlink *NETMOD::findNETlink ( int node ) {
    NETlink *pomlink;
    pomlink=Links.first();
    while ( pomlink!=NULL ) {
        if ( pomlink->node_number == node ) return ( pomlink );
        pomlink=Links.next();
    }
    return ( pomlink );
}

INTlink *NETMOD::findINTlink ( int id ) {
    INTlink *pomlink;
    pomlink=Interpreters.first();
    while ( pomlink!=NULL ) {
        if ( pomlink->ID == id ) return ( pomlink );
        pomlink=Interpreters.next();
    }
    return ( pomlink );
}

/* ----------------    Sending code to a remote node -------------- */
void NETMOD::transmit_file ( int ton, char *fname, int fromINT ) {
    FILE *f;
    MESSAGE msg;
    char fn[80];
    char b[255];
    unsigned char buffer[FILE_BUFFER_SIZE];
    protdescr proto;
    int i,tsock,sock;
    unsigned int sz;
    NETlink *outlink;
    struct sockaddr_in svr;
    fd_set rset,wset;

// **************** CCD FILE
    strcpy ( fn,fname );
    strcat ( fn,".ccd" );
    f = fopen ( fn,"rb" );
    if ( f!=NULL ) {
        fseek ( f,0,SEEK_END );
        msg.param.pword[1] = ftell ( f );
        fclose ( f );
        f = fopen ( fn,"rb" );

        strcpy ( b,rindex ( fname,'/' ) );
        for ( i=0; i<strlen ( b ); i++ )
            b[i] = b[i+1];
        msg.param.pword[0] = NET_CCD_START;
        strcpy ( msg.param.pstr,b );

        outlink = findNETlink ( ton );
        if ( outlink==NULL ) exit ( 1 );
        bzero ( &svr,sizeof ( svr ) );
        sock = socket ( AF_INET,SOCK_STREAM,0 );
        svr.sin_family = AF_INET;
        svr.sin_addr.s_addr = INADDR_ANY;
        svr.sin_port = htons ( CODEPORT );
        bind ( sock, ( struct sockaddr* ) &svr, sizeof ( svr ) );
        listen ( sock,5 );
        send_to_node ( outlink, &msg );
        sz=sizeof ( svr );
        FD_ZERO ( &rset );
        FD_ZERO ( &wset );
        FD_SET ( sock,&rset );
        if ( select ( sock+1,&rset,&wset,0,0 ) )
            if ( FD_ISSET ( sock,&rset ) )
                tsock = accept ( sock, ( struct sockaddr* ) &svr,&sz );
        if ( tsock>0 ) {
            close ( sock );
            while ( !feof ( f ) ) {
                i = fread ( &buffer,1,sizeof ( buffer ),f );
                write ( tsock,&buffer,i );
                FD_ZERO ( &rset );
                FD_ZERO ( &wset );
                FD_SET ( tsock,&wset );
                select ( tsock+1,&rset,&wset,0,0 );
            }
            close ( tsock );
        }
        fclose ( f );
    } // f!=NULL
    else {
        sprintf ( b,"Cannot open file to send %s\n",fname );
        write_at_console ( b );
    }

// *************** PCD FILE
    strcpy ( fn,fname );
    strcat ( fn,".pcd" );
    f = fopen ( fn,"r" );
    if ( f!=NULL ) {
        fseek ( f,0,SEEK_END );
        msg.param.pword[1] = ftell ( f );
        fclose ( f );
        f = fopen ( fn,"rb" );

        strcpy ( b,rindex ( fname,'/' ) );
        for ( i=0; i<strlen ( b ); i++ )
            b[i] = b[i+1];
        msg.param.pword[0] = NET_PCD_START;
        strcpy ( msg.param.pstr,b );

        outlink = findNETlink ( ton );
        if ( outlink==NULL ) exit ( 1 );
        bzero ( &svr,sizeof ( svr ) );
        sock = socket ( AF_INET,SOCK_STREAM,0 );
        svr.sin_family = AF_INET;
        svr.sin_addr.s_addr = INADDR_ANY;
        svr.sin_port = htons ( CODEPORT1 );
        bind ( sock, ( struct sockaddr* ) &svr, sizeof ( svr ) );
        listen ( sock,5 );
        send_to_node ( outlink, &msg );
        sz=sizeof ( svr );
        FD_ZERO ( &rset );
        FD_ZERO ( &wset );
        FD_SET ( sock,&rset );
        if ( select ( sock+1,&rset,&wset,0,0 ) )
            if ( FD_ISSET ( sock,&rset ) )
                tsock = accept ( sock, ( struct sockaddr* ) &svr,&sz );
        if ( tsock>0 ) {
            close ( sock );
            while ( !feof ( f ) ) {
                i = fread ( &proto,1,sizeof ( proto ),f );
                write ( tsock,&proto,i );
                FD_ZERO ( &rset );
                FD_ZERO ( &wset );
                FD_SET ( tsock,&wset );
                select ( tsock+1,&rset,&wset,0,0 );
            }
            close ( tsock );
        }
        fclose ( f );
    } // f!=NULL
    else {
        sprintf ( b,"Cannot open file to send %s\n",fname );
        write_at_console ( b );
    }
    msg.msg_type = MSG_NET;
    msg.param.pword[0] = NET_TRANSMITTED;
    msg.param.pword[1] = fromINT;
    send_to_kernel ( &msg );
}

void NETMOD::conn_info ( int sk ) {
    NETlink *pom;
    MESSAGE m;
    int k=0;
    char poms[255];

    m.msg_type = MSG_NET;
    m.param.pword[0] = NET_INFO;
    strcpy ( m.param.pstr,"" );
    pom=Links.first();
    while ( pom!=NULL ) {
        sprintf ( poms,"%d=%s;",pom->node_number,pom->addr );
        strcat ( m.param.pstr,poms );
        k++;
        if ( k==12 ) {
            m.param.pword[1]=12;
            write ( sk,&m,sizeof ( MESSAGE ) );
            k=0;
        }
        pom=Links.next();
    }
    if ( k>0 ) {
        m.param.pword[1]=k;
        write ( sk,&m,sizeof ( MESSAGE ) );
    }
    m.msg_type = MSG_NET;
    m.param.pword[0] = NET_INFO_END;
    write ( sk,&m,sizeof ( MESSAGE ) );
}

int main ( int argc,char **argv ) {
    NETMOD netter ( argv[1] );
    netter.run();
    return 0;
}
