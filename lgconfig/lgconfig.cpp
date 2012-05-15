

#include <qapp.h>
#include <qframe.h>
#include <qmlined.h>
#include <qmenubar.h>
#include <qpopmenu.h>
#include <qdialog.h>
#include <qbttngrp.h>
#include <qlabel.h>
#include <qlined.h>
#include <qlistbox.h>
#include <qpushbt.h>
#include <qradiobt.h>
#include <qlist.h>
#include <qfile.h>
#include <qcombo.h>
#include <qtooltip.h>
#include <qfont.h>
#include <qpixmap.h>
#include <qmsgbox.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


class VLPEntry
{
public:
 int ID;
 char addr[255];
 int type;  /* 0 - explicit */
 char progdir[255],homedir[255];
 char item[255];
};

class QInstall: public QFrame
{
 Q_OBJECT
public:
  QMenuBar *bar;  
  QListBox *nodelist;
  QPushButton *infob;
  QList<VLPEntry> Nodes;

  QInstall();
  bool check_id(int);
  bool check_addr(char*);

public slots:

  void SetOptions();
  void AddNode();
  void DelNode();
  void Info();

};


QApplication *app;

QInstall::QInstall()
{
  QFont f("Helvetica",12,QFont::Bold);
  QPixmap mp;

  infob = new QPushButton(this);
  
 

  bar = new QMenuBar(this);
  bar->insertItem("Configure",this,SLOT(SetOptions()));
  bar->insertItem("Quit",app,SLOT(quit()));
  bar->setFont(f);
  setCaption("VLP Configuration Tool");
  infob->setGeometry(0,bar->height(),200,30);
  if( mp.load("logo.bmp"))
 {
  infob->setPixmap(mp);
  infob->resize(mp.width(),mp.height());

 }
  resize(infob->width(),infob->height()+bar->height());
  Nodes.clear();
}


bool QInstall::check_id(int id)
{
 VLPEntry *pom;

 pom = Nodes.first();
 while (pom!=NULL)
 {
  if (pom->ID == id) return(FALSE);
  pom=Nodes.next();
  }
 return(TRUE);
}



bool QInstall::check_addr(char *addr)
{
 VLPEntry *pom;

 pom = Nodes.first();
 while (pom!=NULL)
 {
  if (strcmp(pom->addr,addr) == 0) return(FALSE);
  pom=Nodes.next();
  }
 return(TRUE);
}


void QInstall::Info()
{
}


void QInstall::AddNode()
{
 QDialog dlg(this,"",TRUE);
 QLabel* tmpQLabel;
 QLineEdit *id,*addr,*progs,*home;
 QPushButton *okbtn,*cancelbtn;
 VLPEntry *pom;
 char pomstr[255];

tmpQLabel = new QLabel( &dlg, "Label_2" );
tmpQLabel->setGeometry( 110, 10, 180, 30 );
tmpQLabel->setFrameStyle( 49 );
tmpQLabel->setText( "Virtual Processor Properties" );

id = new QLineEdit( &dlg, "LineEdit_1" );
id->setGeometry( 130, 50, 50, 30 );
id->setText( "" );

tmpQLabel = new QLabel( &dlg, "Label_3" );
tmpQLabel->setGeometry( 20, 50, 90, 30 );
tmpQLabel->setText( "Node number" );

tmpQLabel = new QLabel( &dlg, "Label_4" );
tmpQLabel->setGeometry( 20, 90, 80, 30 );
tmpQLabel->setText( "IP Address" );

addr = new QLineEdit( &dlg, "LineEdit_2" );
addr->setGeometry( 130, 90, 120, 30 );
addr->setText( "" );

tmpQLabel = new QLabel( &dlg, "Label_5" );
tmpQLabel->setGeometry( 20, 130, 100, 30 );
tmpQLabel->setText( "Connection type" );

QComboBox* tmpQComboBox;
tmpQComboBox = new QComboBox( FALSE, &dlg, "ComboBox_1" );
tmpQComboBox->setGeometry( 130, 130, 100, 30 );
tmpQComboBox->setSizeLimit( 2 );
tmpQComboBox->setAutoResize( FALSE );
tmpQComboBox->insertItem( "Explicit" );

tmpQLabel = new QLabel( &dlg, "Label_6" );
tmpQLabel->setGeometry( 20, 170, 110, 30 );
tmpQLabel->setText( "Programs directory" );

progs = new QLineEdit( &dlg, "LineEdit_4" );
progs->setGeometry( 130, 170, 230, 30 );
progs->setText( "" );

tmpQLabel = new QLabel( &dlg, "Label_7" );
tmpQLabel->setGeometry( 20, 210, 100, 30 );
tmpQLabel->setText( "VLP directory" );

home = new QLineEdit( &dlg, "LineEdit_5" );
home->setGeometry( 130, 210, 230, 30 );
home->setText( "" );

	okbtn = new QPushButton( &dlg, "PushButton_5" );
	okbtn->setGeometry( 80, 250, 100, 30 );
	okbtn->setText( "Ok" );
        connect(okbtn,SIGNAL(clicked()),&dlg,SLOT(accept()));
	cancelbtn = new QPushButton( &dlg, "PushButton_6" );
	cancelbtn->setGeometry( 210, 250, 100, 30 );
	cancelbtn->setText( "Cancel" );
        connect(cancelbtn,SIGNAL(clicked()),&dlg,SLOT(reject()));
	dlg.resize( 380, 300 );
    if (dlg.exec())
    {
     pom = new VLPEntry;
     pom->ID = atoi(id->text());
     if  (check_id(pom->ID))
     { 
      strcpy(pom->addr,addr->text());
      if (check_addr(pom->addr))
     {
    
     if (strcmp(tmpQComboBox->currentText(),"Explicit")==0) pom->type=0;
     strcpy(pom->progdir,progs->text());
     strcpy(pom->homedir,home->text());
     Nodes.append(pom);
     sprintf(pomstr,"Node: %d       Addr:%s       Home dir: %s",pom->ID,pom->addr,pom->homedir);
     nodelist->insertItem(pomstr);      
     strcpy(pom->item,pomstr);
     } else QMessageBox::message("Error!","Only one VLP on a single computer!","Ok");
     }
      else QMessageBox::message("Error!","ID must be unique!","Ok");
      }

}

void QInstall::DelNode()
{
 char pom[255];
 VLPEntry *vpom;

 if (nodelist->currentItem()>=0)
 {
   strcpy(pom,nodelist->text(nodelist->currentItem()));
   vpom = Nodes.first();
   while (vpom!=NULL)
    {
      if (strcmp(pom,vpom->item)==0) break;
      vpom = Nodes.next();
     }
   if (vpom!=NULL)
       if (QMessageBox::query("Delete VLP","Are you sure?","Yes","No"))
       {
        nodelist->removeItem(nodelist->currentItem());
        Nodes.remove(vpom);
        }
  }
}


void QInstall::SetOptions()
{
  QDialog dlg(this,"",TRUE);
  QLabel* tmpQLabel;
  QPushButton *addbtn,*delbtn,*okbtn,*cancelbtn;
  VLPEntry *pom;
  QFile *vlp_file;
  int i,j;
  char pomstr[255],line[255];
 	
        //dlg.setStyle(WindowsStyle);
	nodelist = new QListBox( &dlg, "ListBox_1" );
	nodelist->setGeometry( 20, 40, 480, 160 );

	tmpQLabel = new QLabel( &dlg, "Label_1" );
	tmpQLabel->setGeometry( 20, 10, 100, 30 );
	tmpQLabel->setText( "Nodes:" );

	addbtn = new QPushButton( &dlg, "PushButton_1" );
	addbtn->setGeometry( 30, 210, 100, 30 );
	addbtn->setText( "Add VLP" );
        connect(addbtn,SIGNAL(clicked()),this,SLOT(AddNode()));
	delbtn = new QPushButton( &dlg, "PushButton_2" );
	delbtn->setGeometry( 150, 210, 100, 30 );
	delbtn->setText( "Del VLP" );
        connect(delbtn,SIGNAL(clicked()),this,SLOT(DelNode()));
	okbtn = new QPushButton( &dlg, "PushButton_3" );
	okbtn->setGeometry( 270, 210, 100, 30 );
	okbtn->setText( "Save files" );
        connect(okbtn,SIGNAL(clicked()),&dlg,SLOT(accept()));
	cancelbtn = new QPushButton( &dlg, "PushButton_4" );
	cancelbtn->setGeometry( 390, 210, 100, 30 );
	cancelbtn->setText( "Cancel" );
        connect(cancelbtn,SIGNAL(clicked()),&dlg,SLOT(reject()));
	dlg.resize( 520, 260 );


         if (dlg.exec()){
              if (!Nodes.isEmpty())
            {
              pom = Nodes.first();
              while (pom!=NULL)
              {
                j = Nodes.at();
                sprintf(pomstr,"%s.cfg",pom->addr);
                vlp_file = new QFile(pomstr);
                vlp_file->open(IO_WriteOnly);
                sprintf(line,"progdir=%s\n",pom->progdir);
                vlp_file->writeBlock(line,strlen(line));
                sprintf(line,"homedir=%s\n",pom->homedir);
                vlp_file->writeBlock(line,strlen(line));
                sprintf(line,"node_number=%d\n",pom->ID);
                vlp_file->writeBlock(line,strlen(line));
                if ( pom->type == 0)
                 {
                   sprintf(line,"type=explicit\n");
                   vlp_file->writeBlock(line,strlen(line));
                  }
                for(i=0;i<Nodes.count();i++)
                  if (pom!=Nodes.at(i)) {
                                         sprintf(line,"host=%s\n",Nodes.at(i)->addr);
                                         vlp_file->writeBlock(line,strlen(line));
                                          }; 
                vlp_file->close();
                pom = Nodes.at(j);
                pom = Nodes.next();
               }
             }

            };



}


#include "lgconfig.moc"

int main( int argc, char **argv )
{

    app = new QApplication(argc,argv);
    QInstall cfg;
    //app->setStyle(WindowsStyle);
    app->setMainWidget(&cfg);
    cfg.show();
    return app->exec();
}
