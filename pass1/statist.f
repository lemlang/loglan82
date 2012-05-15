      SUBROUTINE  MSTAT
C----------------DRUKOWANIE STATYSTYK KOMPILATORA
C
C	      BEZ OPISU W DOKUMENTACJI (CZESC ML2)
C	      WERSJA Z DNIA:		19.01.82
C	      DLUGOSC KODU:	 751
C...........................................................................
C
      IMPLICIT INTEGER (A-Z)
      REAL USED, TOTAL
C
      COMMON /BLANK/ IOP(4), ERRLINE, ERRCNT,
     X		COM(272),
     X	      LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X	      XFIL(17),
     X	      IPMEM(5000)
      COMMON /LYST/ PAGESZ, PAGENR, LINPG,
     X		    LN1, LGTH1, BUFLN1(30),
     X		    LN2, LGTH2, BUFLN2(30),
     X		    LUN
       integer*4 bufln1, bufln2
C
      COMMON /MSTA/ MLFREE, WNFREE, WNSTK, AL1BLK, AL1STK,
     X		       AL2BLK, AL2SYM, AL2OTH, WNBLK, TLP, TLM,
     X		       WNSUS, TOTMEM
C
      IF (IOP(1) .EQ. 9)    RETURN
C     --POWROT GDYZ WYABORTOWANY ZOSTAL PARSER
      CALL  PGINIT

      call ffputspaces(lun,22)
      call ffputcs(lun,'- STATISTICS')
      call ffputnl(lun)
      call ffputspaces(lun,24)
      call ffputcs(lun,'STORAGE')
      call ffputi (lun,TOTMEM,6)
      call ffputcs(lun,'K WORDS')
      call ffputnl(lun)
      call ffputcs(lun,'PARSER')
      call ffputnl(lun)

      IUS = TLM-TLP
      TOTAL = IUS
      IUS = IUS-WNFREE
      USED = IUS
      TOTAL = USED/TOTAL
      TOTAL = TOTAL*100.0
      IUS = TOTAL

      call ffputspaces(lun,30)
      call ffputcs(lun,'DECL. PART')
      call ffputi (lun,WNFREE,6)
      call ffputcs(lun,'WORDS LEFT')
      call ffputi (lun,IUS,6)
      call ffputcs(lun,'% USED')
      call ffputnl(lun)

      USED = WNSUS
      IUS = TLP-3738
      TOTAL = IUS
      TOTAL = USED/TOTAL
      TOTAL = TOTAL*100.0
      IUS = TOTAL

      call ffputspaces(lun,30)
      call ffputcs(lun,'STACK')
      call ffputspaces(lun,5)
      call ffputi (lun,WNSTK,6)
      call ffputcs(lun,' WORDS USED')
      call ffputi (lun,IUS,6)
      call ffputcs(lun,'% USED')
      call ffputnl(lun)

      WNBLK=WNBLK-12

      call ffputspaces(lun,37)
      call ffputi (lun,WNBLK,6)
      call ffputcs(lun,'+12 BLOCK(S) WRITTEN')
      call ffputnl(lun)

      call ffputspaces(lun,24)
      call ffputcs(lun,'SEMANTIC ANALISIS')
      call ffputnl(lun)

      TOTAL = TLP
      IUS = TLP-MLFREE
      USED = IUS
      TOTAL = USED/TOTAL
      TOTAL = TOTAL*100.0
      IUS = TOTAL

      call ffputspaces(lun,30)
      call ffputcs(lun,'DECL. PART')
      call ffputi (lun,MLFREE,6)
      call ffputcs(lun,' WORDS LEFT')
      call ffputi (lun,IUS,6)
      call ffputcs(lun,'% USED')
      call ffputnl(lun)

      IF ((IOP(1) .GE. 10) .AND. (IOP(1) .LE. 12))    RETURN

      call ffputspaces(lun,30)
      call ffputcs(lun,'STACK')
      call ffputspaces(lun,22)
      call ffputi (lun,AL1STK,6)
      call ffputcs(lun,'% USED')
      call ffputnl(lun)

      AL1BLK = AL1BLK+1

      call ffputspaces(lun,40)
      call ffputi (lun,AL1BLK,6)
      call ffputcs(lun,' BLOCK(S) WRITTEN')
      call ffputnl(lun)

      RETURN
      END
