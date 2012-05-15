/**************************************************************

  Copyright (C) 1997  Oskar Swida

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.




 NOTE: This software is using the free software license of 
       the QT library v.1.30 from Troll Tech AS.
       See the file LICENSE.QT.

 
  To contact the author, write:
     e-mail: swida@aragorn.pb.bialystok.pl

************************************************************/


#include <qpixmap.h>
//#include <qwindow.h>
#include <qapp.h>
#include <qframe.h>
#include <qmlined.h>
#include <qpainter.h>
#include <qcolor.h>
#include <qbrush.h>
#include <qmenubar.h>
#include <qpopmenu.h>
#include <qfont.h>
#include <qmsgbox.h>
#include <qfiledlg.h>
#include <qtabdlg.h>
#include <qstring.h>
#include <qrect.h>
#include <qdialog.h>
#include <qbttngrp.h>
#include <qlabel.h>
#include <qlined.h>
#include <qlistbox.h>
#include <qpushbt.h>
#include <qradiobt.h>
#include <qlist.h>
#include <qfile.h>
#include <qcursor.h>
#include <qcombo.h>
#include <qsocknot.h>
#include <qdir.h>
#include <qwindowsstyle.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "../head/genint1.h"
#include "../head/comm.h"
#include "socu.h"
#include <netinet/in.h>

#define GPATH "loggr"
#define IPATH "logi"
#define NPATH "logn"
#define REMOTE_PATH "REMOTE"
#define MAXINTERP 20
#define MAXINSTANCES 256 


#define MESG_COL	0
#define WARN_COL	1
#define NORM_COL	2


char CharLine[25]="________________________";
char myargs[5][255];


/* --------------- interpreter slot -----------------*/
class InterpEntry
{
public:
 int ID;                                // INT identifier
 bool remote;                           // Am I remote ?
 char fullname[255],shortname[255];     // Program name
 int sock;                              // Socket 
 QSocketNotifier *notify;             
 int RInstances[MAXINSTANCES];          // IDs of my remote INT modules
 ctx_struct p_ctx;                      // parent INT info
};
/*++++++++++++++++++++++++++++++++++++++++++*/

/*----------------- connection slot -------------*/
class ConnectEntry
{
public:
 char addr[256];
 ConnectEntry(char *s) {strcpy(addr,s);};
};

/* ++++++++++++++++++++++++++++++++++++++++++  */

QApplication *app;


/* ---------------------------------------------------------- */
/*                 KERNEL CLASS DEFINITION                    */
/* ---------------------------------------------------------- */

class QKernel : public QFrame
{
 Q_OBJECT
public:

  QMultiLineEdit *desktop;                        
  QMenuBar *bar;  
  QPopupMenu *p,*p1,*p2;
  char progdir[256];                              
  int NodeNumber,ConType;
  

  QKernel();


  virtual void resizeEvent( QResizeEvent *ev );

  void WriteMessage(char* msg);
  void InitMessage();


public slots:

  void n_impl();
  void Run_Prog();
  void Edit();
  void Help();
  void SetOptions();
  void AddAddress();
  void DelAddress();
  void LockConsole();
  void UnlockConsole();
  void MessageToNode();
  void QuitProc();
  void NetMessage();
  void IntMessage(int);
  void KillInterpreter();
  void Disconnect();
  void SetMessages();
  void Connect();
  void Info();

protected:
  virtual void closeEvent ( QCloseEvent * e );

private:
  QList<InterpEntry> Interpreters;         
  QList<ConnectEntry> ConnectList;
  QListBox *connections;
  int Tasks;                            // number of working interpreters
  int ActiveConnections;                // number of connected VLPs
  bool LOCKED,synchro,wait_for_info;
  char LockPasswd[25];
  int lockid,unlockid,qid,cwid,
  optid,prid,mid,msgid,toolsid,hid;
  int net_sock;
  int freeINTid;
  QSocketNotifier *Net_Notify;
  char HomeDir[255];
  bool info_messages;
  

  void LoadConfig();
  void RunGraphModule(char*);
  void RunNetModule();
  InterpEntry *findINTbySocket(int);
  InterpEntry *findINTbyID(int);
  InterpEntry *RunIntModule(char *ss,int r);
  void RemoteInstance(InterpEntry*,int);
  void CloseInstances(InterpEntry*);
  

};
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void QKernel::closeEvent ( QCloseEvent * e ) {
  e->ignore();
}

QKernel::QKernel()
{
 QFont f("Helvetica",10,QFont::Bold);
 QFont f1("Helvetica",10,QFont::Normal);
 QFont f2("Times Roman",10,QFont::Normal);
 QDir q(REMOTE_PATH);
 char ss[255];

 if (!q.exists())
   {
    sprintf(ss,"mkdir %s",REMOTE_PATH);
    system(ss);
    } 

 info_messages=TRUE;
 wait_for_info=FALSE;

 setCaption("Virtual LOGLAN Processor");
 setBackgroundColor(white);

 bar = new QMenuBar(this);
 bar->setFont(f); 
 p = new QPopupMenu();
 p->setFont(f2);
 p->insertItem("Execute",this,SLOT(Run_Prog()));
 p->insertItem("Kill",this,SLOT(KillInterpreter()));
 prid = bar->insertItem("&Program",p);
 p1 = new QPopupMenu();
 p1->insertItem("Message",this,SLOT(MessageToNode()));
 p1->insertSeparator();
 p1->insertItem("Connect",this,SLOT(Connect()));
 p1->insertItem("Disconnect",this,SLOT(Disconnect()));
 p1->insertItem("Info",this,SLOT(Info()));
 p1->setFont(f);
 mid = bar->insertItem("&Machine",p1);

 p2 = new QPopupMenu();
 cwid = p2->insertItem("Editor",this,SLOT(Edit()));
 hid = p2->insertItem("Help",this,SLOT(Help()));
 p2->insertSeparator(); 
 optid = p2->insertItem("Options",this,SLOT(SetOptions()));
 msgid = p2->insertItem("Info messages",this,SLOT(SetMessages()));
 p2->setItemChecked(msgid,TRUE);
 p2->insertSeparator(); 
 lockid = p2->insertItem("Lock console",this,SLOT(LockConsole()));
 unlockid = p2->insertItem("Unlock console",this,SLOT(UnlockConsole()));
 p2->setItemEnabled(unlockid,FALSE);
 LOCKED = FALSE;
 p2->setFont(f);
 toolsid = bar->insertItem("&Tools",p2);

 qid = bar->insertItem("&Quit",this,SLOT(QuitProc()));
 p->setFont(f);

 desktop = new QMultiLineEdit(this,"desktop");
 desktop->setAutoUpdate(TRUE);
 desktop->setReadOnly(TRUE);
 desktop->setFont(f1);

 resize(400,200);
 Tasks = 0;
 freeINTid = 1;
 ActiveConnections = 0;
 strcpy(LockPasswd,"");
 LoadConfig();
 RunNetModule();

 Net_Notify = new QSocketNotifier(net_sock,QSocketNotifier::Read,this);
 connect(Net_Notify,SIGNAL(activated(int)),this,SLOT(NetMessage()));

}


void QKernel::resizeEvent( QResizeEvent *ev )
{
    QFrame::resizeEvent(ev);
    if ( desktop )
	desktop->setGeometry( 0,bar->height(), width(),  height() - bar->height() );
}




void QKernel::n_impl()
{
 QMessageBox::information(this,"Function info","This function is not implemented yet...","Ok");
}


/* ###########     load configuration from file  ############# */

void QKernel::LoadConfig()
{
QFile f("vlp.cfg");
 QString line;
 QString val;
 int br=0;

 if (!f.exists())
 {
  WriteMessage("Cannot load configuration file!");sleep(2);exit(3);
  }
 f.open(IO_ReadOnly);
 br = f.readLine(line,256);
 while (br>0)
 {
  QStringList l = QStringList::split("=",line,FALSE);
  QStringList::Iterator it = l.begin();
  line = *it;
  ++it;
  val = *it;
  val = val.stripWhiteSpace();
  if (line == "node_number") {NodeNumber = val.toInt();};
  if (line == "type") {if (val=="explicit") ConType=1; else
                               ConType = 2; };
  if (line == "host" ) {ConnectList.append(new ConnectEntry((char*)val.ascii()));};
  if (line == "progdir") { strcpy(progdir,val.ascii());};
  if (line == "homedir") { strcpy(HomeDir,val.ascii());};
  br = f.readLine(line,256);
 }
 f.close();
}
/* +++++++++++++++++++++++++++++++++++++++++++++++ */


void QKernel::Run_Prog()
{
 int i;
 QString s(QFileDialog::getOpenFileName(progdir,"*.log",this));
 if ( !s.isNull())
 {
    i = s.find(".log");
    if (i>0)  s.remove(i,4);
    RunIntModule((char*)s.ascii(),0);
 }
}


void QKernel::Edit()
{
 char cmd[255];
 sprintf(cmd,"%s/modules/logedit %s %s %s %s %s %s &",HomeDir,HomeDir,
 myargs[0],myargs[1],myargs[2],myargs[3],myargs[4]);
 system(cmd);
}

void QKernel::Help()
{
 char cmd[255];
 sprintf(cmd,"%s/modules/loghelp %s/doc %s %s %s %s %s &",HomeDir,HomeDir,
  myargs[0],myargs[1],myargs[2],myargs[3],myargs[4]);
 system(cmd);
}


void QKernel::RunGraphModule(char *sk)
{
 char cmd[255];

     
     sprintf(cmd,"%s/modules/loggraph %s %s %s %s %s %s",HomeDir,sk,
      myargs[0],myargs[1],myargs[2],myargs[3],myargs[4]);
     strcat(cmd," &");

   if (system(cmd)!=0)
      WriteMessage("Cannot connect GRAPH resources");
   
}



void QKernel::RunNetModule()
{
  struct sockaddr_un svr;
 int len,on;
 int sock;
 char cmd[255];

     
      sprintf(cmd,"%s/modules/lognet %s %s %s %s %s %s",HomeDir,NPATH,
       myargs[0],myargs[1],myargs[2],myargs[3],myargs[4]);
      strcat(cmd," &");
      
      /* -------- socket for NET module -------- */
      unlink(NPATH);
      sock = socket(AF_UNIX,SOCK_STREAM,0);
      bzero(&svr,sizeof(svr));
      svr.sun_family = AF_UNIX;
      strcpy(svr.sun_path,NPATH);
      len = strlen(svr.sun_path)+sizeof(svr.sun_family);
      bind(sock,(struct sockaddr*)&svr, len);      
      listen(sock,5);

     if ( system(cmd) == 0)
     {
      net_sock = accept(sock,(struct sockaddr*)0,(unsigned int*)0);
      // close(sock); 
      if (net_sock != 0)
      {
        WriteMessage("NETWORK successfully connected");
        fcntl(net_sock,F_SETFL, O_NONBLOCK|fcntl(net_sock,F_GETFL,0));
        on=1;
        setsockopt(net_sock,IPPROTO_TCP,TCP_NODELAY,(char*)&on,sizeof(on)); 
       }
      else 
      {
      WriteMessage("Cannot connect NETWORK resources");
      WriteMessage("Exiting...");
      sleep(2);
      QuitProc(); 
      }
   }/* system OK */
      else 
      {
      WriteMessage("Cannot connect NETWORK resources");
      WriteMessage("Exiting...");
      sleep(2);
      QuitProc(); 
      }

}


void QKernel::Connect()
{
 QDialog d(this,"",TRUE);
 QLabel lab(&d,"IP Address:");
 QLineEdit ed(&d,"");
 QPushButton ob(&d,""),cb(&d,"");
 MESSAGE m;
	
        d.setFont(QFont("Helvetica",12,QFont::Bold)); 
        ob.setGeometry( 30, 60, 80, 30 );
	ob.setText( "Ok" );
        ob.setDefault(TRUE);
	lab.setGeometry( 10, 10, 60, 30 );
	lab.setText( "Address" );
	ed.setGeometry( 70, 10, 140, 30 );
	cb.setGeometry( 130, 60, 80, 30 );
	cb.setText( "Cancel" );
	d.resize( 240, 100 );
        
        connect(&ob,SIGNAL(clicked()),&d,SLOT(accept()));
        connect(&cb,SIGNAL(clicked()),&d,SLOT(reject())); 
  if (d.exec())
  {
    m.msg_type = MSG_NET;
    m.param.pword[0] = NET_CONNECT_TO;
    strcpy(m.param.pstr,ed.text());
    write(net_sock,&m,sizeof(MESSAGE)); 
   }
    
}

void QKernel::Disconnect()
{
 MESSAGE msg;

 if (info_messages) WriteMessage("Disconnecting from virtual machine");
 msg.msg_type = MSG_NET;
 msg.param.pword[0] = NET_DISCONNECT;
 write(net_sock,&msg,sizeof(MESSAGE));
}

void QKernel::QuitProc()
{
 MESSAGE msg;

if ( QMessageBox::question(this,"Close VLP","Terminate VLP ?",QMessageBox::Yes,QMessageBox::No,0) == QMessageBox::No ) return;
if (!LOCKED)
{
  /* 
   msg.msg_type = MSG_NET;
   msg.param.pword[0] = NET_DISCONNECT;
   write(net_sock,&msg,sizeof(MESSAGE));*/
 
    
    delete Net_Notify;
  
    msg.msg_type = MSG_NET;
    msg.param.pword[0] = NET_EXIT;
    write(net_sock,&msg,sizeof(MESSAGE));
  //  ::close(net_sock);

 app->quit();
}
}




void QKernel::AddAddress()
{
 QDialog d(this,"",TRUE);
 QLabel lab(&d,"IP Address:");
 QLineEdit ed(&d,"");
 QPushButton ob(&d,""),cb(&d,"");
	
 if (connections)
 {
        ob.setGeometry( 30, 60, 80, 30 );
	ob.setText( "Ok" );
        ob.setDefault(TRUE);
	lab.setGeometry( 10, 10, 60, 30 );
	lab.setText( "Address" );
	ed.setGeometry( 70, 10, 140, 30 );
	cb.setGeometry( 130, 60, 80, 30 );
	cb.setText( "Cancel" );
	d.resize( 240, 100 );
        connect(&ob,SIGNAL(clicked()),&d,SLOT(accept()));
        connect(&cb,SIGNAL(clicked()),&d,SLOT(reject())); 
  if (d.exec())
    if (strcmp(ed.text(),"")!=0)
    {
      connections->insertItem(ed.text());
     }
 }
}

void QKernel::DelAddress()
{
 if (connections)
 {
  if (connections->currentItem()!=-1)
   connections->removeItem(connections->currentItem());
 }
}



void QKernel::MessageToNode()
{
  QDialog *dlg;
  QLineEdit *nodenr;
  MESSAGE m;
        

        dlg = new QDialog(this,"Message",TRUE);

        nodenr = new QLineEdit(dlg,"number"); 
	nodenr->setGeometry( 90, 10, 50, 30 );
        nodenr->setText("");

	QLabel* tmpQLabel;
	tmpQLabel = new QLabel( dlg, "Label_1" );
	tmpQLabel->setGeometry( 10, 10, 77, 30 );
	tmpQLabel->setText( "Node number:" );

	tmpQLabel = new QLabel( dlg, "Label_2" );
	tmpQLabel->setGeometry( 10, 50, 70, 30 );
	tmpQLabel->setText( "Message:" );

	QLineEdit* msg;
	msg = new QLineEdit( dlg, "LineEdit_1" );
	msg->setGeometry( 80, 60, 330, 30 );
	msg->setText( "" );

	QPushButton* ob,*cb;
	ob = new QPushButton( dlg, "PushButton_1" );
	ob->setGeometry(  230, 10, 80, 30 );
	ob->setText( "Send" );
        ob->setDefault(TRUE);
	cb = new QPushButton( dlg, "PushButton_2" );
	cb->setGeometry( 330, 10, 80, 30 );
	cb->setText( "Cancel" );
	dlg->resize( 430, 110 );
        connect(ob,SIGNAL(clicked()),dlg,SLOT(accept()));
        connect(cb,SIGNAL(clicked()),dlg,SLOT(reject())); 
        dlg->setCaption("Send message to node");

   if (dlg->exec())
   {
     m.msg_type = MSG_NET;
     m.param.pword[0] = NET_PROPAGATE;
     m.param.pword[1] = MSG_VLP;
     m.param.pword[2] = NodeNumber;
     m.param.pword[4] = atoi(nodenr->text());
     m.param.pword[6] = VLP_WRITE;
     strcpy(m.param.pstr,msg->text());
     write(net_sock,&m,sizeof(MESSAGE));
  }
}

void QKernel::KillInterpreter()
{
  QDialog *dlg;
  QLineEdit *nodenr;
  MESSAGE m;
  InterpEntry *pom;
        
 dlg = new QDialog(this,"Message",TRUE);

 nodenr = new QLineEdit(dlg,"number"); 
 nodenr->setGeometry( 90, 10, 50, 30 );
 nodenr->setText("");

 QLabel* tmpQLabel;
 tmpQLabel = new QLabel( dlg, "Label_1" );
 tmpQLabel->setGeometry( 10, 10, 77, 30 );
 tmpQLabel->setText( "Interp. ID:" );
 QPushButton* ob,*cb;
	ob = new QPushButton( dlg, "PushButton_1" );
	ob->setGeometry(  160, 10, 80, 30 );
	ob->setText( "Kill" );
        ob->setDefault(TRUE);
	cb = new QPushButton( dlg, "PushButton_2" );
	cb->setGeometry( 260, 10, 80, 30 );
	cb->setText( "Cancel" );
	dlg->resize( 360, 50 );
        connect(ob,SIGNAL(clicked()),dlg,SLOT(accept()));
        connect(cb,SIGNAL(clicked()),dlg,SLOT(reject())); 
        dlg->setCaption("Kill interpreter");
 if (dlg->exec())
 {

   m.msg_type = MSG_INT;
   m.param.pword[0] = INT_KILL;
   pom = findINTbyID(atoi(nodenr->text()));
   if  (pom!=NULL)
   {
    if (!(pom->remote))
    write(pom->sock,&m,sizeof(MESSAGE));
    else WriteMessage("This is a remote instance of a program!");
   }
    else WriteMessage("Interpreter not found");
  
 }

}



void QKernel::NetMessage()
{
 MESSAGE msg;
 int cnt;
 char ss[255];
 InterpEntry *pom;

 cnt = read(net_sock,&msg,sizeof(MESSAGE));
 if ( (cnt>0) && (msg.msg_type==MSG_NET))
 {
   switch(msg.param.pword[0])
   {
   case NET_CSWRITELN: WriteMessage(msg.param.pstr);break;
   case NET_PROPAGATE: 
                  switch(msg.param.pword[1])
                    {
                     case MSG_INT:
                    /*  pom = find_link_by_ID(msg.param.pword[5]);
                     msg.msg_type = MSG_NET;
                     msg.param.pword[0] = NET_PROPAGATE;   
                     send_int(pom,&msg);*/
                     break;
                     case MSG_VLP:
                          switch(msg.param.pword[6])
                          {
                           case VLP_WRITE:
                                   QApplication::beep();
                                   WriteMessage(CharLine);
                                   WriteMessage("### Incoming Messsage ###");  
                                   sprintf(ss,"Mesg from Node %d: %s",msg.param.pword[2],msg.param.pstr);
                                   WriteMessage(ss);
                                   WriteMessage(CharLine);
                                   break;
                           case VLP_REMOTE_INSTANCE:
                            sprintf(ss,"%s/%s",REMOTE_PATH,msg.param.pstr); 
                            if (info_messages)
                            { 
                            WriteMessage("Running program:");
                            WriteMessage(ss);  
                            }
                            pom = RunIntModule(ss,1);
                            if (pom!=NULL)
                            {
                            pom->p_ctx.node = msg.param.pword[2];
                            pom->p_ctx.program_id = msg.param.pword[7];
                            pom->RInstances[msg.param.pword[2]] = msg.param.pword[7];
                            }
                           break;
                          case VLP_CLOSE_INSTANCE:
                             msg.msg_type = MSG_INT;
                             msg.param.pword[0] = INT_CLOSE_INSTANCE;
                             pom = findINTbyID(msg.param.pword[7]);
                             if (pom!=NULL)
                             {
                               write(pom->sock,&msg,sizeof(MESSAGE));
                               MESSAGE m1;
                               m1.msg_type = MSG_VLP;
                               m1.param.pword[0] = VLP_INTERPRETER_DOWN;
                               m1.param.pword[1] = pom->ID;
                               write(net_sock,&m1,sizeof(MESSAGE));
                               } else WriteMessage("Instance not found"); 
                           break; 
                          } /* VLP switch */        
                                      
                          }/* switch */
                       break;
   case NET_CONNECTIONS: ActiveConnections = msg.param.pword[1];
                         WriteMessage(msg.param.pstr);
                         if (!synchro) synchro=TRUE;
                         break;
   case NET_INFO:
                if (wait_for_info) 
                {
                 QString poms,poms1,poms2;
                 poms.sprintf("%s",msg.param.pstr);
                 while (poms.length()>0)
                 {
                  cnt=poms.find(';');
                  if (cnt!=-1)
                  { poms1=poms.left(cnt);
                    poms=poms.right(poms.length()-cnt-1);
                    cnt=poms1.find('=');
                    if (cnt!=-1)
                     {
                       poms2=poms1.left(cnt);
                       poms1=poms1.right(poms1.length()-cnt-1);
                       sprintf(ss,"Node: %s Addr: %s",poms2.data(),poms1.data());
                       WriteMessage(ss); 
                      }
                   } 
                 }
                } 
		break; 
   case NET_INFO_END:wait_for_info=FALSE;
                     WriteMessage(CharLine);
                     break;
   } /* switch */
  }
 
}




void QKernel::IntMessage(int sock)
{
 MESSAGE msg;
 int cnt;
 InterpEntry *e;

 cnt = read(sock,&msg,sizeof(MESSAGE));
 e = findINTbySocket(sock);
 if ( (cnt>0) && (e!=NULL))
 {
 switch (msg.msg_type)
 { 
   case MSG_GRAPH:
                  if (msg.param.pword[0]==GRAPH_ALLOCATE)
                  {
                    RunGraphModule(msg.param.pstr);
                    }  
                  break;
   case MSG_NET:
     write(net_sock,&msg,sizeof(MESSAGE));break;    
   case MSG_VLP:
      switch(msg.param.pword[0])
       {
         case VLP_REMOTE_INSTANCE_PLEASE:
           RemoteInstance(e,msg.param.pword[2]);
         break;
        }/* switch */
       break;  
   case MSG_INT:
       switch(msg.param.pword[0]){
           case INT_EXITING:
             {
               char ss[255];
               
               MESSAGE m;
               m.msg_type = MSG_VLP;
               m.param.pword[0] = VLP_INTERPRETER_DOWN;
               m.param.pword[1] = e->ID;
               write(net_sock,&m,sizeof(MESSAGE));
               if (e->remote==0) CloseInstances(e);
               delete e->notify;
               ::close(e->sock);   
               Interpreters.remove(e);
               delete e;
               if (info_messages)
               {
               sprintf(ss,"%s : End of program execution",msg.param.pstr);
               WriteMessage(ss);
               }
              };break;
            case INT_CTX_REQ:
                
                 msg.msg_type = MSG_INT;
                 msg.param.pword[0] = INT_CTX;
                 msg.param.pword[1] = NodeNumber;
                 msg.param.pword[2] = e->ID;
                if (e->remote)
                 {
                   msg.param.pword[3] = e->p_ctx.node;
                   msg.param.pword[4] = e->p_ctx.program_id;
                  }
                 write(sock,&msg,sizeof(MESSAGE)); 
                 break;
             };break; /* switch param.pword[0] */
 } /* switch type */
 } /* if */
}


void QKernel::WriteMessage(char *msg)
{
 int x,y;
 
 desktop->getCursorPosition(&x,&y);
 if (x>100) desktop->clear();
 desktop->setReadOnly(FALSE);
 desktop->append(msg);
 desktop->setReadOnly(TRUE);
 desktop->setCursorPosition(desktop->numLines(),1);
 desktop->repaint();
 if (desktop->numLines()>100) desktop->clear();
}

void QKernel::SetMessages()
{

 if (p2!=NULL)
 {
 if ( p2->isItemChecked(msgid))
 {
  p2->setItemChecked(msgid,FALSE);
  info_messages=FALSE;
 }
 else
 {
  p2->setItemChecked(msgid,TRUE);  
  info_messages=TRUE;
 }
} // !=NULL 
// bar->repaint();
}

void QKernel::SetOptions()
{
  QDialog dlg(this,"Options",TRUE);
  QFile *vlp_file;
  ConnectEntry *e;
  char line[256];
  unsigned int i;


 	QLineEdit* progs;
	progs = new QLineEdit( &dlg, "progs" );
	progs->setGeometry( 150, 20, 180, 30 );
        progs->setText(progdir);

	QLabel* tmpQLabel;
	tmpQLabel = new QLabel( &dlg, "Label_1" );
	tmpQLabel->setGeometry( 30, 20, 120, 30 );
	tmpQLabel->setText( "Programs directory" );

	QFrame* tmpQFrame;
	tmpQFrame = new QFrame( &dlg, "Frame_2" );
	tmpQFrame->setGeometry( 10, 60, 380, 30 );
	tmpQFrame->setFrameStyle( 52 );

	tmpQLabel = new QLabel( &dlg, "Label_2" );
	tmpQLabel->setGeometry( 10, 80, 340, 30 );
	tmpQLabel->setText( "Virtual Processor properties (activated after restarting VLP):" );

        QLineEdit *nn;
        char nns[256];
	nn = new QLineEdit( &dlg, "LineEdit_2" );
	nn->setGeometry( 110, 110, 40, 30 );
        sprintf(nns,"%d",NodeNumber);
        nn->setText(nns);

	tmpQLabel = new QLabel( &dlg, "Label_3" );
	tmpQLabel->setGeometry( 20, 110, 90, 30 );
	tmpQLabel->setText( "Node number:" );

	QRadioButton *exp,*reg;
	exp = new QRadioButton( &dlg, "RadioButton_3" );
	exp->setGeometry( 30, 170, 100, 30 );
	exp->setText( "Explicit" );
	exp->setChecked( TRUE );

	reg = new QRadioButton( &dlg, "RadioButton_4" );
	reg->setGeometry( 30, 200, 100, 30 );
	reg->setText( "Registration" );
        reg->setEnabled(FALSE);

	connections = new QListBox( &dlg, "ListBox_1" );
	connections->setGeometry( 170, 140, 130, 100 );
        e = ConnectList.first();
        while (e!=NULL)
        {
         connections->insertItem(e->addr);
         e = ConnectList.next();
         }

	tmpQLabel = new QLabel( &dlg, "Label_5" );
	tmpQLabel->setGeometry( 170, 110, 100, 30 );
	tmpQLabel->setText( "Connection list:" );

	QPushButton* addbtn,*delbtn,*okbtn,*cancelbtn;
	addbtn = new QPushButton( &dlg, "PushButton_1" );
	addbtn->setGeometry( 310, 150, 60, 30 );
	addbtn->setText( "Add" );
	delbtn = new QPushButton( &dlg, "PushButton_2" );
	delbtn->setGeometry( 310, 200, 60, 30 );
	delbtn->setText( "Del" );
        connect(addbtn,SIGNAL(clicked()),this,SLOT(AddAddress()));
        connect(delbtn,SIGNAL(clicked()),this,SLOT(DelAddress()));
	okbtn = new QPushButton( &dlg, "PushButton_3" );
	okbtn->setGeometry( 80, 260, 100, 30 );
	okbtn->setText( "Ok" );
        okbtn->setDefault(TRUE);
	cancelbtn = new QPushButton( &dlg, "PushButton_4" );
	cancelbtn->setGeometry( 210, 260, 100, 30 );
	cancelbtn->setText( "Cancel" );
        connect(okbtn,SIGNAL(clicked()),&dlg,SLOT(accept()));
        connect(cancelbtn,SIGNAL(clicked()),&dlg,SLOT(reject()));        
	QButtonGroup* group;
	group = new QButtonGroup( &dlg, "ButtonGroup_1" );
	group->setGeometry( 20, 150, 120, 90 );
	group->setTitle( "Connection type" );
	group->setAlignment( 1 );
	group->lower();
        group->insert(exp,1);
        group->insert(reg,2);  

	dlg.resize( 400, 310 );
        if (dlg.exec())
   {
    unlink("vlp.cfg");
    vlp_file = new QFile("vlp.cfg");
    vlp_file->open(IO_WriteOnly);
    sprintf(line,"progdir=%s\n",progs->text());
    vlp_file->writeBlock(line,strlen(line));
    strcpy(progdir,progs->text());  
    sprintf(line,"node_number=%d\n",atoi(nn->text()));
    vlp_file->writeBlock(line,strlen(line));
    if ( exp->isChecked())
    {
    sprintf(line,"type=explicit\n");
    vlp_file->writeBlock(line,strlen(line));
    for(i=0;i<connections->count();i++)
     {
      sprintf(line,"host=%s\n",connections->text(i));
      vlp_file->writeBlock(line,strlen(line));
     }
    
    }
     else
    { 
    sprintf(line,"type=register\n");
    vlp_file->writeBlock(line,strlen(line));
    } 
    vlp_file->close();
    };
}


void QKernel::LockConsole()
{
 QDialog d(this,"Enter password",TRUE);
 QLabel lab(&d,"Password");
 QLineEdit ed(&d,"");
 QPushButton ob(&d,""),cb(&d,"");
	
        d.setCaption("Lock console"); 
        ob.setGeometry( 30, 60, 80, 30 );
	ob.setText( "Ok" );
        ob.setDefault(TRUE);
	lab.setGeometry( 10, 10, 60, 30 );
	lab.setText( "Password:" );
	ed.setGeometry( 70, 10, 140, 30 );
        ed.setEchoMode(QLineEdit::Password);
	cb.setGeometry( 130, 60, 80, 30 );
	cb.setText( "Cancel" );
	d.resize( 240, 100 );
        connect(&ob,SIGNAL(clicked()),&d,SLOT(accept()));
        connect(&cb,SIGNAL(clicked()),&d,SLOT(reject())); 
 
   if (d.exec())
     if (strcmp(ed.text(),"")!=0)
    {
      strcpy(LockPasswd,ed.text());
      lab.setText("Retype:");
      ed.setText("");
      if (d.exec())
      {
        if (strcmp(ed.text(),LockPasswd)==0)
        {
          bar->setItemEnabled(qid,FALSE);
          bar->setItemEnabled(prid,FALSE);
          bar->setItemEnabled(mid,FALSE);
          p2->setItemEnabled(unlockid,TRUE);
          p2->setItemEnabled(lockid,FALSE);
          p2->setItemEnabled(cwid,FALSE);
          p2->setItemEnabled(optid,FALSE);
          bar->repaint();
          WriteMessage("CONSOLE LOCKED");
          LOCKED = TRUE;
        }
         else
             {
                QMessageBox msg(this);
                msg.setText("Not matching!");
                msg.setButtonText(0,"Close");
                msg.show();

             }   
       }
       else strcpy(LockPasswd,"");
    }
}

void QKernel::UnlockConsole()
{
 QDialog d(this,"Enter password",TRUE);
 QLabel lab(&d,"Password");
 QLineEdit ed(&d,"");
 QPushButton ob(&d,""),cb(&d,"");

        ob.setGeometry( 30, 60, 80, 30 );
	ob.setText( "Ok" );
        ob.setDefault(TRUE);
	lab.setGeometry( 10, 10, 60, 30 );
	lab.setText( "Password:" );
	ed.setGeometry( 70, 10, 140, 30 );
        ed.setEchoMode(QLineEdit::Password);
	cb.setGeometry( 130, 60, 80, 30 );
	cb.setText( "Cancel" );
	d.resize( 240, 100 );
        connect(&ob,SIGNAL(clicked()),&d,SLOT(accept()));
        connect(&cb,SIGNAL(clicked()),&d,SLOT(reject())); 

 if (d.exec())
 {
   if (strcmp(ed.text(),LockPasswd)==0)
   {
          bar->setItemEnabled(qid,TRUE);
          bar->setItemEnabled(prid,TRUE);
          bar->setItemEnabled(mid,TRUE);
          p2->setItemEnabled(unlockid,FALSE);
          p2->setItemEnabled(lockid,TRUE);
          p2->setItemEnabled(cwid,TRUE);
          p2->setItemEnabled(optid,TRUE);
          bar->repaint();
          WriteMessage("CONSOLE UNLOCKED");
          LOCKED = FALSE;         
   }
   else
          {
                QMessageBox msg(this);
                msg.setText("Wrong password!");
                msg.setButtonText(0,"Close");
                msg.show();

             }      
 }
}

void QKernel::InitMessage()
{
 WriteMessage("\n Virtual LOGLAN Processor - ver 1.9: READY \n");
}



InterpEntry *QKernel::findINTbySocket(int _id)
{
 InterpEntry *pom;
 pom = Interpreters.first();
 while (pom!=NULL)
 {
  if (pom->sock == _id) break;
  pom = Interpreters.next();
 }
 return(pom);
}

InterpEntry *QKernel::findINTbyID(int _id)
{
 InterpEntry *pom;
 pom = Interpreters.first();
 while (pom!=NULL)
 {
  if (pom->ID == _id) break;
  pom = Interpreters.next();
 }
 return(pom);
}


/* ------------------ Connect INT module -----------------*/

InterpEntry *QKernel::RunIntModule(char *ss, int r)
{
 char a[256],b[255];
 struct sockaddr_un svr;
 int len,sock,i,on;
 int newint=-1;
 char cmd[255];
 FILE *cf;
 MESSAGE msg;
 InterpEntry *newINT;
 


 newINT = NULL;
 
 strcpy(a,ss);
 strcat(a,".ccd");
 cf = fopen(a,"r");
 if (cf == NULL) {WriteMessage("File not found: no .ccd file");return(NULL);}
 fclose(cf);
 
 strcpy(a,ss);
 strcat(a,".pcd");
 cf = fopen(a,"r");
 if (cf == NULL) {WriteMessage("File not found: no .pcd file");return(NULL);}
 fclose(cf);
 
 newINT = new InterpEntry;
 for(i=0;i<MAXINSTANCES;i++) newINT->RInstances[i]=-1;

 strcpy(b,rindex(ss,'/'));
 for(i=0;i<strlen(b);i++) 
      b[i] = b[i+1];
 if (info_messages)
 {
 sprintf(a,"%s : Start execution",b);
 WriteMessage(a); 
 }

 newint = freeINTid; freeINTid++;
 newINT->ID = newint;
 strcpy(newINT->shortname,b);
 strcpy(newINT->fullname,ss);
    
 sprintf(a,"%s%d",IPATH,newint);
 sprintf(cmd,"%s/modules/logint %s %s",HomeDir,a,ss);
 if (r) strcat(cmd," r");
 sprintf(b," %s %s %s %s %s",myargs[0],myargs[1],myargs[2],myargs[3],myargs[4]);
 strcat(cmd,b);
 strcat(cmd," &");
  

    sock = socket(AF_UNIX,SOCK_STREAM,0);
    unlink(a);
    bzero(&svr, sizeof(svr));
    svr.sun_family = AF_UNIX;
    strcpy(svr.sun_path,a);
    len = strlen(svr.sun_path)+sizeof(svr.sun_family);
    bind(sock,(struct sockaddr*)&svr, len);
    listen(sock,5);
    system(cmd); 
    newINT->sock = accept(sock,(struct sockaddr*)0,(unsigned int *)0);
    //::close(sock);

         
 if (newINT->sock>0) 
{

   fcntl(newINT->sock,F_SETFL,
      O_NONBLOCK|fcntl(newINT->sock,F_GETFL,0));
   on=1; 
   setsockopt(newINT->sock,IPPROTO_TCP,TCP_NODELAY,(char*)&on,sizeof(on)); 
   if (r) newINT->remote = 1;else newINT->remote=0; 
  
   bzero(&msg,sizeof(MESSAGE));
   msg.msg_type = MSG_VLP;
   msg.param.pword[0] = VLP_REGINT;
   msg.param.pword[1] = newINT->ID;
   sprintf(msg.param.pstr,"logi%d.net",newint);
   write(net_sock,&msg,sizeof(MESSAGE)); 
 
   Interpreters.append(newINT);
   newINT->notify = new QSocketNotifier(newINT->sock,QSocketNotifier::Read);
   connect(newINT->notify,SIGNAL(activated(int)),this,SLOT(IntMessage(int)));
   if (info_messages) WriteMessage("INTERPRETER successfully connected");  
   
}
 else WriteMessage("Cannot connect interpreter");
 
 return(newINT);    

}

/* ---------------------------------------------------------*/
/*            Allocate remote instance                      */

void QKernel::RemoteInstance(InterpEntry *interp, int on)
{
 MESSAGE m;
 char s[255];
 
 
 m.msg_type = MSG_NET;
 m.param.pword[0] = NET_NODE_EXIST;
 m.param.pword[1] = on;
 m.param.pword[2] = interp->ID;
 write(net_sock,&m,sizeof(MESSAGE));
 bzero(&m,sizeof(MESSAGE));
 while( (m.msg_type!=MSG_NET) && (m.param.pword[0]!=NET_NODE_EXIST) )
  read(net_sock,&m,sizeof(MESSAGE));
 
if (m.param.pword[1]==1) /* means node exists */
{
 m.msg_type = MSG_NET;
 m.param.pword[0] = NET_TRANSMIT_CODE;
 m.param.pword[1] = interp->ID;
 m.param.pword[2] = on;
 strcpy(m.param.pstr,interp->fullname);
 write(net_sock,&m,sizeof(MESSAGE));
 
 Net_Notify->setEnabled(FALSE);
 while ( (m.msg_type != MSG_NET) || (m.param.pword[0]!=NET_TRANSMITTED) )
 read(net_sock,&m,sizeof(MESSAGE));


 m.msg_type = MSG_NET;
 m.param.pword[0] = NET_PROPAGATE;
 m.param.pword[1] = MSG_VLP;
 m.param.pword[2] = NodeNumber;
 m.param.pword[3] = 0;
 m.param.pword[4] = on;
 m.param.pword[5] = 0;
 m.param.pword[6] = VLP_REMOTE_INSTANCE;
 m.param.pword[7] = interp->ID;
 strcpy(m.param.pstr,interp->shortname);
 write(net_sock,&m,sizeof(MESSAGE));
 
 read(net_sock,&m,sizeof(MESSAGE));
 while (1)
 {
   if ( (m.param.pword[0]==NET_PROPAGATE) && (m.param.pword[6] == VLP_REMOTE_INSTANCE_OK))
   {
    interp->RInstances[on] = m.param.pword[7];
    break;
   }  
  read(net_sock,&m,sizeof(MESSAGE));  
 }

 Net_Notify->setEnabled(TRUE);

 /*bzero(&m,sizeof(MESSAGE));*/
 m.msg_type = MSG_VLP;
 m.param.pword[0] = VLP_REMOTE_INSTANCE_HERE;
 m.param.pword[1] = interp->RInstances[on];
 write(interp->sock,&m,sizeof(MESSAGE));
}
 else /* There is no such a node! */
{
 sprintf(s,"Warning: Node number %d not found!",on); 
 WriteMessage(s);
 WriteMessage("Allocating O-process on the local node");
 bzero(&m,sizeof(MESSAGE));
 m.msg_type = MSG_VLP;
 m.param.pword[0] = VLP_REMOTE_INSTANCE_HERE;
 m.param.pword[1] = interp->ID;
 write(interp->sock,&m,sizeof(MESSAGE));
}
}


/*-----------------------------------------------*/
/*           Close all remote instances         */

void QKernel::CloseInstances(InterpEntry *e)
{
 MESSAGE msg;
 int i;

 if (info_messages)  WriteMessage("Closing remote instances");
 for(i=0;i<MAXINSTANCES;i++)
  if (e->RInstances[i]>=0)
 {
  msg.msg_type = MSG_NET;
  msg.param.pword[0] = NET_PROPAGATE;
  msg.param.pword[1] = MSG_VLP;
  msg.param.pword[2] = NodeNumber;
  msg.param.pword[4] = i;
  msg.param.pword[6] = VLP_CLOSE_INSTANCE;
  msg.param.pword[7] = e->RInstances[i];
  write(net_sock,&msg,sizeof(MESSAGE));
  }
}


void QKernel::Info()
{
 MESSAGE m;

 WriteMessage(CharLine);
 WriteMessage("### Virtual Machine Information ###");
 m.msg_type = MSG_NET;
 m.param.pword[0]=NET_GET_INFO;
 write(net_sock,&m,sizeof(MESSAGE));
 wait_for_info = TRUE;
}

#include "kernel.moc"

int main( int argc, char **argv )
{
    int i;
    for(i=0;i<5;i++) strcpy(myargs[i],"");
    for(i=1;i<argc;i++) strcpy(myargs[i-1],argv[i]);
    
    app = new QApplication(argc,argv);
    app->setStyle(new QWindowsStyle());
    QKernel   draw;
    app->setMainWidget( &draw);
    draw.show();
    draw.InitMessage();
    return app->exec();
}
