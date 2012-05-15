 

#include "depend.h"
#include "genint.h"
#include "int.h"
#include "process.h"
#include "intproto.h"
#include "socu.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <qthread.h>

#ifndef NO_PROTOTYPES
static void load(char *);
static void initiate(int,char **);
int main(int,char **);
#else
static void load();
static void initiate();
int main();
#endif



int internal_sock,graph_sock,net_sock,connected=0;
struct sockaddr_un svr;
int GraphRes=-1;
char ProgName[255],mygname[80], gname[80],mykname[80], nname[80], mynname[80];
fd_set DirSet;
int maxDirSet;

ctx_struct my_ctx;
ctx_struct parent_ctx;

int RInstance[255]; /* IDs of remote instances */
int DirConn[255];   /* Direct connection channels */

/* graphic vars */
int fcol, bcol, curx=0, cury=0;



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


static void load(_filename)     /* Load code and prototypes from file */
   char *_filename;
{
    FILE *fp;
    char *cp;
    word n, left;
    char filename[100]; /* should suffice on all systems */

    strcpy( filename, _filename );
    M = mallocate(memorysize+1);        /* allocate main memory array */
    if (M == NULL) abend("Memory size too large (use /m option)\n");

    addext(filename, ".ccd");
    if ((fp = fopen(filename, BINARYREAD)) == NULL)
        {
           fprintf(stderr,"Cannot open .ccd file\n");
           endrun(10);
         };

    ic = 0;              /* read static data and code */
    left = memorysize+1;               /* from .ccd file */
    do
    {
        if (left == 0) abend("Memory size too small (use /m option)\n");
        n = min(IOBLOCK/sizeof(word), left);
        n = fread((char *) &M[ ic ], sizeof(word), (int) n, fp);
        ic += n;
        left -= n;
    } while (n != 0);      /* now ic = number of words read */

    fclose(fp);
    /* Get various addresses passed by GENERATOR */
    ipradr    = M[ ic-5 ];           /* primitive type desctriptions */
    temporary = M[ ic-4 ];           /* global temporary variables */
    strings   = M[ ic-3 ];           /* string constants */
    lastprot  = M[ ic-2 ];           /* last prototype number */
    freem     = M[ ic-1 ];           /* first free word in memory */

    /* Read prototypes from .pcd file */
    addext(filename, ".pcd");
    if ((fp = fopen(filename, BINARYREAD)) == NULL)
       {
         fprintf(stderr,"Cannot open .pcd file\n");
         endrun(10); 
        }
    for (n = MAINBLOCK;  n <= lastprot;  n++ )
    {
        cp = ballocate(sizeof(protdescr));
        if (cp == NULL) abend("Memory size too large (use /m option)\n");
        prototype[ n ] = (protdescr *) cp;
        if (fread(cp, sizeof(protdescr), 1, fp) != 1)
            abend("Cannot read .pcd file\n");
    }
    fclose(fp);

    /* Open trace file */
    if (debug)
    {
        addext(filename, ".trd");
        if ((tracefile = fopen(filename, "w")) == NULL)
            abend("Cannot open .trd file\n");
    }
} /* end load */


static void initiate(argc, argv)        /* Establish configuration parameters */
int argc;
char **argv;
{
    long m;
    int len,i,on;
    char filename[80];
    int sock;
    fd_set rset,wset;

     ournode = 0;
     network = TRUE;
    if ( (argc==4) && (strcmp(argv[3],"r") == 0) ) remote = TRUE;else remote=FALSE;
      for(i=0;i<255;i++)
      {
        RInstance[i] = -1;
        DirConn[i] = -1;
      }
      
    FD_ZERO(&DirSet);    
    maxDirSet=0;
 
    strcpy(filename,argv[2]);
    strcpy(ProgName,argv[2]);

    strcpy(mynname,argv[1]);
    strcat(mynname,".net");
    unlink(mynname);
    sock = socket(AF_UNIX,SOCK_STREAM,0); 
    bzero(&svr, sizeof(svr));
    svr.sun_family = AF_UNIX;
    strcpy(svr.sun_path,mynname);
    len = strlen(svr.sun_path)+sizeof(svr.sun_family);
    bind(sock,(struct sockaddr*)&svr, len);
    listen(sock,5);


   /* socket for graphic requests */ 
    strcpy(mygname,argv[1]);
    strcat(mygname,".gr");
    unlink(mygname);
   

   /* socket for KERNEL communication */  
      internal_sock = socket(AF_UNIX,SOCK_STREAM,0);
      bzero(&svr, sizeof(svr));
      svr.sun_family = AF_UNIX;
      strcpy(svr.sun_path,argv[1]);
      strcpy(mykname,argv[1]);
      len = strlen(svr.sun_path)+sizeof(svr.sun_family);
      i=connect(internal_sock,(struct sockaddr*)&svr,len);
     
    if (i==0)
       { 
        fcntl(internal_sock,F_SETFL, O_NONBLOCK|fcntl(internal_sock,F_GETFL,0));
        }
    else
     while (i!=0)
    {
      close(internal_sock);
      internal_sock = socket(AF_UNIX,SOCK_STREAM,0);
      fcntl(internal_sock,F_SETFL, O_NONBLOCK|fcntl(internal_sock,F_GETFL,0));
      i=connect(internal_sock,(struct sockaddr*)&svr,len);     
    }    
    on=1;
    setsockopt(internal_sock,IPPROTO_TCP,TCP_NODELAY,(char*)&on,sizeof(on));
    
    /* socket for network requests */
     FD_ZERO(&rset);FD_ZERO(&wset);
     FD_SET(sock,&rset);
     if (select(sock+1,&rset,&wset,0,0))
      net_sock = accept(sock,(struct sockaddr*)0,(int *)0);
    if (net_sock>0)
    {
     fcntl(net_sock,F_SETFL,O_NONBLOCK|fcntl(net_sock,F_GETFL,0));
     setsockopt(net_sock,IPPROTO_TCP,TCP_NODELAY,(char*)&on,sizeof(on));

    }
    close(sock);

    if( filename!=NULL )
       load(filename);                     /* load code and prototypes */
    else
       usage();
}


void decode(){
    extopcode *eop;

    eop = (extopcode *)(M+ic);   /* pointer to extended opcode in M */
    lastic = ic;                     /* save ic for possible redecoding */
    ic += APOPCODE;
    opcode = ((int) eop->opcode ) & 0xFF ;
    getargument(a1, 0);
    getargument(a2, 1);
    getargument(a3, 2);
}

/* -------------------------------------------------------------------- */


void send_to_graph(G_MESSAGE *msg)
{
 write(graph_sock,msg,sizeof(G_MESSAGE));
}

int read_from_graph(G_MESSAGE *msg)
{
 fd_set rset,wset;
 struct timeval tout={0,0};
 
 FD_ZERO(&rset);FD_ZERO(&wset);
 FD_SET(graph_sock,&rset);
 
  
 if (select(graph_sock+1,&rset,&wset,0,(struct timeval *)&tout)>0)
 {
  if (FD_ISSET(graph_sock,&rset))
   return(read(graph_sock,msg,sizeof(G_MESSAGE)));
 }
 return(0);  
}

int read_from_net(MESSAGE *msg)
{
 fd_set rset,wset;
 struct timeval tout={0,0};
 
 FD_ZERO(&rset);FD_ZERO(&wset);
 FD_SET(net_sock,&rset);
 
  
 if (select(net_sock+1,&rset,&wset,0,(struct timeval *)&tout)>0)
 {
  if (FD_ISSET(net_sock,&rset))
   return(read(net_sock,msg,sizeof(MESSAGE)));
 }
 return(0);  
}


/* Get graphic resource number */
int get_graph_res()
{
 MESSAGE msg;
 int sock;
 struct sockaddr_un svr;
 int len,i,on;
 fd_set rset,wset;

 unlink(mygname);
 sock = socket(AF_UNIX,SOCK_STREAM,0);
 bzero(&svr,sizeof(svr));
 svr.sun_family = AF_UNIX;
 strcpy(svr.sun_path,mygname);
 len = strlen(svr.sun_path)+sizeof(svr.sun_family);
 bind(sock,(struct sockaddr*)&svr, len);      
 listen(sock,5);


 msg.msg_type = MSG_GRAPH;
 msg.param.pword[0] = GRAPH_ALLOCATE;
 strcpy(msg.param.pstr,mygname);
 write(internal_sock,&msg,sizeof(MESSAGE));
 bzero(&msg,sizeof(MESSAGE));
 FD_ZERO(&rset);FD_ZERO(&wset);
 FD_SET(sock,&rset);
  if (select(sock+1,&rset,&wset,0,0))
    graph_sock = accept(sock,(struct sockaddr*)0,(int*)0);
 
 if (graph_sock == -1)
 {
   graphics=FALSE;return(0);
 }
  on=1;
  close(sock);
  fcntl(graph_sock,F_SETFL, O_NONBLOCK|fcntl(graph_sock,F_GETFL,0));
  setsockopt(graph_sock,IPPROTO_TCP,TCP_NODELAY,(char*)&on,sizeof(on));

  return(1);
   
} /* get_graph_res */


/* writeln string */
void writeln_str(char *s)
{
 G_MESSAGE msg;
 msg.msg_type = MSG_GRAPH;
 msg.param.pword[1] = GraphRes;
 msg.param.pword[2] = GRAPH_WRITE;
 strcpy(msg.param.pstr,s);
 
 send_to_graph(&msg);
 strcpy(msg.param.pstr,"\n");
 send_to_graph(&msg);
}

/* write string */
void write_str(char *s)
{
 G_MESSAGE msg;
 msg.msg_type = MSG_GRAPH;
 msg.param.pword[1] = GraphRes;
 msg.param.pword[0] = GRAPH_WRITE;
 strcpy(msg.param.pstr,s);
 send_to_graph(&msg);
}

/* write char */
void write_char(char a)
{
 G_MESSAGE msg;

 msg.msg_type = MSG_GRAPH;
 msg.param.pword[1] = GraphRes;
 msg.param.pword[0] = GRAPH_PUTCHAR;
 msg.param.pchar = a;
 send_to_graph(&msg);
}


/* read char */
char read_char()
{
 char ch;
 G_MESSAGE msg;
 int st;


  msg.msg_type = MSG_GRAPH;
  msg.param.pword[1] = GraphRes;
  msg.param.pword[0] = GRAPH_READCHAR;
  send_to_graph(&msg);
  while(TRUE)
      {
       st = read_from_graph(&msg);
       if (st>0)
         {
          if ((msg.msg_type == MSG_GRAPH) && 
              (msg.param.pword[0]==GRAPH_READCHAR_RESPONSE)) 
            {ch = msg.param.pchar;break;}
         }
       }
 return(ch);

}

/* read line */
void read_line()
{
 G_MESSAGE msg;
 int st;


  msg.msg_type = MSG_GRAPH;
  msg.param.pword[1] = GraphRes;
  msg.param.pword[0] = GRAPH_READLN;
  send_to_graph(&msg);
  while(TRUE)
      {
       st = read_from_graph(&msg);
       if (st>0)
         if ((msg.msg_type == MSG_GRAPH) && (msg.param.pword[0]== GRAPH_READLN_RESPONSE))
          break;
       }
}

/* read string */
void read_str(char *s)
{
 char ss[255];
 G_MESSAGE msg;
 int st;

  msg.msg_type = MSG_GRAPH;
  msg.param.pword[1] = GraphRes;
  msg.param.pword[0] = GRAPH_READSTR;
  send_to_graph(&msg);
  while(TRUE)
      {
       st = read_from_graph(&msg);
       if (st>0)
         {
          if ((msg.msg_type == MSG_GRAPH) && 
              (msg.param.pword[0]==GRAPH_READSTR_RESPONSE)) 
            { strcpy(ss,msg.param.pstr);break;}
         }
       }
 strcpy(s,ss);

}


/* send message to kernel */
void send_to_kernel(MESSAGE *msg)
{
 write(internal_sock,msg,sizeof(MESSAGE));
}

/* send message to net */
/* 2010 returns 1 if ok and 0 if node desn't exists */
int send_to_net(MESSAGE *msg)
{
 int k,len;
 MESSAGE m;
 struct sockaddr_in svr;
 char addr[256];

 
 k = msg->int_msg.control.receiver.node; 

 /* 2010 check if node exists */
 m.msg_type = MSG_NET;
 m.param.pword[0] = NET_NODE_EXIST;
 m.param.pword[1] = k;
 m.param.pword[2] = my_ctx.program_id;
 write(net_sock,&m,sizeof(MESSAGE));
 bzero(&m,sizeof(MESSAGE));
 while( (m.msg_type!=MSG_NET) && (m.param.pword[0]!=NET_NODE_EXIST) )
  read(net_sock,&m,sizeof(MESSAGE));
 if (m.param.pword[1]!=1) return 0;
 strcpy(addr,m.param.pstr);

 if (RInstance[k]==-1)
  { 
   bzero(&m,sizeof(MESSAGE));
   m.msg_type = MSG_VLP;
   m.param.pword[0] = VLP_REMOTE_INSTANCE_PLEASE;
   m.param.pword[1] = my_ctx.program_id;
   m.param.pword[2] = k;
   send_to_kernel(&m);
   bzero(&m,sizeof(MESSAGE));
   
   while(1)
     {
       read(internal_sock,&m,sizeof(MESSAGE));
       if ( (m.msg_type == MSG_VLP) && (m.param.pword[0]==VLP_REMOTE_INSTANCE_HERE)) break;
      }
      
   /*printf("DEBUG: remote instance made with id: %d addr %s port %d\n",m.param.pword[1],addr, htons(m.param.pword[8]));*/
   RInstance[k] = m.param.pword[1];
   /* Make direct connection */
   DirConn[k] = socket(AF_INET,SOCK_STREAM,0);
   svr.sin_family = AF_INET;
   svr.sin_addr.s_addr = inet_addr(addr);
   svr.sin_port = htons(m.param.pword[8]);
   len = connect(DirConn[k],(struct sockaddr*)&svr,sizeof(svr));
   if (len!=0) 
    {
     RInstance[k]=-1;
     
     writeln_str("Cannot connect remote instance!");
    }
   else
     {
      fcntl(DirConn[k], F_SETFL,O_NONBLOCK | fcntl(DirConn[k], F_GETFL,0));
     } 
   }
   if (RInstance[k]!=-1)
   {
      
      write(DirConn[k],&(msg->int_msg),sizeof(message)); 
   } 
   return 1;
}



/* -------------------- Check for message on internal socket -------------*/


void get_internal()
{
 MESSAGE m,m1;
 message mx;
 
 int r,max,i;
 char s[80];
 fd_set rset;
 struct timeval tout={0,0};
 
 

 /* 2010 */
 /* ----------- Direct connection messages -----
 FD_ZERO(&DirSet);
 maxDirSet=0;
 for(i=0;i<255;i++)
  if (DirConn[i]!=-1) 
   {
    FD_SET(DirConn[i],&DirSet);
    if (maxDirSet<DirConn[i]) maxDirSet=DirConn[i];
   }
 
 if (select(maxDirSet+1,&DirSet,0,0,(struct timeval *)&tout)>0)
  {
   for(i=0;i<255;i++)
   {
    if ( (DirConn[i]!=-1) && (FD_ISSET(DirConn[i],&DirSet)) )
    {
     r=read(DirConn[i],&mx,sizeof(mx));
     if (r>0)
     {
      memcpy(globmsgqueue+msgtail,&mx,sizeof(message));
      msgtail = (msgtail+1) % MAXMSGQUEUE;
      msgready++;
     } 
    }
   }
   }
 -----------------------------------------*/
 
 
 FD_ZERO(&rset);
 FD_SET(net_sock,&rset);
 FD_SET(internal_sock,&rset); 
 if (net_sock>internal_sock) max = net_sock; else max = internal_sock;
 /* 2010 */
 for(i=0;i<255;i++) {
  if (DirConn[i]!=-1) 
   {
    FD_SET(DirConn[i],&rset);
    if (max<DirConn[i]) max=DirConn[i];
   }
 } /* for */
  
 if (select(max+1,&rset,0,0,(struct timeval *)&tout)>0)
 {
 
  /* 2010 */
  for(i=0;i<255;i++)
   {
    if ( (DirConn[i]!=-1) && (FD_ISSET(DirConn[i],&rset)) )
    {
     /*printf("DEBUG: Interp has message on direct connection: type %d par %d\n",mx.control.type,mx.control.par);  */
     r=read(DirConn[i],&mx,sizeof(mx));
     if (r>0 && r==sizeof(mx))
     {      
      memcpy(globmsgqueue+msgtail,&mx,sizeof(message));
      msgtail = (msgtail+1) % MAXMSGQUEUE;
      msgready++;
     } 
    }
   }




 if (FD_ISSET(net_sock,&rset))
 {
 r = read(net_sock, &m, sizeof(MESSAGE));
 if (r>0)
 {
   switch(m.msg_type)
   {
     case MSG_NET:
                  switch(m.param.pword[0])
                  {
                   case NET_PROPAGATE:
                    /*printf("DEBUG: cint net_sock MSG_NET NET_PROPAGATE cmd %d\n",m.param.pword[6]);*/
                    memcpy(globmsgqueue+msgtail,&m.int_msg,sizeof(message));
                    msgtail = (msgtail+1) % MAXMSGQUEUE;
                    msgready++;
                   break;
                  }; break;
    } /*switch */
 }
  } /* FD_ISSET */
 
 if (FD_ISSET(internal_sock,&rset))
 {
 r = read(internal_sock,&m,sizeof(MESSAGE));

 if (r>0)
 {
  switch(m.msg_type)
  {
    case MSG_INT:
               switch(m.param.pword[0])
               { 
                case INT_CLOSE_INSTANCE: endrun(0);
                case INT_KILL: endrun(1);                     
                default:break;     
               } /* switch int */
    case MSG_NET: 
            if (m.param.pword[0] == NET_PROPAGATE)
            {
             /*printf("DEBUG: cint internal_sock MSG_NET NET_PROPAGATE cmd %d\n",m.param.pword[6]);*/
             memcpy(globmsgqueue+msgtail,&m.int_msg,sizeof(message));
             msgtail = (msgtail+1) % MAXMSGQUEUE;
             msgready++;
            };break;           
    
    
    
  }/*switch type */
 }
 } /* FD_ISSET */
 } /* select */

}


void request_id()
{
 MESSAGE m;
 G_MESSAGE m1; 
 
 m.msg_type = MSG_INT;
 m.param.pword[0] = INT_CTX_REQ;
 send_to_kernel(&m);
 while ( (m.msg_type != MSG_INT) || (m.param.pword[0] != INT_CTX) )
  read(internal_sock,&m,sizeof(MESSAGE));
  
    my_ctx.node = m.param.pword[1];
    my_ctx.program_id = m.param.pword[2];
    if (remote)
            {
              parent_ctx.node = m.param.pword[3];
              parent_ctx.program_id = m.param.pword[4];
              RInstance[parent_ctx.node] = parent_ctx.program_id;
            }
              else
        {
         parent_ctx.node = my_ctx.node;
         parent_ctx.program_id = my_ctx.program_id;
        } 
  ournode = my_ctx.node;
//  strcpy(nname,m.param.pstr);
//  net_sock = open(nname,O_WRONLY);
  m1.msg_type = MSG_GRAPH;
  m1.param.pword[0] = GRAPH_SET_TITLE;
  m1.param.pword[1] = GraphRes;
  sprintf(m1.param.pstr,
  "%s      ID: %d",ProgName,my_ctx.program_id);       
  if (remote) strcat(m1.param.pstr,"  REMOTE instance");
  send_to_graph(&m1);
}



void send_ready()
{
 int sock,len;
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
 
 sock = socket(AF_INET,SOCK_STREAM,0);
 bzero(&svr,sizeof(svr));
 svr.sin_family = AF_INET;
 svr.sin_addr.s_addr = INADDR_ANY;
 svr.sin_port = 0;
 bind(sock, (struct sockaddr*)&svr, sizeof(svr));
 listen(sock,5);
 len=sizeof(svr);
 getsockname(sock,(struct sockaddr*)&svr,&len);
 msg.param.pword[8] = ntohs(svr.sin_port);
 gethostname(name,&len);
 info = (struct hostent*)gethostbyname(name);
 bcopy((char*)info->h_addr,(char*)&svr.sin_addr,info->h_length);
 sprintf(msg.param.pstr,"%s",inet_ntoa(svr.sin_addr));
 send_to_kernel(&msg);
 
 bzero(&svr,sizeof(svr));
 DirConn[parent_ctx.node] = accept(sock,(struct sockaddr*)&svr,&len);
 fcntl(DirConn[parent_ctx.node], F_SETFL,O_NONBLOCK | fcntl(DirConn[parent_ctx.node], F_GETFL,0));
}

int main(argc, argv)
int argc;
char **argv;
{
    initiate(argc, argv);             /* initialize executor */
    runsys();              /* initialize running system */
    init_scheduler();
    GraphRes = get_graph_res();
    if ( GraphRes < 0 ) exit(12); 
     
    request_id();
    if (remote) send_ready(); 


    setjmp(contenv);         /* set label for continue long jump */
    while (TRUE)                     /* repeat until exit() is called */
    {
        get_internal();        
        schedule();         /* reschedule current process */
   if (ready != 0)
    {   
        decode();               /* fetch instruction */
        execute();            /* and execute it */
    } 
       
    }
    return 0;
} /* end main */

