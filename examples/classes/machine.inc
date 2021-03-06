unit Machine: IIUWGRAPH class;

  unit NodeInfo:class;
   var num:integer,addr:array_of char,
        Next:NodeInfo;
  begin
    Next:=none; num:=0;
    addr:=none;
  end NodeInfo;

 unit LocalNode:function:integer;
 var res:array_of char;
 begin
  res:=hfont8(501,0,0,0,"",0,0,0);
  result:=ord(res(0));
 end LocalNode;
  
 unit NodesNum:function:integer;
 var res:array_of char;
 begin
  res:=hfont8(502,0,0,0,"",0,0,0);
  result:=ord(res(0));
 end NodesNum;

 unit NodeExists:function(n:integer):boolean;
 var res:array_of char;
 begin
  res:=hfont8(503,n,0,0,"",0,0,0);
  if ord(res(0))=1 then result:=TRUE; 
  else result:=FALSE;fi;
 end NodeExists;

 unit MachineInfo:function:NodeInfo;
 var t,pomt:array_of char,c:char,i,j,k,l,m,n:integer,
       pomi,List:NodeInfo;
 begin
  t:=hfont8(504,0,0,0,"",0,0,0); 
  List:=none;
  if t<>none then
                      j:=upper(t);i:=lower(t);
                      while i<j do
                            k:=i;
                            while i<j and t(i)<>';' do i:=i+1;od;
                            l:=k; while t(l)<>'=' do l:=l+1; od;
                            if List=none then List:=new NodeInfo;  
                                                      pomi:=List;
                            else pomi.Next:=new NodeInfo;
                                   pomi:=pomi.Next;
                            fi;  
                             array pomi.addr dim (1:i-l-1);
                             for m:=l+1 to i-1 do pomi.addr(m-l):=t(m); od;
                             n:=1;
                             for m:=l-1 downto k do 
                                pomi.num:=pomi.num+(ord(t(m))-48)*n;
                                n:=n*10;
                             od; 
                            i:=i+1;
                        od;     
                   result:=List;
                  else result:=none;
                  fi;
 end MachineInfo;

begin
end Machine;  