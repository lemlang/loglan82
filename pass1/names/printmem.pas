procedure printmem;
var i,j,k:integer;
begin
  writeln; writeln(' ================ zmienne ============================');
  writeln; writeln;
  writeln('   strings      ', strings);
  writeln('   ipradr       ', ipradr);
  writeln('   display(fiz) ', display);
  writeln('   display2     ', display2);
  writeln('   temporary    ',temporary);
  writeln('   main         ', main);
  writeln('   lower        ', lower);
  writeln('   upper        ', upper);
  writeln('   free         ', free);
  writeln('   freeitem     ', freeitem);
  writeln('   lastused     ', lastused);
  writeln('   headk        ', headk);
  writeln('   headk2       ', headk2);
  writeln; writeln;
  writeln('   element slownika dla none ', m^[0],'  ', m^[1]);
  writeln; writeln;
  writeln('================= opisy typow pierwotnych i listy ===============');
  j := 0;
  for i := ipradr to display-1 do
  begin
    if j mod 5 = 0 then
    begin
       writeln;
       write('  ',i:7,'***');
    end;
    j := j+1;
    write(m^[i]:9);
  end;
  writeln; writeln;
  writeln('======================= display ===============');
  writeln; writeln;
  for i := 0 to lastprot do
  begin
     write('  ',display+i:7,display2+i:7, '(',i:5,')',m^[display+i], m^[display2+i]);
     writeln;
  end;
  writeln; writeln;
  writeln(' =================   obiekt main ================');
  i := m^[main];
  j := 0;
  for k := main to main+i-1 do
  begin
    if j mod 5 = 0 then
    begin
      writeln;
      write('  ',k:7,'***');
    end;
    j := j+1;
    write(m^[k]:9);
  end;
  writeln; writeln;
  writeln('===================== obiekty (lower to latused) ==========');
  writeln; writeln;
  j :=0;
  for i := lower to lastused do
  begin
     if j mod 5 = 0 then
     begin
        writeln;
	write('   ',i:7,'***');
    end;
    j := j+1;
    write(m^[i]:9);
 end;
 writeln; writeln;
 writeln('======================= tablica H (upper downto lastitem) =====');
 writeln; writeln;
 i := upper-1;
 while i>= lastitem do
 begin
   writeln('   ',i:7,'***',m^[i]:9, m^[i+1]:9);
   i:=i-2;
 end;
 writeln; writeln;
 writeln('=================================================================');
end (* printmem *);
      

 procedure printkind(kind:protkind);
 begin
    case kind of
      class :     writeln('     class');
      lrecord:    writeln('     record');
      coroutine:  writeln('     coroutine');
      process:    writeln('     process);
      block :     writeln('     block');
      prefblock:  writeln('     prefblock');
      lfunction:  writeln('     function');
      lprocedure: writeln('     procedure');
      handler: writeln('     handler');
   end;
 end;      	  
              
procedure printprot;
var i:integer;
begin
  writeln; writeln;     
  writeln('================ prototypes  ============== ');
  writeln('   lastprot     ', lastprot);
  for i := mainblock to lastprot do
  with prototype[i]^ do
  begin
     writeln; writeln;
     write('   prototyp nr ',i);printkind(kind);
     writeln('             SL        ', slprototype);
     writeln('             codeaddr  ', codeaddr);
     writeln('             appetite  ', appetite);
     writeln('             span      ', span);
     writeln('             reflist   ', reflist, lthreflist);
     writeln('             parlist   ', parlist,lthparlist);
     writeln('             preflist  ',preflist, lthpreflist);
     writeln('             virtlist  ', virtlist);
     if (kind = lfunction) or (kind = lprocedure) then
     begin
        writeln('             pardescr  ', pfdescr);
	if kind = lfunction then 
	writeln('             type      ', finaltype);
     end;
   end;        
 end (* printprot *);    	
             	   