//
// Qt Example Application: drawdemo
//
// Demonstrates the painter and the printer.
//

//#include <qwindow.h>
#include <qpainter.h>
#include <qprinter.h>
#include <qpushbt.h>
#include <qradiobt.h>
#include <qbttngrp.h>
#include <qapp.h>
#include <math.h>
#include <qpixmap.h>
#include <qscrbar.h>
#include <qcolor.h>
#include <stdio.h>
#include <qfont.h>
#include <qfontmet.h>
#include <qlist.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include "../head/genint1.h"
#include "../head/comm.h"
#include "socu.h"
#include <unistd.h>
#include <qsocknot.h>
#include <qobject.h>
#include <errno.h>
#include <qqueue.h>
#include <qmsgbox.h>
#include <qmenubar.h>
#include <qpopmenu.h>
#include <qkeycode.h>

#define MAXWIDTH	640
#define MAXHEIGHT	480
#define TEXT_LINES	50
#define TEXT_COLS	100
#define REQUEST_NAME	"gr.req"
#define PERM		0666

class VGRMap {
public:
    QPixmap *map;
    int number;
    VGRMap ( int n,QPixmap *m ) {
        number=n;
        map = m;
    };
};

class VGR : public QFrame {
    Q_OBJECT
public:
    VGR ( char* );
    ~VGR();
    int resp_sock;
    int fcol,bcol,gfcol,gbcol;
    int curx,cury;
    int tx,ty;
    QQueue<int> CharBuffer;
    bool string_wait, char_wait, line_wait, mouse_wait, inkey_wait, was_line;
    QColor *lcolors[256];
    QFont *prv_font, *italic_font, *bold_font, *normal_font;
    void MakeColors();
    void SetForeground ( int );
    void SetBackground ( int );
    void ClearAll();
    void ClearArea ( int,int,int,int );
    void Line ( int,int,int,int );
    void Ellipse ( int x,int y,int a, int b,int alfa, int beta, int fill );
    void Rect ( int x1, int y1, int x2, int y2,int col, int fill );
    void Point ( int x, int y );
    void TextXY ( int x, int y, char *s );
    void CharXY ( int x, int y, char a );
    void IntXY ( int x, int y, int val );
    void WriteText ( char *s );
    void WriteChar ( char a );
    void WriteLine();
    void PutChar ( char a ); /* Write Char w/o changing position */
    void DelChar();
    void Outstring ( int x, int y, char *s, int b, int f );
    void writeintxy ( int x, int y, int val,int c );
    void CurPos();
    int Getmap ( int w, int h );
    void Putmap ( int map );
    void MagicGraph ( G_MESSAGE* );
    bool GetInput ( int );
public slots:
    void vscrolled ( int );
    void hscrolled ( int );
    void CloseMe();
    void RequestMessage();
protected:
    virtual void resizeEvent ( QResizeEvent * );
    virtual void closeEvent ( QCloseEvent * );
    virtual void keyPressEvent ( QKeyEvent * );
    virtual void mousePressEvent ( QMouseEvent * );
    virtual void paintEvent ( QPaintEvent * );
    virtual void focusInEvent ( QFocusEvent * );
private:
    QPixmap *canvas;
    QScrollBar *hscroll,*vscroll;
    QPushButton *close_btn;
    int ox,oy,lstep,pstep,gx,gy;
    QList<VGRMap> maps;
    int firstfreemap;
    bool Closed, MustRepaint,GraphRead;
    QSocketNotifier *request;
    char internal_buffer[256];
    int strcnt;
};

void VGR::MakeColors() {
    lcolors[0] = new QColor ( 0, 0, 0 ); /* black  */
    lcolors[1] = new QColor ( 0, 0, 139 );  /* blue dark */
    lcolors[2] = new QColor ( 0, 100, 0 );   /* green dark  */
    lcolors[3] = new QColor ( 0, 197, 205 );   /* turquoise dark   */
    lcolors[4] = new QColor ( 205,0 , 0 ); /* red dark */
    lcolors[5] = new QColor ( 238, 130,238 ); /* violet */
    lcolors[6] = new QColor ( 139,35,35 ); /* brown   */
    lcolors[7] = new QColor ( 190,190,190 );     /* grey light */
    lcolors[8] = new QColor ( 97, 97, 97 );     /* grey dark */
    lcolors[9] = new QColor ( 0, 0, 255 );     /* blue */
    lcolors[10] = new QColor ( 0, 255, 0 );     /* green */
    lcolors[11] = new QColor ( 0, 229,238 );     /* turquoise */
    lcolors[12] = new QColor ( 255, 0, 0 );     /* red light */
    lcolors[13] = new QColor ( 255, 110,180 );     /* rose */
    lcolors[14] = new QColor ( 255,255, 0 );     /* yellow */
    lcolors[15] = new QColor ( 255, 255, 255 );     /* white */
}
//
// Construct the DrawView with buttons.
//
VGR::VGR ( char *sockname ) :QFrame() {
    QPixmap mp;
    struct sockaddr_un svr;
    int len,i;
    normal_font = new QFont ( "lucidatypewriter",10,QFont::Normal );
    normal_font->setFixedPitch ( TRUE );
    bold_font = new QFont ( "lucidatypewriter",10,QFont::Bold );
    bold_font->setFixedPitch ( TRUE );
    italic_font = new QFont ( "lucidatypewriter",10,QFont::Normal,TRUE );
    italic_font->setFixedPitch ( TRUE );
    prv_font = normal_font;
    MakeColors();
    setCaption ( "graphic resource" );
    setBackgroundColor ( white );
    canvas = new QPixmap ( 640,480 );
    canvas->fill ( backgroundColor() );
    ox=0;
    oy=0;
    curx=0;
    cury=0;
    tx=0;
    ty=0;
    gx=0;
    gy=0;
    maps.setAutoDelete ( TRUE );
    firstfreemap=1;
    Closed = FALSE;
    GraphRead=FALSE;
    lstep=10;
    pstep=250;
    hscroll = new QScrollBar ( 0,MAXWIDTH,lstep,pstep,0,QScrollBar::Horizontal,this );
    vscroll = new QScrollBar ( 0,MAXHEIGHT,lstep,pstep,0,QScrollBar::Vertical,this );
    hscroll->setTracking ( TRUE );
    vscroll->setTracking ( TRUE );
    resize ( 640,300 );
    hscroll->setGeometry ( 0,height()-16,width()-16,16 );
    vscroll->setGeometry ( width()-16,0,16,height()-16 );
    connect ( hscroll,SIGNAL ( valueChanged ( int ) ),this,SLOT ( hscrolled ( int ) ) );
    connect ( vscroll,SIGNAL ( valueChanged ( int ) ),this,SLOT ( vscrolled ( int ) ) );
    close_btn = new QPushButton ( this,"close" );
    if ( mp.load ( "pics/close.bmp" ) )
        close_btn->setPixmap ( mp );
    else
        close_btn->setText ( "C" );
    close_btn->setGeometry ( width()-16,height()-16,16,16 );
    close_btn->setEnabled ( FALSE );
    connect ( close_btn,SIGNAL ( clicked() ),this,SLOT ( CloseMe() ) );
    gfcol=fcol = 0;
    gbcol=bcol = 15;
    resp_sock = socket ( AF_UNIX,SOCK_STREAM,0 );
    bzero ( &svr,sizeof ( svr ) );
    svr.sun_family = AF_UNIX;
    strcpy ( svr.sun_path,sockname );
    len = strlen ( svr.sun_path ) +sizeof ( svr.sun_family );
    i = ::connect ( resp_sock, ( struct sockaddr* ) &svr,len );
    if ( i==0 )  fcntl ( resp_sock,F_SETFL,O_NONBLOCK|fcntl ( resp_sock,F_GETFL,0 ) );
    request = new QSocketNotifier ( resp_sock,QSocketNotifier::Read );
    connect ( request,SIGNAL ( activated ( int ) ),this,SLOT ( RequestMessage() ) );

    inkey_wait = FALSE;
    string_wait = FALSE;
    char_wait = FALSE;
    line_wait = FALSE;
    mouse_wait = FALSE;
    was_line=FALSE;
    CharBuffer.clear();
    CharBuffer.setAutoDelete ( TRUE );

    setFocusPolicy ( QWidget::StrongFocus );
    hscroll->setRange ( 0,MAXWIDTH-width() +20 );
    vscroll->setRange ( 0,MAXHEIGHT-height() +20 );
    hscroll->setSteps ( ( int ) ( hscroll->width() /hscroll->maxValue() ), ( int ) ( ( hscroll->width() /hscroll->maxValue() ) *4 ) );
    vscroll->setSteps ( ( int ) ( vscroll->height() /vscroll->maxValue() ), ( int ) ( ( vscroll->height() /vscroll->maxValue() ) *4 ) );

    setMaximumSize ( MAXWIDTH+16,MAXHEIGHT+16 );
    setUpdatesEnabled ( FALSE );
};

VGR::~VGR() {
    if ( request!=NULL )
        delete request;
}

void VGR::paintEvent ( QPaintEvent *p ) {
    {
        bitBlt ( this,0,0,canvas,ox,oy,width()-16,height()-16 );
    }
    MustRepaint=FALSE;
}

void VGR::focusInEvent ( QFocusEvent * ) {
    setUpdatesEnabled ( TRUE );
    repaint();
    setUpdatesEnabled ( FALSE );
}

void VGR::resizeEvent ( QResizeEvent *ev ) {
    hscroll->setGeometry ( 0,height()-16,width()-16,16 );
    vscroll->setGeometry ( width()-16,0,16,height()-16 );
    close_btn->setGeometry ( width()-16,height()-16,16,16 );
    hscroll->setRange ( 0,MAXWIDTH-width() +20 );
    vscroll->setRange ( 0,MAXHEIGHT-height() +20 );
    pstep=height()-32;
    hscroll->setSteps ( lstep,pstep );
    vscroll->setSteps ( lstep,pstep );
}

void VGR::closeEvent ( QCloseEvent *ev ) {
    if ( Closed ) QFrame::closeEvent ( ev );
}

void VGR::mousePressEvent ( QMouseEvent *ev ) {
    G_MESSAGE msg;

    if ( mouse_wait ) {
        msg.msg_type = MSG_GRAPH;
        msg.param.pword[0] = GRAPH_MGETPRESS_RESPONSE;
        msg.param.pword[2] = ev->pos().x()-ox;
        msg.param.pword[3] = ev->pos().y()-oy;
        switch ( ev->button() )  {
        case LeftButton:
            msg.param.pword[7] = 1;
            break;
        case RightButton:
            msg.param.pword[7] = 3;
            break;
        case MidButton:
            msg.param.pword[7] = 2;
            break;
        default:
            msg.param.pword[7] = 0;
            break;
        }
        write ( resp_sock,&msg,sizeof ( G_MESSAGE ) );
        mouse_wait = FALSE;
    }
    QFrame::mousePressEvent ( ev );
}

bool VGR::GetInput ( int t ) {
    G_MESSAGE msg;

    if ( !CharBuffer.isEmpty() ) {
        switch ( t ) {
        case 2: {
            msg.msg_type = MSG_GRAPH;
            msg.param.pword[0] = GRAPH_INKEY_RESPONSE;
            msg.param.pword[3] = * ( CharBuffer.dequeue() );
            write ( resp_sock,&msg,sizeof ( G_MESSAGE ) );
            inkey_wait = FALSE;
            return TRUE;
        };
        break;
        case 0: {
            if ( GraphRead ) {
                msg.msg_type = MSG_GRAPH;
                msg.param.pword[0] = GRAPH_MAGIC_RESPONSE;
                msg.param.pword[1]=0;
                msg.param.pchar = ( char ) ( * ( CharBuffer.dequeue() ) );
                WriteChar ( msg.param.pchar );
                GraphRead=FALSE;
            } else  {
                msg.msg_type = MSG_GRAPH;
                msg.param.pword[0] = GRAPH_READCHAR_RESPONSE;
                msg.param.pchar = ( char ) ( * ( CharBuffer.dequeue() ) );
            }

            write ( resp_sock,&msg,sizeof ( G_MESSAGE ) );
            char_wait = FALSE;
            return TRUE;
        };
        break;
        case 1: {
            while ( ( ( !CharBuffer.isEmpty() ) && ( ! ( * ( CharBuffer.dequeue() ) =13 ) ) ) );
            if ( !CharBuffer.isEmpty() )   {
                msg.msg_type = MSG_GRAPH;
                msg.param.pword[0] = GRAPH_READLN_RESPONSE;
                write ( resp_sock,&msg,sizeof ( G_MESSAGE ) );
                line_wait = FALSE;
                return TRUE;
            }

        };
        break;// line_wait
        } //swictch
    } // buffer empty

    if ( ( CharBuffer.isEmpty() ) && ( t==2 ) ) {
        msg.msg_type = MSG_GRAPH;
        msg.param.pword[0] = GRAPH_INKEY_RESPONSE;
        msg.param.pword[3] = 0;
        write ( resp_sock,&msg,sizeof ( G_MESSAGE ) );
        inkey_wait = FALSE;
        return TRUE;
    }
    return FALSE;
}

void VGR::keyPressEvent ( QKeyEvent *ev ) {
    G_MESSAGE msg;
    int *pom;

    pom = new int[1];
    *pom=ev->ascii();
    if ( *pom==0 )
        switch ( ev->key() ) {
        case Key_F1:
            *pom=-10;
            break;
        case Key_F2:
            *pom=-11;
            break;
        case Key_F3:
            *pom=-12;
            break;
        case Key_F4:
            *pom=-13;
            break;
        case Key_F5:
            *pom=-14;
            break;
        case Key_F6:
            *pom=-15;
            break;
        case Key_F7:
            *pom=-16;
            break;
        case Key_F8:
            *pom=-17;
            break;
        case Key_F9:
            *pom=-18;
            break;
        case Key_F10:
            *pom=-19;
            break;
        case Key_Insert:
            *pom=-20;
            break;
        case Key_Home:
            *pom=-21;
            break;
        case Key_End:
            *pom=-22;
            break;
        case Key_PageUp:
            *pom=-23;
            break;
        case Key_PageDown:
            *pom=-24;
            break;
        case Key_Left:
            *pom=-25;
            break;
        case Key_Right:
            *pom=-26;
            break;
        case Key_Up:
            *pom=-27;
            break;
        case Key_Down:
            *pom=-28;
            break;
        }// switch

    if ( ( !string_wait ) && ( !inkey_wait ) && ( !char_wait ) && ( !line_wait ) )
        CharBuffer.enqueue ( pom );

    if ( inkey_wait ) {
        msg.msg_type = MSG_GRAPH;
        msg.param.pword[0] = GRAPH_INKEY_RESPONSE;
        msg.param.pword[3] = *pom;
        write ( resp_sock,&msg,sizeof ( G_MESSAGE ) );
        inkey_wait = FALSE;
    };

    if ( char_wait ) {
        if ( GraphRead ) {
            msg.msg_type = MSG_GRAPH;
            msg.param.pword[0] = GRAPH_MAGIC_RESPONSE;
            msg.param.pword[1]=0;
            WriteChar ( ( char ) ev->ascii() );
            GraphRead=FALSE;
        }  else {
            msg.msg_type = MSG_GRAPH;
            msg.param.pword[0] = GRAPH_READCHAR_RESPONSE;
        }
        msg.param.pchar = ( char ) ev->ascii();
        write ( resp_sock,&msg,sizeof ( G_MESSAGE ) );
        char_wait = FALSE;
    }

    if ( ( line_wait ) && ( ev->ascii() ==13 ) ) {
        msg.msg_type = MSG_GRAPH;
        msg.param.pword[0] = GRAPH_READLN_RESPONSE;
        write ( resp_sock,&msg,sizeof ( G_MESSAGE ) );
        line_wait = FALSE;
    }

    if ( string_wait ) {
        if ( ev->ascii() ==13 ) {
            internal_buffer[strcnt]='\0';
            DelChar();
            if ( GraphRead ) {
                GraphRead=FALSE;
                msg.msg_type = MSG_GRAPH;
                msg.param.pword[0] = GRAPH_MAGIC_RESPONSE;
                msg.param.pword[1]=0;
            } else    {
                msg.msg_type = MSG_GRAPH;
                msg.param.pword[0] = GRAPH_READSTR_RESPONSE;
            }
            strcpy ( msg.param.pstr,internal_buffer );
            write ( resp_sock,&msg,sizeof ( G_MESSAGE ) );
            string_wait = FALSE;
            was_line=TRUE;
            CharBuffer.clear();
        }  else if ( ev->ascii() ==8 ) {
            strcnt--;
            DelChar();
            DelChar();
            WriteChar ( '_' );
        } else  {
            internal_buffer[strcnt]= ( char ) ev->ascii();
            strcnt++;
            DelChar();
            WriteChar ( ( char ) ev->ascii() );
            WriteChar ( '_' );
        }
    }
}

void VGR::CloseMe() {
    qApp->quit();
}

void VGR::hscrolled ( int val ) {
    ox = val;
    setUpdatesEnabled ( TRUE );
    repaint();
    setUpdatesEnabled ( FALSE );
}

void VGR::vscrolled ( int val ) {
    oy = val;
    setUpdatesEnabled ( TRUE );
    repaint();
    setUpdatesEnabled ( FALSE );
}

void VGR::SetForeground ( int col ) {
    fcol = col;
}

void VGR::SetBackground ( int col ) {
    bcol = col;
}

void VGR::Line ( int x1,int y1,int x2,int y2 ) {
    QPainter p;
    p.begin ( canvas );
    p.setPen ( *lcolors[fcol] );
    p.drawLine ( x1,y1,x2,y2 );
    p.end();

    p.begin ( this );
    p.setPen ( *lcolors[fcol] );
    p.drawLine ( x1-ox, y1-oy,x2-ox, y2-oy );
    p.end();
}

void VGR::Ellipse ( int x,int y,int a, int b,int alfa, int beta, int fill ) {
    QPainter p;

    p.begin ( canvas );
    p.setPen ( *lcolors[fcol] );
    p.setBrush ( *lcolors[fcol] );
    if ( fill>0 )
        p.drawPie ( x,y,a,b,alfa*16, ( beta-alfa ) *16 );
    else
        p.drawArc ( x,y,a,b,alfa*16, ( beta-alfa ) *16 );
    p.end();

    p.begin ( this );
    p.setPen ( *lcolors[fcol] );
    p.setBrush ( *lcolors[fcol] );
    if ( fill>0 )
        p.drawPie ( x-ox, y-oy,a,b,alfa*16, ( beta-alfa ) *16 );
    else
        p.drawArc ( x-ox, y-oy,a,b,alfa*16, ( beta-alfa ) *16 );
    p.end();
}

void VGR::Rect ( int x1, int y1, int x2, int y2,int col, int fill ) {
    QPainter p;
    QBrush b ( *lcolors[col] );
    p.begin ( canvas );
    p.setPen ( *lcolors[col] );
    if ( fill>0 ) p.fillRect ( x1,y1,x2-x1,y2-y1,b );
    else p.drawRect ( x1,y1,x2-x1,y2-y1 );
    p.end();

    p.begin ( this );
    p.setPen ( *lcolors[col] );
    if ( fill>0 ) p.fillRect ( x1-ox, y1-oy,x2-x1,y2-y1,b );
    else p.drawRect ( x1-ox, y1-oy,x2-x1,y2-y1 );
    p.end();
}

void VGR::TextXY ( int x, int y, char *s ) {
    QPainter p;
    QFontMetrics fm ( *prv_font );

    p.begin ( canvas );
    p.setPen ( *lcolors[fcol] );
    p.setBackgroundColor ( *lcolors[bcol] );
    p.setBackgroundMode ( OpaqueMode );
    p.setFont ( *prv_font );
    p.drawText ( x,y+fm.height(),s,strlen ( s ) );
    p.end();

    p.begin ( this );
    p.setPen ( *lcolors[fcol] );
    p.setBackgroundColor ( *lcolors[bcol] );
    p.setBackgroundMode ( OpaqueMode );
    p.setFont ( *prv_font );
    p.drawText ( x-ox, y-oy+fm.height(),s,strlen ( s ) );
    p.end();
}


void VGR::CharXY ( int x, int y, char a ) {
    QPainter p;
    char s[2];
    QFontMetrics fm ( *prv_font );

    s[0] = a;
    s[1] = '\0';
    p.begin ( canvas );
    p.setPen ( *lcolors[fcol] );
    p.setBackgroundColor ( *lcolors[bcol] );
    p.setBackgroundMode ( OpaqueMode );
    p.setFont ( *prv_font );
    p.drawText ( x,y+fm.height(),s,strlen ( s ) );
    p.end();

    p.begin ( this );
    p.setPen ( *lcolors[fcol] );
    p.setBackgroundColor ( *lcolors[bcol] );
    p.setBackgroundMode ( OpaqueMode );
    p.setFont ( *prv_font );
    p.drawText ( x-ox, y-oy+fm.height(),s,strlen ( s ) );
    p.end();
}

void VGR::IntXY ( int x, int y, int val ) {
    QPainter p;
    char s[80];
    QFontMetrics fm ( *prv_font );

    sprintf ( s,"%d",val );
    p.begin ( canvas );
    p.setPen ( *lcolors[fcol] );
    p.setBackgroundColor ( *lcolors[bcol] );
    p.setBackgroundMode ( OpaqueMode );
    p.setFont ( *prv_font );
    p.drawText ( x,y+fm.height(),s,strlen ( s ) );
    p.end();

    p.begin ( this );
    p.setPen ( *lcolors[fcol] );
    p.setBackgroundColor ( *lcolors[bcol] );
    p.setBackgroundMode ( OpaqueMode );
    p.setFont ( *prv_font );
    p.drawText ( x-ox, y-oy+fm.height(),s,strlen ( s ) );
    p.end();
}

void VGR::WriteText ( char *s ) {
    QPainter p;
    QFontMetrics fm ( *prv_font );

    p.begin ( canvas );
    p.setPen ( *lcolors[fcol] );
    p.setBackgroundColor ( *lcolors[bcol] );
    p.setBackgroundMode ( OpaqueMode );
    p.setFont ( *prv_font );
    p.drawText ( tx*fm.maxWidth(), ( ty+1 ) *fm.height(),s,strlen ( s ) );
    p.end();

    p.begin ( this );
    p.setPen ( *lcolors[fcol] );
    p.setBackgroundColor ( *lcolors[bcol] );
    p.setBackgroundMode ( OpaqueMode );
    p.setFont ( *prv_font );
    p.drawText ( tx*fm.maxWidth()-ox, ( ty+1 ) *fm.height()-oy,s,strlen ( s ) );
    p.end();

    tx = tx + strlen ( s );
    if ( tx>TEXT_COLS ) {
        tx=0;
        ty++;
    };
    if ( ty>TEXT_LINES ) {
        ClearAll();
    }
}

void VGR::WriteChar ( char a ) {
    QPainter p;
    QFontMetrics fm ( *prv_font );
    char s[2];

    s[0] = a;
    s[1] = '\0';

    if ( GraphRead ) {
        p.begin ( canvas );
        p.setPen ( *lcolors[gfcol] );
        p.setBackgroundColor ( *lcolors[gbcol] );
        p.setBackgroundMode ( OpaqueMode );
        p.setFont ( *prv_font );
        p.drawText ( gx,gy+fm.height(),s,strlen ( s ) );
        p.end();

        p.begin ( this );
        p.setPen ( *lcolors[gfcol] );
        p.setBackgroundColor ( *lcolors[gbcol] );
        p.setBackgroundMode ( OpaqueMode );
        p.setFont ( *prv_font );
        p.drawText ( gx-ox, gy-oy+fm.height(),s,strlen ( s ) );
        p.end();

        gx = gx + strlen ( s ) *fm.maxWidth();
        if ( gx>MAXWIDTH ) {
            gy=gy+fm.height();
            gx=0;
        };
        if ( gy>MAXHEIGHT ) {
            ClearAll();
        };
    } else {
        p.begin ( canvas );
        p.setPen ( *lcolors[fcol] );
        p.setBackgroundColor ( *lcolors[bcol] );
        p.setBackgroundMode ( OpaqueMode );
        p.setFont ( *prv_font );
        p.drawText ( tx*fm.maxWidth(), ( ty+1 ) *fm.height(),s,strlen ( s ) );
        p.end();

        p.begin ( this );
        p.setPen ( *lcolors[fcol] );
        p.setBackgroundColor ( *lcolors[bcol] );
        p.setBackgroundMode ( OpaqueMode );
        p.setFont ( *prv_font );
        p.drawText ( tx*fm.maxWidth()-ox, ( ty+1 ) *fm.height()-oy,s,strlen ( s ) );
        p.end();

        tx = tx + strlen ( s );
        if ( tx>TEXT_COLS ) {
            tx=0;
            ty++;
        }
        if ( ty>TEXT_LINES ) {
            ClearAll();
        }
    }
}

void VGR::DelChar() {
    QPainter p;
    QFontMetrics fm ( *prv_font );

    if ( GraphRead ) {
        p.begin ( canvas );
        p.setPen ( *lcolors[fcol] );
        p.fillRect ( gx-fm.maxWidth(),gy,fm.maxWidth(),
                     fm.height() +fm.descent() +1, *lcolors[bcol] );
        p.end();

        p.begin ( this );
        p.setPen ( *lcolors[fcol] );

        p.fillRect ( gx-ox-fm.maxWidth(), gy-oy,fm.maxWidth(),
                     fm.height() +fm.descent() +1, *lcolors[bcol] );
        p.end();
        gx=gx-fm.maxWidth();

    } else { // Text read
        p.begin ( canvas );
        p.setPen ( *lcolors[fcol] );
        p.fillRect ( ( tx-1 ) *fm.maxWidth(), ( ty ) *fm.height(),tx*fm.maxWidth(),
                     ( ty+1 ) *fm.height() +fm.descent() +1, *lcolors[bcol] );
        p.end();

        p.begin ( this );
        p.setPen ( *lcolors[fcol] );
        p.fillRect ( ( tx-1 ) *fm.maxWidth()-ox, ( ty ) *fm.height()-oy,tx*fm.maxWidth(),
                     ( ty+1 ) *fm.height() +fm.descent() +1, *lcolors[bcol] );
        p.end();

        tx--;
        if ( tx<0 ) tx=0;
    }
}

void VGR::PutChar ( char a ) {
    QPainter p;
    QFontMetrics fm ( *prv_font );
    char s[2];

    s[0] = a;
    s[1] = '\0';
    p.begin ( canvas );
    p.setPen ( *lcolors[fcol] );
    p.setBackgroundColor ( *lcolors[bcol] );
    p.setBackgroundMode ( OpaqueMode );
    p.setFont ( *prv_font );
    p.drawText ( tx*fm.maxWidth(), ( ty+1 ) *fm.height(),s,strlen ( s ) );
    p.end();

    p.begin ( this );
    p.setPen ( *lcolors[fcol] );
    p.setBackgroundColor ( *lcolors[bcol] );
    p.setBackgroundMode ( OpaqueMode );
    p.setFont ( *prv_font );
    p.drawText ( tx*fm.maxWidth()-ox, ( ty+1 ) *fm.height()-oy,s,strlen ( s ) );
    p.end();
}

void VGR::WriteLine() {
    tx=0;
    ty++;
    if ( ty>TEXT_LINES ) {
        ClearAll();
    };
}


void VGR::Point ( int x, int y ) {
    QPainter p;

    p.begin ( canvas );
    p.setPen ( *lcolors[fcol] );
    p.drawPoint ( x,y );
    p.end();

    p.begin ( this );
    p.setPen ( *lcolors[fcol] );
    p.drawPoint ( x-ox, y-oy );
    p.end();
}

void VGR::ClearArea ( int x, int y, int w, int h ) {
    QPainter p;
    p.begin ( canvas );
    p.eraseRect ( x,y,w,h );
    p.end();

    p.begin ( this );
    p.eraseRect ( x-ox,y-oy,w,h );
    p.end();
}

void VGR::ClearAll() {
    QPainter p;
    QBrush b ( QColor ( white ) );

    tx = 0;
    ty = 0;
    curx = 0;
    cury = 0;
    canvas->fill ( backgroundColor() );

    p.begin ( this );
    p.fillRect ( 0,0,width()-16,height()-16,backgroundColor() );
    p.end();
}

int VGR::Getmap ( int w, int h ) {
    QPixmap *m;

    m = new QPixmap ( w,h );
    bitBlt ( m,0,0,canvas,curx,cury,w,h );
    maps.append ( new VGRMap ( firstfreemap,m ) );
    firstfreemap++;
    return ( firstfreemap-1 );
}

void VGR::Putmap ( int map ) {
    VGRMap *m;
    m = maps.first();
    while ( m!=NULL ) {
        if ( m->number == map ) {
            bitBlt ( canvas,curx,cury,m->map,0,0,m->map->width(),m->map->height() );
            bitBlt ( this,curx-ox, cury-oy,m->map,0,0,m->map->width(),m->map->height() );
            break;
        }
        m = maps.next();
    }
}

void VGR::Outstring ( int x, int y, char *s, int b, int f ) {
    QPainter p;
    QFontMetrics fm ( *prv_font );

    p.begin ( canvas );
    p.setPen ( *lcolors[f] );
    p.setBackgroundColor ( *lcolors[b] );
    p.setBackgroundMode ( OpaqueMode );
    p.setFont ( *prv_font );
    p.drawText ( x,y+fm.height(),s,strlen ( s ) );
    p.end();

    p.begin ( this );
    p.setPen ( *lcolors[f] );
    p.setFont ( *prv_font );
    p.setBackgroundColor ( *lcolors[b] );
    p.setBackgroundMode ( OpaqueMode );
    p.drawText ( x-ox, y-oy+fm.height(),s,strlen ( s ) );
    p.end();
}

void VGR::writeintxy ( int x, int y, int val,int c ) {
    QPainter p;
    char s[80];
    QFontMetrics fm ( *prv_font );

    sprintf ( s,"%d",val );
    p.begin ( canvas );
    p.setPen ( *lcolors[c] );
    p.setFont ( *prv_font );
    p.drawText ( x,y,s,strlen ( s ) );
    p.end();

    p.begin ( this );
    p.setPen ( *lcolors[c] );
    p.setFont ( *prv_font );
    p.drawText ( x-ox, y-oy+fm.height(),s,strlen ( s ) );
    p.end();
}

void VGR::CurPos() {
    G_MESSAGE msg;
    msg.msg_type = MSG_GRAPH;
    msg.param.pword[0] = GRAPH_CURPOS_RESPONSE;
    msg.param.pword[3] = curx;
    msg.param.pword[4] = cury;
    write ( resp_sock,&msg,sizeof ( G_MESSAGE ) );
}

void VGR::RequestMessage() {
    G_MESSAGE m,mm;
    int stat;

    bzero ( &m,sizeof ( G_MESSAGE ) );
    stat = read ( resp_sock,&m,sizeof ( G_MESSAGE ) );
    if ( stat>0 )
        if ( m.msg_type == MSG_GRAPH ) {
            switch ( m.param.pword[0] ) {

            case GRAPH_FREE:
                close_btn->setEnabled ( TRUE );
                delete request;
                request=NULL;
                break;
            case GRAPH_SET_TITLE:
                setCaption ( m.param.pstr );
                break;
            case GRAPH_WRITE:
                if ( strcmp ( m.param.pstr,"\n" ) ==0 )
                    WriteLine();
                else
                    WriteText ( m.param.pstr );
                break;
            case GRAPH_WRITEXY:
                TextXY ( m.param.pword[3],
                         m.param.pword[4],m.param.pstr );
                break;
            case GRAPH_READCHAR:
                if ( !GetInput ( 0 ) ) char_wait = TRUE;
                break;
            case GRAPH_READSTR:
                strcpy ( internal_buffer,"" );
                strcnt=0;
                string_wait=TRUE;
                WriteChar ( '_' );
                break;
            case GRAPH_READLN:
                if ( was_line ) {
                    m.msg_type = MSG_GRAPH;
                    m.param.pword[0] = GRAPH_READLN_RESPONSE;
                    write ( resp_sock,&m,sizeof ( G_MESSAGE ) );
                    was_line=FALSE;
                } else  {
                    if ( !GetInput ( 1 ) ) line_wait = TRUE;
                }
                break;
            case GRAPH_PUTCHAR:
                WriteChar ( m.param.pchar );
                break;
            case GRAPH_LINETO:
                Line ( curx,cury,
                       m.param.pword[3],m.param.pword[4] );
                break;
            case GRAPH_ELLIPSE:
                Ellipse ( m.param.pword[3],
                          m.param.pword[4],m.param.pword[5],m.param.pword[6],
                          m.param.pword[7],m.param.pword[8], m.param.pword[9] );
                break;
            case GRAPH_RECT:
                Rect ( m.param.pword[3],
                       m.param.pword[4],m.param.pword[5],
                       m.param.pword[6], m.param.pword[7],
                       m.param.pword[8] );
                break;
            case GRAPH_FOREGROUND:
                SetForeground ( m.param.pword[3] );
                break;
            case GRAPH_BACKGROUND:
                SetBackground ( m.param.pword[3] );
                break	;
            case GRAPH_POINT:
                Point ( m.param.pword[3], m.param.pword[4] );
                break;
            case GRAPH_CLEAR:
                ClearAll();
                break;
            case GRAPH_INKEY:
                if ( !GetInput ( 2 ) ) inkey_wait = TRUE;
                break;
            case GRAPH_CURPOS:
                CurPos();
                break;
            case GRAPH_OUTSTRING:
                Outstring ( m.param.pword[2],m.param.pword[3],m.param.pstr,
                            m.param.pword[4],m.param.pword[5] );
                break;
            case GRAPH_WRITEINTXY:
                writeintxy ( m.param.pword[2],
                             m.param.pword[3],m.param.pword[4],
                             m.param.pword[5] );
                break;
            case GRAPH_GETMAP:
                bzero ( &mm,sizeof ( G_MESSAGE ) );
                mm.msg_type = MSG_GRAPH;
                mm.param.pword[0] = GRAPH_GETMAP_RESPONSE;
                mm.param.pword[2] = Getmap ( m.param.pword[2]-curx,
                                             m.param.pword[3]-cury );
                mm.param.pword[3] = m.param.pword[2]-curx;
                mm.param.pword[4] = m.param.pword[3]-cury;
                write ( resp_sock,&mm,sizeof ( G_MESSAGE ) );
                break;
            case GRAPH_PUTMAP:
                Putmap ( m.param.pword[2] );
                break;
            case GRAPH_MOVE:
                curx = m.param.pword[2];
                cury = m.param.pword[3];
                break;
            case GRAPH_MGETPRESS:
                mouse_wait=TRUE;
                break;
            case GRAPH_MAGIC:
                MagicGraph ( &m );
                break;
            case GRAPH_HASCII:
                if ( m.param.pword[1]!=0 ) CharXY ( curx,cury, ( char ) m.param.pword[1] );
                else CharXY ( curx,cury,' ' );
                break;
            } /* end switch */
        } /* stat > 0 */
}

void VGR::MagicGraph ( G_MESSAGE *msg ) {
    int f,b;
    char ss[255];
    QPixmap p;
    VGRMap *pmap;

    switch ( msg->param.pword[1] ) {
    case 10:
        tx=msg->param.pword[2];
        ty=msg->param.pword[3];
        break; // gotoxy
    case 11:
        fcol=msg->param.pword[2];
        break;                     // forecolor
    case 12:
        bcol=msg->param.pword[2];
        break;                    // bkcolor
    case 13:
        prv_font=bold_font;
        break;
    case 14:
        prv_font=italic_font;
        break;
    case 15:
        prv_font=normal_font;
        break;
    case 16:
        ClearAll();
        break;
    case 303: // Draw array_of char (x,y,fcol,bcol)
        f=fcol;
        b=bcol;
        fcol=msg->param.pword[4];
        bcol=msg->param.pword[5];
        TextXY ( msg->param.pword[2],msg->param.pword[3],
                 msg->param.pstr );
        fcol=f;
        bcol=b;
        break;
    case 300: // Draw int (x,y,int,fcol,bcol)
        f=fcol;
        b=bcol;
        fcol=msg->param.pword[5];
        bcol=msg->param.pword[6];
        IntXY ( msg->param.pword[2],msg->param.pword[3],
                msg->param.pword[4] );
        fcol=f;
        bcol=b;
        break;
    case 301: // Draw char (x,y,char,fcol,bcol)
        f=fcol;
        b=bcol;
        fcol=msg->param.pword[5];
        bcol=msg->param.pword[6];
        CharXY ( msg->param.pword[2],msg->param.pword[3],
                 ( char ) ( msg->param.pword[4] ) );
        fcol=f;
        bcol=b;
        break;
    case 302: // Draw real (x,y,int,frac,fcol,bcol)
        f=fcol;
        b=bcol;
        fcol=msg->param.pword[6];
        bcol=msg->param.pword[7];
        sprintf ( ss,"%d.%d",msg->param.pword[4],msg->param.pword[5] );
        TextXY ( msg->param.pword[2],msg->param.pword[3],ss );
        fcol=f;
        bcol=b;
        break;
    case -304: // ReadText (x,y,fcol,bcol)
    case -305:
    case -307:
        gfcol=msg->param.pword[4];
        gbcol=msg->param.pword[5];
        strcpy ( internal_buffer,"" );
        strcnt=0;
        GraphRead=TRUE;
        gx=msg->param.pword[2];
        gy=msg->param.pword[3];
        string_wait=TRUE;
        WriteChar ( '_' );
        break;
    case -306:
        if ( !GetInput ( 0 ) ) {
            gfcol=msg->param.pword[4];
            gbcol=msg->param.pword[5];
            strcpy ( internal_buffer,"" );
            strcnt=0;
            GraphRead=TRUE;
            gx=msg->param.pword[2];
            gy=msg->param.pword[3];
            char_wait = TRUE;
        }
        break;
    case 308: // Put image from file (x,y,fname)
        if ( p.load ( msg->param.pstr ) ) {
            bitBlt ( canvas,msg->param.pword[2],msg->param.pword[3],&p,0,0,p.width(),p.height() );
            bitBlt ( this,msg->param.pword[2]-ox, msg->param.pword[3]-oy,&p,0,0,p.width(),p.height() );
        }
        break;
    case 309:// Kill map
        pmap = maps.first();
        while ( pmap!=NULL ) {
            if ( msg->param.pword[2] == pmap->number ) {
                maps.remove ( pmap );
                break;
            }
            pmap=maps.next();
        }
        break;
    case 310: // Line (x1,y1,x2,y2,col)
        f=fcol;
        fcol=msg->param.pword[6];
        Line ( msg->param.pword[2],msg->param.pword[3],
               msg->param.pword[4],msg->param.pword[5] );
        fcol=f;
        break;
    case 311: // Rectangle(x1,y1,x2,y2,fcol,icol)
        Rect ( msg->param.pword[2],msg->param.pword[3],
               msg->param.pword[4],msg->param.pword[5],
               msg->param.pword[7],1 );
        Rect ( msg->param.pword[2],msg->param.pword[3],
               msg->param.pword[4],msg->param.pword[5],
               msg->param.pword[6],0 );
        break;
    case 312: // ClearArea(x1,y1,w,h)
        ClearArea ( msg->param.pword[2],msg->param.pword[3],
                    msg->param.pword[4],msg->param.pword[5] );
        break;
    }//switch
}

#include "loggraph.moc"

int main ( int argc, char **argv ) {
    QApplication app ( argc, argv );
    VGR gs ( argv[1] );
    app.setMainWidget ( &gs );
    gs.show();
    return app.exec();
}
