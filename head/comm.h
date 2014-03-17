/*    Communication structures */
typedef struct {
    int node;
    int pix;
    int mark;
} paddr;

typedef struct {
    paddr sender;       /* address of the sender and */
    paddr receiver;   /* receiver of the message */
    int type;      /* message type */
    int par;         /* prototype or error signal number */
} ctrl_msg;

typedef struct {
    ctrl_msg control;
    char params[ 256-sizeof ( ctrl_msg ) ];
} messg;

typedef struct {
    int pword[15];
    char pstr[255];
    char pchar;
} param_struct;

typedef struct {
    int node;
    int program_id;
} ctx_struct;

/* Commm struct for GRAPH element */
#define GRAPH_ALLOCATE	1
#define GRAPH_EXIT	2
#define GRAPH_FREE	3
#define GRAPH_SET_TITLE	4
#define GRAPH_WRITE	5
#define GRAPH_WRITEXY	6
#define GRAPH_READCHAR	7
#define GRAPH_READSTR	8
#define GRAPH_READLN	9
#define GRAPH_PUTCHAR	10
#define GRAPH_LINE	11
#define GRAPH_ELLIPSE	12
#define GRAPH_RECT	13
#define GRAPH_FOREGROUND	14
#define GRAPH_BACKGROUND	15
#define GRAPH_POINT	16
#define GRAPH_CLEAR	17
#define GRAPH_HASCII	18
#define GRAPH_INKEY	19
#define GRAPH_ALLOCATED	20
#define GRAPH_INKEY_RESPONSE	21
#define GRAPH_READCHAR_RESPONSE	22
#define GRAPH_READSTR_RESPONSE	23
#define GRAPH_READLN_RESPONSE	24
#define GRAPH_MOVE		25
#define GRAPH_CURPOS		26
#define GRAPH_CURPOS_RESPONSE	27
#define GRAPH_LINETO		28

#define GRAPH_OPERATION_OK	29
#define GRAPH_OUTSTRING		30
#define GRAPH_WRITEINTXY	31
#define GRAPH_PUTMAP		32
#define GRAPH_GETMAP		33
#define GRAPH_GETMAP_RESPONSE	34

#define GRAPH_MGETPRESS		35
#define GRAPH_MGETPRESS_RESPONSE	36

#define GRAPH_MAGIC	37
#define GRAPH_MAGIC_RESPONSE	38

/* INT element */
#define INT_CONNECTED	1
#define INT_EXITING	    2
#define INT_CTX		    3
#define INT_CTX_REQ	    4
#define INT_REMOTE_INST	5
#define INT_INST_OK	    6
#define INT_READY	    7
#define INT_CLOSE_INSTANCE	8
#define INT_KILL	    9
#define INT_START_OK	10

/* NET element */
#define NET_PROPAGATE	    0
#define NET_PROPAGATE_OUT 	1
/*  msg_type = MSG_NET, param.pword[0] = NET_PROPAGATE
   param.pword[1] = MSG_INT or MSG_VLP
   param.pword[2,3] = sender context or node,0
   param.pword[4,5] = receiver context or node,0
   param.pword[6] = 	       VLP command

 */
#define NET_PROPAGATE_IN	2
#define NET_UNREGISTER		3
#define NET_REGISTER_NODE	4
#define NET_REGISTER_OK		5
#define NET_NODE		    6
#define NET_CCD_START	    7
#define NET_PCD_START	    8
#define NET_CODESTREAM_OK	9
#define NET_TRANSMIT_CODE	10
#define NET_CONNECT		    11
#define NET_ACCEPT		    12
#define NET_EXIT		    13
#define NET_CCD_CODE		14
#define NET_PCD_CODE		15
#define NET_CSWRITELN		16
#define NET_DISCONNECT		17
#define NET_NODE_EXIST		18
#define NET_TRANSMITTED		19
#define NET_CONNECTIONS		20
#define NET_CONNINFO		21
#define NET_CONNECT_TO		22
#define NET_GET_INFO		23
#define	NET_INFO		    24
#define NET_INFO_END		25
#define NET_NODES_NUM		26
#define NET_NODES_NUM_RESPONSE	27

/* VLP message */
#define VLP_WRITE		        0
#define VLP_REMOTE_INSTANCE	    1
#define VLP_REMOTE_INSTANCE_OK	2
#define VLP_REGINT		        3
#define VLP_REMOTE_INSTANCE_PLEASE  4
#define VLP_REMOTE_INSTANCE_HERE    5
#define VLP_CLOSE_INSTANCE		    6
#define VLP_INTERPRETER_DOWN		7
/* ------------------------------------------------------------ */
/*                Message structure                            */
/*------------------------------------------------------------*/
#define MSG_VLP		0
#define MSG_NET		1
#define MSG_GRAPH	2
#define MSG_INT		3

typedef struct {
    short msg_type;
    param_struct param;
    messg int_msg;
} MESSAGE;

typedef struct {
    short msg_type;
    param_struct param;
} G_MESSAGE;
