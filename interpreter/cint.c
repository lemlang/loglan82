#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <errno.h>

#include "depend.h"
#include "genint.h"
#include "int.h"
#include "process.h"
#include "intproto.h"
#include "sockets.h"
#include "debug.h"
#include "../head/comm.h"

static void load(char *);

static void initiate(int, char **);

int main(int, char **);

bool has_network_socket();

int internal_sock, graph_sock, net_sock, connected = 0;
SOCKET network_socket = INVALID_SOCKET;
struct sockaddr_in svr;
int GraphRes = -1;
char ProgName[255];

ctx_struct my_ctx;
ctx_struct parent_ctx;

int RInstance[255];
/* IDs of remote instances */
int DirConn[255]; /* Direct connection channels */

/* graphic vars */
int fcol, bcol, curx = 0, cury = 0;

/* Macro to decode addressing modes : */
#define getargument(a, argnr)                                       \
    switch (eop->args[ argnr ])                                     \
    {                                                               \
        case GLOBAL     : a = M[ ic++ ];             break;         \
        case LOCAL      : a = c1+M[ ic++ ];          break;         \
        case TEMPLOCAL  : a = c2+M[ ic++ ];          break;         \
        case REMOTE     : a = M[ M[ ic+1 ] ]+M[ ic ];  ic+=2; break;\
        case INDIRECT   : a = M[ M[ ic++ ] ];                 break;\
        case IMMEDIATE  : a = ic++;                           break;\
        case CONSTANT  : a = M[ ic++ ];                       break;\
        case DOTACCESS : a = M[ display+M[ ic+1 ] ]+M[ ic ];  ic += 2; break;\
        case NOARGUMENT : return;                          \
    }

/* Load code and prototypes from file */
static void load(char *_filename) {
    FILE *fp;
    char *cp;
    word n, left;
    char filename[100]; /* should suffice on all systems */

    strcpy(filename, _filename);
    M = mallocate (memorysize + 1); /* allocate main memory array */
    if (M == NULL) abend("Memory size too large (use /m option)\n");

    addext(filename, ".ccd");
    if ((fp = fopen(filename, BINARYREAD)) == NULL) {
        fprintf(stderr, "Cannot open .ccd file\n");
        endrun(10);
    };

    ic = 0; /* read static data and code */
    left = memorysize + 1; /* from .ccd file */
    do {
        if (left == 0) abend("Memory size too small (use /m option)\n");
        n = min (IOBLOCK / sizeof(word), left);
        n = fread((char *) &M[ic], sizeof(word), (int) n, fp);
        ic += n;
        left -= n;
    } while (n != 0); /* now ic = number of words read */

    fclose(fp);
    /* Get various addresses passed by GENERATOR */
    ipradr = M[ic - 5]; /* primitive type desctriptions */
    temporary = M[ic - 4]; /* global temporary variables */
    strings = M[ic - 3]; /* string constants */
    lastprot = M[ic - 2]; /* last prototype number */
    freem = M[ic - 1]; /* first free word in memory */

    /* Read prototypes from .pcd file */
    addext(filename, ".pcd");
    if ((fp = fopen(filename, BINARYREAD)) == NULL) {
        fprintf(stderr, "Cannot open .pcd file\n");
        endrun(10);
    }
    for (n = MAINBLOCK; n <= lastprot; n++) {
        cp = ballocate (sizeof(protdescr));
        if (cp == NULL) abend("Memory size too large (use /m option)\n");
        prototype[n] = (protdescr *) cp;
        if (fread(cp, sizeof(protdescr), 1, fp) != 1)
            abend("Cannot read .pcd file\n");
    }
    fclose(fp);

    /* Open trace file */
    if (debug) {
        addext(filename, ".trd");
        if ((tracefile = fopen(filename, "w")) == NULL)
            abend("Cannot open .trd file\n");
    }
} /* end load */

/* Establish configuration parameters */
static void initiate(int argc, char **argv) {
    int c, i;
    remote = FALSE;
    ournode = 0;
    network = TRUE;

    while (optind < argc) {
        if ((c = getopt(argc, argv, "r:i:")) != -1) {
            // Option argument
            switch (c) {
                case 'i': {
                    strcpy(listen_host, optarg);
                    break;
                }
                case 'r': {
                    remote = TRUE;
                    remote_id = atoi(optarg);
                    break;
                }
                default:
                    break;
            }
        } else {
                // Regular argument
                strcpy(ProgName, argv[optind]);
                optind++;  // Skip to the next argument
        }
    }
    for (i = 0; i < 255; i++) {
        RInstance[i] = -1;
        DirConn[i] = -1;
    }

    if (ProgName != NULL) {
        load(ProgName); /* load code and prototypes */
    } else {
        printf("FATAL ERROR: Cannot read program name.\n\nUsage: %s [-r] [-i 127.0.0.1|hostname] program-name.log\n", argv[0]);
        exit(8);
    }
}

void decode() {
    extopcode *eop;

    eop = (extopcode *) (M + ic); /* pointer to extended opcode in M */
    lastic = ic; /* save ic for possible redecoding */
    ic += APOPCODE;
    opcode = ((int) eop->opcode) & 0xFF;
    getargument (a1, 0);
    getargument (a2, 1);
    getargument (a3, 2);
}

void send_to_graph(MESSAGE *msg) {
    if (has_network_socket()) {
        write(network_socket, msg, sizeof(MESSAGE));
    } else {
        errsignal(RTEVMCONERR);
    }
}

/* Get graphic resource number */
int get_graph_res() {
    MESSAGE in, out;
    int i;

    in.msg_type = MSG_GRAPH;
    in.param.pword[0] = GRAPH_ALLOCATE;
    DEBUG_PRINT("Sending request to kernel\n\t type %d;\tparam.pword[0] %d\n", in.msg_type, in.param.pword[0]);
    send_and_select_response(network_socket, &in, &out);


    if (out.msg_type == MSG_GRAPH && out.param.pword[0] == GRAPH_ALLOCATED) {
        graphics = TRUE;
        DEBUG_PRINT("Graphics resource allocated\n");
        return (0);
    } else if (out.msg_type == MSG_GRAPH && out.param.pword[0] == GRAPH_INACCESSIBLE) {
        graphics = FALSE;
        DEBUG_PRINT("Cannot allocate graphics resource\n");
        return (1);
    } else {
        graphics = FALSE;
        DEBUG_PRINT("Wrong answer for GRAPH_ALLOCATE; should throw protocol exception (msg type: %d pword 0: %d\n", out.msg_type, out.param.pword[0]);
        return (-1);
    }
}

/* get_graph_res */

void graph_setstatus() {
    MESSAGE m1;
    m1.msg_type = MSG_GRAPH;
    m1.param.pword[0] = GRAPH_SET_TITLE;
    m1.param.pword[1] = GraphRes;
    sprintf(m1.param.pstr,
            "%s      ID: %d", ProgName, my_ctx.program_id);
    if (remote) strcat(m1.param.pstr, "  REMOTE instance");
    send_message(network_socket, &m1);
}
/*
 * 1. todo przerobić int tak aby przy braku zasobu graficznego wyświetlał informacje na konsoli
 * 2. implementacja modułu graficznego
 */
/* writeln string */
void writeln_str(char *s) {
    if (has_network_socket()) {
        MESSAGE msg;
        msg.msg_type = MSG_GRAPH;
        msg.param.pword[1] = GraphRes;
        msg.param.pword[2] = GRAPH_WRITE;
        strcpy(msg.param.pstr, s);
        send_to_graph(&msg);
        strcpy(msg.param.pstr, "\n");
        send_to_graph(&msg);
    } else {
        printf("%s\n", s);
    }
}

bool has_network_socket() {
    if (network_socket != INVALID_SOCKET) {
        return 1;
    } else {
        return 0;
    }
}

/* write string */
void write_str(char *s) {
    if (has_network_socket()) {
        MESSAGE msg;
        msg.msg_type = MSG_GRAPH;
        msg.param.pword[1] = GraphRes;
        msg.param.pword[0] = GRAPH_WRITE;
        strcpy(msg.param.pstr, s);
        send_to_graph(&msg);
    } else {
        printf("%s", s);
    }
}

/* write char */
void write_char(char a) {
    if (has_network_socket()) {

        MESSAGE msg;

        msg.msg_type = MSG_GRAPH;
        msg.param.pword[1] = GraphRes;
        msg.param.pword[0] = GRAPH_PUTCHAR;
        msg.param.pchar = a;
        send_to_graph(&msg);
    } else {
        printf("%c", a);
    }
}

/* read char */
char read_char() {
    char ch;
    MESSAGE msg;
    if (has_network_socket()) {
        int st;
        msg.msg_type = MSG_GRAPH;
        msg.param.pword[1] = GraphRes;
        msg.param.pword[0] = GRAPH_READCHAR;
        send_to_graph(&msg);
        while (TRUE) {
            st = receive_message(network_socket, &msg);
            if (st == 0) {
                if ((msg.msg_type == MSG_GRAPH) &&
                        (msg.param.pword[0] == GRAPH_READCHAR_RESPONSE)) {
                    ch = msg.param.pchar;
                    break;
                }
            }
        }
        return (ch);
    } else {
        ch = getchar();
        return (ch);
    }

}

/* read line */
void read_line() {
    MESSAGE msg;
    int st;
    if (has_network_socket()) {
        msg.msg_type = MSG_GRAPH;
        msg.param.pword[1] = GraphRes;
        msg.param.pword[0] = GRAPH_READLN;
        send_to_graph(&msg);
        while (TRUE) {
            st = receive_message(network_socket, &msg);
            if (st == 0) if ((msg.msg_type == MSG_GRAPH) && (msg.param.pword[0] == GRAPH_READLN_RESPONSE))
                break;
        }
    } else {
        size_t limit = 255;
        char s[limit];

        _getline((char**)&s, &limit , stdin);
    }
}

/* read string */
    void read_str(char *s) {
        char ss[255];
        size_t limit = 80;
        MESSAGE msg;
        int st;
        if (has_network_socket()) {
            msg.msg_type = MSG_GRAPH;
            msg.param.pword[1] = GraphRes;
            msg.param.pword[0] = GRAPH_READSTR;
            send_to_graph(&msg);
            while (TRUE) {
                st = receive_message(network_socket, &msg);
                if (st == 0) {
                    if ((msg.msg_type == MSG_GRAPH) &&
                            (msg.param.pword[0] == GRAPH_READSTR_RESPONSE)) {
                        strcpy(ss, msg.param.pstr);
                        break;
                    }
                }
            }
            strcpy(s, ss);
        } else {
            char *empty_str_pointer =NULL;
            _getline(empty_str_pointer, &limit , stdin);
            strcpy(s,empty_str_pointer);
        }

    }

/* send message to kernel */
    void send_to_kernel(MESSAGE *msg) {
        write(network_socket, msg, sizeof(MESSAGE));
    }

/* send message to net */
/* 2010 returns 1 if ok and 0 if node desn't exists */
    int send_to_net(MESSAGE *msg) {
        int k, len;
        MESSAGE m;
        struct sockaddr_in svr;
        char addr[256];

        k = msg->int_msg.control.receiver.node;

        /* 2010 check if node exists */
        m.msg_type = MSG_NET;
        m.param.pword[0] = NET_NODE_EXIST;
        m.param.pword[1] = k;
        m.param.pword[2] = my_ctx.program_id;
        write(network_socket, &m, sizeof(MESSAGE));
        bzero(&m, sizeof(MESSAGE));
        while ((m.msg_type != MSG_NET) && (m.param.pword[0] != NET_NODE_EXIST)) {
            read(network_socket, &m, sizeof(MESSAGE));
        }
        DEBUG_PRINT("NET_NODE_EXIST %d: %d\n", k, m.param.pword[1]);
        if (m.param.pword[1] != 1) return 0;
        strcpy(addr, m.param.pstr);

        if (RInstance[k] == -1) {
            bzero(&m, sizeof(MESSAGE));
            m.msg_type = MSG_VLP;
            m.param.pword[0] = VLP_REMOTE_INSTANCE_PLEASE;
            m.param.pword[1] = my_ctx.program_id;
            m.param.pword[2] = k;
            send_to_kernel(&m);
            bzero(&m, sizeof(MESSAGE));
            DEBUG_PRINT("VLP_REMOTE_INSTANCE_PLEASE of program id: %d on node %d\n", my_ctx.program_id, k);

            while (1) {
                read(network_socket, &m, sizeof(MESSAGE));
                if ((m.msg_type == MSG_VLP) && (m.param.pword[0] == VLP_REMOTE_INSTANCE_HERE)) break;
            }

            DEBUG_PRINT("remote instance made with id: %d addr %s port %d\n",m.param.pword[1],addr, htons(m.param.pword[8]));
            RInstance[k] = m.param.pword[1];
            /* Make direct connection */
            DirConn[k] = socket(AF_INET, SOCK_STREAM, 0);
            svr.sin_family = AF_INET;
            svr.sin_addr.s_addr = inet_addr(addr);
            svr.sin_port = htons(m.param.pword[8]);
            len = connect(DirConn[k], (struct sockaddr *) &svr, sizeof(svr));
            if (len != 0) {
                RInstance[k] = -1;
                writeln_str("Cannot connect remote instance!");
                DEBUG_PRINT("Cannot establish remote connection\n");
            } else {
                DEBUG_PRINT("Remote connection established\n");
                //fcntl ( DirConn[k], F_SETFL, O_NONBLOCK | fcntl ( DirConn[k], F_GETFL, 0 ) );
            }
        }
        if (RInstance[k] != -1) {
            DEBUG_PRINT("Remote instance already exists\n");
            write(DirConn[k], &(msg->int_msg), sizeof(message));
        }
        return 1;
    }

/* -------------------- Check for message on internal socket -------------*/
    void get_internal() {
        MESSAGE m, m1;
        message mx;

        int r, max, i;
        char s[80];
        fd_set rset;
        struct timeval tout = {0, 0};

        FD_ZERO (&rset);
        FD_SET (network_socket, &rset);
        //FD_SET (internal_sock, &rset);
        max = network_socket;
        //else max = internal_sock;
        /* 2010 */
        for (i = 0; i < 255; i++) {
            if (DirConn[i] != -1) {
                FD_SET (DirConn[i], &rset);
                if (max < DirConn[i]) max = DirConn[i];
            }
        } /* for */

        if (select(max + 1, &rset, 0, 0, (struct timeval *) &tout) > 0) {
            /* 2010 */
            for (i = 0; i < 255; i++) {
                if ((DirConn[i] != -1) && (FD_ISSET (DirConn[i], &rset))) {
                    /*printf("DEBUG: Interp has message on direct connection: type %d par %d\n",mx.control.type,mx.control.par);  */
                    r = read(DirConn[i], &mx, sizeof(mx));
                    if (r > 0 && r == sizeof(mx)) {
                        memcpy(globmsgqueue + msgtail, &mx, sizeof(message));
                        msgtail = (msgtail + 1) % MAXMSGQUEUE;
                        msgready++;
                    }
                }
            }

            if (FD_ISSET (network_socket, &rset)) {
                r = read(network_socket, &m, sizeof(MESSAGE));
                if (r > 0) {
                    switch (m.msg_type) {
                        case MSG_NET:
                            switch (m.param.pword[0]) {
                                case NET_PROPAGATE:
                                    /*printf("DEBUG: cint net_sock MSG_NET NET_PROPAGATE cmd %d\n",m.param.pword[6]);*/
                                    memcpy(globmsgqueue + msgtail, &m.int_msg, sizeof(message));
                                    msgtail = (msgtail + 1) % MAXMSGQUEUE;
                                    msgready++;
                                    break;
                            };
                            break;
                        case MSG_INT:
                            switch (m.param.pword[0]) {
                                case INT_CLOSE_INSTANCE:
                                    endrun(0);
                                case INT_KILL:
                                    endrun(1);
                                default:
                                    break;
                            } /* switch int */
                    }/*switch type */
                }
            } /* FD_ISSET */
        } /* select */
    }

    void request_id() {
        MESSAGE m;
        MESSAGE m1;

        m.msg_type = MSG_INT;
        m.param.pword[0] = INT_CTX_REQ;
        m.param.pword[1] = remote;
        m.param.pword[2] = remote_id;
        strcpy(m.param.pstr,ProgName);
        send_and_select_response(network_socket, &m, &m1);
        if ((m1.msg_type != MSG_INT) || (m1.param.pword[0] != INT_CTX)) {
            // todo should throw exception, bad response from VM
            DEBUG_PRINT("BAD response from VM! Waiting for INT_CTX but recived: %d %d \n", m1.msg_type, m1.param.pword[0]);
            exit(-8777);
        }

        my_ctx.node = m1.param.pword[1];
        my_ctx.program_id = m1.param.pword[2];
        if (remote) {
            parent_ctx.node = m1.param.pword[3];
            parent_ctx.program_id = m1.param.pword[4];
            DEBUG_PRINT("We are remote, setting parent_ctx node: %d and program_id: %d \n", parent_ctx.node, parent_ctx.program_id);
            RInstance[parent_ctx.node] = parent_ctx.program_id;
        } else {
            DEBUG_PRINT("We are not remote\n");
            parent_ctx.node = my_ctx.node;
            parent_ctx.program_id = my_ctx.program_id;
        }
        ournode = my_ctx.node;
    }

    void send_ready() {
        int sock, len;
        struct sockaddr_in svr;
        char name[255];
        struct hostent *info;

        MESSAGE msg;
        msg.msg_type = MSG_NET;
        msg.param.pword[0] = NET_PROPAGATE;
        msg.param.pword[1] = MSG_VLP;
        msg.param.pword[2] = my_ctx.node;
        msg.param.pword[4] = parent_ctx.node;
        msg.param.pword[6] = VLP_REMOTE_INSTANCE_OK;
        msg.param.pword[7] = my_ctx.program_id;
        msg.param.pword[9] = parent_ctx.program_id;
        DEBUG_PRINT("NET_PROPAGATE VLP_REMOTE_INSTANCE_OK to program_id: %d on node_id: %d\n", msg.param.pword[8], msg.param.pword[4]);
        sock = socket(AF_INET, SOCK_STREAM, 0);
        bzero(&svr, sizeof(svr));
        svr.sin_family = AF_INET;
        svr.sin_addr.s_addr = INADDR_ANY;
        svr.sin_port = 0;
        bind(sock, (struct sockaddr *) &svr, sizeof(svr));
        listen(sock, 5);
        len = sizeof(svr);
        getsockname(sock, (struct sockaddr *) &svr, &len);
        msg.param.pword[8] = ntohs(svr.sin_port);
        gethostname(name, len);
        info = gethostbyname(name);
        bcopy((char *) info->h_addr, (char *) &svr.sin_addr, info->h_length);
        sprintf(msg.param.pstr, "%s", inet_ntoa(svr.sin_addr));
        send_to_kernel(&msg);

        bzero(&svr, sizeof(svr));
        DirConn[parent_ctx.node] = accept(sock, (struct sockaddr *) &svr, &len);
        DEBUG_PRINT("DirConn[parent_ctx.node] = accept(..... finished\n");
        //fcntl ( DirConn[parent_ctx.node], F_SETFL, O_NONBLOCK | fcntl ( DirConn[parent_ctx.node], F_GETFL, 0 ) );
    }

    int main(int argc, char **argv) {

        initiate(argc, argv); /* initialize executor */
        #ifndef BASIC
        socket_setup();
        network_socket = socket_connect();
        #endif
        runsys(); /* initialize running system */
        if (has_network_socket()) {
            request_id();
            GraphRes = get_graph_res();
            graph_setstatus();
            if (GraphRes < 0) exit(12);
            if (remote) send_ready();
        }

        setjmp (contenv); /* set label for continue long jump */
        while (TRUE) { /* repeat until exit() is called */
            get_internal();
            schedule(); /* reschedule current process */
            if (ready != 0) {
                decode(); /* fetch instruction */
                execute(); /* and execute it */
            }
        }
        return 0;
    } /* end main */

