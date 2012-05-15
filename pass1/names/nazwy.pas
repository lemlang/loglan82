program nazwy(input,output,names,result,fhash);
(* program czyta ciag identyfikatorow z pliku names *)
(* na plik result wypisuje pary (nazwa, numer z hash'u ) *)
(* na plik fhash wypisuje ciag podstawien tworzacy zmodyfikowana tablice hash *)
const dl = 10;
type tname = array[1..10] of integer;
     thash = array[1..8000] of integer;
var name:tname;
    hash : thash;
    nlast : integer;
    a1,b1,i,j,n:integer;
    a,b:char;
    names,fhash,result:text;
    str:lstring(20);
function search(vars k:integer; vars name:tname; vars hash:thash; 
                  vars nlast:integer):integer [fortran];extern;    
procedure init(vars hash:thash) [fortran];extern;
begin
   (* inicjalizacja *) 
   reset(names);
   rewrite(result);
   rewrite(fhash);
   for i := 1 to 8000 do hash[i] := 0;
   nlast := 8001;
   (* inicjalizacja tablicy hash *)
   init(hash);
   writeln('  koniec inicjalizacji tablicy hash ');
   
   while not eof(names) do
   begin
   i := 0;
   j := 0;
   str.len := wrd(20);
     while not eoln(names) do
     begin
        read(names,a);
	j := j+1;
	str[j] := a;
        if a<'a'then a1 := ord(a) - ord('0') else
            a1 := ord(a) - ord('a')+10;
	if not eoln(names) then
	begin
	  read(names,b);
	  j := j+1;
	  str[j] := b;
	  i := i+1;
          if b<'a' then b1 := ord(b) - ord('0') else
	     b1 := ord(b) - ord('a')+10;
          if a1 = 0 then a1 := 60;                
	  name[i] := a1*64+b1;
	end else
	begin
	  i := i+1;
	  name[i] := a1;
	end;
      end;
      (* koniec nazwy *)
      n := search(i,name,hash,nlast);
      str.len := wrd(j);
      writeln(result,'   ',str, '   ',n);
      readln(names);
    end;
   
    (* wypisanie tablicy hash *)
    for i := 1 to 8000 do
    begin
      if hash[i] <> 0 then
      writeln(fhash,'      ','HASH (',i:6,' ) =',hash[i]:6);
    end;
  end.    
        
      	       
