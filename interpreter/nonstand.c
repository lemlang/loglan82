#include "depend.h"
#include "genint.h"
#include "int.h"
#include "process.h"
#include "intproto.h"

#include "nonstand.h"


/* Call (non)standard procedures.
 * Almost totaly implementation dependent.
 */

bool graphmode = FALSE;			/* TRUE iff graphics mode active */
param_struct par;

char *s;
char ss[255];
char ii[50],ff[50];
int scc,lastmsg;

int wait_for_key() {
    MESSAGE msg;

    bzero ( &msg,sizeof ( MESSAGE ) );
    while ( ( msg.msg_type != MSG_GRAPH ) && ( msg.param.pword[0] != GRAPH_INKEY_RESPONSE ) ) {
        read_from_graph ( &msg );

    }
    return ( msg.param.pword[3] );
}


void nonstandard ( nrproc )		/* Call (non)standard procedure */
word nrproc;
{

    word am;
    int cnt=0;
    float r1, r2;
    word ax,bx,cx,dx,i,t1,t2;
    unsigned int v,p,h,l,r,c,sc;
    unsigned int Res_graph_X,Res_graph_Y;
    MESSAGE msg;
    MESSAGE msg1;


    switch ( ( int ) nrproc ) {
    case GRON :
    case GROFF :
        break;

    case CLS :
        msg.msg_type = MSG_GRAPH;
        msg.param.pword[0] = GRAPH_CLEAR;
        msg.param.pword[1] = GraphRes;
        send_to_graph ( &msg );
        break;

    case POINT :
        msg.msg_type = MSG_GRAPH;
        msg.param.pword[0]=GRAPH_POINT;
        msg.param.pword[1] = GraphRes;
        msg.param.pword[3]=param[0].xword;
        msg.param.pword[4]=param[1].xword;
        send_to_graph ( &msg );
        break;
    case MOVE :
        msg.msg_type = MSG_GRAPH;
        msg.param.pword[0]=GRAPH_MOVE;
        msg.param.pword[1] = GraphRes;
        msg.param.pword[2]=param[0].xword;
        msg.param.pword[3]=param[1].xword;
        send_to_graph ( &msg );
        break;

    case DRAW :
        msg.msg_type = MSG_GRAPH;
        msg.param.pword[0]=GRAPH_LINETO;
        msg.param.pword[1] = GraphRes;
        msg.param.pword[3]=param[0].xword;
        msg.param.pword[4]=param[1].xword;
        send_to_graph ( &msg );
        break;

    case INXPOS :
        msg.msg_type = MSG_GRAPH;
        msg.param.pword[0]=GRAPH_CURPOS;
        msg.param.pword[1] = GraphRes;
        send_to_graph ( &msg );
        while ( ( msg.msg_type!=MSG_GRAPH ) && ( msg.param.pword[0]!=GRAPH_CURPOS_RESPONSE ) )
            read_from_graph ( &msg );
        param[0].xword = msg.param.pword[3];
        break;

    case INYPOS :
        msg.msg_type = MSG_GRAPH;
        msg.param.pword[0]=GRAPH_CURPOS;
        msg.param.pword[1] = GraphRes;
        send_to_graph ( &msg );
        while ( ( msg.msg_type!=MSG_GRAPH ) || ( msg.param.pword[0]!=GRAPH_CURPOS_RESPONSE ) )
            read_from_graph ( &msg );
        param[0].xword = msg.param.pword[4];
        break;

    case HFILL :
        break;
    case VFILL :
        break;

    case HASCII :
        msg.msg_type = MSG_GRAPH;
        msg.param.pword[0]=GRAPH_HASCII;
        msg.param.pword[1]=param[0].xword;
        send_to_graph ( &msg );
        break;

    case COLOR :
        msg.msg_type = MSG_GRAPH;
        msg.param.pword[0]=GRAPH_FOREGROUND;
        msg.param.pword[1] = GraphRes;
        msg.param.pword[3] = param[0].xword;
        send_to_graph ( &msg );
        break;

    case BORDER :
        msg.msg_type = MSG_GRAPH;
        msg.param.pword[0]=GRAPH_BACKGROUND;
        msg.param.pword[1] = GraphRes;
        msg.param.pword[3] = param[0].xword;
        send_to_graph ( &msg );
        break;

    case STYLE :
        break;

    case INPIX :
        break;

    case OUTSTRING :
        msg.msg_type = MSG_GRAPH;
        msg.param.pword[0]=GRAPH_OUTSTRING;
        msg.param.pword[1] = GraphRes;
        msg.param.pword[2] = param[0].xword;
        msg.param.pword[3] = param[1].xword;
        msg.param.pword[4] = param[3].xword;
        msg.param.pword[5] = param[4].xword;
        {
            s= ( char * ) ( M + strings + param[ 2 ].xword + 1 );
            strcpy ( msg.param.pstr,s );
            send_to_graph ( &msg );
        }
        break;

    case GETMAP :
        msg.msg_type = MSG_GRAPH;
        msg.param.pword[0] = GRAPH_GETMAP;
        msg.param.pword[1] = GraphRes;
        msg.param.pword[2] = param[0].xword;
        msg.param.pword[3] = param[1].xword;
        send_to_graph ( &msg );
        while ( ( msg.msg_type!=MSG_GRAPH ) || ( msg.param.pword[0]!=GRAPH_GETMAP_RESPONSE ) )
            read_from_graph ( &msg );

        {
            int map;

            map =msg.param.pword[2];
            newarry ( ( word ) 1, 3, ( word ) AINT, &param[ 2 ].xvirt, &am );
            M[ am+3 ] = map;
            M[ am+4 ] = msg.param.pword[3];
            M[ am+5 ] = msg.param.pword[4];

        }
        break;


    case PUTMAP :
        if ( member ( &param[ 0 ].xvirt, &am ) ) {
            msg.msg_type = MSG_GRAPH;
            msg.param.pword[0] = GRAPH_PUTMAP;
            msg.param.pword[1] = GraphRes;
            msg.param.pword[2] = M[am+3];
            msg.param.pword[4] = M[am+4];
            msg.param.pword[5] = M[am+5];
            send_to_graph ( &msg );
        }
        break;

    case ORMAP :
    case XORMAP :
        break;


    case PATERN :
        msg.msg_type = MSG_GRAPH;
        msg.param.pword[0]=GRAPH_RECT;
        msg.param.pword[1] = GraphRes;
        msg.param.pword[3] = param[0].xword;
        msg.param.pword[4] = param[1].xword;
        msg.param.pword[5] = param[2].xword;
        msg.param.pword[6] = param[3].xword;
        msg.param.pword[7] = param[4].xword;
        msg.param.pword[8] = param[5].xword;
        send_to_graph ( &msg );
        break;
    case PALLET:
        break;

    case TRACK :
        msg.msg_type = MSG_GRAPH;
        msg.param.pword[0] = GRAPH_WRITEINTXY;
        msg.param.pword[1] = GraphRes;
        msg.param.pword[2] = param[0].xword;
        msg.param.pword[3] = param[1].xword;
        msg.param.pword[4] = param[4].xword;
        msg.param.pword[5] = param[2].xword;
        send_to_graph ( &msg );
        break;

    case INKEY :
        msg.msg_type = MSG_GRAPH;
        msg.param.pword[0]=GRAPH_INKEY;
        msg.param.pword[1] = GraphRes;
        send_to_graph ( &msg );
        param[0].xword = wait_for_key();
        break;


    case CIRB :
        msg.msg_type = MSG_GRAPH;
        msg.param.pword[0]=GRAPH_ELLIPSE;
        msg.param.pword[1] = GraphRes;
        msg.param.pword[3]=param[0].xword;
        msg.param.pword[4]=param[1].xword;
        msg.param.pword[5]=param[2].xword;
        msg.param.pword[6]=param[3].xword;
        msg.param.pword[7]= ( double ) param[4].xword;
        msg.param.pword[8]= ( double ) param[5].xword;
        msg.param.pword[9]=param[7].xword;
        send_to_graph ( &msg );
        break;

    case HFONT8: /* ___________ magic library ____________ */

        if ( param[0].xword==1000 ) {
            strcpy ( ss,"" );
        } else if ( param[0].xword==1001 ) {
            sprintf ( ss,"%s%c",ss, ( char ) ( param[1].xword ) );
        } else if ( param[0].xword==303 ) {
            msg.param.pword[1] = param[0].xword;
            msg.param.pword[2] = param[1].xword;
            msg.param.pword[3] = param[2].xword;
            msg.param.pword[4] = param[3].xword;
            msg.param.pword[5] = param[5].xword;
            msg.msg_type = MSG_GRAPH;
            msg.param.pword[0] = GRAPH_MAGIC;
            strcpy ( msg.param.pstr,ss );
            send_to_graph ( &msg );
        } else if ( param[0].xword>500 ) {
            msg1.param.pword[1] = param[0].xword;
            msg1.param.pword[2] = param[1].xword;
            msg1.param.pword[3] = param[2].xword;
            msg1.param.pword[4] = param[3].xword;
            s= ( char * ) ( M + strings + param[ 4 ].xword + 1 );
            strcpy ( msg1.param.pstr,s );
            msg1.param.pword[5] = param[5].xword;
            msg1.param.pword[6] = param[6].xword;
            msg1.param.pword[7] = param[7].xword;

            switch ( msg1.param.pword[1] ) { /* Machine class */
            case 501: /* get local node */
                newarry ( ( word ) 0,2, ( word ) AINT,&param[8].xvirt,&ax );
                ax+=3;
                M[ax++]=my_ctx.node;
                break;
            case 502: /* number of nodes */
                msg1.msg_type = MSG_NET;
                msg1.param.pword[0]=NET_NODES_NUM;
                write ( net_sock,&msg1,sizeof ( MESSAGE ) );
                while ( ( msg1.msg_type!=MSG_NET ) || ( msg1.param.pword[0]!=NET_NODES_NUM_RESPONSE ) )
                    read_from_net ( &msg1 );
                newarry ( ( word ) 0,2, ( word ) AINT,&param[8].xvirt,&ax );
                ax+=3;
                M[ax++]=msg1.param.pword[1];
                break;
            case 503: /* node exists */
                msg1.msg_type = MSG_NET;
                msg1.param.pword[0]=NET_NODE_EXIST;
                msg1.param.pword[1]=msg1.param.pword[2];
                write ( net_sock,&msg1,sizeof ( MESSAGE ) );
                bzero ( &msg1,sizeof ( msg1 ) );
                while ( ( msg1.msg_type!=MSG_NET ) || ( msg1.param.pword[0]!=NET_NODE_EXIST ) )
                    read_from_net ( &msg1 );
                newarry ( ( word ) 0,2, ( word ) AINT,&param[8].xvirt,&ax );
                ax+=3;
                M[ax++]=msg1.param.pword[1];
                break;
            case 504: { /* get nodes info */
                char ss[1024];
                strcpy ( ss,"" );
                msg1.msg_type = MSG_NET;
                msg1.param.pword[0]=NET_GET_INFO;
                write ( net_sock,&msg1,sizeof ( MESSAGE ) );
                while ( ( msg1.msg_type!=MSG_NET ) || ( msg1.param.pword[0]!=NET_INFO_END ) ) {
                    read_from_net ( &msg1 );
                    if ( msg1.param.pword[0]==NET_INFO )
                        strcat ( ss,msg1.param.pstr );
                }
                newarry ( ( word ) 0,strlen ( ss ), ( word ) AINT,
                          &param[8].xvirt,&ax );
                ax+=3;
                s=ss;
                while ( *s!='\0' )
                    M[ax++]=* ( s++ );
            } ;
            break;


            }/* switch */
        } else {
            /* graphic */

            msg.param.pword[1] = param[0].xword;
            msg.param.pword[2] = param[1].xword;
            msg.param.pword[3] = param[2].xword;
            msg.param.pword[4] = param[3].xword;
            s= ( char * ) ( M + strings + param[ 4 ].xword + 1 );
            strcpy ( msg.param.pstr,s );
            msg.param.pword[5] = param[5].xword;
            msg.param.pword[6] = param[6].xword;
            msg.param.pword[7] = param[7].xword;

            msg.msg_type = MSG_GRAPH;
            msg.param.pword[0] = GRAPH_MAGIC;

            send_to_graph ( &msg );

            if ( msg.param.pword[1]<0 ) {
                lastmsg=msg.param.pword[1];
                bzero ( &msg,sizeof ( MESSAGE ) );
                while ( msg.param.pword[0]!=GRAPH_MAGIC_RESPONSE )
                    read_from_graph ( &msg );
                if ( lastmsg==-305 ) { // Read integer
                    newarry ( ( word ) 0,10, ( word ) AINT,&param[8].xvirt,&ax );
                    ax+=3;
                    M[ax++]=atoi ( msg.param.pstr );
                } else if ( lastmsg==-306 ) { // Read char
                    newarry ( ( word ) 0,10, ( word ) AINT,&param[8].xvirt,&ax );
                    ax+=3;
                    M[ax++]=msg.param.pchar;
                } else if ( lastmsg==-307 ) { // Read real
                    newarry ( ( word ) 0,10, ( word ) AINT,&param[8].xvirt,&ax );
                    ax+=3;
                    strcpy ( ii,"" );
                    strcpy ( ff,"" );
                    sc=0;
                    while ( ( sc<strlen ( msg.param.pstr ) ) && ( msg.param.pstr[sc]!='.' ) ) {
                        ii[sc]=msg.param.pstr[sc];
                        sc++;
                    }
                    ii[sc++]='\0';
                    scc=sc;
                    sc=0;
                    while ( scc<strlen ( msg.param.pstr ) ) {
                        ff[sc++]=msg.param.pstr[scc++];
                    }
                    ff[sc]='\0';
                    M[ax++]=atoi ( ii );
                    M[ax++]=atoi ( ff );

                } else if ( msg.param.pword[1]==1 ) {
                    newarry ( ( word ) 0,10, ( word ) AINT,&param[8].xvirt,&ax );
                    ax+=3;
                    for ( l=2; l<7; l++ )
                        M[ax++]=msg.param.pword[l];
                } else {
                    newarry ( ( word ) 0,strlen ( msg.param.pstr ) +2, ( word ) AINT,
                              &param[8].xvirt,&ax );
                    ax+=3;
                    s=msg.param.pstr;
                    while ( *s!='\0' )
                        M[ax++]=* ( s++ );
                }
            }
        } /* graphic */
        break;

    case INIT :

        break;

    case STATUS :
        break;

    case GETPRESS :
        msg.msg_type = MSG_GRAPH;
        msg.param.pword[0]=GRAPH_MGETPRESS;
        msg.param.pword[1] = GraphRes;
        send_to_graph ( &msg );
        while ( 1 ) {
            read_from_graph ( &msg );
            if ( ( msg.msg_type==MSG_GRAPH ) && ( msg.param.pword[0]==GRAPH_MGETPRESS_RESPONSE ) ) break;
        }
        param[0].xword = msg.param.pword[2];
        param[1].xword = msg.param.pword[3];
        param[2].xword = msg.param.pword[4];
        param[3].xword = msg.param.pword[5];
        param[4].xword = msg.param.pword[6];
        param[5].xword = msg.param.pword[7];
        break;


        break;
    case GETRELEASE :
        break;

    case SHOWCURSOR :
    case HIDECURSOR :
    case SETPOSITION :
    case SETWINDOW :
    case DEFCURSOR :
    case SETSPEED :
    case SETMARGINS :
    case SETTHRESHOLD :
        break;

    case GETMOVEMENT :
        break;




    default  :
        errsignal ( RTEUNSTP );
    }

}

