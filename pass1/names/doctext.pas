program doctext(input,output,infile,outfile);
const ff = 12;
      lf = 10;
      cr = 13;
var c:char;
    infile,outfile:file of char;
begin
   reset(infile); rewrite(outfile);
   read(infile,c);
   if c <> '1' then write(outfile,c);
   while not eof(infile) do
   begin
      read(infile,c);
      if ord(c) = lf then
      begin
        if not eof(infile) then
	begin
	   read(infile,c);
	   if c = '1' then write(outfile,chr(ff)) else
	   if c <> '+' then write(outfile,chr(lf));
	end else write(outfile,chr(lf));
      end else write(outfile,c);
    end
  end.    	       