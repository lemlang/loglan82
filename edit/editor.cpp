

#include <qapp.h>
#include <qmenubar.h>
#include <qpopmenu.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qfile.h>
#include <qfiledlg.h>
#include <qfontmet.h>
#include <qtooltip.h>
#include <qfont.h>
#include <qpixmap.h>
#include <qmsgbox.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <qscrbar.h>
#include <qpainter.h>
#include <qcolor.h>
#include <qpoint.h>
#include <qtstream.h>
#include <qchkbox.h>
#include <qkeycode.h>

#include "editor.h"

#define TYPENUM	5


Editor *e;


char *UnitTypes[5] = {"CLASS","PROCEDURE","FUNCTION","PROCESS","COROUTINE"};




My_Edit::My_Edit(QWidget *parent,const char *name)
: QMultiLineEdit(parent,name)
{
 parent = NULL;
 name = NULL;
}

void My_Edit::keyPressEvent(QKeyEvent *ev)
{
 QMultiLineEdit::keyPressEvent(ev);
 emit cursorMove();
}


void Editor::closeEvent ( QCloseEvent * e ) {
  e->ignore();
}

Editor::Editor( char *hdir,QWidget * parent , const char * name)
    : QWidget( parent, name )
{
    QFont f1("Helvetica",10,QFont::Bold);


    strcpy(HomeDir,hdir);
    strcpy(find_text,"");
    sensitive=FALSE;
    m = new QMenuBar( this, "menu" );
    m->setFont(f1);
    QPopupMenu * file = new QPopupMenu();
    QPopupMenu * comp = new QPopupMenu();
    QPopupMenu * loglan = new QPopupMenu();
    QPopupMenu *medit = new QPopupMenu();

    file->setFont(f1);
    comp->setFont(f1);
    loglan->setFont(f1);  
    medit->setFont(f1);
    CHECK_PTR( file );
    CHECK_PTR( comp );
    m->insertItem( "&File ", file );
    m->insertItem( "&Edit", medit );
    m->insertItem( "&Compile", this,SLOT(cmp()) );

//    m->insertItem( "&LOGLAN ", loglan );
    m->insertItem( "&Properties", this, SLOT(props()));    




    file->insertItem( "New ",   this, SLOT(create()), CTRL+Key_N);
    file->insertItem( "Open ",  this, SLOT(load()), CTRL+Key_O);
    file->insertItem( "Save ",  this, SLOT(save()),CTRL+Key_S);
    file->insertItem( "Save as",  this, SLOT(save_as()),CTRL+Key_A);
    file->insertSeparator();
    file->insertItem( "Quit ", qApp,  SLOT(quit()));

//    comp->insertItem( "Compile ",   this, SLOT(cmp()), CTRL+Key_C);
//    comp->insertItem( "Gen ",  this, SLOT(gen()), CTRL+Key_G);
//    comp->insertItem( "Compile & Gen ",  this, SLOT(comp_all()));

//    loglan->insertItem( "Program structure",  this, SLOT(log_prog()));
//    loglan->insertItem( "Unit structure",   this, SLOT(log_unit()));


    
    e = new My_Edit( this, "editor" );
    connect(e,SIGNAL(cursorMove()),this,SLOT(updateline()));
    medit->insertItem("Copy ",e,SLOT(copy()),CTRL+Key_Insert);
    medit->insertItem("Paste ",e,SLOT(paste()), SHIFT+Key_Insert);
    medit->insertItem("Cut ",e,SLOT(cut()), CTRL+Key_Delete);
    medit->insertItem("Clear All ",e,SLOT(clear()));
    medit->insertSeparator();
    medit->insertItem("Find ",this,SLOT(findText()), CTRL+Key_F);
    medit->insertItem("Find Next ",this,SLOT(find_next()),CTRL+Key_L);    

    msg = new QMultiLineEdit( this, "messages" );
    msg->setReadOnly(TRUE);
    compiler_path.sprintf("%s/%s",HomeDir,"compile/logcomp");
    gen_path.sprintf("%s/%s",HomeDir,"compile/gen");
    file_path.sprintf("%s",HomeDir);

    QFont f2("Times",14,QFont::Bold);
    e->setFont(f1);
    QColor col(200,200,200);
    QColorGroup grp(black,col,col.light(),col.dark(),col.dark(),black,col);
    
    msg->setPalette(QPalette(grp,grp,grp));
    position = new QLabel(this);
    position->setFont(f2);
    position->setFrameStyle(QFrame::NoFrame);
    position->setAutoResize(TRUE);
    resize(400,300);
    
}


Editor::~Editor()
{
   
}




void Editor::updateline()
{
 char pom[255];
 int cx,cy;
 
 e->getCursorPosition(&cx,&cy);
 sprintf(pom," %d:%d ",cx,cy);
 position->setText(pom);
}

void Editor::resizeEvent( QResizeEvent * )
{
    if ( e && m )
     {
	e->setGeometry( 0, m->height(), width(), 3*(int)(( height() - m->height() )/4) );
	msg->setGeometry( 0, m->height()+ e->height(), width(), (int)(( height() - m->height() )/4) );
        position->setGeometry(width()-80,m->height()+ e->height()-10,
                              position->width(),position->height());
     }
}


void Editor::load()
{
    QString fn = QFileDialog::getOpenFileName(file_path.data(),"*.log");
    if ( !fn.isEmpty() ) 
	load( fn );
}


void Editor::load( const char *fileName )
{
   fname.sprintf("%s",fileName);


    QFile f( fileName );
    if ( !f.open( IO_ReadOnly ) )
	return;

    e->setAutoUpdate( FALSE );
    e->clear();

    QTextStream t(&f);
    while ( !t.eof() ) {
	QString s = t.readLine();
	e->append( s );
    }
    f.close();

    e->setAutoUpdate( TRUE );
    e->repaint();
    setCaption( fileName );
}


void Editor::save()
{
  if (fname.isEmpty())
  {
  QString fn = QFileDialog::getSaveFileName(file_path.data(),"*.log");
    if ( !fn.isEmpty() )
       { 
        fname.sprintf("%s",fn.data());
	save( fn );
       }
  }
   else
    save(fname);
 setCaption(fname);
}

void Editor::save_as()
{
 QString fn = QFileDialog::getSaveFileName(file_path.data(),"*.log");
    if ( !fn.isEmpty() )
       { 
        fname.sprintf("%s",fn.data());
	save( fn );
       }
 setCaption(fname);
}

void Editor::save(const char *fileName)
{

  QFile f(fileName);
  if (!f.open(IO_WriteOnly)) return;
  f.reset();
  f.writeBlock( e->text().data(),e->text().length() );
  f.close();
  
   
}


void Editor::create()
{
 e->clear();
 fname.sprintf("%s","");   
}


void Editor::print()
{
    
}



void Editor::cmp()
{
 save();
 compile(COMP_MODE);
}

void Editor::gen()
{
 compile(GEN_MODE);
}

void Editor::comp_all()
{
 save();
 compile(ALL_MODE);
}


void Editor::compile(int mode)
{
 char cmd[255];

    msg->setAutoUpdate( FALSE );
    msg->setReadOnly(FALSE);
    msg->clear();
    msg->repaint(); 


 //i = fname.find('.');
// if (i>=0)
 {
  QString fn = fname.data();
//  fn.truncate(i);

 switch(mode)
 {

 case COMP_MODE:
            sprintf(cmd,"%s %s > comp_data!",compiler_path.data(),fname.data());
            break;
 case GEN_MODE:
            sprintf(cmd,"%s %s > comp_data!",gen_path.data(),fn.data());
            break;
 case ALL_MODE:
            sprintf(cmd,"%s %s > comp_data!",compiler_path.data(),fn.data());
            system(cmd);    
            sprintf(cmd,"%s %s >> comp_data!",gen_path.data(),fn.data());
            break;

 } /*switch */

  system(cmd);
  QFile f( "comp_data!" );
    if ( !f.open( IO_ReadOnly ) )
	return;


    QTextStream t(&f);
    while ( !t.eof() ) {
	QString s = t.readLine();
	msg->append( s );
    }
    f.close();
    msg->setReadOnly(TRUE);
    msg->setAutoUpdate( TRUE );
    msg->repaint(); 
    unlink("comp_data!");
 }
}



void Editor::props()
{
 QDialog dlg(this,"Properties",TRUE);


	QLineEdit* files;
	files = new QLineEdit( &dlg, "f_path" );
	files->setGeometry( 130, 20, 250, 30 );
	files->setText( file_path.data() );
	files->setMaxLength( 32767 );
	files->setEchoMode( QLineEdit::Normal );
	files->setFrame( TRUE );

	QLabel* tmpQLabel;
	tmpQLabel = new QLabel( &dlg, "Label_1" );
	tmpQLabel->setGeometry( 10, 20, 100, 30 );
	tmpQLabel->setText( "Path to files:" );
	tmpQLabel->setAlignment( 289 );
	tmpQLabel->setMargin( -1 );

	tmpQLabel = new QLabel( &dlg, "Label_2" );
	tmpQLabel->setGeometry( 10, 60, 100, 30 );
	tmpQLabel->setText( "Path to compiler:" );
	tmpQLabel->setAlignment( 289 );
	tmpQLabel->setMargin( -1 );

/*	tmpQLabel = new QLabel( &dlg, "Label_3" );
	tmpQLabel->setGeometry( 10, 100, 100, 30 );
	tmpQLabel->setText( "Path to gen:" );
	tmpQLabel->setAlignment( 289 );
	tmpQLabel->setMargin( -1 );*/

	QLineEdit* compp;
	compp = new QLineEdit( &dlg, "l_path" );
	compp->setGeometry( 130, 60, 250, 30 );
	compp->setText( compiler_path.data() );
	compp->setMaxLength( 32767 );
	compp->setEchoMode( QLineEdit::Normal );
	compp->setFrame( TRUE );

/*	QLineEdit* genp;
	genp = new QLineEdit( &dlg, "g_path" );
	genp->setGeometry( 130, 100, 250, 30 );
	genp->setText( gen_path.data() );
	genp->setMaxLength( 32767 );
	genp->setEchoMode( QLineEdit::Normal );
	genp->setFrame( TRUE );*/


	QPushButton* tmpQPushButton;
	tmpQPushButton = new QPushButton( &dlg, "OkBtn" );
	tmpQPushButton->setGeometry( 90, 100, 70, 30 );
	tmpQPushButton->setText( "Ok" );
	tmpQPushButton->setAutoRepeat( FALSE );
	tmpQPushButton->setAutoResize( FALSE );
        connect(tmpQPushButton,SIGNAL(clicked()),&dlg,SLOT(accept()));

	tmpQPushButton = new QPushButton( &dlg, "CancelBtn" );
	tmpQPushButton->setGeometry( 180, 100, 70, 30 );
	tmpQPushButton->setText( "Cancel" );
	tmpQPushButton->setAutoRepeat( FALSE );
	tmpQPushButton->setAutoResize( FALSE );
        connect(tmpQPushButton,SIGNAL(clicked()),&dlg,SLOT(reject()));
        dlg.resize(400,140);

   if (dlg.exec())
   {
    compiler_path.sprintf("%s",compp->text().ascii());
//    gen_path.sprintf("%s",genp->text());
    file_path.sprintf("%s",files->text().ascii());
    };
 }

/* --------------------------------------- */





void Editor::log_unit()
{
 QString txt;
 QDialog dlg(this,"unit",TRUE);
 int cx,cy,i;
 char uname[255];


 QLineEdit* files;
	files = new QLineEdit( &dlg, "f_path" );
	files->setGeometry( 130, 20, 250, 30 );
	files->setText("" );
	files->setMaxLength( 32767 );
	files->setEchoMode( QLineEdit::Normal );
	files->setFrame( TRUE );

	QLabel* tmpQLabel;
	tmpQLabel = new QLabel( &dlg, "Label_1" );
	tmpQLabel->setGeometry( 10, 20, 100, 30 );
	tmpQLabel->setText( "Unit name:" );
	tmpQLabel->setAlignment( 289 );
	tmpQLabel->setMargin( -1 );

	QPushButton* tmpQPushButton;
	tmpQPushButton = new QPushButton( &dlg, "OkBtn" );
	tmpQPushButton->setGeometry( 40, 170, 70, 30 );
	tmpQPushButton->setText( "Ok" );
	tmpQPushButton->setAutoRepeat( FALSE );
	tmpQPushButton->setAutoResize( FALSE );
        connect(tmpQPushButton,SIGNAL(clicked()),&dlg,SLOT(accept()));

	tmpQPushButton = new QPushButton( &dlg, "CancelBtn" );
	tmpQPushButton->setGeometry( 130, 170, 100, 30 );
	tmpQPushButton->setText( "Cancel" );
	tmpQPushButton->setAutoRepeat( FALSE );
	tmpQPushButton->setAutoResize( FALSE );
        connect(tmpQPushButton,SIGNAL(clicked()),&dlg,SLOT(reject()));

	tmpQLabel = new QLabel( &dlg, "Label_1" );
	tmpQLabel->setGeometry( 10, 50, 100, 60 );
	tmpQLabel->setText( "Unit type:" );


  QListBox lst(&dlg,"type");
  for(i=0;i<TYPENUM;i++) lst.insertItem(UnitTypes[i]); 
  lst.setGeometry(130,60,180,80);
  lst.setCurrentItem(0);

 if (dlg.exec())
 {
  strcpy(uname,files->text());
  e->getCursorPosition(&cx,&cy);
  
  txt.sprintf("UNIT %s : %s( <params> );\nBEGIN\n\nEND %s;",uname,
              lst.text(lst.currentItem()).ascii(), uname);
  e->insertAt(txt,cx,cy);
 };
}


void Editor::log_prog()
{
 QString txt;
 QDialog dlg(this,"unit",TRUE);
 int cx,cy;
 char uname[255];


 QLineEdit* files;
	files = new QLineEdit( &dlg, "f_path" );
	files->setGeometry( 130, 20, 250, 30 );
	files->setText("" );
	files->setMaxLength( 32767 );
	files->setEchoMode( QLineEdit::Normal );
	files->setFrame( TRUE );

	QLabel* tmpQLabel;
	tmpQLabel = new QLabel( &dlg, "Label_1" );
	tmpQLabel->setGeometry( 10, 20, 100, 30 );
	tmpQLabel->setText( "Program name:" );

	QPushButton* tmpQPushButton;
	tmpQPushButton = new QPushButton( &dlg, "OkBtn" );
	tmpQPushButton->setGeometry( 40, 70, 70, 30 );
	tmpQPushButton->setText( "Ok" );
	tmpQPushButton->setAutoRepeat( FALSE );
	tmpQPushButton->setAutoResize( FALSE );
        connect(tmpQPushButton,SIGNAL(clicked()),&dlg,SLOT(accept()));

	tmpQPushButton = new QPushButton( &dlg, "CancelBtn" );
	tmpQPushButton->setGeometry( 130, 70, 100, 30 );
	tmpQPushButton->setText( "Cancel" );
	tmpQPushButton->setAutoRepeat( FALSE );
	tmpQPushButton->setAutoResize( FALSE );
        connect(tmpQPushButton,SIGNAL(clicked()),&dlg,SLOT(reject()));


 if (dlg.exec())
 {
  strcpy(uname,files->text());
  e->getCursorPosition(&cx,&cy);
  
  txt.sprintf("PROGRAM %s\n\nBEGIN\n\nEND ",uname);
  e->insertAt(txt,cx,cy);
 };

}


void Editor::findText()
{
 QDialog dlg(this,"",TRUE);
 QString *txt;
 int res,line,pom;


	QLineEdit* tmpQLineEdit;
	tmpQLineEdit = new QLineEdit( &dlg, "LineEdit_1" );
	tmpQLineEdit->setGeometry( 60, 10, 180, 30 );
	tmpQLineEdit->setText( "" );

	QLabel* tmpQLabel;
	tmpQLabel = new QLabel( &dlg, "Label_1" );
	tmpQLabel->setGeometry( 10, 10, 50, 30 );
	{
		QFont font( "helvetica", 12, 75, 0 );
		font.setStyleHint( (QFont::StyleHint)0 );
		//font.setCharSet( (QFont::CharSet)0 );
		tmpQLabel->setFont( font );
	}
	tmpQLabel->setText( "Text:" );

	QCheckBox* tmpQRadioButton;
	tmpQRadioButton = new QCheckBox( &dlg, "RadioButton_1" );
	tmpQRadioButton->setGeometry( 70, 50, 150, 30 );
	tmpQRadioButton->setText( "Case sensitive" );
	tmpQRadioButton->setAutoRepeat( FALSE );
	tmpQRadioButton->setAutoResize( FALSE );

	QPushButton *okbtn, *cbtn;
	okbtn = new QPushButton( &dlg, "PushButton_1" );
	okbtn->setGeometry( 260, 10, 100, 30 );
	okbtn->setText( "Find" );
        okbtn->setDefault(TRUE);
        connect(okbtn,SIGNAL(clicked()),&dlg,SLOT(accept()));

	cbtn = new QPushButton( &dlg, "PushButton_2" );
	cbtn->setGeometry( 260, 50, 100, 30 );
	cbtn->setText( "Close" );
        connect(cbtn,SIGNAL(clicked()),&dlg,SLOT(reject()));
	dlg.resize( 380, 90 );

    if (dlg.exec())
    {
     e->getCursorPosition(&pom,&res);
     sensitive=tmpQRadioButton->isChecked();
     for(line=pom+1;line<e->numLines();line++)
     {
      txt = new QString(e->textLine(line)); 
      if (tmpQRadioButton->isChecked())
       res=txt->find(tmpQLineEdit->text(),0,TRUE);
      else
       res=txt->find(tmpQLineEdit->text(),0,FALSE);
      delete txt;
      if (res>=0) { e->setCursorPosition(line,1);strcpy(find_text,tmpQLineEdit->text());
                    break;}
     } //for
     }  

}

void Editor::find_next()
{
 int pom,res,line;
 QString *txt;

    e->getCursorPosition(&pom,&res);
    for(line=pom+1;line<e->numLines();line++)
     {
      txt = new QString(e->textLine(line)); 
      if (sensitive)
       res=txt->find(find_text,0,TRUE);
      else
       res=txt->find(find_text,0,FALSE);
      delete txt; 
      if (res>=0) { e->setCursorPosition(line,1);
                    break;}
     } //for
 
}

int main( int argc, char **argv )
{
    QApplication a( argc, argv );
    //a.setStyle(WindowsStyle);
    e = new Editor(argv[1]);
    e->resize( 600, 400 );
    e->show();
    return a.exec();
}

