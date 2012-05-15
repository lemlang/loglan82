
#ifndef QWERTY_H
#define QWERTY_H

#include <qwidget.h>
#include <qmenubar.h>
#include <qmlined.h>
#include <qlist.h>
#include <qstrlist.h>
#include <qframe.h>
#include <qmlined.h>
#include <qlined.h>
#include <qpushbt.h>
#include <qcombo.h>
#include <qlabel.h>


#define COMP_MODE	1
#define GEN_MODE	2
#define ALL_MODE	3



typedef struct 
{
 char name[255],filename[255];
} FuncEntry;

typedef struct 
{
 QList<FuncEntry> dictionary;
 QStrList names;
 char name[255];
} CategoryEntry;




class My_Edit:public QMultiLineEdit
{
Q_OBJECT
public:
 My_Edit(QWidget *parent=0,const char *name=0);
 virtual void keyPressEvent(QKeyEvent *ev);
signals:
 void cursorMove(); 
};

class Editor : public QWidget
{
    Q_OBJECT
public:
    QString compiler_path;
    QString gen_path;
    QString file_path;

    Editor(char *hdir=0, QWidget *parent=0, const char *name=0);
   ~Editor();
   
    void compile(int mode);

public slots:
    void load();
    void load( const char *fileName );
    void save();
    void save( const char *fileName );
    void save_as();
    void create();
    void props();
    void print();
    void cmp();
    void gen();
    void comp_all();
    
    void log_unit();
    void log_prog();
    
    void findText();
    void find_next();
//    void gotoline();
    void updateline();

protected:
    void resizeEvent( QResizeEvent * );
    virtual void closeEvent ( QCloseEvent * e );

private:
    QMenuBar 	   *m;
    My_Edit *e;
    QMultiLineEdit *msg;
    QLabel *position;
    QString fname;
    char find_text[256];
    bool sensitive;
    char HomeDir[255]; 
    
};


#endif // QWERTY_H
