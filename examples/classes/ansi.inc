unit ANSI: IIUWGRAPH class;
var res:array_of char;

  unit GotoXY:procedure(x,y:integer);
    begin
      res:=hfont8(10,x,y,0,"",0,0,0);
    end GotoXY;
  unit SetColor:procedure(col:integer);
     begin
       res:=hfont8(11,col,0,0,"",0,0,0);
     end SetColor;
  unit SetBackground:procedure(col:integer);
     begin
        res:=hfont8(12,col,0,0,"",0,0,0);
     end SetBackground;
  unit Bold:procedure;
     begin
       res:=hfont8(13,0,0,0,"",0,0,0);
     end Bold;
  unit Normal:procedure;
     begin
       res:=hfont8(15,0,0,0,"",0,0,0);
     end Normal;
  unit Clear:procedure;
     begin
       res:=hfont8(16,0,0,0,"",0,0,0);
     end Clear;
begin
end ANSI;
