module execprog[];
(*CBC B.Ciesielski *)
(* PASCAL routines for EXEC standard function *)

var
    pspdd    : word;
    defdsdd  : word;
    envdd    : word;
    cesxqq [external] : word;               ! ES register from Pascal
    addr_ptr          : ads of word;
    ax,bx,cx,dx,si,di : word;        ! General Registers
    ds,es             : word;        ! Segment Registers
    errcode           : word;        ! Error code returned from DOS

procedure dos(var ax,bx,cx,dx,si,di,ds,es,errcode : word); external;

procedure initdd(var ax,bx,cx,dx,si,di,ds,es : word);
{INITDD Initialize registers for call to DOS.}
  begin {initdd}
    ds := defdsdd;                          ! Program Data Segment
    es := ds;
    si := 0;
    di := 0;
    ax := 0;
    bx := 0;
    cx := 0;
    dx := 0
  end;  {initdd}

procedure asciizdd(const lstr : lstring; var astr : string);
{ASCIIZDD  This procedure converts a lstring variable to a ASCIIZ string.
 An asciiz string is terminated by a byte of zeroes.}
  var
    len,alen : byte;
  begin {asciizdd}
    len  := lstr.len;
    alen := wrd(upper(astr));
    if (len >= alen) then
       len := alen - 1;
    movel(adr lstr[1],adr astr[1],len);
    astr[len + 1] := chr(0)
  end;  {asciizdd}

procedure initsup;
(* initialize system pointers *)
  begin
    pspdd      := cesxqq;                   ! Program segment prefix
    addr_ptr   := ads cesxqq;
    defdsdd    := addr_ptr.s;               ! Default data segment
    addr_ptr.s := pspdd;                    ! Program environment address
    addr_ptr.r := #2c;                      ! is at offset #2c in PSP.
    envdd      := addr_ptr^
  end; {initsup}

procedure setblkmm(blkseg,blksize : word;  var block,ercode : word);
{SETBLKMM  Modifies Allocated Blocks.  The memory block pointed to by
 blkseg is modified to the size (in paragraphs) of blksize.  The
 block increases or decreases in size to that specified in blksize.
 If the block cannot grow to blksize, the largest possible expansion
 is made, and new block size is returned in block.  The values for
 the returned error code are:

	      0 - Successful reallocation.  The new block size
		  (in paragraphs) is in block.
	      1 - Memory control blocks destroyed
	      2 - Insufficient memory on a grow request.  The
		  new block size is in block.
	      3 - The pointer in blkseg points to a block which
		  has not been allocated.
	    >99 - Unidentified error.					}

  begin {setblkmm}

    initdd(ax,bx,cx,dx,si,di,ds,es);	    ! Initialize registers
    ax := byword(#4a,0);		    ! DOS Function 4ah
    es := blkseg;
    bx := blksize;

    dos(ax,bx,cx,dx,si,di,ds,es,errcode);

    block := bx;
    case errcode of
      0 : ercode := 0;
      7 : begin 			    ! Destroyed control blocks
	    block  := 0;
	    ercode := 1
	  end;
      8 : ercode := 2;			    ! Not enough memory
      9 : begin
	    block  := 0;		    ! Illegal block request
	    ercode := 3
	  end
      otherwise
	    block  := 0;
	    ercode := 100 + errcode
    end  {case}

  end;	{setblkmm}

procedure shrinkmm(var ercode : word);
{SHRINKMM  Release all memory not needed by the executing program.  This
 procedure makes a call to SETBLKMM to release all memory which has
 been allocated, but which is not needed.  DOS normally allocates all
 memory to a program, so memory must be released before other calls to
 ALLOCMM are made.  The required memory is calculated as the code size
 plus the data space.  The code size is essentially the default data
 segment minus the program segment prefix.  The data space is the mini-
 mum of #1000 paragraphs (64K bytes) or the remainder of memory.  The
 value of the error code returned is that returned by SETBLKMM.}

  var
    topmem_ptr : ads of word;               ! Pointer to word containing
    topmem     : word;                      ! top of memory.
    data_seg   : word;                      ! data segment start
    code_size  : word;
    data_space : word;
    blksize    : word;                      ! Total block size needed

  begin {shrinkmm}

    topmem_ptr.s := pspdd;
    topmem_ptr.r := 2;                      ! At offset 2 in PSP
    topmem       := topmem_ptr^;
    data_seg     := defdsdd;                ! Default data segment
    if (data_seg < pspdd) then              ! data_seg can be "negative"
       data_seg := topmem - (pspdd - data_seg);
    code_size    := data_seg - pspdd;
    data_space   := topmem - data_seg;
    if (data_space < #1000) then
       blksize   := code_size + data_space
    else
       blksize   := code_size + #1000;

    setblkmm(pspdd,blksize,blksize,ercode)

  end;  {shrinkmm}

procedure execmm  (const proc_name, cmd_line : lstring;
                                                   var ercode : word) [public];
{EXECMM  Load and execute a program.  This procedure loads and executes
 another program.  The program path name is given in proc_name; the
 command line is given in cmd_line.  The specified program will be
 loaded and executed as if invoked from DOS with the given command
 line.  The spawned program returns control to the calling procedure
 when execution ends, either normally or using a Ctrl/Break sequence.
 The values of the returned error code are:

              0 - Successful execution and return.
       (*)    1 - Memory control blocks destroyed when trying
                  to reserve memory.
       (*)    2 - Insufficient memory when trying to reserve
                  memory.
       (*)    3 - Illegal block change request when trying to
                  reserve memory.
              4 - Executable file not found.
              5 - Access denied.  The file does not allow read
                  access.
              6 - Insufficient memory to load the new procedure.
       (*)    7 - Invalid environment created in the called
                  program.
       (*)    8 - Invalid format in the environment.
              9 - Invalid Path Name.  Probably illegal characters
                  are present in the proc_name parameter.
            >99 - Unidentified error

            Errors marked with (*) are internal errors and should not
            normally occur.  Errors 1 - 3 indicate problems with the
            SHRINKMM procedure, and errors 7 & 8 are internal errors
            within EXECMM (the construction of the parameter block).}

  type
    parm_block = record                ! Parameter block
                   env_sadd : word;    ! Segment address of environment
                   cmd_addr : adsmem;  ! Address of command line
                   fb1_addr : adsmem;  ! Address of first FCB
                   fb2_addr : adsmem   ! Address of second FCB
                 end;
  var
    procz      : string(255);          ! ASCIIZ procedure name
    block_val  : parm_block;           ! Constructed parameter block

  begin {execmm}
    initsup;
    shrinkmm(ercode);                  ! Release available memory

    if (ercode = 0) then               ! Now load the program
       begin
         with block_val do
           begin
             env_sadd   := envdd;      ! Copy parent's environment
             cmd_addr   := ads cmd_line[0];
             fb1_addr.s := pspdd;
             fb1_addr.r := #5c;
             fb2_addr.s := pspdd;
             fb2_addr.r := #6c
           end;
         initdd(ax,bx,cx,dx,si,di,ds,es);
         ax  := byword(#4b,0);         ! DOS function 4bh
         bx  := wrd(adr block_val);
         cx  := 0;
         asciizdd(proc_name,procz);
         dx  := wrd(adr procz);        ! Path name as an asciiz string

         dos(ax,bx,cx,dx,si,di,ds,es,errcode);

         case errcode of
           0 : ercode := 0;
           1 : ercode := 9;            ! Invalid path name
           2 : ercode := 4;            ! File not found
           5 : ercode := 5;            ! Access denied
           8 : ercode := 6;            ! Insufficient memory
          10 : ercode := 7;            ! Invalid environoment
          11 : ercode := 8;            ! Invalid format
           otherwise
               ercode := 100 + errcode
         end  {case}

       end

  end;  {execmm}

end.
