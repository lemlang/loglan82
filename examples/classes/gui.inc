
unit GUI: IIUWGRAPH class;
const
      c_white=15,c_yellow=14,c_rose=13,c_red=12,
      c_turq=11,c_green=10,c_blue=9,c_darkgrey=8,
      c_lightgrey=7,c_brown=6,c_violet=5, c_darkred=4,
      c_darkturq=3,c_darkgreen=2,c_darkblue=1,
     c_black=0;

var res:array_of char, cursor_x,cursor_y:integer;

unit GUI_Clear:procedure;
begin
 call cls;
end GUI_Clear;

unit GUI_ClearArea:procedure(x1,y1,w,h:integer);
begin
 res:=hfont8(312,x1,y1,w,"",h,0,0);
end GUI_ClearArea;


unit GUI_Point:procedure(x,y,col:integer);
begin
 call color(col);
 call point(x,y);
end GUI_Point;

unit GUI_Move:procedure(x,y:integer);
begin
 cursor_x:=x;
 cursor_y:=y;
end GUI_Move;

unit GUI_Line:procedure(x1,y1,x2,y2,col:integer);
begin
 res:=hfont8(310,x1,y1,x2,"",y2,col,0);
end GUI_Line;

unit GUI_LineTo:procedure(x,y,col:integer);
begin
 res:=hfont8(310,cursor_x,cursor_y,x,"",y,col,0); 
end GUI_LineTo;

unit GUI_Rect:procedure(x1,y1,x2,y2,fcol,icol:integer);
begin
 res:=hfont8(311,x1,y1,x2,"",y2,fcol,icol);
end GUI_Rect;

unit GUI_Ellipse:procedure(x,y,a,b,sa,ea,fcol,icol:integer);
begin
 call color(icol);
 call cirb(x,y,a,b,sa,ea,icol,1);
 call color(fcol);
 call cirb(x,y,a,b,sa,ea,fcol,0);
end GUI_Ellipse;

unit GUI_WriteInt:procedure(x,y,i,fcol,bcol:integer);
begin
 res:=hfont8(300,x,y,i,"",fcol,bcol,0);
end GUI_WriteInt;

unit GUI_WriteChar:procedure(x,y:integer;c:char;fcol,bcol:integer);
begin
 res:=hfont8(301,x,y,ord(c),"",fcol,bcol,0);
end GUI_WriteChar;


unit GUI_WriteReal:procedure(x,y:integer;r:real;
                                            fcol,bcol:integer);
var  rr,i:real,int,frac:integer;
begin
 int:=entier(r);
 rr:=r-int;
 i:=1.0;
 while rr-entier(rr)>0 do
  i:=i*10.0;
  rr:=r*i;
od;
 frac:=entier(r*i-int*i);
 res:=hfont8(302,x,y,int,"",frac,fcol,bcol); 
end GUI_WriteReal;



unit GUI_WriteText:procedure(x,y:integer;t:array_of char;
                                              fcol,bcol:integer);
var i:integer;
begin
if t<>none then
res:=hfont8(1000,0,0,0,"",0,0,0);
for i:=lower(t) to upper(t) do
 res:=hfont8(1001,ord(t(i)),0,0,"",0,0,0);
od;
 res:=hfont8(303,x,y,fcol,"",bcol,0,0);
fi;
end GUI_WriteText;

unit GUI_ReadText:function(x,y,
                             fcol,bcol:integer):array_of char;
begin
result:=hfont8(-304,x,y,fcol,"",bcol,0,0);
end GUI_ReadText;


unit GUI_ReadInt:function(x,y,fcol,bcol:integer):integer;
begin
res:=hfont8(-305,x,y,fcol,"",bcol,0,0);
result:=ord(res(0));
end GUI_ReadInt;

unit GUI_ReadChar:function(x,y,
                             fcol,bcol:integer):char;
begin
res:=hfont8(-306,x,y,fcol,"",bcol,0,0);
result:=res(0);
end GUI_ReadChar;

unit GUI_ReadReal:function(x,y,
                             fcol,bcol:integer):real;
var p1,p2,i:real;
begin
res:=hfont8(-307,x,y,fcol,"",bcol,0,0);
p1:=ord(res(0));
p2:=ord(res(1));
while  entier(p2)>0 do
p2:=p2/10.0;
od;
result:=p1+p2;
end GUI_ReadReal;


unit GUI_PutImgFile:procedure(x,y:integer;fname:string);
begin
 res:=hfont8(308,x,y,0,fname,0,0,0);
end GUI_PutImgFile;

unit GUI_GetImg:function(x,y,w,h:integer):array_of integer;
begin
 call move(x,y);
 result:=getmap(x+w,y+h);
end GUI_GetImg;

unit GUI_PutImg:procedure(x,y:integer;map:array_of integer);
begin
 call move(x,y);
 call putmap(map);
end GUI_PutImg;

unit GUI_KillImg:procedure(map:array_of integer);
begin
if map<>none then
 res:=hfont8(309,map(1),0,0,"",0,0,0);
fi;
end GUI_KillImg;

unit GUI_MousePressed:MOUSE 
              procedure(INOUT x,y,btn:integer);
var l,p,r:integer,e:boolean;
begin
 e:=getpress(x,y,l,p,r,btn);
end GUI_MousePressed;

unit GUI_KeyPressed:function:integer;
begin
 result:=inkey;
end GUI_KeyPressed;

begin
cursor_x:=0;cursor_y:=0;
end GUI;
