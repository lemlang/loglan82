

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
#include <qfile.h>
#include <qtstream.h>
#include <qstring.h>
#include <qfiledlg.h>
#include <qfontmet.h>
#include <qpainter.h>
#include <qscrbar.h>


#define TAG_TEXT	0
#define TAG_OTHER	1

#define TAG_LINK	2 
#define TAG_ANCHOR	3 
#define TAG_BOLD_ON	4 
#define TAG_BOLD_OFF	5 
#define TAG_ITALIC_ON	6 
#define TAG_ITALIC_OFF  7 
#define TAG_BREAK	8 
#define TAG_LINE	9 
#define TAG_LINK_END	10 
#define TAG_LIST_ON	11
#define TAG_LIST_OFF	12
#define TAG_LIST_ITEM	13

#define PIX_HEIGHT	2000

char names[14][40] = {"text","other","link","anchor","bold on",
                       "bold off","italic on","italic off","break",
                       "line","end link","list on","list off","list item"};



class Tag
{
public:
  Tag() {tag_type=0; strcpy(tag_label,""); strcpy(tag_link,""); 
         strcpy(tag_text,"");};
  int tag_type;
  char tag_label[255],tag_link[255],tag_text[255]; 
  int x,y,w,h;
};

class HTMLAnalyzer
{
public:
  QList<Tag> tags;
  bool verbatim;

 HTMLAnalyzer();


 bool LoadFile(char*);
 void AnalyzeTag(QString*);
 void PackLinks();
 void DumpList();
 Tag *CheckTag(int,int);
 Tag *FindAnchor(char*);

 
};

HTMLAnalyzer::HTMLAnalyzer()
{
 tags.clear();
 verbatim=FALSE;
}


bool HTMLAnalyzer::LoadFile(char *fname)
{

 QFile f(fname);
 QString poms,poms1;
 int i;
 Tag *pomt;
 bool not_ended;
 
 tags.clear();
 if (!f.open(IO_ReadOnly)) return(FALSE);
 
 QTextStream fs(&f);
 
 while (!fs.eof())
 {
  poms = fs.readLine();
  while (poms.length()>0)
  {
     i = poms.find('<');
      if (i!=-1)
        {
          if (i>1)
          {
           poms1 = poms.left(i);
           pomt = new Tag;
           pomt->tag_type = TAG_TEXT;
           if (!verbatim) poms1=poms1.simplifyWhiteSpace();
           sprintf(pomt->tag_text," %s",poms1.data());
           tags.append(pomt);
          }
           poms = poms.right(poms.length()-i);
           
           i=poms.find('>');
           if (i!=-1)
            {
               poms1 = poms.mid(1,i-1);
               AnalyzeTag(&poms1);   
               poms=poms.right(poms.length()-i-1);  
              }
              else { not_ended=TRUE;break;}
         }
          else
           {
             pomt = new Tag;
             pomt->tag_type = TAG_TEXT;
             if (!verbatim) poms=poms.simplifyWhiteSpace();
             sprintf(pomt->tag_text," %s",poms.data());
             tags.append(pomt);
             break;
           }
  }// while length>0
  if (verbatim) {pomt = new Tag; pomt->tag_type=TAG_BREAK; tags.append(pomt);}
 } //eof 

 f.close();

 return(TRUE);
}

void HTMLAnalyzer::AnalyzeTag(QString *t)
{
 Tag *pom;
 int i;
 QString poms,poms1;

 *t = t->simplifyWhiteSpace();
 pom = new Tag;

 if ( (t->data()[0]!='A') && (t->data()[0]!='a') )
 {
 *t = t->upper();
 if (strcmp(t->data(),"B")==0 ) {pom->tag_type = TAG_BOLD_ON;}
 else
 if (strcmp(t->data(),"/B")==0 ) {pom->tag_type = TAG_BOLD_OFF;}
 else
 if (strcmp(t->data(),"I")==0 ) {pom->tag_type = TAG_ITALIC_ON;}
 else
 if (strcmp(t->data(),"/I")==0 ) {pom->tag_type = TAG_ITALIC_OFF;}
 else
 if (strcmp(t->data(),"BR")==0 ) {pom->tag_type = TAG_BREAK;}
 else
 if (strcmp(t->data(),"HR")==0 ) {pom->tag_type = TAG_LINE;}
 else
 if (strcmp(t->data(),"/A")==0 ) {pom->tag_type = TAG_LINK_END;}
 else
 if (strcmp(t->data(),"UL")==0 ) {pom->tag_type = TAG_LIST_ON;}
 else
 if (strcmp(t->data(),"/UL")==0 ) {pom->tag_type = TAG_LIST_OFF;}
 else
 if (strcmp(t->data(),"LI")==0 ) {pom->tag_type = TAG_LIST_ITEM;}
 else
 if (strcmp(t->data(),"PRE")==0 ) {verbatim=TRUE;}
 else
 if (strcmp(t->data(),"/PRE")==0 ) {verbatim=FALSE;}
 }
  
 else // 'a' or 'A'

 {
  // links
   i = t->find('=');
     if (i!=-1)
     {
       poms=t->mid(2,i-2);
       poms=poms.simplifyWhiteSpace();
       poms=poms.upper();
       poms1=t->right(t->length()-i-1);
       poms1=poms1.simplifyWhiteSpace();
       
       if  (strcmp(poms.data(),"HREF")==0) 
         {
           pom->tag_type = TAG_LINK;
           strcpy(pom->tag_link,poms1.data());
          }
       else
       if  (strcmp(poms.data(),"NAME")==0)
        {
          pom->tag_type = TAG_ANCHOR;
          if (poms1.data()[0]=='"') poms1=poms1.right(poms1.length()-1);
          if (poms1.data()[poms1.length()-1]=='"') poms1=poms1.left(poms1.length()-1);
          strcpy(pom->tag_label,poms1.data()); 
         }    
      }
  }
 tags.append(pom);
}

void HTMLAnalyzer::DumpList()
{
 Tag *pom;
 pom=tags.first();
 while (pom!=NULL)
 {
  fprintf(stderr,"%s:%s,%s,%s\n",names[pom->tag_type], pom->tag_text,
    pom->tag_link,pom->tag_label);
  pom=tags.next();
  }
 
}

void HTMLAnalyzer::PackLinks()
{
 Tag *pom,*pom1;
 char s[255];

 pom = tags.first();
 while (pom!=NULL)
 {
    if ( (pom->tag_type==TAG_LINK ) || (pom->tag_type==TAG_ANCHOR) )
    {
     pom1=tags.next();
     strcpy(s,"");
     while ( (pom1!=NULL) && (pom1->tag_type!=TAG_LINK_END) )
     {
      if (pom1->tag_type==TAG_TEXT) strcat(s,pom1->tag_text);
      tags.remove(pom1);
      pom1=tags.current();
      }
     strcpy(pom->tag_text,s); 
     tags.remove(pom1);
     pom=tags.current();
    }
    else
    pom=tags.next();
  }
}


Tag *HTMLAnalyzer::CheckTag(int x,int y)
{
 Tag *pom;
 pom=tags.first();
 while(pom!=NULL)
 {
  if ( pom->tag_type==TAG_LINK)
     if ( (x>=pom->x) && (x<=pom->x+pom->w) &&
          (y>=pom->y) && (y<=pom->y+pom->h)) return(pom);
  pom=tags.next();
 }
 return(NULL);
}

Tag *HTMLAnalyzer::FindAnchor(char *name)
{
 Tag *pom;
 pom=tags.first();
 while(pom!=NULL)
 {
  if ( (pom->tag_type==TAG_ANCHOR) && (strcmp(pom->tag_label,name)==0) )
   return(pom);
  pom=tags.next();
  }
 return(pom);
}
//******************************



class QHTML: public QFrame
{
 Q_OBJECT
public:
  QScrollBar *vscroll;
  QMenuBar *bar;  
  HTMLAnalyzer *analyzer;
  QPixmap *map;
  int cx,cy,oy,lstep,pstep;
  bool Bold,Italic;
  QFont *normal,*bold,*italic,*bold_italic,*actual_font;
  char homedir[255];

  QHTML(char*);
  void DrawList();
  virtual void paintEvent(QPaintEvent *ev);
  virtual void resizeEvent(QResizeEvent *ev);
  virtual void mousePressEvent(QMouseEvent *ev);
public slots:
 void load();
 void back();
 void vscrolled(int);
 void contents();
 void user_guide();
 void lang_guide();

};


QApplication *app;

QHTML::QHTML(char *d)
{

  QPopupMenu *pp;
  char s[255];

  QFont f("Helvetica",12,QFont::Bold);
  normal = new QFont("Helvetica",12,QFont::Normal);
  bold = new QFont("Helvetica",12,QFont::Bold);
  italic = new QFont("Helvetica",12,QFont::Normal,TRUE);
  bold_italic = new QFont("Helvetica",12,QFont::Bold,TRUE);
  strcpy(homedir,d);

  actual_font = normal;
  bar = new QMenuBar(this);
  pp = new QPopupMenu;
  pp->insertItem("Index",this,SLOT(contents()));
  pp->insertItem("User Guide",this,SLOT(user_guide()));
  pp->insertItem("Language reference",this,SLOT(lang_guide()));
  pp->setFont(f);
  //pp->setStyle(WindowsStyle);
  bar->insertItem("File",this,SLOT(load()));
  bar->insertItem("Contents",pp);
  bar->insertItem("Quit",app,SLOT(quit()));
  bar->setFont(f);
  setCaption("LOGLAN Help");
  setBackgroundColor(gray);
  analyzer = new HTMLAnalyzer;
  map = new QPixmap(500,PIX_HEIGHT);
  map->fill(backgroundColor());
  resize(500,height());
  setFixedSize(width(),height());
  oy=0;lstep=10;pstep=height()-bar->height();
  vscroll = new QScrollBar(0,PIX_HEIGHT,lstep,pstep,0,QScrollBar::Vertical,
            this);
  vscroll->setTracking(TRUE);  
  vscroll->setGeometry(width()-16,bar->height(),16,height()-bar->height());
  connect(vscroll,SIGNAL(valueChanged(int)),this,SLOT(vscrolled(int)));  
  sprintf(s,"%s/index.html",homedir);
  analyzer->LoadFile(s);
  analyzer->PackLinks();
  DrawList();

}


void QHTML::vscrolled(int v)
{
 oy=v;
 repaint();
}

void QHTML::load()
{
 QString s(QFileDialog::getOpenFileName(homedir,"*",this));
 if ( !s.isNull())
 {
  vscroll->setValue(0); 
  analyzer->LoadFile((char*)s.ascii());
  analyzer->PackLinks();
  DrawList();
 }
}


void QHTML::contents()
{
 char ss[255];
 sprintf(ss,"%s/index.html",homedir);
  analyzer->LoadFile(ss);
  analyzer->PackLinks();
  DrawList(); 
}


void QHTML::user_guide()
{
 char ss[255];
 sprintf(ss,"%s/userg.html",homedir);
  analyzer->LoadFile(ss);
  analyzer->PackLinks();
  DrawList(); 
}

void QHTML::lang_guide()
{
 char ss[255];
 sprintf(ss,"%s/langg.html",homedir);
  analyzer->LoadFile(ss);
  analyzer->PackLinks();
  DrawList(); 
}

void QHTML::paintEvent(QPaintEvent *ev)
{
 if (map!=NULL) bitBlt(this,0,bar->height(),map,0,oy,width()-16,height()-16);
}

void QHTML::resizeEvent(QResizeEvent *ev)
{
 DrawList();
}

void QHTML::mousePressEvent(QMouseEvent *ev)
{
 Tag *pom,*pom1;
 QString poms;
 char ss[255];

 pom=analyzer->CheckTag(ev->x(),ev->y()+oy);
 if (pom!=NULL)
 {
  poms.sprintf(pom->tag_link);
  if (poms.data()[0]=='"') poms=poms.right(poms.length()-1);
  if (poms.data()[poms.length()-1]=='"') poms=poms.left(poms.length()-1);
  if (poms.data()[0]=='#') 
   {
     poms=poms.right(poms.length()-1);
     pom1=analyzer->FindAnchor((char*)poms.ascii());
     if (pom1!=NULL)
     {
       vscroll->setValue(pom1->y);
      }
    }
  else
  {
   sprintf(ss,"%s/%s",homedir,poms.data());
   analyzer->LoadFile(ss);
   analyzer->PackLinks();
   DrawList();
  }
  }
}

void QHTML::back()
{
}

void QHTML::DrawList()
{
 Tag *pom;
 QPainter p;
 
if (!analyzer->tags.isEmpty())
{
 cx=5;cy=15;
 map->fill(backgroundColor());
 p.begin(map);
 
 pom=analyzer->tags.first();
 while (pom!=NULL)
 {
  switch(pom->tag_type)
  {
   case TAG_TEXT:p.setFont(*actual_font);
                 if (cx+p.fontMetrics().width(pom->tag_text)>width()-16)
                 {cx=5;cy=cy+p.fontMetrics().height();}  
                 p.drawText(cx,cy,pom->tag_text);
                 cx=cx+p.fontMetrics().width(pom->tag_text);
                 break; 
   case TAG_BREAK: p.setFont(*actual_font);
                   cy=cy+p.fontMetrics().height();cx=5;
                   break;
   case TAG_ITALIC_ON:
                  if (actual_font==bold) actual_font=bold_italic;
                  else actual_font=italic;
                  break;
   case TAG_ITALIC_OFF:
                  if (actual_font==bold_italic) actual_font=bold;
                  else actual_font=normal;
                  break;               
   case TAG_BOLD_ON:
                  if (actual_font==italic) actual_font=bold_italic;
                  else actual_font=bold;
                  break;
   case TAG_BOLD_OFF:
                  if (actual_font==bold_italic) actual_font=italic;
                  else actual_font=normal;
                  break;                 
   case TAG_LINK:p.setFont(*actual_font);
                 if (cx+p.fontMetrics().width(pom->tag_text)>width()-16)
                 {cx=5;cy=cy+p.fontMetrics().height();}  
                 p.setPen(QColor(255,0,0));
                 p.drawText(cx,cy,pom->tag_text);
                 pom->x = cx;pom->y=cy+p.fontMetrics().height();
                 pom->w = p.fontMetrics().width(pom->tag_text);
                 pom->h = p.fontMetrics().height();
                 p.setPen(QColor(0,0,0));
                 cx=cx+p.fontMetrics().width(pom->tag_text);
                 break; 
  case TAG_ANCHOR: p.setFont(*actual_font);
                 if (cx+p.fontMetrics().width(pom->tag_text)>width()-16)
                 {cx=5;cy=cy+p.fontMetrics().height();}  
                 //p.setPen(QColor(0,255,0));
                 p.drawText(cx,cy,pom->tag_text);
                 pom->x = cx;pom->y=cy-p.fontMetrics().height();
                 pom->w = p.fontMetrics().width(pom->tag_text);
                 pom->h = p.fontMetrics().height();
                 p.setPen(QColor(0,0,0));
                 cx=cx+p.fontMetrics().width(pom->tag_text);
                 break; 
  case TAG_LIST_OFF:
  case TAG_LIST_ON:p.setFont(*actual_font);
                   cx=5;cy=cy+p.fontMetrics().height();
                   break;
  case TAG_LIST_ITEM:p.setFont(*actual_font);
                     p.setBrush(QBrush(QColor(0,0,255)));
                     cx=5;cy=cy+p.fontMetrics().height();
                     p.drawPie(cx,cy-5,5,5,0,5760); 
                     cx=cx+15;
                   break;
  case TAG_LINE:p.setFont(*actual_font);
                cx=5;cy=cy+p.fontMetrics().height();
                p.drawLine(cx,cy-(int)(p.fontMetrics().height()/2),
                           width()-16-5,cy-(int)(p.fontMetrics().height()/2)); 
                cx=5;cy=cy+p.fontMetrics().height();
                break;


  }//switch
  pom = analyzer->tags.next();
  }
 p.end();
 repaint();
}
}



#include "help.moc"

int main( int argc, char **argv )
{
 QString ps;

    app = new QApplication(argc,argv);
    if (argc==2) ps.sprintf(argv[1]);
     else ps.sprintf(".");
    QHTML cfg((char*)ps.ascii());
    //app->setStyle(WindowsStyle);
    app->setMainWidget(&cfg);
    cfg.show();
    return app->exec();
}
