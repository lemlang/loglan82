C    Loglan82 Compiler&Interpreter
C     Copyright (C) 1981-1993 Institute of Informatics, University of Warsaw
C     Copyright (C)  1993, 1994 LITA, Pau
C     
C     This program is free software; you can redistribute it and/or modify
C     it under the terms of the GNU General Public License as published by
C     the Free Software Foundation; either version 2 of the License, or
C     (at your option) any later version.
C     
C     This program is distributed in the hope that it will be useful,
C     but WITHOUT ANY WARRANTY; without even the implied warranty of
C     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
C     GNU General Public License for more details. File: LICENSE.GNU
C  ===============================================================     

      SUBROUTINE DSW
C
C  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C      ( BYLY ) PROGRAM GLOWNY
C  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C
      IMPLICIT INTEGER (A - Z )
C
C
      COMMON /MJLMSG/ IERC,MSG
      character*4 msg
C
C
C   BUFORY
C
C
C.....BLOK KOMUNIKACJI ZE STRUMIENIAMI
      LOGICAL  ERRFLG
      COMMON /STREAM/  ERRFLG, LINE, IBUF2(265), IBUF3(7),JUNK(260)
C
C
C.....
c$include:'blank.for'

cdeb --------------------- added -------------------
      common /debug/ deb, breakt(500),brnr,maxbr
      logical deb
cdeb ------------------------------------------------
C
      IERC=0
      msg = 'dsw '
C
C
       CALL INITMK
C   SUBROUTINE INIT RENAMED TO INITMK         03.01.84   **********************
       CALL DPASS
C
C
C
      CALL MESS
cdeb      CALL AL1
cdeb ------------- added --------------
       if(deb.and..not.errflg) go to 1000
       call al1
       return
1000   call ts2
cdeb ----------------------------------
       END

      BLOCK DATA  BLKD
      IMPLICIT INTEGER (A-Z)
C
cdeb
      common /pr/ prot(5000),ind
c   prot - tablica na prototypy debuggera
c   ind - ostatnie zajete miejsce w tablicy
cdeb
C
C
      COMMON  / YNIT /  NULLWD(23), SIZEPR(23), NULLPOZ(23), CONSTWD,
     *  VARWD,VARPOM,INPFW,OUTPFW, INOUT
cdsw  INTEGER  SIZEPR, CONSTWD, VARWD, VARPOM, OUTPFW, INOUT
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C    NULLWD(I)  -  WZORZEc SLOWA ZEROWEGO DLA PROTOTYPU RODZAJU I
C    SIZEPR(I)  -  ROZMIAR POLA W IPMEM   --   --   --
C    NULLPOZ(I)  -  POZYCJA SLOWA ZEROWEGO  --   --    --
C    CONSTWD  -  WZORZEC SLOWA ZEROWEGO DLA    CONST
C    VARWD  -     --   --   --   --          DLA ZMIENNEJ
C    VARPOM  -     --   --   --   --           ZMIENNEJ POMOCNICZEJ
C    INPFW  -     --   --    --    --          ZMIEMNEJ INPUT
C    OUTPFW  -     --   --    --    --         ZMIENNEJ OUTPUT
C    INOUT  -     --   --   --   ---           ZMIENNEJ INOUT
C
C
      COMMON  / MASKS /  MTP,MSPR,MOTHERS,MPAR,MASKTP,NOTTP,
     * MPROCES, MCOR, MERPF, MBLOCK, MHAND
     *, MNOTVIR
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C   MASKI I WZORCE:
C    MTP - MASKA DO WYCINANIA INFORMACJI DOTYCZACYCH TYPOW ZE SLOWA ZEROWEGO
C    MOTHERS -      --     --      --      --     --  INNYCH PROTOTYPOW
C    MPAR -    --    --    --    --    --    --       ZMIENNYCH I PARAMETROW
C    MSPR - MASKA DLA SYSPREF  ( DLA PROTOTYPOW SYNTAKTYCZNYCH )
C    MASKTP - ZAPRZECZENIE MASKI  MTP
C    NOTTP - WZORZEC DLA NIE-TYPU  ( 1 )
C    MPROCES - WZORZEC DLA PROCESU  ( 5 )
C    MCOR - WZORZEC DLA COROUTINY (7)
C    MERPF - MASKA DO WYKRYWANIA BLEDNYCH LIST PARAMETROW
C    MBLOCK - WZORZEC DLA BLOKU  ( 0 PRZY PRZECIECIU Z  MOTHERS )
C    MNOTVIR - WZORZEC DO KASOWANIA BITU "VIRTUAL"
C    MHAND - WZORZEC DLA HANDLERA
C
C
cdeb------------------------------------------
cdeb      COMMON /NAMES/ RESNM,MAINM
      common /names/ resnm,mainm,brenam
cdeb------------------------------------------
C
C   NAZWY ZE SCANNERA
C  * * * * * * * * * * * * * * * * *  * * * * * * * * * * * * *
C
C
C     COMMON  / WYDR /  KD(8), KSP(4)
C     REAL KD,KSP
C
C    BLOK UZYWANY W PROCEDURYCH DRUKUJACYCH.
C    KD(KIND+1)  -  ODPOWIEDNI TEKST DLA PROTOTYPU RODZAJU KIND
C    KSP(KSPEC+1)  -  ODPOWIEDNI TEKST DLA PROTOTYPU RODZAJU KSPEC.
C   **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C  RODZAJE PROTOTYPOW:
C   1 - BLOK, 2 - REKORD, 3 - PROCEDURA, 4 - FUNKCJA, 5 - PROC. Z BLEDNA PF
C   6 - FUNKCJA Z BLEDNA LISTA PF, 7 - KLASA Z BLEDNA LISTA PF
C   8 - BLOK PREFIKSOWANY, 9 - PROCEDURA VIRTUALNA, 10 I FUNKCJA VIRTUALNA
C   11 - PROC. VIRTUALNA Z BLEDNA PF, 12 - FUNKCJA VIRTUALNA Z BLEDNA PF
C   13 - PROCEDURA FORMALNA, 14 - FUNKCJA FORMALNA, 15 - PROC. FORMALNA
C   Z BLEDNA LISTA PF, 16 - FUNKCJA FORMALNA Z BLEDNA LISTA PF,
C  17 - TYP FORMALNY, 18 - PROCEDURA FORMALNA II-GO RZEDU,
C   19 - FUNKCJA FORMALNA II-GO RZEDU
C   20 - TYP FORMALNY II-GO RZEDU
C   21 - SYGNAL, 22 - SYGNAL Z USZKODZONA LISTA PF, 23 - HANDLER
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C
C
      DATA SIZEPR/23,33,28,31,28,31,33,28,30,33,30,33,20,23,20,23,
     * 5,5,7,5,19, 19, 21/
      DATA NULLPOZ/2,7,2,5,2,5,7,2,2,5,2,5,2,5,2,5,2,2,4,2,1,1,2/
      DATA CONSTWD,VARWD,VARPOM,INPFW,OUTPFW / 129,113,65,81,97 /
      DATA INOUT /145/
      DATA NOTTP,MPROCES,MCOR,MBLOCK /1,5,7,0 /
C     DATA KD(1) /5HTYP F/
C     DATA KD(2)/4HKLAS/
C     DATA KD(3) / 4HFUN   /
C     DATA KD(4) / 4HPROC/
C     DATA KD(5) / 4HBLOK/
C     DATA KD(6) /4HBLPR/
C     DATA KD(7) /6HSIGNAL/
C     DATA KD(8) /5HHANDL/
C     DATA KSP(1) /4HREK  /
C     DATA KSP(2) / 4HKLAS/
C     DATA KSP(3) / 4HPRSS/
C     DATA KSP(4) / 4HCORO/
      DATA RESNM,MAINM / 2769,819 /
cdeb ------------- added ---------------
      data brenam /7797/
cdeb ----------------------------------
      DATA NULLWD / 1,2,1025,513,9217,8705,8194,257,3073,2561,11265,
     * 10753,1073,545,9265,8737,22,1073,545,22,177,8369,1793/
cdeb
      data prot /5000*-100/
cdeb
      END
      SUBROUTINE DPASS
C
C  * * * * * * * * * * * * * * * * * * ** * * * * * * * * * * * *
C    PODPROGRAM REALIZUUACY PRZETWARZANIE PROTOTYPOW
C  * * * * * * * * * * * * * *  * * * * * * * * * * * * * * * * *
C
      IMPLICIT iNTEGER (A-Z)
C
      COMMON  / QUEUE /  BQUEUE, EQUEUE, IFIRST, LAST, EMPTY
cdsw      INTEGER  BQUEUE, EQUEUE
      LOGICAL EMPTY
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C    ZMIENNE SLUZACE DO ORGANIZACJI KOLEJKI PROTOTYPOW
C      BQUEUE  -  POCZATEK POLA W IPMEM PRZEZNACZONEGO NA KOLEJKE
C      EQUEUE  -  KONIEC      --      --       --      --      --
C      IFIRST  -  PIERWSZY ELEMENT KOLEJKI
C      LAST  -  OSTATNI ELEMENT KOLEJKI
C      EMPTY = TRUE, GDY KOLEJKA JEST PUSTA
C
C
C
      COMMON  / YNIT /  NULLWD(23), SIZEPR(23), NULLPOZ(23), CONSTWD,
     *  VARWD,VARPOM,INPFW,OUTPFW, INOUT
cdsw  INTEGER  SIZEPR, CONSTWD, VARWD, VARPOM, OUTPFW
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C    NULLWD(I)  -  WZORZEc SLOWA ZEROWEGO DLA PROTOTYPU RODZAJU I
C    SIZEPR(I)  -  ROZMIAR POLA W IPMEM   --   --   --
C    NULLPOZ(I)  -  POZYCJA SLOWA ZEROWEGO  --   --    --
C    CONSTWD  -  WZORZEC SLOWA ZEROWEGO DLA    CONST
C    VARWD  -     --   --   --   --          DLA ZMIENNEJ
C    VARPOM  -     --   --   --   --           ZMIENNEJ POMOCNICZEJ
C    INPFW  -     --   --    --    --          ZMIEMNEJ INPUT
C    OUTPFW  -     --   --    --    --         ZMIENNEJ OUTPUT
C    INOUT  -    --   --   --  --    ---       ZMIENNEJ INOUT
C
C
C ..... ZMIENNE GLOBALNE
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
C             COM    - OBSZAR KOMUNIKACYJNY STRUMIENI
C             LMEM   - (=5000) ROZMIAR CALEJ PAMIECI GLOWNEJ
C             LPMEM  - PODZIAL PAMIECI NA CZESCI  IPMEM  I  ISMEM
C             IRECN  - INDEKS SZCZYTU STOSU STALYCH REAL
C             ISFIN  - INDEKS SZCZYTU SLOWNIKA PROTOTYPOW
C
C             LPMSYS - INDEKS PIERWSZEGO SLOWA W IPMEM OBSZARU NIEPRZEZ-
C                      NACZONEGO NA PROTOTYPY SYSTEMOWE
C             LPML   - INDEKS PIERWSZEGO SLOWA OBSZARU WOLNEGO W IPMEM
C             LPMF   - INDEKS OSTATNIEGO SLOWA WOLNEGO OBSZARU W IPMEM
C
C     IDENTYFIKATORY PROTOTYPOW SYSTEMOWYCH
C             NRINT  - IDENTYFIKATOR PROTOTYPU  INTEGER
C             NRRE   -                          REAL
C             NRBOOL -                          BOOLEAN
C             NRCHR  -                          CHARACTER
C             NRCOR  -                          COROUTINE
C             NRPROC -                          PROCESS
C             NRTEXT -                          STRING (TEXT)
C             NRUNIV - IDENTYFIKATOR PROTOTYPU UNIWERSALNEGO
C             NATTR  - IDENTYFIKATOR PSEUDO-ATRYBUTU (Z NUMEREM -1)
C             NRNONE - IDENTYFIKATOR PROTOTYPU TYPU NONE (UNIWERSALNY
C                      REFERENCYJNY)
C             NBLSYS - IDENTYFIKATOR BLOKU SYSTEMOWEGO
C             NBLUS  -               BLOKU GLOWNEGO UZYTKOWNIKA
C
C             INSYS  - FLAGA SPOSOBU REZERWACJI (PRZEZ  MGETM) PAMIECI
C                      W IPMEM - .TRUE. JESLI REZERWACJA W CZESCI SYSTE-
C                      MOWEJ
C             LOCAL  - FLAGA DOSTEPU DO OBIEKTOW - 2 JESLI OBIEKT
C         BYL LOKALNY, 1 - GDY Z SL, 0 - GDY Z BLOKU GLOWNEGO
C             OWN    - FLAGA DOSTEPU DO OBIEKTOW - .TRUE. JESLI OBIEKT NIE
C                      POCHODZI Z PREFIKSOW (TYLKO Z WLASCIWEGO OBIEKTU)
C             OBJECT - PROTOTYP OBIEKTU, Z KTOREGO POCHODZIL OSTATNIO
C                     SZUKANY IDENTYFIKATOR (OBIEKT POCHODZACY Z CIAGU SL)
C
C

      COMMON / DGLOB/ INDICT,INDSPR,INDPR,IHBEG,LASTPR,INDPREF
C
C  **  **  **  **  ***  **  **  **  **  **  **  **  **  **  **  **  **
C    BLOK ZAWIERA DANE O PRZETWARZANYM PROTOTYPIE
C        INDICT  -  INDEKS PRZETWARZANEGO PROTTYPU W IDICT
C        INDSPR  -  IDENTYFIKATOR PROTOTYPU W ISMEM
C        INDPR  -  IDENTYFIKATOR PROTOTYPU W IPMEM
C        IHBEG  -  ADRES PIERWSZEGO SLOWA TABLICY HASHU
C        IDPREF  -  IDENTYFIKATOR BEZPOSREDNIEGO PREFIKSU PRZETWARZANEGO
C                  PROTOTYPU
C        LASTPR  -  IDENTYFIKATOR POPRZEDNIO PRZETWARZANEGO PROTOTYPU
C
C
cdeb       COMMON/NAMES/RESNM,MAINM
cdeb -------------------------------------
       common /names/ resnm, mainm, brenam
cdeb ------------------------------------
C   NAZWY ZE SCANNERA
C
C
      COMMON  /PREFS/  LPREFS
C
C   **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C    LPREFS  -  OSTATNIO PRZYDZIELONY NUMER W PREFIXSET
C
C
C
C.....BLOK KOMUNIKACJI ZE STRUMIENIAMI
      LOGICAL  ERRFLG
      COMMON /STREAM/  ERRFLG, LINE, IBUF2(265), IBUF3(7),JUNK(260)

c system class prototypes:
      common /syspro/ prgraph, prmouse
C
C
C   UTWORZENIE PROTOTYPU BLOKU GLOWNEGO
      I=IPMEM(LPMEM)
      LINE = IPMEM(I+9)
      LASTPR = NBLSYS
      NBLUS=INITPR(1,0)
C   USTAWIENIE SL BLOKU GLOWNEGO NA SYSTEMOWY
      IPMEM(NBLUS-1) = NBLSYS
cdsw
      ipmem(nblus+2) = prgraph
      ipmem(prgraph+2) = prmouse
      lastpr = prmouse

C   DOLACZENIE NAZWY I ATRYBUTU MAIN
      I=MGETM(6,41)+4
      IPMEM(I)=VARWD
      IPMEM(I+1)=1
      CALL MADATR(I,NBLUS,41)
      IPMEM(I-3)=NRPROC
      IPMEM(I)=IAP(I)
      K=IDPUT(MAINM,IPMEM(NBLUS+10))
      IPMEM(K+2)=I
      CALL DPUTQ(LPMEM,NBLUS)
 100  IF(EMPTY) GO TO 300
      CALL DGETQ
      CALL PROTP1
      CALL PROTP2
      GO TO 100
 300  CONTINUE
C   KONIEC PRZETWARZANIA PROTOTYPOW
      IPMEM(NBLSYS+3) = LPREFS
C       ... PRZECHOWANIE INFORMACJI O LICZBIE KLAS
      RETURN
      END
      SUBROUTINE INITMK
C
C  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C    INICJALIZACJA LOKALNA
C  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C
      IMPLICIT INTEGER(A-Z)
C
C
cdsw ------------------------------------------------
      common/signs/nrsig,hliste
cdsw -----------------------------------------------
cdsw  COMMON /SIGNALS/ NRSIG, HLISTE
C
C   NRSIG - OSTATNIO PRZYDZIELONY NUMER SYGNALU
C   HLISTE - OSTATNI ELEMENT LISTY SYGNALOW (W KAZDYM MODULE )
C
C
C
      COMMON  / QUEUE /  BQUEUE, EQUEUE, IFIRST, LAST, EMPTY
cdsw  INTEGER  BQUEUE, EQUEUE
      LOGICAL EMPTY
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C    ZMIENNE SLUZACE DO ORGANIZACJI KOLEJKI PROTOTYPOW
C      BQUEUE  -  POCZATEK POLA W IPMEM PRZEZNACZONEGO NA KOLEJKE
C      EQUEUE  -  KONIEC      --      --       --      --      --
C      IFIRST  -  PIERWSZY ELEMENT KOLEJKI
C      LAST  -  OSTATNI ELEMENT KOLEJKI
C      EMPTY = TRUE, GDY KOLEJKA JEST PUSTA
C
C
      COMMON  /PREFS/  LPREFS
C
C   **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C    LPREFS  -  OSTATNIO PRZYDZIELONY NUMER W PREFIXSET
C
C
      COMMON  / MASKS /  MTP,MSPR,MOTHERS,MPAR,MASKTP,NOTTP,
     * MPROCES, MCOR, MERPF, MBLOCK, MHAND
     *, MNOTVIR
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C   MASKI I WZORCE:
C    MTP - MASKA DO WYCINANIA INFORMACJI DOTYCZACYCH TYPOW ZE SLOWA ZEROWEGO
C    MOTHERS -      --     --      --      --     --  INNYCH PROTOTYPOW
C    MPAR -    --    --    --    --    --    --       ZMIENNYCH I PARAMETROW
C    MSPR - MASKA DLA SYSPREF  ( DLA PROTOTYPOW SYNTAKTYCZNYCH )
C    MASKTP - ZAPRZECZENIE MASKI  MTP
C    NOTTP - WZORZEC DLA NIE-TYPU  ( 1 )
C    MPROCES - WZORZEC DLA PROCESU  ( 5 )
C    MCOR - WZORZEC DLA COROUTINY (7)
C    MERPF - MASKA DO WYKRYWANIA BLEDNYCH LIST PARAMETROW
C    MBLOCK - WZORZEC DLA BLOKU  ( 0 PRZY PRZECIECIU Z  MOTHERS )
C    MNOTVIR - MASKA DO KASOWANIA BITU "VIRTUAL"
C
C
      COMMON  / YNIT /  NULLWD(23), SIZEPR(23), NULLPOZ(23), CONSTWD,
     *  VARWD,VARPOM,INPFW,OUTPFW, INOUT
cdsw  INTEGER  SIZEPR, CONSTWD, VARWD, VARPOM, OUTPFW, INOUT
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C    NULLWD(I)  -  WZORZEc SLOWA ZEROWEGO DLA PROTOTYPU RODZAJU I
C    SIZEPR(I)  -  ROZMIAR POLA W IPMEM   --   --   --
C    NULLPOZ(I)  -  POZYCJA SLOWA ZEROWEGO  --   --    --
C    CONSTWD  -  WZORZEC SLOWA ZEROWEGO DLA    CONST
C    VARWD  -     --   --   --   --          DLA ZMIENNEJ
C    VARPOM  -     --   --   --   --           ZMIENNEJ POMOCNICZEJ
C    INPFW  -     --   --    --    --          ZMIEMNEJ INPUT
C    OUTPFW  -     --   --    --    --         ZMIENNEJ OUTPUT
C    INOUT  -     --   --   --   ---           ZMIENNEJ INOUT
C
C
C ..... ZMIENNE GLOBALNE
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
C             COM    - OBSZAR KOMUNIKACYJNY STRUMIENI
C             LMEM   - (=5000) ROZMIAR CALEJ PAMIECI GLOWNEJ
C             LPMEM  - PODZIAL PAMIECI NA CZESCI  IPMEM  I  ISMEM
C             IRECN  - INDEKS SZCZYTU STOSU STALYCH REAL
C             ISFIN  - INDEKS SZCZYTU SLOWNIKA PROTOTYPOW
C
C             LPMSYS - INDEKS PIERWSZEGO SLOWA W IPMEM OBSZARU NIEPRZEZ-
C                      NACZONEGO NA PROTOTYPY SYSTEMOWE
C             LPML   - INDEKS PIERWSZEGO SLOWA OBSZARU WOLNEGO W IPMEM
C             LPMF   - INDEKS OSTATNIEGO SLOWA WOLNEGO OBSZARU W IPMEM
C
C     IDENTYFIKATORY PROTOTYPOW SYSTEMOWYCH
C             NRINT  - IDENTYFIKATOR PROTOTYPU  INTEGER
C             NRRE   -                          REAL
C             NRBOOL -                          BOOLEAN
C             NRCHR  -                          CHARACTER
C             NRCOR  -                          COROUTINE
C             NRPROC -                          PROCESS
C             NRTEXT -                          STRING (TEXT)
C             NRUNIV - IDENTYFIKATOR PROTOTYPU UNIWERSALNEGO
C             NATTR  - IDENTYFIKATOR PSEUDO-ATRYBUTU (Z NUMEREM -1)
C             NRNONE - IDENTYFIKATOR PROTOTYPU TYPU NONE (UNIWERSALNY
C                      REFERENCYJNY)
C             NBLSYS - IDENTYFIKATOR BLOKU SYSTEMOWEGO
C             NBLUS  -               BLOKU GLOWNEGO UZYTKOWNIKA
C
C             INSYS  - FLAGA SPOSOBU REZERWACJI (PRZEZ  MGETM) PAMIECI
C                      W IPMEM - .TRUE. JESLI REZERWACJA W CZESCI SYSTE-
C                      MOWEJ
C             LOCAL  - FLAGA DOSTEPU DO OBIEKTOW - 2 JESLI OBIEKT
C                   BYL LOKALNY, 1 - JESLI POCHODZIL Z SL, 0 - GDY Z BL. GL.
C             OWN    - FLAGA DOSTEPU DO OBIEKTOW - .TRUE. JESLI OBIEKT NIE
C                      POCHODZI Z PREFIKSOW (TYLKO Z WLASCIWEGO OBIEKTU)
C
C
C
C  INICJALIZACJA ZMIENNYCH Z BLOKOW WSPOLNYCH, KTORE NIE SA
C   INICJALIZOWANE W BLOCK DATA
C
C  COMMON /YNIT/
C    NADANIE APETYTOW DLA TYPOW FORMALNYCH
C    DLA TYPOW FORMALNYCH - BIT 14 JEST ZAPALONY
       MTP = ISHFT (1,14)
       NULLWD(17) = IOR ( NULLWD(17),MTP )
       NULLWD(20) = IOR ( NULLWD(20),MTP )
C   DLA POCEDUR/FUNKCJI FORMALNYCH - BIT 15 JEST ZAPALONY
       MTP = ISHFT (1,15)
       NULLWD(13) = IOR (NULLWD(13),MTP )
       NULLWD(14) = IOR (NULLWD(14),MTP )
       NULLWD(15) = IOR ( NULLWD(15),MTP )
       NULLWD(16) = IOR ( NULLWD(16),MTP )
       NULLWD(18) = IOR ( NULLWD(18),MTP )
       NULLWD(19) = IOR ( NULLWD(19),MTP )
C
C  COMMON / QUEUE /
      BQUEUE=MGETM(LMEM/50,341)
      EQUEUE=LPML-1
      EMPTY=.TRUE.
      LAST=BQUEUE-2
      IFIRST=BQUEUE
C
C   COMMON / PREFS /
cdsw      LPREFS=IPMEM(NBLSYS+3)
cdsw       MAXPF=47
C
C   COMMON / MASKS /
       MTP=15
      MSPR=7
      MOTHERS=ISHFT(7,8)
      MPAR=ISHFT(15,4)
       MASKTP=NOT(MTP)
       MNOTVIR=ISHFT(1,11)
       MNOTVIR=NOT(MNOTVIR)
      MHAND = ISHFT ( 7,8 )
      MERPF = ISHFT (1,13)
C
C   COMMON /SIGNALS/
      NRSIG = 100
C
C   INICJALIZACJA ZMIENNYCH GLOBALNYCH - CHWILOWA
      INSYS=.FALSE.
      RETURN
      END
      SUBROUTINE PROTP1
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C   PODPROGRAM SLUZY DO PRZETWARZANIA(OSTATECZNEGO) PROTOTYPU,
C    RAZEM Z PRZETWARZANIEM ZEWNETRZNYM ( WSTEPNYM) JEGO ATRYBUTOW LOKALNYCH.
C   PROTOTYP JEST ZADANY PRZEZ ZMIENNE Z BLOKU /DGLOB/
C   PIERWSZA CZESC PRZETWARZANIA
C  **  **  **  **  **  **  **  **  **  ** *  **  **  **  **  **  **  **
C
       IMPLICIT INTEGER (A-Z)
C     INSERTION OF
      LOGICAL BTEST
C     BECAUSE OF TYPECONFLICT 03.01.84
C
      COMMON / DGLOB/ INDICT,INDSPR,INDPR,IHBEG,LASTPR,INDPREF
C
C  **  **  **  **  ***  **  **  **  **  **  **  **  **  **  **  **  **
C    BLOK ZAWIERA DANE O PRZETWARZANYM PROTOTYPIE
C        INDICT  -  INDEKS PRZETWARZANEGO PROTTYPU W ISDICT
C        INDSPR  -  IDENTYFIKATOR PROTOTYPU W ISMEM
C        INDPR  -  IDENTYFIKATOR PROTOTYPU W IPMEM
C        IHBEG  -  ADRES PIERWSZEGO SLOWA TABLICY HASHU
C        IDPREF  -  IDENTYFIKATOR BEZPOSREDNIEGO PREFIKSU PRZETWARZANEGO
C                  PROTOTYPU
C        LASTPR  -  IDENTYFIKATOR POPRZEDNIO PRZETWARZANEGO PROTOTYPU
C
C
C ..... ZMIENNE GLOBALNE
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
C
C
C
      COMMON  / YNIT /  NULLWD(23), SIZEPR(23), NULLPOZ(23), CONSTWD,
     *  VARWD,VARPOM,INPFW,OUTPFW, INOUT
cdsw  INTEGER  SIZEPR, CONSTWD, VARWD, VARPOM, OUTPFW
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C    NULLWD(I)  -  WZORZEC SLOWA ZEROWEGO DLA PROTOTYPU RODZAJU I
C    SIZEPR(I)  -  ROZMIAR POLA W IPMEM   --   --   --
C    NULLPOZ(I)  -  POZYCJA SLOWA ZEROWEGO  --   --    --
C    CONSTWD  -  WZORZEC SLOWA ZEROWEGO DLA    CONST
C    VARWD  -     --   --   --   --          DLA ZMIENNEJ
C    VARPOM  -     --   --   --   --           ZMIENNEJ POMOCNICZEJ
C    INPFW  -     --   --    --    --          ZMIEMNEJ INPUT
C    OUTPFW  -     --   --    --    --         ZMIENNEJ OUTPUT
C    INOUT   -     --   --    --   --          ZMIENNEJ INOUT
C
C
      COMMON  / MASKS /  MTP,MSPR,MOTHERS,MPAR,MASKTP,NOTTP,
     * MPROCES, MCOR, MERPF, MBLOCK, MHAND, MNOTVIRT
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C   MASKI I WZORCE:
C    MTP - MASKA DO WYCINANIA INFORMACJI DOTYCZACYCH TYPOW ZE SLOWA ZEROWEGO
C    MOTHERS -      --     --      --      --     --  INNYCH PROTOTYPOW
C    MPAR -    --    --    --    --    --    --       ZMIENNYCH I PARAMETROW
C    MSPR - MASKA DLA SYSPREF  ( DLA PROTOTYPOW SYNTAKTYCZNYCH )
C    MASKTP - ZAPRZECZENIE MASKI  MTP
C    NOTTP - WZORZEC DLA NIE-TYPU  ( 1 )
C    MPROCES - WZORZEC DLA PROCESU  ( 5 )
C    MCOR - WZORZEC DLA COROUTINY (7)
C    MERPF - MASKA DO WYKRYWANIA BLEDNYCH LIST PARAMETROW
C    MBLOCK - WZORZEC DLA BLOKU  ( 0 PRZY PRZECIECIU Z  MOTHERS )
C
C
C
C.....BLOK KOMUNIKACJI ZE STRUMIENIAMI
      LOGICAL  ERRFLG
      COMMON /STREAM/  ERRFLG, LINE, IBUF2(265), IBUF3(7),JUNK(260)
C
C
C
C   BUFORY
C
cdeb      COMMON /NAMES/ RESNM, MAINM
cdeb ----------------------------------
      common /names/ resnm, mainm,brenam
cdeb --------------------------------
C
C   NAZWY ZE SCANNERA
C
C
C
C
cbc
      common /option/ opt
cbc
      LOGICAL POM, HAND
C    HAND - CZY TO JEST PROTOTYP HANDLERA
C
C
C   NADANIE WARTOSCI POZOSTALYM ZMIENNYM Z BLOKU /DGLOB/
      INDSPR = IPMEM(INDICT)
      IHBEG = INDPR+10
       I=IPMEM(INDPR)
       HAND = .FALSE.
       IF ( IAND(I,MOTHERS) .EQ. MHAND ) HAND = .TRUE.
       INDPREF = IPMEM(INDPR+21)
C  JESLI BLOK LUB HANDLER - TO ZERO
       IF(IAND(I,MTP).EQ.NOTTP.AND.IAND(I,MOTHERS).EQ.MBLOCK.OR.HAND)
     * INDPREF = 0
C   .  .  .  .
C   PRZEPISANIE INFORMACJI DLA ANDRZEJA
      IPMEM(INDPR+8)=IPMEM(INDSPR-3)
      IPMEM(INDPR+9)=IPMEM(INDSPR-2)
      IPMEM(INDPR+18)=IPMEM(INDSPR-1)
C   EWENTUALNE POPRAWIENIE SPECYFIKACJI - JESLI PREFIKS
C    NIE JEST REKORDEM
      IF(INDPREF.EQ.0) GO TO 50
      IF(BTEST(IPMEM(INDPREF),0)) CALL CHECK(INDPR)
      GO TO 60
C    JESLI JEST LISTA TAKEN - TO BLAD
 50   I=IPMEM(INDSPR+7)
      IF(I.EQ.0) GO TO 60
      LINE=IPMEM(I+1)
      CALL MERR(310,NEMPTY)
 60   INSYS=.FALSE.
      IF(HAND) GO TO 70
C
C---- ---------------------------------------------------------------
C     PRZETWARZANIE NAGLOWKA
      CALL HEADER
C
C  ----------------------------------------------------------------
C   DLA PROCESU - SPRAWDZENIE, CZY NIE MA PF OUTPUT LUB INOUT
C
cbc check if parameters fit into one message
c
      I = IAND(IPMEM(INDPR),MTP)
      IF(I.NE.MPROCES) GO TO 70
C   PROCES
      I = IPMEM(INDPR+3)
      K = IPMEM(INDPR+4)
      IF(K.EQ.0) GO TO 76
      K = K+I-1
cbc check if first parameter is integer
      j = ipmem(indpr+22)
80    p = ipmem(j)
      tp = iand(ipmem(p), mtp)
      if (tp .eq. mproces) goto 81
      j = j+1
      goto 80
c p = address of first process prototype in prefix sequence
81    l = ipmem(p+4)
      p = ipmem(p+21)
      if (p .eq. 0) goto 82
      if (l .eq. ipmem(p+4)) goto 76
      i = i+ipmem(p+4)
82    continue
c i = address of first parameter of process
      if (ipmem(ipmem(i)-3) .eq. nrint) goto 75
      line = ipmem(indspr+9)
      call merr(370, nempty)
75    continue
      i = ipmem(indpr+3)
C
cbc
      apet = 0
cbc
      DO 77 J = I,K
      NM = IPMEM(J)
C   NM - IDENTYFIKATOR PARAMETRU
CBC
c   check for formal type
      zp = ishft(iand(ipmem(nm), mpar), -4)
      if (zp .eq. 1) goto 74
c   check for formal procedure
      if (zp .eq. 3) goto 73
c   check if not array
      if (ipmem(nm-4) .gt. 0) goto 74
      tp = iand(ipmem(ipmem(nm-3)), mtp)
c   check if formal parameter type is process or int,real,char,bool,string
cpat      if (tp .eq. mproces .or. tp .eq. 8 .or. tp .eq. 10 .or.
cpat     *    tp .eq. 12) goto 78
      goto 78
74    line = ipmem(indspr+9)
      call merr(370 ,nempty)
      goto 77
78    continue
c   compute formal parameter appetite in bytes
      if (zp .eq. 2) goto 73
c   variable
      ap = sapet(0, ipmem(nm-3))
      if (ap .eq. 4) ap = 2
      goto 72
c   formal procedure or function
73    ap = 3
c   sum up appetites
72    apet = apet + ap
cbc      
c      NM = ISHFT(IAND(IPMEM(NM),MPAR),-4)
c      IF(NM.NE.6.AND.NM.NE.9) GO TO 77
c    BLAD - JEST PARAMETR OUTPUT LUB INOUT
c      LINE = IPMEM(INDSPR+9)
c      CALL MERR(370,NEMPTY)
cbc
c
  77  CONTINUE
cbc
      if (.not. btest(opt, 12)) maxap = 34
      if (      btest(opt, 12)) maxap = 15
      if (apet .le. maxap) goto 70
      line = ipmem(indspr+9)
      call merr(370, nempty)
      goto 70
  76  line = ipmem(indspr+9)
      call merr(370, nempty)
c
Cbc
C -----------------------------------------------------------------
C      PRZETWARZANIE WSTEPNE LOKALNYCH PROTOTYPOW -- ETAP I
C
C   POM = FALSE, GDY PRZETWARZAMY TYPY
C   POM = TRUE, GDY PRZETWARZAMY PROCEDURY/FUNKCJE/BLOKI/HANDLERY
 70   POM=.FALSE.
      I=IPMEM(INDSPR+5)
 100  IF(I.EQ.0) GO TO 200
      J=IPMEM(I)
      J=IPMEM(J)
C   J - IDENTYFIKATOR PROTOTYPU LOKALNEGO W ISMEM
C   NM - NAZWA PROTOTYPU
      NM=NEMPTY
      IF(IPMEM(J).NE.1 .AND. IPMEM(J).NE.8) NM=IPMEM(J+10)
C   OKRESLENIE RODZAJU PROTOTYPU ( W SENSIE BLOKU INIT)
      K=IPMEM(J)
C   WYKRYCIE HANDLERA
      IF(K.EQ.8) K=K+15
C      WYKRYCIE BLOKU PREFIKSOWANEGO
      IF(K.EQ.1.AND.IPMEM(J+2).NE.0) K=8
C   WYKRYCIE WIRTUALI
      LINE = IPMEM(J+9)
C        INSERTION OF LAST STATEMENT DUE TO CORRECTION GIVEN TO ME IN WARSAW
C     IF(BTEST(IPMEM(J+8),15)) K=K+6
C   INSERTION OF THE FOLLOWING STATEMENTS DUE TO CORRECTIONS (SEE ABOVE)
      IF (.NOT. BTEST(IPMEM(J+8),15) ) GOTO 110
      IF (K.LT.3.OR.K.GT.6) GOTO 120
      K=K+6
      IF (INDPREF.NE.0) GOTO 110
      IF (IAND(IPMEM(INDPR),MTP).NE.NOTTP) GOTO 110
      CALL MERR(330,NM)
      K=K-6
      GOTO 110
120   CALL MERR(329,NM)
C      END OF INSERTION OF STATEMENTS
C   WYWOLANIE INITPR
C     LINE=IPMEM(J+9)
C     DELETION OF PREVIOUS STATEMENT DUE TO CORRECTION
C     K=INITPR(K,NM)
110   K=INITPR(K,NM)
C     INSERTION OF LABEL 110 DUE TO CORRECTIONS
C   W POLU SL PROTOTYPU (W ISMEM) ZAPAMIETUJEMY JEGO IDENTYFIKATOR
C   W IPMEM
      IPMEM(J+1) = K
C   PRZEJSCIE DO NASTEPNEGO PROTOTYPU W LISCIE
      I=IPMEM(I+1)
      GO TO 100
C   PRZETWARZANIE BLOKOW, FUNKCJI I PROCEDUR
 200  IF(POM) GO TO 300
      POM=.TRUE.
      I=IPMEM(INDSPR+6)
C  JESLI PROTOTYP ZAWIERA BLOKI, FUNKCJE LUB PROCEDURY, TO ZMIENIAMY
C   MU KWALIFIKACJE NA PELNA KLASE
      IF(I.NE.0) CALL CHECK(INDPR)
      GO TO 100
C
C
 300  CONTINUE
C
C  ------------------------------------------------------------------------
C    PRZETWARZANIE LISTY SYGNALOW
C
      I = IPMEM(INDSPR-4)
 350  IF (I.EQ.0) GO TO 500
      NM = IPMEM(I+2)
      LINE = IPMEM(I+1)
      K = IPMEM(I) +12
C   UTWORZENIE PROTOTYPU
      K=INITPR(K,NM)
C   ZAPAMIETANIE IDENTYFIKATORA PROTOTYPU SEMANTYCZNEGO
      IPMEM(I) = K
      I=IPMEM(I+3)
      GO TO 350
C
C
 500  CONTINUE
C
C    JESLI HANDLER TO KONIEC
      IF ( HAND ) GO TO 1100
C  ---  ---  ---  ---  ---  ---  ---  ---  ---  ---  ---  ---   ----  -----
C      PRZETWARZANIE LISTY STALYCH
C
      I=IPMEM(INDSPR+4)
 600  IF(I.EQ.0) GO TO 700
      J=MGETM(6,41)+4
C   J - IDENTYFIKATOR OPISU STALEJ
      IPMEM(J)=CONSTWD
C  THIS AND THE NEXT 3 LINES ARE IRRELEVANT            8.5.84
C   SZUKAMY TYPU STALEJ - ZAKLADAMY POPRAWNOSC TYPU
C       ZERO OZNACZA STALA ZDEFINIOWANA PRZEZ WYRAZENIE
      IF(IPMEM(I+2).EQ.0)GO TO 630
C
C     TYLKO DLA STALEJ TEKSTOWEJ :
      K=MEMBER(IPMEM(I+2),IPMEM(NBLSYS+10))
C LAST STATEMENT CHANGED TO COMMENT DUE TO CORRECTIONS FROM WARSAW   8.5.84
C THIS AND THE NEXT 2 LINES ARE IRRELEVANT 8.5.84
      IPMEM(J-3)=IPMEM(K+2)
C LAST STATEMENT CHANGED TO COMMENT DUE TO CORRECTIONS FROM WARSAW  8.5.84
C   PRZEPISANIE ADRESU STALEJ
 630  IPMEM(J-1)=IPMEM(I+4)
C   WSTAWIENIE STALEJ DO ZBIORU IDENTYFIKATOROW
      LINE=IPMEM(I+1)
      K=IDPUT(IPMEM(I),IPMEM(IHBEG))
      IF(K.EQ.0) GO TO 650
C   NIE MA PODWOJNEJ DEKLARACJI
      IPMEM(K+2) = J
C   USTAWINIE BITU CLOSE
      IPMEM(K+1) = 1
C   PRZEJSCIE DO NASTEPNEGO ELEMENTU LISTY
 650   I=IPMEM(I+5)
      GO TO 600
C
C
 700  CONTINUE
C
C  ----  ----  ----  ----  ----  ----  ----  ----  ----  ----   ----  ----
C    PRZETWARZANIE LISTY ZMIENNYCH
C
C
      I=IPMEM(INDSPR+3)
 800  IF(I.EQ.0) GO TO 1000
      J=MGETM(6,41)+4
C   J- IDENTYFIKATOR OPISU ZMIENNEJ
      IPMEM(J) = VARWD
C   WSTAWIENIE DO ZBIORU IDENTYFIKATOROW
      LINE=IPMEM(I+1)
      K=IDPUT(IPMEM(I),IPMEM(IHBEG))
      IF(K.EQ.0) GO TO 900
C   NIE MA PODWOJNEJ DEKLARACJI
      IPMEM(K+2)=J
C   WSTAWIENIE DO LISTY ATRYBUTOW
 900  CALL MADATR(J,INDPR,41)
C   ZAPAMIETANIE 1 W POLU USED - DLA AIL
      IPMEM(J+1)=1
C   ZAMIAST NAZWY ZMIENNEJ(W ISMEM) ZAPAMIETUJEMY JEJ IDENTYFIKATOR W IPMEM
      IPMEM(I)=J
C   PRZEJSCIE DO NASTEPNEGO ELEMENTU LISTY
      I=IPMEM(I+4)
      GO TO 800
C
C
 1000 CONTINUE
C
C
C  DOLACZENIE DO ZBIORU IDENTYFIKATOROW ID. RESULT DLA FUNKCJI
      I=ISHFT(IAND(IPMEM(INDPR),MOTHERS),-8)
      IF(I.LT.2.OR.I.GT.3) GO TO 1050
      J=MEMBER(RESNM,IPMEM(IHBEG))
      IF(J.NE.0) GO TO 1050
      J=INSERT(RESNM,IPMEM(IHBEG),41)
      IPMEM(J+2)= IPMEM(INDPR-5)
 1050 CONTINUE
C
C
C---- ---- ---- ----- ---- ----- ----- ---- ----- ----- ----- ----- --
C   TWORZENIE ZBIORU IDENTYFIKATOROW
C
C   JESLI JEST PREFIKS, TO UZUPELNIAMY ZBIOR IDENTYFIKATOROW
      IF(INDPREF.EQ.0) GO TO 1100
      CALL MERGEID
C
C
 1100 CONTINUE
      RETURN
      END
      SUBROUTINE PROTP2
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C   PODPROGRAM SLUZY DO PRZETWARZANIA(OSTATECZNEGO) PROTOTYPU,
C    RAZEM Z PRZETWARZANIEM ZEWNETRZNYM ( WSTEPNYM) JEGO ATRYBUTOW LOKALNYCH.
C   PROTOTYP JEST ZADANY PRZEZ ZMIENNE Z BLOKU /DGLOB/
C   DRUGA CZESC PRZETWARZANIA
C  **  **  **  **  **  **  **  **  **  ** *  **  **  **  **  **  **  **
C
       IMPLICIT INTEGER (A-Z)
      LOGICAL POM
C     INSERTION OF
      LOGICAL BTEST
C     BECAUSE OF TYPECONFLICT 03.01.84
C
      COMMON / DGLOB/ INDICT,INDSPR,INDPR,IHBEG,LASTPR,INDPREF
C
C  **  **  **  **  ***  **  **  **  **  **  **  **  **  **  **  **  **
C    BLOK ZAWIERA DANE O PRZETWARZANYM PROTOTYPIE
C        INDICT  -  INDEKS PRZETWARZANEGO PROTTYPU W ISDICT
C        INDSPR  -  IDENTYFIKATOR PROTOTYPU W ISMEM
C        INDPR  -  IDENTYFIKATOR PROTOTYPU W IPMEM
C        IHBEG  -  ADRES PIERWSZEGO SLOWA TABLICY HASHU
C        IDPREF  -  IDENTYFIKATOR BEZPOSREDNIEGO PREFIKSU PRZETWARZANEGO
C                  PROTOTYPU
C        LASTPR  -  IDENTYFIKATOR POPRZEDNIO PRZETWARZANEGO PROTOTYPU
C
C
C
C
      COMMON  / VIRT /  LISTVB,LISTVE,OWNVIR
      LOGICAL OWNVIR
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C    ROBOCZY BLOK WSPOLNY.
C    LISTVB  -  POCZATEK ROBOCZEJ LISTY VIRTLIST
C    LISTVE  -  KONIEC ROBOCZEJ LISTY  VIRTLIST
C    OWNVIR = TRUE, JESLI W PROTOTYPIE BYLY WLASNE VIRTUALE
C
C ..... ZMIENNE GLOBALNE
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
C
C
C.....
C     KOMUNIKACJA Z PROCEDURA MEMPRF
      COMMON /MEM/  NME, NH
C            NME - SZUKANA NAZWA
C            NH - JEJ HASZ
C
C
cdsw  COMMON /SIGNALS/ NRSIG, HLISTE
cdsw     -------------------------------------------------
      common /signs/ nrsig, hliste
cdsw     -------------------------------------------------
C
C   NRSIG - OSTATNIO PRZYDZIELONY NUMER SYGNALU
C   HLISTE - OSTATNI ELEMENT LISTY SYGNALOW (W KAZDYM MODULE )
C
C
C
      COMMON  / MASKS /  MTP,MSPR,MOTHERS,MPAR,MASKTP,NOTTP,
     * MPROCES, MCOR, MERPF, MBLOCK, MHAND, MNOTVIRT
C
C
C.....BLOK KOMUNIKACJI ZE STRUMIENIAMI
      LOGICAL  ERRFLG
      COMMON /STREAM/  ERRFLG, LINE, IBUF2(265), IBUF3(7) ,JUNK(260)
C
C
C
C  ----  ------  ------  ------  -------  -------  -------  ------------
C
C   WSTEPNE PRZETWARZANIE PROTOTYPOW LOKALNYCH  --  ETAP  II
C
C
C    -----   ------   ------  -----  -----  -----  -----  -----  -----  -----
C    PRZETWARZANIE SYGNALOW
C
      LN = 0
      I=IPMEM(INDSPR-4)
 100  IF ( I.EQ.0 ) GO TO 500
      CALL SIGNAL(I)
      I = IPMEM(I+3)
      GO TO 100
C
C
 500  CONTINUE
C
C   ------    ------    ------    ------    ------    -------    ------  ----
C   PRZEPISANIE LISTY VIRTLIST Z PREFIKSU
C
       LISTVE=0
      LISTVB=LPML
      IF(INDPREF.EQ.0) GO TO 1200
      I=IPMEM(INDPREF+25)
C   I - DLUGOSC LISTY WIRTLIST Z PREFIKSU
      IF(I.EQ.0) GO TO 1200
      IF(I.LT.0) I=-I
C   JEST LISTY WIRTLIST W PREFIKSIE
      INSYS=.TRUE.
      LISTVB=MGETM(I,41)
      LISTVE=LPML-1
      INSYS=.FALSE.
C   PRZEPISYWANIE
      K=IPMEM(INDPREF+24)
C   K - POCZATEK LISTY VIRTLIST W PREFIKSIE
      DO 1111 J=1,I
      IJ1 = LISTVB+J-1
      IJ2 = K+J-1
 1111 IPMEM(IJ1) = IPMEM(IJ2)
 1200 OWNVIR=.FALSE.
C   PUSTA LISTA LOKALNYCH HANDLEROW
       HLISTE = 0
C   PRZETWARZANIE WSZYSTKICH PROTOTYP6W
       POM=.FALSE.
      I=IPMEM(INDSPR+5)
C   ZACZYNAMY OD TYPOW
 1300 IF(I.EQ.0) GO TO 1400
      CALL BEGPROT(IPMEM(I))
      I=IPMEM(I+1)
      GO TO 1300
 1400 IF(POM) GO TO 1600
      POM=.TRUE.
C   PROCEDURY, FUNKCJE I BLOKI
      I=IPMEM(INDSPR+6)
	GO TO 1300
C
1600  CONTINUE
C   JESLI HANDLER TO KONIEC
      IF ( IAND(IPMEM(INDPR),MOTHERS).EQ.MHAND) GO TO 4000
C
C   UTUPELNIENIE INFORMACJI I WIRTUALACH
      IF(OWNVIR) GO TO 1700
C   NIE BYLO WLASNYCH WIRTUALI
      IF(INDPREF.EQ.0) GO TO 1900
C   DOWIAZUJEMY SIE DO LISTY VIRTLIST Z PREFIKSU, ZMIENIAJAC DLUGOSC NA UJEMNA
      IPMEM(INDPR+24) = IPMEM(INDPREF+24)
      IPMEM(INDPR+25) = -IPMEM(INDPREF+25)
      GO TO 1900
C   BYLY WLSNE WIRTUALE - PRZEPISUJEMY LISTE VIRTLIST
 1700 I=LISTVE-LISTVB+1
      K = MGETM(I,41)
      IPMEM(INDPR+24) = K
      DO 1777 J=1,I
      IJ1=K+J-1
      IJ2=LISTVB+J-1
 1777 IPMEM(IJ1) = IPMEM(IJ2)
      IPMEM(INDPR+25) = I
C   ZWALNIAMY PAMIEC PRZEZNACZONA NA VIRTLIST W CZESCI SYSTEMOWEJ
 1900 CONTINUE
       LPML=LISTVB
C
C
 2000 CONTINUE
C
C  ----  -----   -----  --------   ----------   -------  ----  -----   -------
C        NADAWANIE TYPOW ZMIENNYM
C
      I=IPMEM(INDSPR+3)
      IF(I.EQ.0) GO TO 2400
 2100 J=IPMEM(I)
C  J - IDENTYFIKATOR OPISU ZMIENNEJ W IPMEM
      NM=IPMEM(I+2)
C   NM - NAZWA TYPU
      LINE=IPMEM(I+1)
       K=IFTYPE(NM)
      IPMEM(J-3) = K
      IPMEM(J-4) = IPMEM(I+3)
C   NADANIE APETYTU ZMIENNEJ
      IPMEM(J) = IAP(J)
C   JESLI TO JEST TYP FORMALNY, TO POPRAWIAMY SLOWO ZEROWE OPISU ZMIENNEJ
      IF(IAND(IPMEM(K),MTP).NE.6) GO TO 2350
      IPMEM(J) = IOR(IPMEM(J),ISHFT(1,12))
C    JESLI TYP FORMALNY JEST NIELOKALNY, TO ZMIENIAMY RODZAJ PROTOTYPU
C    NA KLASE PELNA
      IF(LOCAL.EQ.2) GO TO 2350
      CALL CHECK(INDPR)
 2350  I=IPMEM(I+4)
      IF(I.NE.0) GO TO 2100
C
C
 2400 CONTINUE
C
C  ----  ------  -----  ---  ----  ------  -----   ------  ----------
C      SPRAWDZENIE POPRAWNOSCI LIST HIDDEN I CLOSE I UZUPELNIENIE INFORMACJI
C
C   JESLI PROTOTYP NIE JEST KLASA TO PRZECHODZIMY DALEJ
      IF(IPMEM(INDSPR).NE.2.AND.IPMEM(INDSPR).NE.7) GO TO 3000
      I=IPMEM(INDSPR+12)
C   K = 0 -- CLOSE, K=1 -- HIDDEN
       K=1
 2500 IF(I.EQ.0) GO TO 2700
      NME=IPMEM(I)
      LN=LINE
      LINE=IPMEM(I+1)
C  NME - NAZWA W LISCIE HIDDEN(CLOSE)
      NH=IAND(ISHFT(NME,-1),7)+1
      J=MEMPRF(INDPR)
      IF(J.EQ.0) GO TO 2600
C  NAZWA JEST ZADEKLAROWANA
C   JESLI NAZWA JEST HIDDEN LUB NOT TAKEN, TO BLAD
      NM=IPMEM(J+1)
      IF(BTEST(NM,2)) GO TO 2660
      IF(OWN) GO TO 2550
      IF(BTEST(NM,1)) GO TO 2650
C   NAZWA POCHODZI Z PREFIKSU
      NM=INSERT(NME,IPMEM(IHBEG),41)
      IPMEM(NM+2) = IPMEM(J+2)
      IPMEM(NM+1)= IPMEM(J+1)
      J=NM
C   USTAWIAMY BIT K W ELEMENCIE LISTY HASHU
 2550 IPMEM(J+1) = IOR(IPMEM(J+1),ISHFT(1,K))
C   PRZECHODZIMY DO NASTEPNEGO ELEMENTU LISTY
 2560 I=IPMEM(I+2)
      GO TO 2500
C   NAZWA NIEZADEKLAROWANA
 2600 J=INSERT(NME,IPMEM(IHBEG),41)
      CALL MERR(305,NME)
      GO TO 2550
C   NAZWA HIDDEN - NIEDOSTEPNA
 2650 CALL MERR(319,NME)
      GO TO 2560
C   NAZWA NOT TAKEN - NIEDOSTEPNA
 2660 CALL MERR(320,NME)
      GO TO 2560
 2700 IF(K.EQ.0) GO TO 3000
      K=0
      I=IPMEM(INDSPR+13)
      GO TO 2500
C
C
 3000 CONTINUE
C
C----   ------   ------  ------  -------  -------  --------  -------
C    KOMPATYBILNOSC VIRTUALI
C
      IF(.NOT.BTEST(IPMEM(INDPR),11)) GO TO 4000
C   PROTOTYP JEST VIRTUALEM
C   SPRAWDZAMY, CZY ISTNIEJE WYZSZY VIRTUAL
      LINE=LN
      IF(IPMEM(INDPR+26).EQ.0) GO TO 4000
      CALL VIRTCOM
C
C
 4000 CONTINUE
C
C  ------  ------  ------   ------  ------  ------  -------------  ---
C     ZAKONCZENIE - ZAPAMIETUJEMY IDENTYFIKATOR PROTOTYPU W SLOWNIKU
C    ISDICT
C
      IPMEM(INDICT) = INDPR
C
      RETURN
      END
      INTEGER FUNCTION IAP(IND)
C
C   WYLICZA APETYT ATRYBUTU IND :
C           0  (00) - INTEGER,BOOLEAN,CHAR,STRING
C           1  (01) - REAL, FORMAL TYPE
C           2  (10) - FORMAL PROCEDURE,FORMAL FUNCTION
C           3  (11) - REFERENCE
C   W WYNIKU ZMIENIA SLOWO ZEROWE
C
      IMPLICIT INTEGER(A-Z)
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
C
      COMMON  / MASKS /  MTP,MSPR,MOTHERS,MPAR,MASKTP,NOTTP,
     * MPROCES, MCOR, MERPF, MBLOCK, MHAND, MNOTVIRT
C
C
      J=IPMEM(IND-3)
C   J - IDENT TYPU
      I=IAND(IPMEM(J),MTP)
      IAP=0
      IF(I.LT.8.OR.I.EQ.11) IAP = 3
      IF(J.EQ.NRRE) IAP=1
      IF(IPMEM(IND-4).NE.0)  IAP=3
C   ZMIANA SLOWA ZEROWEGO
      IAP=IOR(IPMEM(IND),ISHFT(IAP,14))
      RETURN
      END
      SUBROUTINE BEGPROT(NRSDIC)
C
C   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *
C   PODPROGRAM SLUZY DO WSTEPNEGO PRZETWARZANIA PROTOTYPOW LOKALNYCH
C   W PROTOTYPIE IDENTYFIKOWANYM PRZEZ  INDPR.
C   DLA KAZDEGO PROTOTYPU:
C        - ANALIZUJE JEGO PREFIKS
C        - UZUPELNIA INFORMACJE O RODZAJU PROTOTYPU
C        - JESLI PROTOTYP JEST PROCEDURA LUB FUNKCJA VIRTUALNA, TO SZUKA
C          BEZPOSREDNIO WYZSZEGO VIRTUALA I WSTAWIA IDENTYFIKATOR PROTOTYPU
C          DO LISTY VIRTLIST PROTOTYPU OBEJMUJACEGO.
C        - DLA FUNKCJI  --  ZNAJDUJE JEJ TYP.
C   PODPROGRAM WSTAWIA PARE  ( NRSDIC, IDENT. W IPMEM)  DO KOLEJKI
C    PROTOTYPOW.
C   NRSDIC - INDEKS PROTOTYPU W IPMEM.
C   W  IPMEM  W POLU  SL  ZAPAMIETANY JEST IDENTYFIKATOR TEGO PROTOTYPU W
C      IPMEM.
C   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *
C
C
      IMPLICIT INTEGER(A-Z)
C
C
      LOGICAL IFCLASS,BPREF,ONLY
C
C     INSERTION OF
      LOGICAL BTEST
C     BECAUSE OF TYPECONFLICT 03.01.84
C
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
C
      COMMON / DGLOB/ INDICT,INDSPR,INDPR,IHBEG,LASTPR,INDPREF
C
C  **  **  **  **  ***  **  **  **  **  **  **  **  **  **  **  **  **
C    BLOK ZAWIERA DANE O PRZETWARZANYM PROTOTYPIE
C        INDICT  -  INDEKS PRZETWARZANEGO PROTTYPU W ISDICT
C        INDSPR  -  IDENTYFIKATOR PROTOTYPU W ISMEM
C        INDPR  -  IDENTYFIKATOR PROTOTYPU W IPMEM
C        IHBEG  -  ADRES PIERWSZEGO SLOWA TABLICY HASHU
C        IDPREF  -  IDENTYFIKATOR BEZPOSREDNIEGO PREFIKSU PRZETWARZANEGO
C                  PROTOTYPU
C        LASTPR  -  IDENTYFIKATOR POPRZEDNIO PRZETWARZANEGO PROTOTYPU
C
C
      COMMON  / VIRT /  LISTVB,LISTVE,OWNVIR
      LOGICAL OWNVIR
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C    ROBOCZY BLOK WSPOLNY.
C    LISTVB  -  POCZATEK ROBOCZEJ LISTY VIRTLIST
C    LISTVE  -  KONIEC ROBOCZEJ LISTY  VIRTLIST
C    OWNVIR = TRUE, JESLI W PROTOTYPIE BYLY WLASNE VIRTUALE
C
C
      COMMON  / MASKS /  MTP,MSPR,MOTHERS,MPAR,MASKTP,NOTTP,
     * MPROCES, MCOR, MERPF, MBLOCK, MHAND
     *, MNOTVIR
C
C
      COMMON  /PREFS/  LPREFS
C
C   **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C    LPREFS  -  OSTATNIO PRZYDZIELONY NUMER W PREFIXSET
C
C
C.....BLOK KOMUNIKACJI ZE STRUMIENIAMI
      LOGICAL  ERRFLG
      COMMON /STREAM/  ERRFLG, LINE, IBUF2(265), IBUF3(7),JUNK(260)
C
C
      COMMON / DONLY / IONLY,ONIL
      LOGICAL ONIL
C
C  ROBOCZY BLOK,UZYWANY PRZY TWORZENIU ZBIORU IDENTYFIKAROROW
C   IONLY  - POCZATEK LISTY TAKEN
C   ONIL - TRUE, GDY JEST TAKEN NONE
C
C
C
C
C   *   *   *   *   *   *   *   *   *   *   *   *   *
C   NADANIE WARTOSCI ZMIENNYM.
C   IDSMEM - IDENTYFIKATOR W IPMEM PRZETWARZANEGO PROTOTYPU.
C   IDPMEM - IDENTYFIKATOR PROTOTYPU W IPMEM.
C   ISYS = 1  DLA COROUTINE, ISYS = 2 DLA PROCESS.
C  LINE - NR LINII DEKLARACJI PROTOTYPU.
C   *   *   *   *   *   *   *   *   *   *   *   *   *
C
C
      IDSMEM = IPMEM(NRSDIC)
      IDPMEM = IPMEM(IDSMEM+1)
C   JESLI HANDLER - TO DO PRZETWARZANIE HANDLERA
      IF(IPMEM(IDSMEM).EQ.8) GO TO 2500
      ISYS=IAND(IPMEM(IDSMEM+8),MSPR)
      LINE = IPMEM(IDSMEM+9)
C
C
C   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *
C    PRZETWARZANIE PREFIKSU
C   *   *   *   *   *   *   *   *   *   *   *   *   *   *
C
C   I - NAZWA PREFIKSU
      I = IPMEM(IDSMEM+2)
C        IDPR - BEDZIE IDENTYFIKATOREM PREFIKSU
      IDPR = 0
      IF( I.EQ.0) GO TO 500
C         JEST PREFIKS
      IDPR = MEMSL(I,INDPR)
      IF(IDPR.NE.0) GO TO 50
C        PREFIKS JEST NIEZADEKLAROWANY
      CALL MERR(308,I)
      GO TO 500
C   BADAMY, CZY PREFIKS JEST DOSTEPNY
 50   IRODZ=IPMEM(IDPR+1)
      IF(BTEST(IRODZ,2)) GO TO 60
      IF(.NOT.BTEST(IRODZ,1)  .OR.OWN)GO TO 100
C   NAZWA PROFIKSU JEST HIDDEN - BLAD
      CALL MERR(322,I)
      GO TO 150
C   NAZWA PREFIKSU JEST NOT-TAKEN
 60   CALL MERR(323,I)
      GO TO 150
C        BADAMY, CZY PREFIKS JEST KLASA
 100  IDPR = IPMEM(IDPR+2)
      IF(IDPR.EQ.NRUNIV) GO TO 150
C        IRODZ - CZESC T W SLOWIE ZEROWYM PREFIKSU
      IRODZ=IAND(IPMEM(IDPR),MTP)
      IF(IFCLASS(IRODZ)) GO TO 200
C        PREFIKS NIE JEST KLASA
      CALL MERR(301,I)
 150  IDPR=0
      GO TO 210
C
C   PREFIKS JEST POPRAWNY
 200  CONTINUE
C   JESLI PREFIKS MIAL BLEDNA LISTE PF, TO POPRAWIAMY
C    SLOWO ZEROWE
      IF(IAND(IPMEM(IDPR),MERPF).EQ.0) GO TO 250
C   POPRAWIAMY SLOWO ZEROWE
 210  IPMEM(IDPMEM) = IOR(IPMEM(IDPMEM),MERPF)
 250  CONTINUE
      IF(IDPR.EQ.0) GO TO 500
C        WSTAWIAMY IDENTYFIKATOR BEZPOSREDMIEGO PREFIKSU DO OPISU PROTOTYPU
       IPMEM(IDPMEM+21) = IDPR
C        PRZEPISANIE LISTY PREFIKSLIST Z PREFIKSU I DOLACZENIE SIEBIE
C        NA KONCU LISTY
C        I - DLUGOSC LISTY PREFIKSLIST Z PREFIKSU - 1
C        J - POCZATEK PREFIXLIST Z PREFIKSU
C        J1 - POCZATEK TWORZONEJ LISTY PREFIKSLIST
	I=IPMEM(IDPR+23)
      J = IPMEM(IDPR+22)
      J1 = MGETM(I+1,41)
      DO 222 II=1,I
      IJ1=J1+II-1
      IJ2=J+II-1
 222  IPMEM(IJ1) = IPMEM(IJ2)
C  DOLOCZAMY SIEBIE DO LISTY PREFIXLIST I WSTAWIAMY PREFIXLIST DO
C      OPISU PROTOTYPU
      IJ1=J1+I
      IPMEM(IJ1) = IDPMEM
      IPMEM(IDPMEM+22)=J1
      IPMEM(IDPMEM+23) = I+1
C
C  SPRAWDZENIE POPRAWNOSCI PREFIKSOW SYSTEMOWYCH
C  TWORZENIE PREFIXSET
C
C
C
C  I=1  JESLI PREFIKS JEST COROUTINA
C  I=2  JESLI PREFIKS JEST PRECESEM
C  I=0  W PRZECIWNYM PRZYPADKU
      I=0
      IF(BPREF(IDPR,IPMEM(NRCOR-6))) I=1
      IF(BPREF(IDPR,IPMEM(NRPROC-6))) I=2
C  JESLI PROTOTYP NIE JEST KLASA, TO PRZECHODZIMY DO BADANIA POPRAWNOSCI
C     PREFIKSOWANIA
      J=IAND(IPMEM(IDPMEM),MTP)
      IF(J.EQ.NOTTP) GO TO 800
C  PRZEPISANIE PREFIXSET Z PREFIKSU
      IPMEM(IDPMEM-3) = IPMEM(IDPR-3)
      IPMEM(IDPMEM-4) = IPMEM(IDPR-4)
      IPMEM(IDPMEM-5) = IPMEM(IDPR-5)
C   ROZPOZNAMIE RODZAJU PROTOTYPU
      IF(ISYS.LT.I) ISYS=I
 300  IF(ISYS.EQ.0) GO TO 400
C
C   PROTOTYP JEST COROUTINA LUB PROCESEM
      CALL CHECK(INDPR)
C  USTAWIAMY ODPOWIEDNIE BITY W PREFIXSET
      I=IPMEM(IDPMEM)
      CALL MSETB(IDPMEM,IPMEM(NRCOR-6))
      IF(ISYS.NE.2) GO TO 350
      CALL MSETB(IDPMEM,IPMEM(NRPROC-6))
C  POPRAWIAMY SLOWO ZEROWE PROTOTYPU - TO JEST PROCES
      IPMEM(IDPMEM) = IOR(IAND(I,MASKTP),MPROCES)
      GO TO 400
C  POPRAWIAMY SLOWO ZEROWE - TO JEST COROUTINA
 350  IPMEM(IDPMEM) = IOR(IAND(I,MASKTP),MCOR)
C  PRZYDZIELENIE NUMERU W SENSIE PREFIXSET
 400  LPREFS = LPREFS+1
      CALL MSETB(IDPMEM,LPREFS)
      IPMEM(IDPMEM-6) = LPREFS
C   JESLI KLASA MA BLEDNA LISTE PF, TO POPRAWIAMY NA PELNA
      IF(IAND(IPMEM(IDPMEM),MERPF).NE.0)
     * CALL CHECK(IDPMEM)
C   JESLI W KLASIE BYLY INSTRUKCJE, TO POPRAWIAMY NA KLASE PELNA
      IF(BTEST(IPMEM(IDSMEM+8),13))  CALL CHECK(IDPMEM)
      GO TO 1000
C
C  NIE BYLO PREFIKSU, LUB BYL BLEDNY PREFIKS.
C  JESLI PROTOTYP JEST BLOKIEM, TO KONCZYMY PRZETWARZANIE PREFIKSOW
 500  CONTINUE
      J1 = IPMEM(IDPMEM)
      IF(IAND(J1,MTP).EQ.NOTTP.AND.IAND(J1,MOTHERS).EQ.MBLOCK)
     *   GO TO 1000
C  DOLOCZAMY SIEBIE JAKO JEDYNY ELEMENT LISTY PREFIKSOW
      J = MGETM(1,41)
      IPMEM(J) = IDPMEM
      IPMEM(IDPMEM+22) = J
      IPMEM(IDPMEM+23) = 1
C  JESLI PROTOTYP NIE JEST KLASA, TO KONIEC PRZETWARZANIA PREFIKSOW
       IF(IAND(J1,MTP).EQ.NOTTP) GO TO 1000
C  USTAWIAMY BIT 2 W PREFIKSSET NA 1 I PRZECHODZIMY DO USTALENIA RODZAJU
C   KLASY
      CALL MSETB(IDPMEM,2)
      GO TO 300
C
C  SPRAWDZENIE POPRAWNOSCI PREFIKSOW SYSTEMOWYCH
 800  IF(I.EQ.0) GO TO 1000
C   COROUTINE LUB PROCES NIE PREFIKSUJE KLASY
      CALL MERR(303,IPMEM(IDSMEM+2))
C
C
C --- --- --- --- --- --- --- --- --- --- --- ---
C     PRZETWARZANIE INFORMACJI O WIRTUALACH
C
 1000 CONTINUE
C
C  JESLI TO NIE JEST WIRTUAL, TO PRZECHODZIMY DALEJ
      IF(.NOT.BTEST(IPMEM(IDSMEM+8),15)) GO TO 2000
C   TO JEST WIRTUAL.
C   JESLI PROTOTYP OBEJMUJACY JEST BLOKIEM NIEPREFIKSOWANYM, TO KASUJEMY
C      WIRTUALE
       IF(IPMEM(INDPR).NE.1) GO TO 1001
       IPMEM(IDPMEM)=IAND(IPMEM(IDPMEM),MNOTVIR)
       GO TO 2000
C
C  BIT NR 11 W SLOWIE ZEROWYM JEST JUZ USTAWIONY PRZEZ PODPROGRAM INITPR
C  OWNVIR = .TRUE., JESLI W PROTOTYPIE INDPR SA WLASNE WIRTUALE
C
 1001 OWNVIR=.TRUE.
C   CZUKA,Y BEZPOSREDNIO WYZSZEGO VIRTUALA
       IF(INDPREF.EQ.0) GO TO 1300
C  PROTOTYP OBEJMUJACY MA PREFIKS
C  I - NAZWA VIRTUALA
      I=IPMEM(IDSMEM+10)
C   SPRAWDZAMY,CZY WIRTUAL JEST NA LISCIE TAKEN Z PREFIKSU
      IONLY=IPMEM(INDSPR+7)
      ONIL=BTEST(IPMEM(INDSPR+8),14)
      IF(.NOT.ONLY(I)) GO TO 1300
      J=MEMSL(I,INDPREF)
      IF(J.EQ.0) GO TO 1300
      IF(LOCAL.NE.2.OR.BTEST(IPMEM(J+1),1)) GO TO 1300
      IF(BTEST(IPMEM(J+1),2)) GO TO 1300
      J = IPMEM(J+2)
C   SPRAWDZAMY,CZY TO JEST WIRTUAL
      IF(.NOT.BTEST(IPMEM(J),11))  GO TO 1300
C   ZNALEZLISMY BEZPOSREDNIO WYZSZY WIRTUAL
C  J - IDENTYFIKATOR BEZPOSREDNIO WYZSZEGO WIRTUALA
C  I  - NUMER WIRTUALNY
      I = IPMEM(J+27)
      IPMEM(IDPMEM+26) = J
      IPMEM(IDPMEM+27) = I
C  WSTAWIAMY WIRTUAL DO VIRTLIST (ROBOCZEJ) PROTOTYPU OBEJMUJACEGO
      IJ1=LISTVB+I
      IPMEM(IJ1) = IDPMEM
      GO TO 2000
C
C  NIE BYLO BEZPOSREDNIO WYZSZEGO WIRTUALA
 1300 CONTINUE
      INSYS = .TRUE.
      LISTVE = MGETM(1,41)
C  DOKLADAMY NUMER WIRTUALNY
      IPMEM(IDPMEM+27) = LISTVE-LISTVB
      INSYS=.FALSE.
      IPMEM(LISTVE)=IDPMEM
C
C
C -- -- -- -- -- -- -- -- --- -- -- -- --- -- --
C     DLA FUNKCJI  --  PRZETWARZANIE JEJ TYPU
C
 2000 CONTINUE
C
C  BADAMY, CZY PROTOTYP JEST FUNKCJA
      I = ISHFT(IAND(IPMEM(IDPMEM),MOTHERS),-8)
      IF(I.NE.2) GO TO 3000
C   SZUKAMY TYPU;  J - NAZWA TYPU
      J = IPMEM(IDSMEM+12)
      I=IFTYPE(J)
C  SPRAWDZAMY, CZY TO JEST TYP FORMALNY
C  I - IDENTYFIKATOR TYPU
      IF(IAND(IPMEM(I),MTP).NE.6) GO TO 2100
C  TO JEST TYP FORMALBY - ZMIANA SLOWA ZEROWEGO W PROTOTYPIE FUNKCJI
      IPMEM(IDPMEM)= IOR(IPMEM(IDPMEM),ISHFT(1,12))
C  WSTAWIENIE TYPU
 2100 IPMEM(IDPMEM-3) = I
      IPMEM(IDPMEM-4) = IPMEM(IDSMEM+13)
      GO TO 3000
C
C
 2500 CONTINUE
C
C  ---  ----  ----  ----  ----   ----  ----  ----  ----  ----  ----  ----  ----
C  PRZETWARZANIE HANDLERA
C
      CALL HANDLER ( IDSMEM )
C
C
C
C  ----  ----  ----  ----  ----  ----  ----  ----
C     WSTAWINEI PARY  (NRSDIC,IDPMEM)  DO KOLEJKI PROTOTYPIOW
C
 3000 CONTINUE
      CALL DPUTQ(NRSDIC,IDPMEM)
      RETURN
      END
      LOGICAL FUNCTION IFCLASS(IX)

C  *****************
C   FUNKCJA DAJE ODPOWIEDZ, CZY DANY PROTOTYP JEST KLASA
C   IX - CZESC T ZE SLOWA ZEROWEGO PROTOTYPU
C
C   ****************
C
      IFCLASS=IX.EQ.2 .OR. IX.EQ.3 .OR. IX.EQ.5 .OR. IX.EQ.7
      RETURN
      END
      INTEGER FUNCTION INITPR(KIND,NAME)
C
C   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *
C   FUNKCJA WYKONUJE WSTEPNE CZYNNOSCI  ( REZERWACJA MIEJSCA,USTAWIENIE
C   LISTY ATRYBUTOW I TABLICY HASHU ) DLA PROTOTYPU.
C      NAME - NAZWA PROTOTYPU
C      KIND - RODZAJ PROTOTYPU
C   WARTOSCIA FUNKCJI JEST IDENTYFIKATOR UTWORZONEGO PROTOTYPU.
C   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *
C
C
      IMPLICIT INTEGER(A-Z)
C
C
      COMMON  / YNIT /  NULLWD(23), SIZEPR(23), NULLPOZ(23), CONSTWD,
     *  VARWD,VARPOM,INPFW,OUTPFW, INOUT
cdsw  INTEGER  SIZEPR, CONSTWD, VARWD, VARPOM, OUTPFW
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C    NULLWD(I)  -  WZORZEC SLOWA ZEROWEGO DLA PROTOTYPU RODZAJU I
C    SIZEPR(I)  -  ROZMIAR POLA W IPMEM   --   --   --
C    NULLPOZ(I)  -  POZYCJA SLOWA ZEROWEGO  --   --    --
C    CONSTWD  -  WZORZEC SLOWA ZEROWEGO DLA    CONST
C    VARWD  -     --   --   --   --          DLA ZMIENNEJ
C    VARPOM  -     --   --   --   --           ZMIENNEJ POMOCNICZEJ
C    INPFW  -     --   --    --    --          ZMIEMNEJ INPUT
C    OUTPFW  -     --   --    --    --         ZMIENNEJ OUTPUT
C    INOUT   -     --   --   --   --           ZMIENNEJ INOUT
C
C
C
      COMMON / DGLOB/ INDICT,INDSPR,INDPR,IHBEG,LASTPR,INDPREF
C
C  **  **  **  **  ***  **  **  **  **  **  **  **  **  **  **  **  **
C    BLOK ZAWIERA DANE O PRZETWARZANYM PROTOTYPIE
C        INDICT  -  INDEKS PRZETWARZANEGO PROTTYPU W ISDICT
C        INDSPR  -  IDENTYFIKATOR PROTOTYPU W ISMEM
C        INDPR  -  IDENTYFIKATOR PROTOTYPU W IPMEM
C        IHBEG  -  ADRES PIERWSZEGO SLOWA TABLICY HASHU
C        IDPREF  -  IDENTYFIKATOR BEZPOSREDNIEGO PREFIKSU PRZETWARZANEGO
C                  PROTOTYPU
C        LASTPR  -  IDENTYFIKATOR POPRZEDNIO PRZETWARZANEGO PROTOTYPU
C
C ..... ZMIENNE GLOBALNE
C
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
cdeb ----------- added ---------------------
      common /names/ resnm, mainm, brenam
c   nazwy ze scannera

      common /brid/ breaklid
c   numer w displayu (dla interpretera) prototypu breakl
cdeb ---------------------------------------
C
C
C        REZERWACJA MIEJSCA I USTAWIENIE SLOWA ZEROWEGO
       IF(KIND.GE.13 .AND.KIND.LE. 16) INSYS=.TRUE.
      INITPR = MGETM(SIZEPR(KIND),341)
      INSYS=.FALSE.
      INITPR=INITPR+NULLPOZ(KIND)
      IPMEM(INITPR) = NULLWD(KIND)
C   DOLACZENIE DO LISTY NEXTDECL
      IPMEM(LASTPR+2) = INITPR
      LASTPR = INITPR
cdeb ----------- added --------------------
      if(name.ne.brenam) go to 82
c  przekazanie na zmiennej breaklid numeru prototypu
c procedury breakl
c  obliczenie numeru prototypu
      i = nblus
      breaklid = 0
  80  k = ipmem(i)
c  formaLNE i sygnaly sa pomijane
      if(iand(ishft(k,-4),15).ne.0) go to 81
      if(i.eq.lastpr) go to 82
      breaklid = breaklid+1
 81   i = ipmem(i+2)
      if(i.ne.0) go to 80
 82   continue
cdeb ---------------------------------------
      IF ( KIND .GE.17.AND.KIND.LE.20) GO TO 100
C   INICJALIZACJA LISTY ATRYBUTOW
      IPMEM(INITPR+7) = INITPR+5
      IPMEM(INITPR+5) = NATTR
C   USTAWIENIE SL
 100  IPMEM(INITPR-1) = INDPR
C   WSTAWIENIE 1 DO POLA USED - DLA AIL
      IPMEM(INITPR+1) = 1
C   DLA BLOKOW I HANDLEROW - KONIEC
      IF(KIND.EQ.1.OR. KIND.EQ.8 .OR. KIND.EQ.23) RETURN
C   WSTAWIENIE NAZWY PROTOTYPU DO TABLICY HASH'U
C   JESLI TO JEST PROTOTYP FORMALNY II-GO RZEDU, TO ELEMENTY LISTY HASH'U SA
C   TWORZONE W CZESCI SYSTEMOWEJ
      IF( KIND.GE.18 .AND. KIND.LE.20 ) INSYS = .TRUE.
      IF( NAME .EQ.NEMPTY) GO TO 200
      I = IDPUT(NAME,IPMEM(IHBEG))
      IF ( I.EQ.0) GO TO 200
      IPMEM(I+2) = INITPR
C    DLA SYGNALOW - KONIEC
 200  IF ( KIND.GE.21 ) RETURN
C        WSTAWIENIE DO LISTY ATRYBUTOW ( PROTOTYPU INDPR )
C        JESLI INDPR JEST FORMALNY, TO LISTA ATRYBUTOW JEST TWORZONA
C          W CZESCI SYSTEMOWEJ
      CALL MADATR(INITPR,INDPR,41)
      INSYS = .FALSE.
      RETURN
      END
      SUBROUTINE MERGEID
C
C   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *
C  PODPROGRAM DOKLADA DO ZBIORU IDENTYFIKATOROW PROTOTYPU INDPR
C   IDENTYFIKATORY Z PREFIKSU TAKIE,ZE:
C         -   JESZCZE ICH NIE MA
C          - NIE MA ICH NA LISCIE TAKEN
C  PODPROGRAM JEST WYWOLYWANY O ILE BYL PREFIKS.
C   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *
C
       IMPLICIT INTEGER (A-Z)
C
      LOGICAL ONLY
C
C     INSERTION OF
      LOGICAL BTEST
C     BECAUSE OF TYPECONFLICT 03.01.84
      COMMON / DGLOB/ INDICT,INDSPR,INDPR,IHBEG,LASTPR,INDPREF
C
C  **  **  **  **  ***  **  **  **  **  **  **  **  **  **  **  **  **
C    BLOK ZAWIERA DANE O PRZETWARZANYM PROTOTYPIE
C        INDICT  -  INDEKS PRZETWARZANEGO PROTTYPU W ISDICT
C        INDSPR  -  IDENTYFIKATOR PROTOTYPU W ISMEM
C        INDPR  -  IDENTYFIKATOR PROTOTYPU W IPMEM
C        IHBEG  -  ADRES PIERWSZEGO SLOWA TABLICY HASHU
C        IDPREF  -  IDENTYFIKATOR BEZPOSREDNIEGO PREFIKSU PRZETWARZANEGO
C                  PROTOTYPU
C        LASTPR  -  IDENTYFIKATOR POPRZEDNIO PRZETWARZANEGO PROTOTYPU
C
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
      LOGICAL ERRFLG
      COMMON/STREAM/ERRFLG,LINE,IBUF2(265),IBUF3(7),JUNK(260)
      COMMON/MEM/NM,NH
      COMMON/DONLY/IONLY,ONIL
      LOGICAL ONIL
C
C   **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C    ROBOCZY BLOK, UZYWANY PRZY TWORZENIU ZBIORU IDENTYFIKATOROW
C    IONLY  -  POCZATEK LISTY TAKEN W ISMEM
C   ONIL=TRUE, GDY JEST TAKEN NIL
C
C
C
C   SPRAWDZAMY, CZY BYLO  TAKEN NIL
      ONIL=BTEST(IPMEM(INDSPR+8),14)
      IF(ONIL) GO TO 500
C
C   *   *   *   *   *   *
C  SPRAWDZENIE POPRAWNOSCI LISTY TAKEN
C   *   *   *   *   *   *
C
C  IHBEGP  --  POCZATEK LISTY IDENTYFIKATOROW W PREFIKSIE
      IHBEGP=INDPREF+10
      IONLY=IPMEM(INDSPR+7)
      IF(IONLY .EQ.0) GO TO 500
C  J - POPRZEDNI ELEMENT LISTY
C  I - BIEZACY ELEMENT LISTY
      J=0
      I=IONLY
 100  NM=IPMEM(I)
      LINE=IPMEM(I+1)
      NH=IAND(ISHFT(NM,-1),7)+1
      M=MEMPRF(INDPREF)
C  M - ELEMENT LISTY HASHU, JESLI NAZWA NM JEST ZADEKLAROWANA W PREFIKSACH
      IF(M .EQ. 0) GO TO 400
C  SPRAWDZAMY, CZY NAZWA JEST HIDDEN
      IF(BTEST(IPMEM(M+1),1))  GO TO 300
C   SPRAWDZAMY,CZY NAZWA JEST NOT TAKEN
      IF(BTEST(IPMEM(M+1),2)) GO TO 250
C        POPRAWNY ELEMENT LISTY TAKEN
      J=I
 200  I=IPMEM(I+2)
      IF(I.NE.0) GO TO 100
      GO TO 500
C   NAZWA JEST NOT TAKEN - USUWAMY Z LISTY TAKEN
 250  CALL MERR(321,NM)
      GO TO 200
C  NAZWA JEST HIDDEN
 300  CALL MERR(304,NM)
      GO TO 200
C  NAZWA NIEZADEKLAROWANA
 400  CALL MERR(305,NM)
C  DOKLADAMY NAZWE DO ZBIORU IDENTYFIKATOROW
      M = MEMBER(NM,IPMEM(IHBEG))
      IF(M.NE.0) GO TO 200
      M=INSERT(NM,IPMEM(IHBEG),341)
      GO TO 200
C
C  *   *   *   *   *   *
C       LACZENIE ZBIOROW IDENTYFIKATOROW
C  *  *   *   *   *  *
C
 500  CONTINUE
C   PRZEGLADAMY KOLEJNE PREFIKSY
      IDP= INDPREF
 800  CONTINUE
C        IE - KONIEC TABLICY IDENTYFIKATOROW W PREFIKSIE
      IE= IDP+17
C   IHBEGP - POCZATEK LISTY IDENTYFIKATOROW W PREFIKSIE
      IHBEGP=IDP+10
      DO 555 I=IHBEGP,IE,1
C        I - INDEKS KOLEJNEGO ELEMENTU TABLICY HASHU
C        J - ELEMENT TABLICY HASHU
      J=IPMEM(I)
      IF(J.EQ.0) GO TO 555
 600   CONTINUE
C   JESLI NAZWA JEST HIDDEN - TO DALEJ
      IF(BTEST(IPMEM(J+1),1)) GO TO 700
C JESLI NAZWA JEST NOT TAKEN - TO DALEJ
      IF(BTEST(IPMEM(J+1),2)) GO TO 700
      NM=IPMEM(J)
      IF(ONLY(NM)) GO TO 700
C   NAZWY NIE MA NA LISCIE TAKEN
      NH=IAND(ISHFT(NM,-1),7)+1
C   SZUKAMY NAZWY OD INDPR PO PREFIKSACH
      IND = INDPR
 610  M = IND+9+NH
      M = IPMEM(M)
 620  IF(M.EQ.0) GO TO 650
      IF(IPMEM(M).EQ.NM) GO TO 670
      M = IPMEM(M+3)
      GO TO 620
 650  IND = IPMEM(IND+21)
      GO TO 610
C   NAZWA MUSI ZOSTAC ZNALEZIONA
C   JESLI NAZWA BYLA ZNALEZIONA PONIZEJ IDP, TO
C   ALBO BYLA JUZ ROZPATRYWANA, ALBO BYLA JUZ NOT TAKEN,
C   ALBO JEST LOKALNA W INDPR
 670  IF(IND.NE.IDP) GO TO 700
C   NAZWE TRZEBA DOSTAWIC
       M=INSERT(NM,IPMEM(IHBEG),341)
      IPMEM(M+1) = 4
      IPMEM(M+2) = IPMEM(J+2)
 700  J=IPMEM(J+3)
      IF(J.NE.0) GO TO 600
 555  CONTINUE
C   PRZECHODZIMY DO NATEPNEGO PREFIKSU
      IDP=IPMEM(IDP+21)
      IF(IDP.NE.0) GO TO 800
      RETURN
      END
      LOGICAL FUNCTION ONLY(NAME)
C
C   *  *  *  *   *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
C     FUNKCJA SPRAWDZA, CZY NAZWA NAME JEST NA LISCIE TAKEN
C    POCZATEK LISTY TAKEN - IONLY
C  *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *
C
C
      IMPLICIT INTEGER(A-Z)
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
C
      COMMON  / DONLY /  IONLY,ONIL
      LOGICAL ONIL
C
C   **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C    ROBOCZY BLOK, UZYWANY PRZY TWORZENIU ZBIORU IDENTYFIKATOROW
C    IONLY  -  POCZATEK LISTY TAKEN W ISMEM
C   ONIL=TRUE, GDY JEST TAKEN NIL

C
C   JESLI LISTA TAKEN JEST PUSTA, TO ZAKLADAMY, ZE SA W NIEJ WSZYSTKIE
C            NAZWY
C
       ONLY=.FALSE.
       IF(ONIL) RETURN
       IF(IONLY.EQ.0) GO TO 200
       I=IONLY
 100   IF(IPMEM(I).EQ.NAME) GO TO 200
       I=IPMEM(I+2)
       IF(I.NE.0) GO TO 100
C   NAZWY NIE MA NA LISCIE TAKEN
       RETURN
C  NAZWA JEST NA LISCIE TAKEN
 200   ONLY=.TRUE.
       RETURN
       END
      SUBROUTINE CHECK ( IND )
C
C   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C   POPRAWIA BIT OZNACZAJACY PELNA KLASE POCZAWSZY OD PROTOTYPU  IND
C   KONCZY, JESLI TEN BIT JEST 1
C  * * * * * * * * * * * * * * * * *  *  * * * * * * * * * * * * * * *
C
      IMPLICIT INTEGER(A-Z)
C
C     INSERTION OF
      LOGICAL BTEST
C     BECAUSE OF TYPECONFLICT 03.01.84
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
C             COM    - OBSZAR KOMUNIKACYJNY STRUMIENI
C             LMEM   - (=5000) ROZMIAR CALEJ PAMIECI GLOWNEJ
C             LPMEM  - PODZIAL PAMIECI NA CZESCI  IPMEM  I  ISMEM
C             IRECN  - INDEKS SZCZYTU STOSU STALYCH REAL
C             ISFIN  - INDEKS SZCZYTU SLOWNIKA PROTOTYPOW
C
C             LPMSYS - INDEKS PIERWSZEGO SLOWA W IPMEM OBSZARU NIEPRZEZ-
C                      NACZONEGO NA PROTOTYPY SYSTEMOWE
C             LPML   - INDEKS PIERWSZEGO SLOWA OBSZARU WOLNEGO W IPMEM
C             LPMF   - INDEKS OSTATNIEGO SLOWA WOLNEGO OBSZARU W IPMEM
C
C
C
      J=IND
 100  I=IPMEM(J)
      IF(BTEST(I,0)) RETURN
      IPMEM(J) = IOR(I,1)
      J=IPMEM(J-1)
      GO TO 100
       END
      INTEGER FUNCTION IDPUT(NAME,THASH)
C
C   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *
C  FUNKCJA WSTAWIA NAZWE NAME DO TABLICY THASH UPRZEDNIO SPRWDZAJAC,
C   CZY NAZWA JUZ TAM JEST.
C   JESLI JEST, TO WARTOSCIA FUNKCJI JEST 0 I WYKONYWANE SA REAKCJE NA BLAD
C     ( BLAD NIE JEST SYGNALIZOWANY )
C   JESLI NIE MA , TO WARTOSCIA FUNKCJI JEST WSTAWIANY ELEMENT
C   LISTY HASHU.
C   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *
C
C
      IMPLICIT INTEGER(A-Z)
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
C             COM    - OBSZAR KOMUNIKACYJNY STRUMIENI
C             LMEM   - (=5000) ROZMIAR CALEJ PAMIECI GLOWNEJ
C             LPMEM  - PODZIAL PAMIECI NA CZESCI  IPMEM  I  ISMEM
C             IRECN  - INDEKS SZCZYTU STOSU STALYCH REAL
C             ISFIN  - INDEKS SZCZYTU SLOWNIKA PROTOTYPOW
C
C             LPMSYS - INDEKS PIERWSZEGO SLOWA W IPMEM OBSZARU NIEPRZEZ-
C                      NACZONEGO NA PROTOTYPY SYSTEMOWE
C             LPML   - INDEKS PIERWSZEGO SLOWA OBSZARU WOLNEGO W IPMEM
C             LPMF   - INDEKS OSTATNIEGO SLOWA WOLNEGO OBSZARU W IPMEM
C
C
cdsw  INTEGER THASH(8)
      dimension thash(8)
C
C
C        SPRAWDZENIE, CZY NAZWA JEST W TABLICY
      I=MEMBER(NAME,THASH)
      IF(I.NE.0) GO TO 100
C        NAZWY NIE MA - WSTAWIAMY
      IDPUT = INSERT ( NAME,THASH,341)
      RETURN
C
C        NAZWA JEST - PODWOJNA DEKLARACJA
 100  IDPUT = 0
      CALL MERR(309,NAME)
C        SKASOWANIE W ELEMENCIE LISTY HASHU INFORMACJI O HIDDEN I CLOSE
      IPMEM(I+1) = 0
C        DOWIAZANIE NAZWY DO OBIEKTU UNIVERSAL
      IPMEM(I+2) = NRUNIV
      RETURN
      END
      INTEGER FUNCTION IFTYPE ( NAME)
C
C   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *
C   FUNKCJA ZNAJDUJE NAZWE NAME W PROTOTYPIE O IDENTYFIKATORZE  INDPR
C   I DALEJ O SL-ACH.  SPRAWDZA,CZY JEST TO NAZWA TYPU.
C   IFTYPE =   IDENTYFIKATOR TYPU, JESLI TYP JEST POPRAWNY
C      IFTYPE = NRUNIV, GDY TYP JEST NIEZADEKLAROWANY LUB NIEDOSTEPNY
C   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *
C
      IMPLICIT INTEGER(A-Z)
C
C     INSERTION OF
      LOGICAL BTEST
C     BECAUSE OF TYPECONFLICT 03.01.84
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
C             COM    - OBSZAR KOMUNIKACYJNY STRUMIENI
C             LMEM   - (=5000) ROZMIAR CALEJ PAMIECI GLOWNEJ
C             LPMEM  - PODZIAL PAMIECI NA CZESCI  IPMEM  I  ISMEM
C             IRECN  - INDEKS SZCZYTU STOSU STALYCH REAL
C             ISFIN  - INDEKS SZCZYTU SLOWNIKA PROTOTYPOW
C
C             LPMSYS - INDEKS PIERWSZEGO SLOWA W IPMEM OBSZARU NIEPRZEZ-
C                      NACZONEGO NA PROTOTYPY SYSTEMOWE
C             LPML   - INDEKS PIERWSZEGO SLOWA OBSZARU WOLNEGO W IPMEM
C             LPMF   - INDEKS OSTATNIEGO SLOWA WOLNEGO OBSZARU W IPMEM
C
C
C
      COMMON / DGLOB/ INDICT,INDSPR,INDPR,IHBEG,LASTPR,INDPREF
C
C  **  **  **  **  ***  **  **  **  **  **  **  **  **  **  **  **  **
C    BLOK ZAWIERA DANE O PRZETWARZANYM PROTOTYPIE
C        INDICT  -  INDEKS PRZETWARZANEGO PROTTYPU W ISDICT
C        INDSPR  -  IDENTYFIKATOR PROTOTYPU W ISMEM
C        INDPR  -  IDENTYFIKATOR PROTOTYPU W IPMEM
C        IHBEG  -  ADRES PIERWSZEGO SLOWA TABLICY HASHU
C        IDPREF  -  IDENTYFIKATOR BEZPOSREDNIEGO PREFIKSU PRZETWARZANEGO
C                  PROTOTYPU
C        LASTPR  -  IDENTYFIKATOR POPRZEDNIO PRZETWARZANEGO PROTOTYPU
C
C
      COMMON  / MASKS /  MTP,MSPR,MOTHERS,MPAR,MASKTP,NOTTP,
     * MPROCES, MCOR, MERPF, MBLOCK, MHAND, MNOTVIRT
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C   MASKI I WZORCE:
C    MTP - MASKA DO WYCINANIA INFORMACJI DOTYCZACYCH TYPOW ZE SLOWA ZEROWEGO
C    MOTHERS -      --     --      --      --     --  INNYCH PROTOTYPOW
C    MPAR -    --    --    --    --    --    --       ZMIENNYCH I PARAMETROW
C    MSPR - MASKA DLA SYSPREF  ( DLA PROTOTYPOW SYNTAKTYCZNYCH )
C    MASKTP - ZAPRZECZENIE MASKI  MTP
C    NOTTP - WZORZEC DLA NIE-TYPU  ( 1 )
C    MPROCES - WZORZEC DLA PROCESU  ( 5 )
C    MCOR - WZORZEC DLA COROUTINY (7)
C    MERPF - MASKA DO WYKRYWANIA BLEDNYCH LIST PARAMETROW
C    MBLOCK - WZORZEC DLA BLOKU  ( 0 PRZY PRZECIECIU Z  MOTHERS )
C
C
C
C
      IFTYPE=MEMSL(NAME,INDPR)
      IF(IFTYPE.EQ.0) GO TO 200
      I=IPMEM(IFTYPE+1)
      IF(BTEST(I,2)) GO TO  100
      IF(.NOT.BTEST(I,1) .OR. OWN) GO TO 400
C   NAZWA JEST HIDDEN
      CALL MERR(317,NAME)
      GO TO 150
C   NAZWA JEST NOT TAKEN
 100  CALL MERR(318,NAME)
 150  IFTYPE=NRUNIV
      RETURN
C        NAZWA JEST ZADEKLAROWANA
 400  IFTYPE = IPMEM(IFTYPE + 2)
C        SPRAWDZENIE, CZY TO JEST NAZWA TYPU
      IF(IAND(IPMEM(IFTYPE),MTP).NE.NOTTP) RETURN
C        TO NIE JEST NAZWA TYPU
      IFTYPE = NRUNIV
      CALL MERR(307,NAME)
      RETURN
C        NAZWA JEST NIEZADEKLAROWANA - DKLADAMY JA DO BIEZACEGO PROTOTYPU
 200  CALL MERR(306,NAME)
      IFTYPE = INSERT(NAME,IPMEM(IHBEG),341)
      IFTYPE = NRUNIV
      RETURN
      END
      SUBROUTINE DPUTQ (NSDIC,IDPMEM)
C
C   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *
C   PODPROGRAM WSTAWIA PARE (NSDIC,IDPMEM)  DO KOLEJKI PROTOTYPOW
C   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *
C
      IMPLICIT INTEGER(A-Z)
C
C
      COMMON  / QUEUE /  BQUEUE, EQUEUE, IFIRST, LAST, EMPTY
cdsw  INTEGER  BQUEUE, EQUEUE
      LOGICAL EMPTY
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C    ZMIENNE SLUZACE DO ORGANIZACJI KOLEJKI PROTOTYPOW
C      BQUEUE  -  POCZATEK POLA W IPMEM PRZEZNACZONEGO NA KOLEJKE
C      EQUEUE  -  KONIEC      --      --       --      --      --
C      IFIRST  -  PIERWSZY ELEMENT KOLEJKI
C      LAST  -  OSTATNI ELEMENT KOLEJKI
C      EMPTY = TRUE, GDY KOLEJKA JEST PUSTA
C
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
C             COM    - OBSZAR KOMUNIKACYJNY STRUMIENI
C             LMEM   - (=5000) ROZMIAR CALEJ PAMIECI GLOWNEJ
C             LPMEM  - PODZIAL PAMIECI NA CZESCI  IPMEM  I  ISMEM
C             IRECN  - INDEKS SZCZYTU STOSU STALYCH REAL
C             ISFIN  - INDEKS SZCZYTU SLOWNIKA PROTOTYPOW
C
C             LPMSYS - INDEKS PIERWSZEGO SLOWA W IPMEM OBSZARU NIEPRZEZ-
C                      NACZONEGO NA PROTOTYPY SYSTEMOWE
C             LPML   - INDEKS PIERWSZEGO SLOWA OBSZARU WOLNEGO W IPMEM
C             LPMF   - INDEKS OSTATNIEGO SLOWA WOLNEGO OBSZARU W IPMEM
C
C
C
C        JESLI DOSZLISMY DO KONCA OBSZARU PRZEZNACZONEGO NA KOLJKE, TO
C        ZACZYNAMY OD POCZATKU
      IF ( LAST.EQ.EQUEUE-1) LAST=BQUEUE-2
       IF(LAST.EQ.IFIRST-2.AND..NOT.EMPTY) GO TO 100
C        JEST MIEJSCE NA DOSTAWIANIE ELEMENTU DO KOLEJKI
      EMPTY=.FALSE.
      LAST = LAST+2
      IPMEM(LAST) = NSDIC
      IPMEM(LAST+1) = IDPMEM
      RETURN
C
C        PRZEPELNIENIE OBSZARU PRZEZNACZONEGO NA KOLEJKE
C        PRZERWANIE KOMPILACJI
 100  CALL MDROP(343)
      RETURN
      END
      SUBROUTINE DGETQ
C
C
C   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *
C   PODPROGRAM POBIERA PIERWSZY ELEMENT Z KOLEJKI PROTOTYPOW I
C   WSTAWIA NA ZMIENNE INDICT I INDPR Z BLOKU  DGLOB.
C   NIE SPRAWDZA, CZY KOLEJKA JEST PUSTA.
C   JESLI NA SKUTEK WYKONANIA OPERACJI KOLEJKA BEDZIE PUSTA, TO ZMIENNA
C   EMPTY DOSTAJE WARTOSC  TRUE  .
C   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *
C
C
      IMPLICIT INTEGER(A-Z)
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
C             COM    - OBSZAR KOMUNIKACYJNY STRUMIENI
C             LMEM   - (=5000) ROZMIAR CALEJ PAMIECI GLOWNEJ
C             LPMEM  - PODZIAL PAMIECI NA CZESCI  IPMEM  I  ISMEM
C             IRECN  - INDEKS SZCZYTU STOSU STALYCH REAL
C             ISFIN  - INDEKS SZCZYTU SLOWNIKA PROTOTYPOW
C
C             LPMSYS - INDEKS PIERWSZEGO SLOWA W IPMEM OBSZARU NIEPRZEZ-
C                      NACZONEGO NA PROTOTYPY SYSTEMOWE
C             LPML   - INDEKS PIERWSZEGO SLOWA OBSZARU WOLNEGO W IPMEM
C             LPMF   - INDEKS OSTATNIEGO SLOWA WOLNEGO OBSZARU W IPMEM
C
C
C
      COMMON  / QUEUE /  BQUEUE, EQUEUE, IFIRST, LAST, EMPTY
cdsw  INTEGER  BQUEUE, EQUEUE
      LOGICAL EMPTY
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C    ZMIENNE SLUZACE DO ORGANIZACJI KOLEJKI PROTOTYPOW
C      BQUEUE  -  POCZATEK POLA W IPMEM PRZEZNACZONEGO NA KOLEJKE
C      EQUEUE  -  KONIEC      --      --       --      --      --
C      IFIRST  -  PIERWSZY ELEMENT KOLEJKI
C      LAST  -  OSTATNI ELEMENT KOLEJKI
C      EMPTY = TRUE, GDY KOLEJKA JEST PUSTA
C
      COMMON / DGLOB/ INDICT,INDSPR,INDPR,IHBEG,LASTPR,INDPREF
C
C  **  **  **  **  ***  **  **  **  **  **  **  **  **  **  **  **  **
C    BLOK ZAWIERA DANE O PRZETWARZANYM PROTOTYPIE
C        INDICT  -  INDEKS PRZETWARZANEGO PROTTYPU W ISDICT
C        INDSPR  -  IDENTYFIKATOR PROTOTYPU W ISMEM
C        INDPR  -  IDENTYFIKATOR PROTOTYPU W IPMEM
C        IHBEG  -  ADRES PIERWSZEGO SLOWA TABLICY HASHU
C        IDPREF  -  IDENTYFIKATOR BEZPOSREDNIEGO PREFIKSU PRZETWARZANEGO
C                  PROTOTYPU
C        LASTPR  -  IDENTYFIKATOR POPRZEDNIO PRZETWARZANEGO PROTOTYPU
C
C
C
      INDICT = IPMEM(IFIRST)
      INDPR = IPMEM(IFIRST+1)
      IF(LAST.EQ.IFIRST) GO TO 100
C        KOLEJKA MA CO NAJMNIEJ 2 ELEMENTY
      IF(IFIRST.EQ.EQUEUE-1) GO TO 50
      IFIRST = IFIRST+2
      RETURN
 50   IFIRST = BQUEUE
      RETURN
C
C        KOLEJKA BEDZIE PUSTA
 100  EMPTY = .TRUE.
      IFIRST=BQUEUE
      LAST=BQUEUE-2
      RETURN
      END
      SUBROUTINE HEADER
C
C
C**********************************************C
C   PODPROGRAM PRZETWARZA LISTE PARAMETROW FORMALNYCH
C   PROTOTYPU INDPR
C **********************************************
C
      IMPLICIT INTEGER(A-Z)
      LOGICAL PQ,ISTPF,FORM2
C     INSERTION OF
      LOGICAL BTEST
C     BECAUSE OF TYPECONFLICT 03.01.84
C
C ..... ZMIENNE GLOBALNE
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
C             COM    - OBSZAR KOMUNIKACYJNY STRUMIENI
C             LMEM   - (=5000) ROZMIAR CALEJ PAMIECI GLOWNEJ
C             LPMEM  - PODZIAL PAMIECI NA CZESCI  IPMEM  I  ISMEM
C             IRECN  - INDEKS SZCZYTU STOSU STALYCH REAL
C             ISFIN  - INDEKS SZCZYTU SLOWNIKA PROTOTYPOW
C
C             LPMSYS - INDEKS PIERWSZEGO SLOWA W IPMEM OBSZARU NIEPRZEZ-
C                      NACZONEGO NA PROTOTYPY SYSTEMOWE
C             LPML   - INDEKS PIERWSZEGO SLOWA OBSZARU WOLNEGO W IPMEM
C             LPMF   - INDEKS OSTATNIEGO SLOWA WOLNEGO OBSZARU W IPMEM
C
C
C
      COMMON / DGLOB/ INDICT,INDSPR,INDPR,IHBEG,LASTPR,INDPREF
C
C  **  **  **  **  ***  **  **  **  **  **  **  **  **  **  **  **  **
C    BLOK ZAWIERA DANE O PRZETWARZANYM PROTOTYPIE
C        INDICT  -  INDEKS PRZETWARZANEGO PROTTYPU W ISDICT
C        INDSPR  -  IDENTYFIKATOR PROTOTYPU W ISMEM
C        INDPR  -  IDENTYFIKATOR PROTOTYPU W IPMEM
C        IHBEG  -  ADRES PIERWSZEGO SLOWA TABLICY HASHU
C        IDPREF  -  IDENTYFIKATOR BEZPOSREDNIEGO PREFIKSU PRZETWARZANEGO
C                  PROTOTYPU
C        LASTPR  -  IDENTYFIKATOR POPRZEDNIO PRZETWARZANEGO PROTOTYPU
C
C
      COMMON /DWORK/ IDPAR,LFORMB,LFORME,FORM2,SYGN
      LOGICAL SYGN
C
C  *  *  *  *  *  *   *  *  *  *  *  *  *  *  *  *  *   *  *  *
C   IDPAR - INDEKS SLOWA ZEROWEGO BIEZACEGO PARAMETRU W ISMEM
C   LFORMB,LFORME - PIERWSZY I OSTATNI ELEMENT ROBOCZEJ LISTY PF
C   FORM2=TRUE, GDY PRZETWAEZAMY PARAMETRY II-GO RZEDU
C   SYGN = TRUE, GDY SA TO PARAMETRY SYGNALU
C
C
      COMMON  / MASKS /  MTP,MSPR,MOTHERS,MPAR,MASKTP,NOTTP,
     * MPROCES, MCOR, MERPF, MBLOCK,MHAND,MNOTVIR
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C   MASKI I WZORCE:
C    MTP - MASKA DO WYCINANIA INFORMACJI DOTYCZACYCH TYPOW ZE SLOWA ZEROWEGO
C    MOTHERS -      --     --      --      --     --  INNYCH PROTOTYPOW
C    MPAR -    --    --    --    --    --    --       ZMIENNYCH I PARAMETROW
C    MSPR - MASKA DLA SYSPREF  ( DLA PROTOTYPOW SYNTAKTYCZNYCH )
C    MASKTP - ZAPRZECZENIE MASKI  MTP
C    NOTTP - WZORZEC DLA NIE-TYPU  ( 1 )
C    MPROCES - WZORZEC DLA PROCESU  ( 5 )
C    MCOR - WZORZEC DLA COROUTINY (7)
C    MERPF - MASKA DO WYKRYWANIA BLEDNYCH LIST PARAMETROW/
C    MBLOCK - WZORZEC DLA BLOKU  ( 0 PRZY PRZECIECIU Z  MOTHERS )
C    MNOTVIR - WZORZEC DO KASOWANIA BITU "WIRTUAL"
C    MHAND - MASKA DLA HANDLERA
C
C
C
      COMMON  / YNIT /  NULLWD(23), SIZEPR(23), NULLPOZ(23), CONSTWD,
     *  VARWD,VARPOM,INPFW,OUTPFW, INOUT
cdsw  INTEGER  SIZEPR, CONSTWD, VARWD, VARPOM, OUTPFW
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C    NULLWD(I)  -  WZORZEC SLOWA ZEROWEGO DLA PROTOTYPU RODZAJU I
C    SIZEPR(I)  -  ROZMIAR POLA W IPMEM   --   --   --
C    NULLPOZ(I)  -  POZYCJA SLOWA ZEROWEGO  --   --    --
C    CONSTWD  -  WZORZEC SLOWA ZEROWEGO DLA    CONST
C    VARWD  -     --   --   --   --          DLA ZMIENNEJ
C    VARPOM  -     --   --   --   --           ZMIENNEJ POMOCNICZEJ
C    INPFW  -     --   --    --    --          ZMIEMNEJ INPUT
C    OUTPFW  -     --   --    --    --         ZMIENNEJ OUTPUT
C    INOUT   -     ---  --   --   ---          ZMIENNEJ INOUT
C
C  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
C
C   SPRAWDZENIE,CZY TYP FORMALNY NIE WYSTEPUJE PO UZYCIU
C   JESLI BLOK - TO IDZIEMY DALEJ
      IF(IPMEM(INDSPR).EQ.1) GO TO 1000
      SYGN = ISHFT(IAND(IPMEM(INDPR),MPAR),-4).EQ.11
      IDPAR = IPMEM(INDSPR+11)
      IF(SYGN) IDPAR = IPMEM(INDSPR+4)
      FORM2=.FALSE.
 100  IF(IDPAR.EQ.0) GO TO 1000
      K=IPMEM(IDPAR)
 150  IF (FORM2) GO TO 350
      IF(K.EQ.7) GO TO 900
      IF(K.EQ.3.OR.K.EQ.5) GO TO 200
      I1=IDPAR+5
      IF(K.GE.8) I1=IDPAR+4
      I=IPMEM(I1)
C  I - NAZWA TYPU PARAMETRU
C  SPRAWDZAMY,CZY TEN TYP JEST POZNIEJ W LISCIE PARAMETROW
C   OD IDPAR DO KONCA LISTY
      PQ=ISTPF(I,.TRUE.)
      IF(PQ) IPMEM(I1)=NEMPTY
      IF(K.GE.8) GO TO 900
C  TERAZ SPRAWDZAMY PARAMETRY II-GO RZEDU
 200  LFORMB=IDPAR
      FORM2=.TRUE.
      IDPAR=IPMEM(LFORMB+4)
 300  IF(IDPAR.EQ.0) GO TO 800
      K=IPMEM(IDPAR)
      GO TO 150
 350  IF(K.LT.8) GO TO 700
      I=IPMEM(IDPAR+4)
C  SZUKAMY TYPU OD IDPAR DO KONCA LISTY II-GO RZEDU
      PQ=ISTPF(I,.TRUE.)
      IF(.NOT.PQ) GO TO 400
      IPMEM(IDPAR+4) = NEMPTY
      GO TO 700
C  SZUKAMY TYPU WCZESNIEJ W LISCIE II-GO RZEDU BEZ
C   SYGNALIZACJI BLEDU
 400  PQ = ISTPF(I,.FALSE.)
      IF( PQ ) GO TO 700
C  SZUKAMY W ZEWNETRZNEJ LISCIE PF OD PRZERABIANEJ
C   PROCEDURY/FUNKCJI DO KONCA
      K=IDPAR
      IDPAR=LFORMB
      PQ = ISTPF(I,.TRUE.)
      IF(PQ) IPMEM(K+4) = NEMPTY
      IDPAR=K
 700  IDPAR = IPMEM(IDPAR+3)
      GO TO 300
 800  FORM2=.FALSE.
      IDPAR=LFORMB
 900  IDPAR = IPMEM(IDPAR+3)
      GO TO 100
C
C  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
C
C              PRZETWARZANIE PARAMETROW
C
 1000 CONTINUE
C
C   PRZYGOTOWANIA DO PRZETWARZANIA LISTY PF
      BEGM=LPML
      INSYS=.TRUE.
      LFORMB=MGETM(2,341)
      LFORME=LFORMB
      FORM2=.FALSE.
      INSYS=.FALSE.
C  JESLI BLOK TO NIE POSIADA PARAMETROW
      IF(IPMEM(INDSPR).EQ.1) GO TO 1500
C
C   PRZETWARZANIE PARAMETROW
      IDPAR=IPMEM(INDSPR+11)
      IF(SYGN) IDPAR = IPMEM(INDSPR+4)
 1100 IF(IDPAR.EQ.0) GO TO 1500
      K=IPMEM(IDPAR)
      IF(K.EQ.7) GO TO 1200
      IF(K.GE.8) GO TO 1300
C  PROCEDURA / FUNKCJA FORMALNA
      CALL CHECK(INDPR)
      CALL PROCFUN
      GO TO 1400
C   TYP FORMALNY
 1200 CALL TYPEF
      GO TO 1400
C   ZMIENNA
 1300 CALL VARIAB
 1400 IDPAR=IPMEM(IDPAR+3)
      GO TO 1100
C
 1500 CONTINUE
C  DOLACZENIE RESULT
C JESLI TO JEST FUNKCJA
C   UWAGA - NAZWA RESULT DOLACZANA W PROTP1
      IF(IPMEM(INDSPR).NE.4.AND.IPMEM(INDSPR).NE.6) GO TO 2000
      I=MGETM(6,341) + 4
      IPMEM(I) = OUTPFW
      CALL PUTPF(I)
      CALL MADATR(I,INDPR,341)
C   ZAPAMIETANIE 1 W POLU USED - DLA AIL
      IPMEM(I+1)=1
      IPMEM(INDPR-5) = I
C  DOLACZENIE TYPU
      IPMEM(I-4) = IPMEM(INDPR-4)
      IPMEM(I-3) = IPMEM(INDPR-3)
C   WYLICZENIE APETYTU RESULT
      IPMEM(I) = IAP(I)
C   JESLI TO BYL TYP FORMALNY, TO ZMIANA SLOWA ZEROWEGO
      IF(BTEST(IPMEM(INDPR),12))
     * IPMEM(I) = IOR(IPMEM(I),ISHFT(1,12 ))
C
C
 2000 CONTINUE
      FORM2=.FALSE.
      IF(INDPREF.EQ.0) GO TO 1700
C   JESLI PREFIKS MIAL BLEDNA LISTE PF, TO NIE DOKLADAMY WLASNEJ
      IF (IAND(IPMEM(INDPREF),MERPF).NE.0) GO TO 1850
C   DOPISANIE SWOJEJ LISTY PF
 1700  CALL COPY
C   DOLACZENIE LISTY PF Z PREFIKSU
      IF(INDPREF.EQ.0) GO TO 3000
 1850 I=IPMEM(INDPREF+3)
      J=IPMEM(INDPREF+4)
      IF(J.EQ.0) GO TO 3000
      K=MGETM(J,341)
      DO 1666 I1=1,J
      I2=K+I1-1
      I3=I+I1-1
 1666 IPMEM(I2) = IPMEM(I3)
      IPMEM(INDPR+3) = K
      IPMEM(INDPR+4) = IPMEM(INDPR+4)+J
 3000 CONTINUE
C   ZAKONCZENIE
      LPML=BEGM
      RETURN
      END
      LOGICAL FUNCTION ISTPF(NM,PQ)
C
C * * * * * * * * * * * * * * * * * * * * * * * *
C   FUNKCJA SPRAWDZA,CZY TYP O NAZWIE NM WYSTEPUJE JAKO
C   FORMALNY W LISCIE PARAMETROW.
C   JESLI PQ=.TRUE., TO SZUKAMY OD IDPAR DO KONCA LISTY
C   I W RAZIE ZNALEZIENIA SYGNALIZUJEMY BLAD.
C   JESLI PQ=.FALSE., TO SZUKAMY OD POCZATKU LOKALNEJ LISTY
C   PARAMETROW(LFORMAB) DO IDPAR I NIE SYBNALIZUJEMY BLEDU.
C   * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C
      IMPLICIT INTEGER(A-Z)
      LOGICAL PQ
C
C ..... ZMIENNE GLOBALNE
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
C             COM    - OBSZAR KOMUNIKACYJNY STRUMIENI
C             LMEM   - (=5000) ROZMIAR CALEJ PAMIECI GLOWNEJ
C             LPMEM  - PODZIAL PAMIECI NA CZESCI  IPMEM  I  ISMEM
C             IRECN  - INDEKS SZCZYTU STOSU STALYCH REAL
C             ISFIN  - INDEKS SZCZYTU SLOWNIKA PROTOTYPOW
C
C             LPMSYS - INDEKS PIERWSZEGO SLOWA W IPMEM OBSZARU NIEPRZEZ-
C                      NACZONEGO NA PROTOTYPY SYSTEMOWE
C             LPML   - INDEKS PIERWSZEGO SLOWA OBSZARU WOLNEGO W IPMEM
C             LPMF   - INDEKS OSTATNIEGO SLOWA WOLNEGO OBSZARU W IPMEM
C
      COMMON /DWORK/ IDPAR,LFORMB,LFORME,FORM2,SYGN
      LOGICAL FORM2, SYGN
C
C  *  *  *  *  *  *   *  *  *  *  *  *  *  *  *  *  *   *  *  *
C   IDPAR - INDEKS SLOWA ZEROWEGO BIEZACEGO PARAMETRU W IPMEM
C   LFORMB,LFORME - PIERWSZY I OSTATNI ELEMENT ROBOCZEJ LISTY PF
C   FORM2=TRUE, GDY PRZETWAEZAMY PARAMETRY II-GO RZEDU
C   SYNG = TRUE, GDY SA TO PARAMETRY SYGNALU
C
C
C.....BLOK KOMUNIKACJI ZE STRUMIENIAMI
      LOGICAL  ERRFLG
      COMMON /STREAM/  ERRFLG, LINE, IBUF2(265), IBUF3(7),JUNK(260)
C
C
C
C
      K=0
      IF(.NOT. PQ) K=IDPAR
      I=IPMEM(IDPAR+3)
      IF(.NOT.PQ) I=IPMEM(LFORMB+4)
      ISTPF=.FALSE.
 100  IF(I.EQ.K) RETURN
      IF(IPMEM(I).NE.7) GO TO 200
C   TYP FORMALNY
      IF(IPMEM(I+2) .EQ. NM) GO TO 400
 200  I=IPMEM(I+3)
      GO TO 100
C  ZNALEZIONY TYP
 400  ISTPF=.TRUE.
      IF(.NOT.PQ)RETURN
      LINE=IPMEM(IDPAR+1)
      CALL MERR(316,NM)
      RETURN
      END
      SUBROUTINE PUTPF(ID)
C
C  * * * * * * * * * * * * * * * * * * * * * * * * *
C   WSTAWIA PARAMETR O IDENTYFIKATORZE ID DO LISTY PF
C  * * * * * * * * * * * * * * * * * * * * * * * ***
C
      IMPLICIT INTEGER(A-Z)
C
C ..... ZMIENNE GLOBALNE
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
C             COM    - OBSZAR KOMUNIKACYJNY STRUMIENI
C             LMEM   - (=5000) ROZMIAR CALEJ PAMIECI GLOWNEJ
C             LPMEM  - PODZIAL PAMIECI NA CZESCI  IPMEM  I  ISMEM
C             IRECN  - INDEKS SZCZYTU STOSU STALYCH REAL
C             ISFIN  - INDEKS SZCZYTU SLOWNIKA PROTOTYPOW
C
C             LPMSYS - INDEKS PIERWSZEGO SLOWA W IPMEM OBSZARU NIEPRZEZ-
C                      NACZONEGO NA PROTOTYPY SYSTEMOWE
C             LPML   - INDEKS PIERWSZEGO SLOWA OBSZARU WOLNEGO W IPMEM
C             LPMF   - INDEKS OSTATNIEGO SLOWA WOLNEGO OBSZARU W IPMEM
C
C
C
      COMMON /DWORK/ IDPAR,LFORMB,LFORME,FORM2,SYGN
      LOGICAL FORM2, SYGN
C
C  *  *  *  *  *  *   *  *  *  *  *  *  *  *  *  *  *   *  *  *
C   IDPAR - INDEKS SLOWA ZEROWEGO BIEZACEGO PARAMETRU W ISMEM
C   LFORMB,LFORME - PIERWSZY I OSTATNI ELEMENT ROBOCZEJ LISTY PF
C   FORM2=TRUE, GDY PRZETWAEZAMY PARAMETRY II-GO RZEDU
C   SYGN = TRUE, GDY TO SA PARAMETRY SYGNALU
C
C
C
C
      INSYS=.TRUE.
      K=MGETM(2,341)
      IPMEM(K) = ID
      IPMEM(LFORME+1) = K
      LFORME=K
      INSYS=.FALSE.
      RETURN
      END
      SUBROUTINE COPY
C
C  * * * * * * * * * * * * * * * * * * * * * * * * *
C   KOPIUJE LISTE PF DO PAMIECI UZYTKOWNIKA
C   I DOWIAZUJE DO PROTOTYPU INDPR
C  * * * * * * * * * * * * * * * * * * * ** * * *  *
C
      IMPLICIT INTEGER(A-Z)
C
C ..... ZMIENNE GLOBALNE
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
C             COM    - OBSZAR KOMUNIKACYJNY STRUMIENI
C             LMEM   - (=5000) ROZMIAR CALEJ PAMIECI GLOWNEJ
C             LPMEM  - PODZIAL PAMIECI NA CZESCI  IPMEM  I  ISMEM
C             IRECN  - INDEKS SZCZYTU STOSU STALYCH REAL
C             ISFIN  - INDEKS SZCZYTU SLOWNIKA PROTOTYPOW
C
C             LPMSYS - INDEKS PIERWSZEGO SLOWA W IPMEM OBSZARU NIEPRZEZ-
C                      NACZONEGO NA PROTOTYPY SYSTEMOWE
C             LPML   - INDEKS PIERWSZEGO SLOWA OBSZARU WOLNEGO W IPMEM
C             LPMF   - INDEKS OSTATNIEGO SLOWA WOLNEGO OBSZARU W IPMEM
C
C
C
      COMMON /DWORK/ IDPAR,LFORMB,LFORME,FORM2,SYGN
      LOGICAL FORM2, SYGN
C
C  *  *  *  *  *  *   *  *  *  *  *  *  *  *  *  *  *   *  *  *
C   IDPAR - INDEKS SLOWA ZEROWEGO BIEZACEGO PARAMETRU W ISMEM
C   LFORMB,LFORME - PIERWSZY I OSTATNI ELEMENT ROBOCZEJ LISTY PF
C   FORM2=TRUE, GDY PRZETWAEZAMY PARAMETRY II-GO RZEDU
C   SYN = TRUE, GDY TO SA PARAMETRY SYGNALU
C
C
      COMMON / DGLOB/ INDICT,INDSPR,INDPR,IHBEG,LASTPR,INDPREF
C
C  **  **  **  **  ***  **  **  **  **  **  **  **  **  **  **  **  **
C    BLOK ZAWIERA DANE O PRZETWARZANYM PROTOTYPIE
C        INDICT  -  INDEKS PRZETWARZANEGO PROTTYPU W ISDICT
C        INDSPR  -  IDENTYFIKATOR PROTOTYPU W ISMEM
C        INDPR  -  IDENTYFIKATOR PROTOTYPU W IPMEM
C        IHBEG  -  ADRES PIERWSZEGO SLOWA TABLICY HASHU
C        IDPREF  -  IDENTYFIKATOR BEZPOSREDNIEGO PREFIKSU PRZETWARZANEGO
C                  PROTOTYPU
C        LASTPR  -  IDENTYFIKATOR POPRZEDNIO PRZETWARZANEGO PROTOTYPU
C
C
C
C
      I=IPMEM(LFORMB+1)
      K=0
      L=0
      J=0
      IF(I.EQ.0) GO TO 400
      L=MGETM(1,341)
      J=L
 100  X=IPMEM(I)
      IPMEM(J) = X
C   JESLI TO SA PARAMETRY II-GO RZEDU,TO POPRAWIAMY SL NA INDPR
      IF(FORM2) IPMEM(X-1) = INDPR
      I=IPMEM(I+1)
      K=K+1
      IF(I.EQ.0) GO TO 200
      J=MGETM(1,341)
      GO TO 100
 200  CONTINUE
C   L - PIERWSZY PARAMETR
C   J - OSTATNI PARAMETR
C   TRZEBA ZAMIENIC ICH KOLEJNOSC
      I1=J
      I2=L
 300  IF (I1.GE.I2) GO TO 400
      X=IPMEM(I1)
      IPMEM(I1)=IPMEM(I2)
      IPMEM(I2)=X
      I1=I1+1
      I2=I2-1
      GO TO 300
 400  IPMEM(INDPR+3) = J
      IPMEM(INDPR+4) = K
      RETURN
      END
      SUBROUTINE TYPEF
C
C  * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C   PRZETWARZA TYP FORMALNY.
C   INFORMACJE O PARAMETRZE - W BLOKU DWORK
C  * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C
      IMPLICIT INTEGER(A-Z)
C
C ..... ZMIENNE GLOBALNE
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
C             COM    - OBSZAR KOMUNIKACYJNY STRUMIENI
C             LMEM   - (=5000) ROZMIAR CALEJ PAMIECI GLOWNEJ
C             LPMEM  - PODZIAL PAMIECI NA CZESCI  IPMEM  I  ISMEM
C             IRECN  - INDEKS SZCZYTU STOSU STALYCH REAL
C             ISFIN  - INDEKS SZCZYTU SLOWNIKA PROTOTYPOW
C
C             LPMSYS - INDEKS PIERWSZEGO SLOWA W IPMEM OBSZARU NIEPRZEZ-
C                      NACZONEGO NA PROTOTYPY SYSTEMOWE
C             LPML   - INDEKS PIERWSZEGO SLOWA OBSZARU WOLNEGO W IPMEM
C             LPMF   - INDEKS OSTATNIEGO SLOWA WOLNEGO OBSZARU W IPMEM
C
C
C
      COMMON /DWORK/ IDPAR,LFORMB,LFORME,FORM2,SYGN
      LOGICAL FORM2, SYGN
C
C  *  *  *  *  *  *   *  *  *  *  *  *  *  *  *  *  *   *  *  *
C   IDPAR - INDEKS SLOWA ZEROWEGO BIEZACEGO PARAMETRU W ISMEM
C   LFORMB,LFORME - PIERWSZY I OSTATNI ELEMENT ROBOCZEJ LISTY PF
C   FORM2=TRUE, GDY PRZETWAEZAMY PARAMETRY II-GO RZEDU
C   SYGN = TRUE, GDY TO SA PARAMETRY SYGNALU
C
C
C
C.....BLOK KOMUNIKACJI ZE STRUMIENIAMI
      LOGICAL  ERRFLG
      COMMON /STREAM/  ERRFLG, LINE, IBUF2(265), IBUF3(7),JUNK(260)
C
C
C   UTWORZENIE OPISU
      LINE=IPMEM(IDPAR+1)
      KIND=17
      IF(FORM2) KIND=20
      ID=INITPR(KIND,IPMEM(IDPAR+2))
C   WSTAWIENIE DO LISTY PF
      CALL PUTPF(ID)
      RETURN
      END
      SUBROUTINE VARIAB
C
C  * * * * * * * * * * * * * * * * * * * * * * ** * * * *
C   PRZETWARZA PARAMETR BEDACY ZMIENNA
C   INFORMACJE O PARAMETRZE - W BLOKU DWORK
C  * * * * * * * * * * * * * * * * * * * * * * * * * * *
C
      IMPLICIT INTEGER(A-Z)
C
C ..... ZMIENNE GLOBALNE
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
C             COM    - OBSZAR KOMUNIKACYJNY STRUMIENI
C             LMEM   - (=5000) ROZMIAR CALEJ PAMIECI GLOWNEJ
C             LPMEM  - PODZIAL PAMIECI NA CZESCI  IPMEM  I  ISMEM
C             IRECN  - INDEKS SZCZYTU STOSU STALYCH REAL
C             ISFIN  - INDEKS SZCZYTU SLOWNIKA PROTOTYPOW
C
C             LPMSYS - INDEKS PIERWSZEGO SLOWA W IPMEM OBSZARU NIEPRZEZ-
C                      NACZONEGO NA PROTOTYPY SYSTEMOWE
C             LPML   - INDEKS PIERWSZEGO SLOWA OBSZARU WOLNEGO W IPMEM
C             LPMF   - INDEKS OSTATNIEGO SLOWA WOLNEGO OBSZARU W IPMEM
C
C
C
      COMMON /DWORK/ IDPAR,LFORMB,LFORME,FORM2,SYGN
      LOGICAL FORM2, SYGN
C
C  *  *  *  *  *  *   *  *  *  *  *  *  *  *  *  *  *   *  *  *
C   IDPAR - INDEKS SLOWA ZEROWEGO BIEZACEGO PARAMETRU W ISMEM
C   LFORMB,LFORME - PIERWSZY I OSTATNI ELEMENT ROBOCZEJ LISTY PF
C   FORM2=TRUE, GDY PRZETWAEZAMY PARAMETRY II-GO RZEDU
C   SYGN = TRUE, GDY TO SA PARAMETRY SYGNALU
C
C
C
      COMMON / DGLOB/ INDICT,INDSPR,INDPR,IHBEG,LASTPR,INDPREF
C
C  **  **  **  **  ***  **  **  **  **  **  **  **  **  **  **  **  **
C    BLOK ZAWIERA DANE O PRZETWARZANYM PROTOTYPIE
C        INDICT  -  INDEKS PRZETWARZANEGO PROTTYPU W ISDICT
C        INDSPR  -  IDENTYFIKATOR PROTOTYPU W ISMEM
C        INDPR  -  IDENTYFIKATOR PROTOTYPU W IPMEM
C        IHBEG  -  ADRES PIERWSZEGO SLOWA TABLICY HASHU
C        IDPREF  -  IDENTYFIKATOR BEZPOSREDNIEGO PREFIKSU PRZETWARZANEGO
C                  PROTOTYPU
C        LASTPR  -  IDENTYFIKATOR POPRZEDNIO PRZETWARZANEGO PROTOTYPU
C
C
      COMMON  / MASKS /  MTP,MSPR,MOTHERS,MPAR,MASKTP,NOTTP,
     * MPROCES, MCOR, MERPF, MBLOCK,MHAND,MNOTVIR
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C   MASKI I WZORCE:
C    MTP - MASKA DO WYCINANIA INFORMACJI DOTYCZACYCH TYPOW ZE SLOWA ZEROWEGO
C    MOTHERS -      --     --      --      --     --  INNYCH PROTOTYPOW
C    MPAR -    --    --    --    --    --    --       ZMIENNYCH I PARAMETROW
C    MSPR - MASKA DLA SYSPREF  ( DLA PROTOTYPOW SYNTAKTYCZNYCH )
C    MASKTP - ZAPRZECZENIE MASKI  MTP
C    NOTTP - WZORZEC DLA NIE-TYPU  ( 1 )
C    MPROCES - WZORZEC DLA PROCESU  ( 5 )
C    MCOR - WZORZEC DLA COROUTINY
C    MERPF - MASKA DO WYKRYWANIA BLEDNYCH LIST PARAMETROW
C    MBLOCK - WZORZEC DLA BLOKU  ( 0 PRZY PRZECIECIU Z  MOTHERS )
C
C
C
      COMMON  / YNIT /  NULLWD(23), SIZEPR(23), NULLPOZ(23), CONSTWD,
     *  VARWD,VARPOM,INPFW,OUTPFW, INOUT
cdsw INTEGER  SIZEPR, CONSTWD, VARWD, VARPOM, OUTPFW
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C    NULLWD(I)  -  WZORZEC SLOWA ZEROWEGO DLA PROTOTYPU RODZAJU I
C    SIZEPR(I)  -  ROZMIAR POLA W IPMEM   --   --   --
C    NULLPOZ(I)  -  POZYCJA SLOWA ZEROWEGO  --   --    --
C    CONSTWD  -  WZORZEC SLOWA ZEROWEGO DLA    CONST
C    VARWD  -     --   --   --   --          DLA ZMIENNEJ
C    VARPOM  -     --   --   --   --           ZMIENNEJ POMOCNICZEJ
C    INPFW  -     --   --    --    --          ZMIEMNEJ INPUT
C    OUTPFW  -     --   --    --    --         ZMIENNEJ OUTPUT
C    INOUT   -     --    --     ---   --       ZMIENNEJ INOUT
C
C
C.....BLOK KOMUNIKACJI ZE STRUMIENIAMI
      LOGICAL  ERRFLG
      COMMON /STREAM/  ERRFLG, LINE, IBUF2(265), IBUF3(7),JUNK(260)
C
C
      LINE = IPMEM(IDPAR+1)
      NM=IPMEM(IDPAR+2)
C   .  .  .
C   UTWORZENIE OBIEKTU
      ID=MGETM(6,341)+4
      IPMEM(ID) = INPFW
      IF(IPMEM(IDPAR).EQ.9) IPMEM(ID)=OUTPFW
      IF(IPMEM(IDPAR).EQ.10) IPMEM(ID)=INOUT
C   WSTAWIENIE DO ZBIORU IDENTYFIKATOROW
      IF(FORM2) INSYS=.TRUE.
      K=IDPUT(NM,IPMEM(IHBEG))
      IF(K.EQ.0) GO TO 200
      IPMEM(K+2) = ID
C   WSTAWIENIE DO LISTY ATRYBUTOW
 200  CALL MADATR(ID,INDPR,341)
C   ZAPAMIETANIE 1 W POLU USED - DLA AIL
      IPMEM(ID+1)=1
C   WSTAWIENIE DO LISTY PF
      CALL PUTPF(ID)
C
C   ROZPOZNANIE TYPU
      NM=IPMEM(IDPAR+4)
      K=IFTYPE(NM)
      IPMEM(ID-3) = K
      IPMEM(ID-4) = IPMEM(IDPAR+5)
C   WSTAWIENIE APETYTU
      IPMEM(ID) = IAP(ID)
C   JESLI TYP JEST FORMALNY, TO POPRAWIAMY SLOWO ZEROWE
      IF(IAND(IPMEM(K),MTP) .NE. 6) RETURN
      IPMEM(ID) = IOR(IPMEM(ID),ISHFT(1,12))
C   JESLI TYP FORMALNY JEST NIELOKALNY, TO POPRAWIAMY SPECYFIKACJE
C   PROTOTYPU
      IF(LOCAL.EQ.2) RETURN
      IF(FORM2) GO TO 300
      CALL CHECK(INDPR)
C   JESLI SYGNAL - TO BLAD
 250  IF(SYGN) CALL MERR(361,NM)
      RETURN
C   JESLI TYP POCHODZI Z TEJ SAMEJ LISTY PARAMETROW CO PROCEDUURA FORMALNA,
C   TO DOBRZE
 300  IF(IPMEM(K-1).EQ.IPMEM(INDPR-1)) RETURN
      GO TO 250
      END
      SUBROUTINE PROCFUN
C
C  * * * * * * ** * * * * * * * * * * * * * * * * * * * * *
C   PRZETWARZA PROCEDURE LUB FUNKCJE FORMALNA I-GO RZEDU
C   INFORMACJE O PARAMETRZE W BLOKU DWORK
C  * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C
      IMPLICIT INTEGER(A-Z)
C
C ..... ZMIENNE GLOBALNE
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
C             COM    - OBSZAR KOMUNIKACYJNY STRUMIENI
C             LMEM   - (=5000) ROZMIAR CALEJ PAMIECI GLOWNEJ
C             LPMEM  - PODZIAL PAMIECI NA CZESCI  IPMEM  I  ISMEM
C             IRECN  - INDEKS SZCZYTU STOSU STALYCH REAL
C             ISFIN  - INDEKS SZCZYTU SLOWNIKA PROTOTYPOW
C
C             LPMSYS - INDEKS PIERWSZEGO SLOWA W IPMEM OBSZARU NIEPRZEZ-
C                      NACZONEGO NA PROTOTYPY SYSTEMOWE
C             LPML   - INDEKS PIERWSZEGO SLOWA OBSZARU WOLNEGO W IPMEM
C             LPMF   - INDEKS OSTATNIEGO SLOWA WOLNEGO OBSZARU W IPMEM
C
C
C
      COMMON / DGLOB/ INDICT,INDSPR,INDPR,IHBEG,LASTPR,INDPREF
C
C  **  **  **  **  ***  **  **  **  **  **  **  **  **  **  **  **  **
C    BLOK ZAWIERA DANE O PRZETWARZANYM PROTOTYPIE
C        INDICT  -  INDEKS PRZETWARZANEGO PROTTYPU W ISDICT
C        INDSPR  -  IDENTYFIKATOR PROTOTYPU W ISMEM
C        INDPR  -  IDENTYFIKATOR PROTOTYPU W IPMEM
C        IHBEG  -  ADRES PIERWSZEGO SLOWA TABLICY HASHU
C        IDPREF  -  IDENTYFIKATOR BEZPOSREDNIEGO PREFIKSU PRZETWARZANEGO
C                  PROTOTYPU
C        LASTPR  -  IDENTYFIKATOR POPRZEDNIO PRZETWARZANEGO PROTOTYPU
C
C
C
      COMMON /DWORK/ IDPAR,LFORMB,LFORME,FORM2,SYGN
      LOGICAL FORM2, SYGN
C
C  *  *  *  *  *  *   *  *  *  *  *  *  *  *  *  *  *   *  *  *
C   IDPAR - INDEKS SLOWA ZEROWEGO BIEZACEGO PARAMETRU W ISMEM
C   LFORMB,LFORME - PIERWSZY I OSTATNI ELEMENT ROBOCZEJ LISTY PF
C   FORM2=TRUE, GDY PRZETWAEZAMY PARAMETRY II-GO RZEDU
C   SYGN = TRUE, GDY TO SA PARAMETRY SYGNALU
C
C
      COMMON /DCOPIES/ INDPRC,IHBEGC, IDPARC,LFBC,LFEC
C
C  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
C   KOPIE ZMIENNYCH Z DGLOB I DWORK
C   INDPRC - KOPIA INDPR
C   IHBEGC - KOPIA IHBEG
C   IDPARC - KOPIA IDPAR
C   LFBC - KOPIA LFORMB
C   LFEC - KOPIA LFORME
C
C
      COMMON  / YNIT /  NULLWD(23), SIZEPR(23), NULLPOZ(23), CONSTWD,
     *  VARWD,VARPOM,INPFW,OUTPFW, INOUT
cdsw  INTEGER  SIZEPR, CONSTWD, VARWD, VARPOM, OUTPFW
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C    NULLWD(I)  -  WZORZEC SLOWA ZEROWEGO DLA PROTOTYPU RODZAJU I
C    SIZEPR(I)  -  ROZMIAR POLA W IPMEM   --   --   --
C    NULLPOZ(I)  -  POZYCJA SLOWA ZEROWEGO  --   --    --
C    CONSTWD  -  WZORZEC SLOWA ZEROWEGO DLA    CONST
C    VARWD  -     --   --   --   --          DLA ZMIENNEJ
C    VARPOM  -     --   --   --   --           ZMIENNEJ POMOCNICZEJ
C    INPFW  -     --   --    --    --          ZMIEMNEJ INPUT
C    OUTPFW  -     --   --    --    --         ZMIENNEJ OUTPUT
C   INOUT    -       --   --   --   ---        ZMIENNEJ INOUT
C
C
      COMMON  / MASKS /  MTP,MSPR,MOTHERS,MPAR,MASKTP,NOTTP,
     * MPROCES, MCOR, MERPF, MBLOCK,MHAND,MNOTVIR
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C   MASKI I WZORCE:
C    MTP - MASKA DO WYCINANIA INFORMACJI DOTYCZACYCH TYPOW ZE SLOWA ZEROWEGO
C    MOTHERS -      --     --      --      --     --  INNYCH PROTOTYPOW
C    MPAR -    --    --    --    --    --    --       ZMIENNYCH I PARAMETROW
C    MSPR - MASKA DLA SYSPREF  ( DLA PROTOTYPOW SYNTAKTYCZNYCH )
C    MASKTP - ZAPRZECZENIE MASKI  MTP
C    NOTTP - WZORZEC DLA NIE-TYPU  ( 1 )
C    MPROCES - WZORZEC DLA PROCESU  ( 5 )
C    MCOR - WZORZEC DLA COROUTINY (7)
C    MERPF - MASKA DO WYKRYWANIA BLEDNYCH LIST PARAMETROW
C    MBLOCK - WZORZEC DLA BLOKU  ( 0 PRZY PRZECIECIU Z  MOTHERS )
C
C.....BLOK KOMUNIKACJI ZE STRUMIENIAMI
      LOGICAL  ERRFLG
      COMMON /STREAM/  ERRFLG, LINE, IBUF2(265), IBUF3(7),JUNK(260)
C
C
C
C
      NM=IPMEM(IDPAR+2)
      LN=IPMEM(IDPAR+1)
      LINE=LN
C   .  .  .
C   BEGM - POCZATEK WOLNEGO POLA W PAMIECI SYSTEMOWEJ
      BEGM=LPML
      LP=LASTPR
C   UTWORZENIE OPISU PARAMETRU
      K=IPMEM(IDPAR) + 10
      I=INITPR(K,NM)
C   ZAPAMIETANIE KOPII
      INDPRC=INDPR
      IHBEGC=IHBEG
      IDPARC=IDPAR
      LFBC=LFORMB
      LFEC=LFORME
C   ZAMIANA ZMIENNYCH OKRESLAJACYCH PRZETWARZANY PROTOTYP
      INDPR=I
      IHBEG=I+10
      INSYS=.TRUE.
      LFORMB=MGETM(2,341)
      LFORME=LFORMB
      FORM2=.TRUE.
C
C  -  -  -  -  -  -  -  -  -  -  -   -  -  -  -  -  - -  -
C   PRZETWARZANIE LISTY PF II-GO RZEDU
C
C
      INSYS=.FALSE.
      IDPAR=IPMEM(IDPAR+4)
 100  IF(IDPAR.EQ.0) GO TO 500
      KD=IPMEM(IDPAR)
      IF(KD.EQ.7) GO TO 200
      IF(KD.GE.8) GO TO 300
C  PROCEDURA/FUNKCJA II-GO RZEDU
      CALL PROCF2
      GO TO 400
C  TYP FORMALNY
 200  CALL TYPEF
      GO TO 400
C  ZMIENNA
 300  CALL VARIAB
 400  IDPAR = IPMEM(IDPAR+3)
      GO TO 100
C
 500  CONTINUE
C
C   -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
C   CZYNNOSCI ORGANIZACYJNE(PAMIEC) I ZAKONCZENIE
C   PRZETWARZANIA  PROCEDURY/FUNKCJI
C
      LINE=LN
C   DOLACZENIE RESULT - JESLI TO JEST FUNKCJA
      IF(K.EQ.13.OR.K.EQ.15) GO TO 700
      I=MGETM(6,341) +4
      IPMEM(I) = OUTPFW
      CALL MADATR(I,INDPR,341)
      CALL PUTPF(I)
C   ZAPAMIETANIE IDENTYFIKATORA RESULT
      IPMEM(INDPR-5)=I
C  UWAGA - NAZWY RESULT NIE TRZEBA ZAPAMIETYWAC
 700  CONTINUE
C  PRZEPISANIE PROTOTYPU DO CZESCI UZYTKOWNIKA
      IF(K.EQ.14 .OR. K.EQ.16) GO TO 750
      I=INDPR-2
      J=7
      KD=MGETM(7,341)
      INDPR=KD+2
      GO TO 800
 750  I=INDPR-5
      J=10
      KD=MGETM(10,341)
      INDPR=KD+5
 800  CONTINUE
      DO 888 II=1,J
      I1=KD+II-1
      I2=I+II-1
 888  IPMEM(I1) = IPMEM(I2)
C   ZMIANA DOWIAZANIA NEXTDECL W PROTOTYPIE POPRZEDZAJACYM ( LP )
      IPMEM(LP+2) = INDPR
      IF(IPMEM(INDPR+2).EQ.0) LASTPR = INDPR
C   PRZEPISANIE LISTY PF
      CALL COPY
C   POPRAWIENIE ID PROTOTY@PU W TABLICY HASH
      I=MEMBER(NM,IPMEM(IHBEGC))
      IPMEM(I+2) = INDPR
C   POPRAWIENIE ID PROTOTYPU W LISCIE ATRYBUTOW
      I=IPMEM(INDPR-2)
C  I - NUMER ATRYBUTU
      KD=IPMEM(INDPRC+6)
 920  J=IPMEM(KD)
      IF(IPMEM(J-2).EQ.I) GO TO 950
       KD=IPMEM(KD+1)
      GO TO 920
C  KD - ATRYBUT
 950  IPMEM(KD) = INDPR
C   COFNIECIE PAMIECI SYSTEMOWEJ
      LPML=BEGM
C   COFNIECIE ZMIENNYCH Z DWORK
      LFORMB=LFBC
      LFORME=LFEC
      IDPAR=IDPARC
      FORM2=.FALSE.
C   DOLACZENIE SIEBIE DO LISTY PF
      CALL PUTPF(INDPR)
C   COFNIECIE ZMIENNYCH Z DGLOB
      I=INDPR
      INDPR=INDPRC
      IHBEG=IHBEGC
C
C   JESLI FUNKCJA - TO NADANIE TYPU
      IF(K.EQ.13 .OR. K.EQ.15) GO TO 1000
      NM=IPMEM(IDPAR+5)
      J=IFTYPE(NM)
      IPMEM(I-3) = J
      IPMEM(I-4) = IPMEM(IDPAR+6)
      K=IPMEM(I-5)
      IPMEM(K-4) = IPMEM(I-4)
      IPMEM(K-3) = J
C   JESLI TO JEST TYP FORMALNY, TO POPRAWIAMY SLOWO ZEROWE
      IF(IAND(IPMEM(J),MTP).NE.6)  GO TO 1000
      IPMEM(I) = IOR(IPMEM(I),ISHFT(1,12))
      IPMEM(K) = IOR(IPMEM(K),ISHFT(1,12))
C   JESLI TYP FORMALNY JEST NIELOKALNY, TO ZLE DLA SYGNALU
      IF(LOCAL.EQ.2 .OR. .NOT.SYGN) GO TO 1000
      CALL MERR(361,NM)
 1000 CONTINUE
C   WYPISUJEMY INFORMACJE O PARAMETRACH II-GO RZEDU
C     K=IPMEM(I+3)
C     IF(IPMEM(I+4).EQ.0)RETURN
C     J=IPMEM(I+4)+K-1
C     DO 1111 II=K,J
C     I=IPMEM(II)
C     NM=ISHFT(IAND(IPMEM(I),MPAR),-4)
C     IF(NM.GE.4) GO TO 1112
C     CALL ffwrite(BO(2),"IDENTYFIKATOR =",17)
C     CALL ffwrint(BO(2),I)
C     CALL WRITEPR(I)
C     GO TO 1111
C1112 CALL WATTR(I)
C1111 CONTINUE
C  .  .  .
      RETURN
      END
	SUBROUTINE PROCF2
C
C  **  **  **  ** * * * * * * * * * * * * * * * * *
C   PRZETWARZA PROCEDURE/FUNKCJE FORMALNA II-GO RZEDU
C   PARAMETR DANY PRZEZ ZMIENNE Z BLOKU DWORK
C  * * * * * * * * * * * * * * * * * * * * * * *** *
C
	IMPLICIT INTEGER(A-Z)
C
C
C ..... ZMIENNE GLOBALNE
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
C             COM    - OBSZAR KOMUNIKACYJNY STRUMIENI
C             LMEM   - (=5000) ROZMIAR CALEJ PAMIECI GLOWNEJ
C             LPMEM  - PODZIAL PAMIECI NA CZESCI  IPMEM  I  ISMEM
C             IRECN  - INDEKS SZCZYTU STOSU STALYCH REAL
C             ISFIN  - INDEKS SZCZYTU SLOWNIKA PROTOTYPOW
C
C             LPMSYS - INDEKS PIERWSZEGO SLOWA W IPMEM OBSZARU NIEPRZEZ-
C                      NACZONEGO NA PROTOTYPY SYSTEMOWE
C             LPML   - INDEKS PIERWSZEGO SLOWA OBSZARU WOLNEGO W IPMEM
C             LPMF   - INDEKS OSTATNIEGO SLOWA WOLNEGO OBSZARU W IPMEM
C
C
C
C
      COMMON /DWORK/ IDPAR,LFORMB,LFORME,FORM2,SYGN
      LOGICAL FORM2, SYGN
C
C  *  *  *  *  *  *   *  *  *  *  *  *  *  *  *  *  *   *  *  *
C   IDPAR - INDEKS SLOWA ZEROWEGO BIEZACEGO PARAMETRU W ISMEM
C   LFORMB,LFORME - PIERWSZY I OSTATNI ELEMENT ROBOCZEJ LISTY PF
C   FORM2=TRUE, GDY PRZETWAEZAMY PARAMETRY II-GO RZEDU
C   SYGN = TRUE, GDY TO SA PARAMETRY SYGNALU
C
C
C.....BLOK KOMUNIKACJI ZE STRUMIENIAMI
      LOGICAL  ERRFLG
      COMMON /STREAM/  ERRFLG, LINE, IBUF2(265), IBUF3(7),JUNK(260)
C
C
	LINE=IPMEM(IDPAR+1)
	NM=IPMEM(IDPAR+2)
C   DOLACZENIE DO LISTY PF ATRYBUTU DODATKOWEGO(BRAK)
C   UTWORZENIE PROTOTYPU
	K=IPMEM(IDPAR)+15
	I=INITPR(K,NM)
C   DOLACZENIE DO LISTY PF
	CALL PUTPF(I)
C   EWENTUALNIE TYP FUNKCJI - BRAK
       IF(K.EQ.18) RETURN
      IPMEM(I-3)=NRUNIV
	RETURN
	END
      SUBROUTINE VIRTCOM
C
C  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C   SPRAWDZA KOMPATYBILNOSC WIRTUALI

C   * * * * * * * * * * * * * * * * * ** * * * * * * * * * * *
C
      IMPLICIT INTEGER (A-Z)
      LOGICAL PARCOM,P
C
C ..... ZMIENNE GLOBALNE
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
C             COM    - OBSZAR KOMUNIKACYJNY STRUMIENI
C             LMEM   - (=5000) ROZMIAR CALEJ PAMIECI GLOWNEJ
C             LPMEM  - PODZIAL PAMIECI NA CZESCI  IPMEM  I  ISMEM
C             IRECN  - INDEKS SZCZYTU STOSU STALYCH REAL
C             ISFIN  - INDEKS SZCZYTU SLOWNIKA PROTOTYPOW
C
C             LPMSYS - INDEKS PIERWSZEGO SLOWA W IPMEM OBSZARU NIEPRZEZ-
C                      NACZONEGO NA PROTOTYPY SYSTEMOWE
C             LPML   - INDEKS PIERWSZEGO SLOWA OBSZARU WOLNEGO W IPMEM
C             LPMF   - INDEKS OSTATNIEGO SLOWA WOLNEGO OBSZARU W IPMEM
C
C
C
      COMMON / DGLOB/ INDICT,INDSPR,INDPR,IHBEG,LASTPR,INDPREF
C
C  **  **  **  **  ***  **  **  **  **  **  **  **  **  **  **  **  **
C    BLOK ZAWIERA DANE O PRZETWARZANYM PROTOTYPIE
C        INDICT  -  INDEKS PRZETWARZANEGO PROTTYPU W ISDICT
C        INDSPR  -  IDENTYFIKATOR PROTOTYPU W ISMEM
C        INDPR  -  IDENTYFIKATOR PROTOTYPU W IPMEM
C        IHBEG  -  ADRES PIERWSZEGO SLOWA TABLICY HASHU
C        IDPREF  -  IDENTYFIKATOR BEZPOSREDNIEGO PREFIKSU PRZETWARZANEGO
C                  PROTOTYPU
C        LASTPR  -  IDENTYFIKATOR POPRZEDNIO PRZETWARZANEGO PROTOTYPU
C
C
C
      COMMON  / MASKS /  MTP,MSPR,MOTHERS,MPAR,MASKTP,NOTTP,
     * MPROCES, MCOR, MERPF, MBLOCK,MHAND,MNOTVIR
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C   MASKI I WZORCE:
C    MTP - MASKA DO WYCINANIA INFORMACJI DOTYCZACYCH TYPOW ZE SLOWA ZEROWEGO
C    MOTHERS -      --     --      --      --     --  INNYCH PROTOTYPOW
C    MPAR -    --    --    --    --    --    --       ZMIENNYCH I PARAMETROW
C    MSPR - MASKA DLA SYSPREF  ( DLA PROTOTYPOW SYNTAKTYCZNYCH )
C    MASKTP - ZAPRZECZENIE MASKI  MTP
C    NOTTP - WZORZEC DLA NIE-TYPU  ( 1 )
C    MPROCES - WZORZEC DLA PROCESU  ( 5 )
C    MCOR - WZORZEC DLA COROUTINY (7)
C    MERPF - MASKA DO WYKRYWANIA BLEDNYCH LIST PARAMETROW
C    MBLOCK - WZORZEC DLA BLOKU  ( 0 PRZY PRZECIECIU Z  MOTHERS )
C
C
C
C.....BLOK KOMUNIKACJI ZE STRUMIENIAMI
      LOGICAL  ERRFLG
      COMMON /STREAM/  ERRFLG, LINE, IBUF2(265), IBUF3(7),JUNK(260)
C
C
      COMMON /DWV/ NM,INDV,FORM2,TPVI,INDPR1,INDV1
      LOGICAL FORM2,TPVI
C
C   ** ** ** ** ** ** ** ** ** ** *** ** ** ** ** ** **
C  ROBOCZY BLOK DO KOMPATYBILNOSCI VIRTUALI
C   NM - NAZWA WIRTUALA
C   INDV - IDENTYFIKATOR WYZSZEGO VIRTUALA
C   FORM2 - TRUE, GDY PRZETWARZAMY PF II-GO RZEDU
C   TPVI - GDY CHODZI O TYP FUNKCJI WIRTUALNEJ
C   INDPR1,INDV1 - IDENTYFIKATORY PRZETWARZANYCH PROCEDUR/FUNKCJI
C     FORMALNYCH
C
C
      TPVI=.FALSE.
      FORM2=.FALSE.
      INDV=IPMEM(INDPR+26)
      NM=IPMEM(INDSPR+10)
      LINE=IPMEM(INDSPR+9)
C   KONTROLA RODZAJOW WIRTUALI
      IRU=ISHFT(IAND(IPMEM(INDV),MOTHERS),-8)
      IRL=ISHFT(IAND(IPMEM(INDPR),MOTHERS),-8)
      IF (IRU.EQ.IRL) GO TO 50
C   BLAD RODZAJOW
      CALL MERR(331,NM)
C   JESLI TO SA FUNKCJE - TO SPRAWDZAMY TYPY
 50   IF(IRL.EQ.4 .OR. IRU.EQ.4) GO TO 100
      TPVI=.TRUE.
      CALL TYPECOM(INDV,INDPR)
      TPVI = .FALSE.
 100  I=IPMEM(INDPR+3)
      IL=IPMEM(INDPR+4)
C   DLA FUNKCJI TRZEBA POMINAC RESULT
      IF(IRL.EQ.2) IL=IL-1
      J=IPMEM(INDV+3)
      JU=IPMEM(INDV+4)
      IF(IRU.EQ.2) JU=JU-1
C  I,IL - POCZATEK I DLUGOSC LISTY PF DLA INDPR
C  J,JU - POCZATEK I DLUGOSC LISTY PF DLA INDV
      IF(IL+JU.EQ.0) GO TO 1000
      IF(IL.NE.JU) GO TO 800
C   ZGODNA LICZBA PARAMETROW
 200  IL=IL+I-1
      JU=JU+J-1
      NM=NEMPTY
C   SPRAWDZENIE ZGODNOSCI PARAMETROW
C   PROCEDURA PARCOM DAJE TRUE, GDY TRZEBA
C   DALEJ SPRAWDZAC ZGODNOSC PF II-GO RZEDU
C  (TZN. SA TO PROCEDURY/FUNKCJE)
 300  IF(.NOT.PARCOM(IPMEM(J),IPMEM(I))) GO TO 700
C   SPRAWDZAMY ZGODNOSC PF II-GO RZEDU
      FORM2=.TRUE.
      INDPR1=IPMEM(I)
      INDV1=IPMEM(J)
      IRL1=ISHFT(IAND(IPMEM(INDPR1),MOTHERS),-8)
      IRU1=ISHFT(IAND(IPMEM(INDV1),MOTHERS),-8)
      I1=IPMEM(INDPR1+3)
      IL1=IPMEM(INDPR1+4)
C   DLA FUNKCJI - POMIJAMY RESULT
      IF(IRL1.EQ.2) IL1 = IL1-1
      J1=IPMEM(INDV1+3)
      JU1=IPMEM(INDV1+4)
      IF(IRU1.EQ.2) JU1 = JU1-1
      IF(IL1+JU1.EQ.0) GO TO 600
      IF(IL1.NE.JU1) GO TO 500
 350  IL1=IL1+I1-1
      JU1=JU1+J1-1
C   SPRAWDZANIE ZGODNOSCI PARAMETROW II-GO RZEDU
 400  P=PARCOM(IPMEM(J1),IPMEM(I1))
      I1=I1+1
      J1=J1+1
      IF(I1.LE.IL1.AND.J1.LE.JU1) GO TO 400
      GO TO 600
C   NIEZGODNA LICZBA PARAMETROW
 500  CONTINUE
      IF(IL1.LT.JU1) GO TO 530
C   SPRAWDZAMY,CZY LISTA KROTSZA JEST BLEDNA
      IF(IAND(IPMEM(INDV1),MERPF).NE.0) GO TO 550
      CALL MERR(336,NM)
      GO TO 550
 530  IF(IAND(IPMEM(INDPR1),MERPF).NE.0) GO TO 550
      CALL MERR(336,NM)
 550  IF(IL1*JU1.NE.0) GO TO 350
 600  CONTINUE
C   KONIEC SPRAWDZANIA PARAMETROW II-GO RZEDU
      FORM2=.FALSE.
 700  I=I+1
      J=J+1
      IF(I.LE.IL.AND.J.LE.JU) GO TO 300
C   KONIEC PARAMETROW
      GO TO 1000
C  NIEZGODNA LICZBA PARAMETROW
 800  IF(IL.LT.JU) GO TO 850
      IF(IAND(IPMEM(INDV),MERPF).NE.0) GO TO 900
C   KROTSZA LISTA PF NIE JEST BLEDNA
      CALL MERR(333,NM)
      GO TO 900
 850  IF(IAND(IPMEM(INDPR),MERPF).NE.0) GO TO 900
      CALL MERR(333,NM)
 900  IF(IL*JU.NE.0) GO TO 200
C
 1000 CONTINUE
C   ZAKONCZENIE
      RETURN
      END
      LOGICAL FUNCTION PARCOM(PARU,PARL)
C
C  * * * * * * * * * * * * * * * * * * * * * * ** * * * * * * * * * *
C   SPRAWDZA ZGODNOSCI PARAMETROW O IDENTYFIKATORACH PARU I PARL
C   PARCOM=.TRUE., GDY OBA PARAMETRY SA PROCEDURA LUB FUNKCJA
C   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C
      IMPLICIT INTEGER(A-Z)
C
C ..... ZMIENNE GLOBALNE
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
C             COM    - OBSZAR KOMUNIKACYJNY STRUMIENI
C             LMEM   - (=5000) ROZMIAR CALEJ PAMIECI GLOWNEJ
C             LPMEM  - PODZIAL PAMIECI NA CZESCI  IPMEM  I  ISMEM
C             IRECN  - INDEKS SZCZYTU STOSU STALYCH REAL
C             ISFIN  - INDEKS SZCZYTU SLOWNIKA PROTOTYPOW
C
C             LPMSYS - INDEKS PIERWSZEGO SLOWA W IPMEM OBSZARU NIEPRZEZ-
C                      NACZONEGO NA PROTOTYPY SYSTEMOWE
C             LPML   - INDEKS PIERWSZEGO SLOWA OBSZARU WOLNEGO W IPMEM
C             LPMF   - INDEKS OSTATNIEGO SLOWA WOLNEGO OBSZARU W IPMEM
C
C
C
      COMMON /DWV/ NM,INDV,FORM2,TPVI,INDPR1,INDV1
      LOGICAL FORM2,TPVI
C
C   ** ** ** ** ** ** ** ** ** ** *** ** ** ** ** ** **
C  ROBOCZY BLOK DO KOMPATYBILNOSCI VIRTUALI
C   NM - NAZWA WIRTUALA
C   INDV - IDENTYFIKATOR WYZSZEGO VIRTUALA
C   FORM2 - TRUE, GDY PRZETWARZAMY PF II-GO RZEDU
C   TPVI - GDY CHODZI O TYP FUNKCJI WIRTUALNEJ
C   INDPR1,INDV1 - IDENTYFIKATORY PRZETWARZANYCH PROCEDUR/FUNKCJI
C     FORMALNYCH
C
C
C
      COMMON  / MASKS /  MTP,MSPR,MOTHERS,MPAR,MASKTP,NOTTP,
     * MPROCES, MCOR, MERPF, MBLOCK,MHAND,MNOTVIR
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C   MASKI I WZORCE:
C    MTP - MASKA DO WYCINANIA INFORMACJI DOTYCZACYCH TYPOW ZE SLOWA ZEROWEGO
C    MOTHERS -      --     --      --      --     --  INNYCH PROTOTYPOW
C    MPAR -    --    --    --    --    --    --       ZMIENNYCH I PARAMETROW
C    MSPR - MASKA DLA SYSPREF  ( DLA PROTOTYPOW SYNTAKTYCZNYCH )
C    MASKTP - ZAPRZECZENIE MASKI  MTP
C    NOTTP - WZORZEC DLA NIE-TYPU  ( 1 )
C    MPROCES - WZORZEC DLA PROCESU  ( 5 )
C    MCOR - WZORZEC DLA COROUTINY (7)
C    MERPF - MASKA DO ROZPOZNANIA BLEDNYCH LIST PF
C    MBLOCK - WZORZEC DLA BLOKU  ( 0 PRZY PRZECIECIU Z  MOTHERS )
C
C
C
C.....BLOK KOMUNIKACJI ZE STRUMIENIAMI
      LOGICAL  ERRFLG
      COMMON /STREAM/  ERRFLG, LINE, IBUF2(265), IBUF3(7),JUNK(260)
C
C
      IDU=ISHFT(IAND(IPMEM(PARU),MPAR),-4)
      IDL=ISHFT(IAND(IPMEM(PARL),MPAR),-4)
      PARCOM=.FALSE.
C   .  .  .
C   KONTROLA RODZAJOW
      IF(IDU.EQ.IDL) GO TO 100
C   NIEZGODNE RODZAJE
      I=334
      IF (FORM2) I=337
      CALL MERR(I,NM)
C   JESLI OBA PARAMETRY SA ZMIENNYMI, TO KONTROLA TYPOW
 100  IF((IDU.EQ.5.OR.IDU.EQ.6.OR.IDU.EQ.9)
     * .AND. (IDL.EQ.5.OR.IDL.EQ.6.OR.IDL.EQ.9)) GO TO 300
      IF(IDU.NE.2.AND.IDU.NE.3 .OR. IDL.NE.2.AND.IDL.NE.3)
     *  RETURN
      PARCOM=.TRUE.
C   JESLI OBIE FUNKCJE I-GO RZEDU - TO KONTROLA TYPOW
      IF(IDU.NE.2.OR.IDL.NE.2) RETURN
      IF(FORM2) RETURN
C   KONTROLA TYPOW
 300  CALL TYPECOM(PARU,PARL)
      RETURN
      END
      SUBROUTINE TYPECOM (TPU,TPL)
C
C  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C   KONTROLA ZGODNOSCI TYPOW
C   TPU,TPL - IDENTYFIKATORY ZMIENNYCH(FUNKCJI)
C   TPVI=.TRUE., GDY TO SA TYPY FUNKCJI VIRTUALNYCH
C  * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C
      IMPLICIT INTEGER(A-Z)
      LOGICAL POMU,POML,BPREF
C
C ..... ZMIENNE GLOBALNE
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
C             COM    - OBSZAR KOMUNIKACYJNY STRUMIENI
C             LMEM   - (=5000) ROZMIAR CALEJ PAMIECI GLOWNEJ
C             LPMEM  - PODZIAL PAMIECI NA CZESCI  IPMEM  I  ISMEM
C             IRECN  - INDEKS SZCZYTU STOSU STALYCH REAL
C             ISFIN  - INDEKS SZCZYTU SLOWNIKA PROTOTYPOW
C
C             LPMSYS - INDEKS PIERWSZEGO SLOWA W IPMEM OBSZARU NIEPRZEZ-
C                      NACZONEGO NA PROTOTYPY SYSTEMOWE
C             LPML   - INDEKS PIERWSZEGO SLOWA OBSZARU WOLNEGO W IPMEM
C             LPMF   - INDEKS OSTATNIEGO SLOWA WOLNEGO OBSZARU W IPMEM
C
C
C
      COMMON  / MASKS /  MTP,MSPR,MOTHERS,MPAR,MASKTP,NOTTP,
     * MPROCES, MCOR, MERPF, MBLOCK,MHAND,MNOTVIR
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C   MASKI I WZORCE:
C    MTP - MASKA DO WYCINANIA INFORMACJI DOTYCZACYCH TYPOW ZE SLOWA ZEROWEGO
C    MOTHERS -      --     --      --      --     --  INNYCH PROTOTYPOW
C    MPAR -    --    --    --    --    --    --       ZMIENNYCH I PARAMETROW
C    MSPR - MASKA DLA SYSPREF  ( DLA PROTOTYPOW SYNTAKTYCZNYCH )
C    MASKTP - ZAPRZECZENIE MASKI  MTP
C    NOTTP - WZORZEC DLA NIE-TYPU  ( 1 )
C    MPROCES - WZORZEC DLA PROCESU  ( 5 )

C    MERPF - MASKA DO ROZPOZNAWANIA BLEDNYCH LIST PARAMETROW
C    MBLOCK - WZORZEC DLA BLOKU  ( 0 PRZY PRZECIECIU Z  MOTHERS )
C
C
C
      COMMON /DWV/ NM,INDV,FORM2,TPVI,INDPR1,INDV1
      LOGICAL FORM2,TPVI
C
C   ** ** ** ** ** ** ** ** ** ** *** ** ** ** ** ** **
C  ROBOCZY BLOK DO KOMPATYBILNOSCI VIRTUALI
C   NM - NAZWA WIRTUALA
C   INDV - IDENTYFIKATOR WYZSZEGO VIRTUALA
C   FORM2 - TRUE, GDY PRZETWARZAMY PF II-GO RZEDU
C   TPVI - GDY CHODZI O TYP FUNKCJI WIRTUALNEJ
C   INDPR1,INDV1 - IDENTYFIKATORY PRZETWARZANYCH PROCEDUR/FUNKCJI
C     FORMALNYCH
C
C
C
C  U - TYP WYZSZY
C   IARU,IARL - ILOSC ARRAY OF
C  ITU,ITL - IDENTYFIKATORY TYPOW
C   IDENU,IDNEL - WARTOSCI MTP ZE SLOWA ZEROWEGO TYPOW
      ITU=IPMEM(TPU-3)
      ITL=IPMEM(TPL-3)
      IARU=IPMEM(TPU-4)
      IARL=IPMEM(TPL-4)
C   JESLI TYPY SA IDENTYCZNE, TO DOBRZE
      IF(IARU.EQ.IARL.AND.ITL.EQ.ITU) RETURN
C   JESLI JEDEN Z TYPOW JEST UNIWERSALNY, TO DOBRZE
      IF(ITL.EQ.NRUNIV.OR.ITU.EQ.NRUNIV) RETURN
C   JESLI TYPY ROZNIA SIE TYLKO ARRAY OF , TO ZLE
      IF(IARU.NE.IARL.AND.ITL.EQ.ITU) GO TO 999
      IDENU=IAND(IPMEM(ITU),MTP)
      IDENL=IAND(IPMEM(ITL),MTP)
      IF(IDENU.EQ.6) GO TO 500
C   TYP WYZSZY NIE JEST FORMALNY
C   JESLI NIE ZGADZA SIE ARRAY OF , TO BLAD
      IF(IARU.NE.IARL) GO TO 999
C   JESLI SA TABLICOWE, TO MUSZA BYC ROWNE
      IF(IARU.NE.0.AND.ITU.NE.ITL) GO TO 999
C   JESLI JESZCZE SA TU TYPY PRYMITYWNE, TO BLAD(MUSZA BYC ROWNE)
      IF(IDENU.GE.8 .AND. IDENU.LT.13) GO TO 999
      IF(IDENL .GE. 8 .AND. IDENL .LT. 13) GO TO 999
      IF(ITU.EQ.NRCOR) GO TO 200
      IF(ITU.EQ.NRPROC) GO TO 300
C   TYP WYZSZY JEST KLASOWY
C   TYPY MAJA BYC W SEKWENCJI PREFIKSOWEJ
      IF(IDENL.EQ.6) GO TO 999
      I=IPMEM(ITU-6)
      J=IPMEM(ITL-6 )
      IF(BPREF(ITL,I)) RETURN
      IF(TPVI) GO TO 999
      IF(BPREF(ITU,J)) RETURN
      GO TO 999
C   WYZSZY - SAMA COROUTINA
 200  IF(IDENL.EQ.5 .OR. IDENL.EQ.7) RETURN
      IF(BPREF(ITL,IPMEM(NRCOR-6))) RETURN
      IF(BPREF(ITL,IPMEM(NRPROC-6))) RETURN
      GO TO 999
C   WYZSZY - SAM PROCESS
 300  IF(IDENL.EQ.5) RETURN
      IF(BPREF(ITL,IPMEM(NRPROC-6))) RETURN
      GO TO 999
C
C   WYZSZY - TYP FORMALNY
 500  CONTINUE
      I=NRPAR(ITU,.TRUE.)
      J=NRPAR(ITL,.FALSE.)
C   I,J - NUMERY TYPOW W LISCIE INDV(INDPR)
C   JESLI TO SA PARAMETRY II-GO RZEDU, TO TAKZE
C   W LISCIE PF INDV1(INDPR1)
      IF(I+J.EQ.0) GO TO 700
      IF(I.NE.J) GO TO 999
      IF(IARU.NE.IARL) GO TO 999
C   TRZEBA SPARWDZIC, CZY OBA TYPY SA PARAMETRAMI
C    TEGO SAMEGO RZEDU
      I=IPMEM(ITU-1)
      J=IPMEM(ITL-1)
C  I,J - SLE
      POMU=IAND(IPMEM(I),MPAR).NE.0
      POML=IAND(IPMEM(J),MPAR).NE.0
      IF(POMU.AND.POML.OR..NOT.(POMU.OR.POML))RETURN
      GO TO 999
C
C   TO NIE JEST WLASNY PARAMETR
 700  CONTINUE
      IF(IARU.NE.0) GO TO 800
C  JESLI WYZSZY NIE JEST TABLICOWY, TO ZLE, GDY
C   NIZSZY JEST PRYMITYWNY NIETABLICOWY
      IF(IARL.NE.0) RETURN
      IF(IDENL.GE.8.AND.IDENL.LT.13) GO TO 999
      RETURN
C   WYZSZY JEST TYPEM TABLICOWYM
 800  IF(IDENL.EQ.6) RETURN
      IF(IARU.LE.IARL) RETURN
C  SYGNALIZACJA BLEDOW
 999  I=335
      IF (TPVI) I=332
      IF(FORM2) I=338
      CALL MERR(I,NM)
      RETURN
      END
      INTEGER FUNCTION NRPAR(IDT,UP)
C
C  * * * * * * *** * * * * * * * * * * * * * * * * * * * * *
C   SPRAWDZA,CZY TYP IDT JEST PARAMETREM INDPR(INDV)
C   UP=.TRUE. - CHODZI O WIRTUAL WYZSZY (INDV)
C   NRPAR - NUMER IDT JAKO PARAMETRU ( LUB 0)
C   JESLI FORM2=.TRUE., TO  BADA TEZ, CZY TYP JEST PARAMETREM/
C   INDV1(ODP. INDPR1)
C   * * * * * * * * * * * * * * * * * * * * * * * * * ** * * *
C
      IMPLICIT INTEGER(A-Z)
      LOGICAL UP,BPREF
C
C ..... ZMIENNE GLOBALNE
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
C             COM    - OBSZAR KOMUNIKACYJNY STRUMIENI
C             LMEM   - (=5000) ROZMIAR CALEJ PAMIECI GLOWNEJ
C             LPMEM  - PODZIAL PAMIECI NA CZESCI  IPMEM  I  ISMEM
C             IRECN  - INDEKS SZCZYTU STOSU STALYCH REAL
C             ISFIN  - INDEKS SZCZYTU SLOWNIKA PROTOTYPOW
C
C             LPMSYS - INDEKS PIERWSZEGO SLOWA W IPMEM OBSZARU NIEPRZEZ-
C                      NACZONEGO NA PROTOTYPY SYSTEMOWE
C             LPML   - INDEKS PIERWSZEGO SLOWA OBSZARU WOLNEGO W IPMEM
C             LPMF   - INDEKS OSTATNIEGO SLOWA WOLNEGO OBSZARU W IPMEM
C
C
C
      COMMON / DGLOB/ INDICT,INDSPR,INDPR,IHBEG,LASTPR,INDPREF
C
C  **  **  **  **  ***  **  **  **  **  **  **  **  **  **  **  **  **
C    BLOK ZAWIERA DANE O PRZETWARZANYM PROTOTYPIE
C        INDICT  -  INDEKS PRZETWARZANEGO PROTTYPU W ISDICT
C        INDSPR  -  IDENTYFIKATOR PROTOTYPU W ISMEM
C        INDPR  -  IDENTYFIKATOR PROTOTYPU W IPMEM
C        IHBEG  -  ADRES PIERWSZEGO SLOWA TABLICY HASHU
C        IDPREF  -  IDENTYFIKATOR BEZPOSREDNIEGO PREFIKSU PRZETWARZANEGO
C                  PROTOTYPU
C        LASTPR  -  IDENTYFIKATOR POPRZEDNIO PRZETWARZANEGO PROTOTYPU
C
C
C
      COMMON  / MASKS /  MTP,MSPR,MOTHERS,MPAR,MASKTP,NOTTP,
     * MPROCES, MCOR, MERPF, MBLOCK,MHAND,MNOTVIR
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C   MASKI I WZORCE:
C    MTP - MASKA DO WYCINANIA INFORMACJI DOTYCZACYCH TYPOW ZE SLOWA ZEROWEGO
C    MOTHERS -      --     --      --      --     --  INNYCH PROTOTYPOW
C    MPAR -    --    --    --    --    --    --       ZMIENNYCH I PARAMETROW
C    MSPR - MASKA DLA SYSPREF  ( DLA PROTOTYPOW SYNTAKTYCZNYCH )
C    MASKTP - ZAPRZECZENIE MASKI  MTP
C    NOTTP - WZORZEC DLA NIE-TYPU  ( 1 )
C    MPROCES - WZORZEC DLA PROCESU  ( 5 )
C    MCOR - WZORZEC DLA COROUTINY (7)
C    MERPF - MASKA DO ROZPOZNAWANIA BLEDNYCH LIST PARAMETROW
C    MBLOCK - WZORZEC DLA BLOKU  ( 0 PRZY PRZECIECIU Z  MOTHERS )
C
C
C
      COMMON /DWV/ NM,INDV,FORM2,TPVI,INDPR1,INDV1
      LOGICAL FORM2,TPVI
C
C   ** ** ** ** ** ** ** ** ** ** *** ** ** ** ** ** **
C  ROBOCZY BLOK DO KOMPATYBILNOSCI VIRTUALI
C   NM - NAZWA WIRTUALA
C   INDV - IDENTYFIKATOR WYZSZEGO VIRTUALA
C   FORM2 - TRUE, GDY PRZETWARZAMY PF II-GO RZEDU
C   TPVI - GDY CHODZI O TYP FUNKCJI WIRTUALNEJ
C   INDPR1,INDV1 - IDENTYFIKATORY PRZETWARZANYCH PROCEDUR/FUNKCJI
C     FORMALNYCH
C
C
C
C
      NRPAR=0
C   JESLI TYP NIE JEST FORMALNY, TO KONIEC
      I=IAND(IPMEM(IDT),MTP)
      IF(I.NE.6) RETURN
C  IND - TU SZUKAMY PARAMETRU
      IF(FORM2) GO TO 500
 300  IND=INDPR
      IF(UP) IND=INDV
      IS=IPMEM(IDT-1)
C   IS - SL TYPU
C   SPRAWDZAMY, CZY I=IND LUB JEGO PREFIKS
      IF(IS.EQ.IND) GO TO 100
C   JESLI IS NIE JEST KLASA, TO KONIEC
      IF(IAND(IPMEM(IS),MTP).EQ.1) RETURN
      IF(.NOT.BPREF(IND,IPMEM(IS-6))) RETURN
C   TO JEST PARAMETR
 100  I=IPMEM(IND+3)
 200  NRPAR=NRPAR+1
      IF(IPMEM(I).EQ.IDT) RETURN
      I=I+1
      GO TO 200
C   SZUKAMY W LISCIE II-GO RZEDU
 500  IND=INDPR1
      IF(UP) IND=INDV1
      IF(IS.NE.IND) GO TO 300
      GO TO 100
      END
      SUBROUTINE SIGNAL ( IDSIG )
C
      IMPLICIT INTEGER (A-Z)
C
C  * * * * * * ** * * * * * * * * * * ** * * * * * * * * * * * * * * * * *
C   PRZETWARZA SYGNAL O IDENTYFIKATORZE ( SYNTAKTYCZNYM ) IDSIG
C    TWORZY DLA NIEGO KOMPLETNY PROTOTYP
C  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
C             COM    - OBSZAR KOMUNIKACYJNY STRUMIENI
C             LMEM   - (=5000) ROZMIAR CALEJ PAMIECI GLOWNEJ
C             LPMEM  - PODZIAL PAMIECI NA CZESCI  IPMEM  I  ISMEM
C             IRECN  - INDEKS SZCZYTU STOSU STALYCH REAL
C             ISFIN  - INDEKS SZCZYTU SLOWNIKA PROTOTYPOW
C
C             LPMSYS - INDEKS PIERWSZEGO SLOWA W IPMEM OBSZARU NIEPRZEZ-
C                      NACZONEGO NA PROTOTYPY SYSTEMOWE
C             LPML   - INDEKS PIERWSZEGO SLOWA OBSZARU WOLNEGO W IPMEM
C             LPMF   - INDEKS OSTATNIEGO SLOWA WOLNEGO OBSZARU W IPMEM
C
C
      COMMON / DGLOB/ INDICT,INDSPR,INDPR,IHBEG,LASTPR,INDPREF
C
C  **  **  **  **  ***  **  **  **  **  **  **  **  **  **  **  **  **
C    BLOK ZAWIERA DANE O PRZETWARZANYM PROTOTYPIE
C        INDICT  -  INDEKS PRZETWARZANEGO PROTTYPU W ISDICT
C        INDSPR  -  IDENTYFIKATOR PROTOTYPU W ISMEM
C        INDPR  -  IDENTYFIKATOR PROTOTYPU W IPMEM
C        IHBEG  -  ADRES PIERWSZEGO SLOWA TABLICY HASHU
C        IDPREF  -  IDENTYFIKATOR BEZPOSREDNIEGO PREFIKSU PRZETWARZANEGO
C                  PROTOTYPU
C        LASTPR  -  IDENTYFIKATOR POPRZEDNIO PRZETWARZANEGO PROTOTYPU
C
C
C
cdsw  COMMON /SIGNALS/ NRSIG, HLISTE
cdsw   -----------------------------------------------------
      common /signs/ nrsig, hliste
cdsw   -----------------------------------------------------
C
C   NRSIG - OSTATNIO PRZYDZIELONY NUMER SYGNALU
C   HLISTE - OSTATNI ELEMENT LISTY SYGNALOW (W KAZDYM MODULE )
C
C.....BLOK KOMUNIKACJI ZE STRUMIENIAMI
      LOGICAL  ERRFLG
      COMMON /STREAM/  ERRFLG, LINE, IBUF2(265), IBUF3(7),JUNK(260)
C  . . .
C
C   NRPR - NUMER PROTOTYPU SEMANTYCZNEGO
      NRPR = IPMEM(IDSIG)
      NM = IPMEM(IDSIG+2)
      LINE = IPMEM(IDSIG+1)
C   USTAWIENIE BITU CLOSE
      I = MEMSL (NM, INDPR )
      IPMEM(I+1) = 1
C   PRZYDZIELENIE NUMERU SYGNALU
      NRSIG = NRSIG+1
      IPMEM(NRPR+1) = NRSIG
C   ZAPAMIETANIE SYNTAKTYCZNEJ LISTY PARAMETROW W PROTOTYIE SYGNALU (KONTROLA)
      IPMEM(NRPR+8) = IPMEM(IDSIG+4)
C
C   PRZETWARZANIE NAGLOWKA
C   ZAPAMIETANIE KOPII ZMIENNYZCH OKRESLAJACYCH PRZETWARZANY PROTOTYP
      INDC = INDSPR
      INDPRC = INDPR
      PREFC = INDPREF
      IHBEGC = IHBEG
C   NADANIE NOWYCH WARTOSCI
      INDPR = NRPR
      INDSPR = IDSIG
      IHBEG = INDPR+10
      INDPREF = 0
C   PRZETWARZANIE NAGLOWKA
      CALL HEADER
C   PRZYWROCENIE WARTOSCI ZMIENNYM
      INDPR = INDPRC
      INDSPR = INDC
      IHBEG = IHBEGC
      INDPREF = PREFC
C  .  .  .
      RETURN
      END
      SUBROUTINE HANDLER ( IDSMEM )
C
      IMPLICIT INTEGER ( A-Z )
C     INSERTION OF
      LOGICAL BTEST
C     BECAUSE OF TYPECONFLICT 03.01.84
C
C   ** * * * * ** * * * * * * * * * ** * * * * * ** ** ** * ** * * *** * **
C   PRZETWARZA PROTOTYP HANDLERA
C   IDSMEM - IDENTYFIKATOR PROTOTYPU SYNTAKTYCZNEGO
C   * * * * * * * * * * * * * * * * *** * * * * * * * * * *** * * * * * *
C
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
C             COM    - OBSZAR KOMUNIKACYJNY STRUMIENI
C             LMEM   - (=5000) ROZMIAR CALEJ PAMIECI GLOWNEJ
C             LPMEM  - PODZIAL PAMIECI NA CZESCI  IPMEM  I  ISMEM
C             IRECN  - INDEKS SZCZYTU STOSU STALYCH REAL
C             ISFIN  - INDEKS SZCZYTU SLOWNIKA PROTOTYPOW
C
C             LPMSYS - INDEKS PIERWSZEGO SLOWA W IPMEM OBSZARU NIEPRZEZ-
C                      NACZONEGO NA PROTOTYPY SYSTEMOWE
C             LPML   - INDEKS PIERWSZEGO SLOWA OBSZARU WOLNEGO W IPMEM
C             LPMF   - INDEKS OSTATNIEGO SLOWA WOLNEGO OBSZARU W IPMEM
C
C
C
      COMMON / DGLOB/ INDICT,INDSPR,INDPR,IHBEG,LASTPR,INDPREF
C
C  **  **  **  **  ***  **  **  **  **  **  **  **  **  **  **  **  **
C    BLOK ZAWIERA DANE O PRZETWARZANYM PROTOTYPIE
C        INDICT  -  INDEKS PRZETWARZANEGO PROTTYPU W ISDICT
C        INDSPR  -  IDENTYFIKATOR PROTOTYPU W ISMEM
C        INDPR  -  IDENTYFIKATOR PROTOTYPU W IPMEM
C        IHBEG  -  ADRES PIERWSZEGO SLOWA TABLICY HASHU
C        IDPREF  -  IDENTYFIKATOR BEZPOSREDNIEGO PREFIKSU PRZETWARZANEGO
C                  PROTOTYPU
C        LASTPR  -  IDENTYFIKATOR POPRZEDNIO PRZETWARZANEGO PROTOTYPU
C
C
C
cdsw  COMMON /SIGNALS/ NRSIG, HLISTE
cdsw  ----------------------------------------------------------
      common /signs/ nrsig, hliste
cdsw  ----------------------------------------------------------
C
C   NRSIG - OSTATNIO PRZYDZIELONY NUMER SYGNALU
C   HLISTE - OSTATNI ELEMENT LISTY SYGNALOW (W KAZDYM MODULE )
C
C
C
      COMMON  / MASKS /  MTP,MSPR,MOTHERS,MPAR,MASKTP,NOTTP,
     * MPROCES, MCOR, MERPF, MBLOCK, MHAND,MNOTVIR
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C   MASKI I WZORCE:
C    MTP - MASKA DO WYCINANIA INFORMACJI DOTYCZACYCH TYPOW ZE SLOWA ZEROWEGO
C    MOTHERS -      --     --      --      --     --  INNYCH PROTOTYPOW
C    MPAR -    --    --    --    --    --    --       ZMIENNYCH I PARAMETROW
C    MSPR - MASKA DLA SYSPREF  ( DLA PROTOTYPOW SYNTAKTYCZNYCH )
C    MASKTP - ZAPRZECZENIE MASKI  MTP
C    NOTTP - WZORZEC DLA NIE-TYPU  ( 1 )
C    MPROCES - WZORZEC DLA PROCESU  ( 5 )
C    MCOR - WZORZEC DLA COROUTINY (7)
C    MERPF - MASKA DO ROZPOZNAWANIA BLEDNYCH LIST PARAMETROW
C    MBLOCK - WZORZEC DLA BLOKU  ( 0 PRZY PRZECIECIU Z  MOTHERS )
C    MNOTVIR - WZORZEC DO KASOWANIA BITU "VIRTUAL"
C
C
      COMMON /COPSIG/ BEGADR, IDHAND
C
C   BLOK SLUZACY DO KOMUNIKACJI Z PROCEDURA KOPIUJACA POSZCZEGOLNE PROTOTYPY
C   IDHAND - IDENTYFIKATOR HANDLERA
C   BEGADR - PIERWSZY ELEMENT SLOWNIKA ZAMIANY STARYCH ADRESOW NA NOWE
C    KAZDY ELEMENT SLOWNIKA ZAJMUJE 2 SLOWA: STARY ADRES, NOWY ADRES.
C    OSTATNI ELEMENT SLOWNIKA - LPML-2
C
C
C.....BLOK KOMUNIKACJI ZE STRUMIENIAMI
      LOGICAL  ERRFLG
      COMMON /STREAM/  ERRFLG, LINE, IBUF2(265), IBUF3(7),JUNK(260)
C
C
C
      IDHAND = IPMEM(IDSMEM+1)
      LINE = IPMEM(IDSMEM+9)
C
C   PRZEJSCIE PO LISCIE NAZW SYGNALOW
C   ODNALEZIENIE SYGNALOW, SPRAWDZENIE ICH POPRAWNOSCI, UTWORZENIE LISTY
C     HANDLEROW
C   LS - KOLEJNY ELEMENT LISTY
C   POR - ELEMENT LISTY Z KTORYM BEDZIEMY POROWNYWAC
C     NAGLOWKI KOLEJNYCH SYGNALOW
      LS = IPMEM(IDSMEM+10)
      I = 0
      IF(LS.EQ.0) GO TO 810
C   TO NIE JEST HANDLER OTHERS
      POR = LS
 50   NM = IPMEM(LS)
C   .  .  .
      I = MEMSL(NM, INDPR)
      IF (I.EQ.0) GO TO 100
      IF ( BTEST(IPMEM(I+1),2)) GO TO 150
      IF ( .NOT. BTEST(IPMEM(I+1),1) .OR. OWN ) GO TO 300
C   NAZWA JEST HIDDEN
      CALL MERR(351,NM)
      GO TO 200
C   NAZWA JEST NOT TAKEN
 150  CALL MERR (352, NM)
      GO TO 200
C   NAZWA NIEZADEKLAROWANA
 100  CALL MERR (350, NM)
C   JESLI TO JEST ELEMENT, Z KTORYM MAMY POROWNYWAC, TO GO PRZESUWAMY
 200  IF(POR.EQ.LS) POR = IPMEM(LS+1)
      GO TO 1000
C
C    NAZWA JEST ZADEKLAROWANA
C    SPRAWDZAMY, CZY TO JEST NAZWA SYGNALU
 300  IDSIG = IPMEM(I+2)
      J = IPMEM(IDSIG)
      J = ISHFT(IAND(J,MPAR),-4)
      IF(J.NE.11) GO TO 250
C   TO JEST PROTOTYP SYGNALU
C   JESLI TO JEST PIERWSZY, TO PRZECHODZIMY DO NASTEPNEGO
      IF(LS.EQ.IPMEM(IDSMEM+10)) GO TO 800
      IF(LS.EQ.POR) GO TO 800
C   SPRAWDZAMY ZGODNOSC PARAMETROW
      IF(IAND(IPMEM(IDSIG),MERPF).NE.0) GO TO 400
C   JESLI LS MA DOBRA LISTE PARAMETROW, A POR ZLA - TO ZMIENIAMY POR
      J = IPMEM(POR)
      IF(IAND(IPMEM(J),MERPF).EQ.0) GO TO 400
      POR = LS
      GO TO 800
C     TO NIE JEST PROTOTYP SYGNALU
 250  CALL MERR ( 353,NM )
      GO TO 200
C
C   SPRAWDZAMY ZGODNOSC LISTY PARAMETROW
 400  CALL SPRPAR ( IDSIG, IPMEM(POR), NM)
C     DOLACZAMY DO LISTY HANDLEROW
 800  I = IPMEM(IDSIG+1)
C   SPRAWDZAMY, CZY HANDLER SI NIE POWTARZA
 810   IF(HLISTE.EQ.0) GO TO 830
      J=IPMEM(INDPR+20)
 820  IF(IPMEM(J).NE.I) GO TO 840
      CALL MERR(362,NM)
      GO TO 950
 840  J=IPMEM(J+2)
      IF(J.NE.0) GO TO 820
 830  J = MGETM(3,341)
      IPMEM(J) = I
      IPMEM(J+1) = IDHAND
C   .  .  .
      IF(HLISTE.EQ.0) GO TO 850
      IPMEM(HLISTE+2)=J
      GO TO 900
 850  IPMEM(INDPR+20) = J
 900  HLISTE = J
      IPMEM(INDPR+19) = IPMEM(INDPR+19) + 1
C   JESLI HANDLER OTHERS, TO KONIEC
      IF(I.EQ.0) RETURN
C   ZAPAMIETUJEMY IDENTYFIKATOR PROTOTYPU W POLU NAZWY
 950  IPMEM(LS) = IDSIG
 1000 LS = IPMEM(LS+1)
      IF(LS.NE.0) GO TO 50
C
C   KOPIOWANIE ATRYBUTOW
C    POR - Z TEGO SIE KOPIUJE DO HANDLERA
C    POR = 0  -  NIE BYLO ANI JEDNEGO POPRAWNEGO SYGNALU
      IF(POR.EQ.0) RETURN
      BEGADR = LPML
      IDSIG = IPMEM(POR)
C   WSTAWIENIE DO HANDLERA DOWIAZANIA DO SYGNALU
      IPMEM(IDHAND+3) = IDSIG
C
C   KOPIOWANIE LISTY ATRYBUTOW RAZEM Z KOPIOWANIEM PROTOTYPOW
      I = IPMEM(IDSIG+6)
      J = IDHAND+5
C   I - KOLEJNY ELEMENT LISTY ATRYBUTOW PROTOTYPU IDSIG
C   J - OSTATNIO SKOPIOWANY ELEMENT LISTY PROTOTYPU IDHAND
      IF(I.EQ.0) RETURN
 1100 IPMEM(J+1) = MGETM(2,341)
      J = IPMEM(J+1)
      IPMEM(J) = ICPROT(IPMEM(I))
      I = IPMEM(I+1)
      IF(I.NE.0) GO TO 1100
C   USTAWIENIE OSTATNIEGO ATRYBUTU PROTOTYPU IDHAND
      IPMEM(IDHAND+7) = J
C
C   KOPIOWANIE TABLICY HASH'U
      LPML = LPML-2
C   LPML - OSTATNI ELEMENT SLOWNIKA ZAMIANY ADRESOW
      IHSIG = IDSIG+9
      IHHAND = IDHAND+9
C
      DO 1500 I=1,8
C   I - KOLEJNY ELEMENT TABLICY HASH'U PROTOTYPU IDSIG
      J = IHSIG+I
      J = IPMEM(J)
C   J - KOLEJNY ELEMENT LISTY HASH'U PROTOTYPU IDSIG
      IF(J.EQ.0) GO TO 1500
      K = IHHAND+I-3
C   K - OSTATNIO SKOPIOWANY ELEMENT LISTY HASH'U PROTOTYPU IDHAND
 1200 IPMEM(K+3) = MGETM(4,341)
      K = IPMEM(K+3)
      IPMEM(K) = IPMEM(J)
      IPMEM(K+1) = IPMEM(J+1)
C   SZUKANIE ODPOWIEDNIEGO ADRESU
      II = IPMEM(J+2)
      DO 1300 IJ = BEGADR, LPML, 2
      IF(IPMEM(IJ).EQ.II) GO TO 1400
 1300 CONTINUE
 1400 IPMEM(K+2) = IPMEM(IJ+1)
      J = IPMEM(J+3)
      IF(J.NE.0) GO TO 1200
 1500 CONTINUE
C
C   KONIEC KOPIOWANIA - ZWALNIAMY PAMIEC PRZEZNACZONA NA SLOWNIK
      LPML = BEGADR
      RETURN
      END
      SUBROUTINE SPRPAR ( EL, ELPOR, NM )
C
C   * * * * * * * * * ** * * * * ** * * * * * * * * * * * * * * * * * * * *
C    POROWNUJE LISTY PARAMETROW SYGNALU O IDENTYFIKATORZE EL I SYGNALU
C    O ODENTYFIKATORZE ELPOR
C     ELPOR - WZORCOWY SYGNAL DO POOWNYWANIA
C    NM - NAZWA SYGNALU EL
C     * * * * * * * * * * * * **** * * * * * ** * * * ** * * * * * * ** ***
C
      IMPLICIT INTEGER ( A - Z )
C
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
C             COM    - OBSZAR KOMUNIKACYJNY STRUMIENI
C             LMEM   - (=5000) ROZMIAR CALEJ PAMIECI GLOWNEJ
C             LPMEM  - PODZIAL PAMIECI NA CZESCI  IPMEM  I  ISMEM
C             IRECN  - INDEKS SZCZYTU STOSU STALYCH REAL
C             ISFIN  - INDEKS SZCZYTU SLOWNIKA PROTOTYPOW
C
C             LPMSYS - INDEKS PIERWSZEGO SLOWA W IPMEM OBSZARU NIEPRZEZ-
C                      NACZONEGO NA PROTOTYPY SYSTEMOWE
C             LPML   - INDEKS PIERWSZEGO SLOWA OBSZARU WOLNEGO W IPMEM
C             LPMF   - INDEKS OSTATNIEGO SLOWA WOLNEGO OBSZARU W IPMEM
C
C
      COMMON  / MASKS /  MTP,MSPR,MOTHERS,MPAR,MASKTP,NOTTP,
     * MPROCES, MCOR, MERPF, MBLOCK, MHAND,MNOTVIR
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C   MASKI I WZORCE:
C    MTP - MASKA DO WYCINANIA INFORMACJI DOTYCZACYCH TYPOW ZE SLOWA ZEROWEGO
C    MOTHERS -      --     --      --      --     --  INNYCH PROTOTYPOW
C    MPAR -    --    --    --    --    --    --       ZMIENNYCH I PARAMETROW
C    MSPR - MASKA DLA SYSPREF  ( DLA PROTOTYPOW SYNTAKTYCZNYCH )
C    MASKTP - ZAPRZECZENIE MASKI  MTP
C    NOTTP - WZORZEC DLA NIE-TYPU  ( 1 )
C    MPROCES - WZORZEC DLA PROCESU  ( 5 )
C    MCOR - WZORZEC DLA COROUTINY (7)
C    MERPF - MASKA DO ROZPOZNAWANIA BLEDNYCH LIST PARAMETROW
C    MBLOCK - WZORZEC DLA BLOKU  ( 0 PRZY PRZECIECIU Z  MOTHERS )
C    MNOTVIR - WZORZEC DO KASOWANIA BITU "VIRTUAL"
C
C
C
C    LISTE - LISTA PARAMETROW ( SYNTAKTYCZNA! ) SYGNALU EL
C    LPOR - LISTA PARAMETROW SYGNALU ELPOR
C    LISTY SA PRZECHOWANE W SLOWIE +8 PROTOTYPU SYGNALU
C
      LISTE = IPMEM(EL+8)
      LPOR = IPMEM(ELPOR+8)
 50   IF ( LISTE + LPOR .EQ.0 ) RETURN
      IF ( LISTE*LPOR.EQ.0) GO TO 900
C   POROWNYWANIE
      KIND = IPMEM(LPOR)
      KD = IPMEM(LISTE)
      NMP = IPMEM(LISTE+2)
C   NMP - NAZWA PARAMETRU LISTE
C  KONTROLA RODZAJOW
      IF(KD.EQ.KIND) GO TO 70
C   MOZE SA NIEZGODNE RODZAJE
      IF ( KIND.GE.7 .OR. KD.GE.7 ) GO TO 100
C   SPRAWDZAMY, CZY RODZAJE SA ZGODNE Z DOKLADNOSCIA DO BLEDNYCH LIST PF
      IF(IABS(KIND-KD).NE.2) GO TO 100
C    KONTROLA NAZW
 70   IF ( NMP.NE.IPMEM(LPOR+2) ) GO TO 200
      IF (KIND.LE.6) GO TO 300
      IF (KIND.EQ.7) GO TO 400
C   ZMIENNE - POROWNUJEMY TYPY
      IF(IPMEM(LISTE+4).NE.IPMEM(LPOR+4)) GO TO 250
      IF(IPMEM(LISTE+5).NE.IPMEM(LPOR+5)) GO TO 250
C   NIE MA BLEDU - NASTEPNY ELEMENT LISTY
 400  LISTE = IPMEM(LISTE+3)
      LPOR = IPMEM(LPOR+3)
      GO TO 50
C
C   NIEZGODNE RODZAJE
 100  CALL MERR (355,NMP)
      GO TO 400
C   ROZNE NAZWY
 200  CALL MERR (356,NMP)
      GO TO 400
C   ROZNE TYPY
 250  CALL MERR(354,NMP)
      GO TO 400
C
C   PROCEDURY/FUNKCJE
C   SPRAWDZAMY PARAMETRY II-GO RZEDU
 300  I = IPMEM(LISTE+4)
      K = IPMEM(LPOR+4)
C   I - ELEMENTY LISTY II-GO RZEDU  PARAMETRU LISTE
C   K - ELEMENTY LISTY II-GO RZEDU PARAMETRU LPOR
 350  IF ( I+K.EQ.0) GO TO 800
      IF ( I*K.EQ.0 ) GO TO 700
C   KONTROLA
      IF ( IPMEM(I).NE.IPMEM(K) ) GO TO 500
C   NIE KONTROLUJE SIE NAZW
      IF ( IPMEM(I).LE.7 ) GO TO 600
C   KONTROLA TYPOW
      IF ( IPMEM(I+4).NE.IPMEM(K+4) ) GO TO 550
      IF ( IPMEM(I+5).NE.IPMEM(K+5) ) GO TO 550
C   NIE MA BLEDOW
 600  I = IPMEM(I+3)
      K = IPMEM(K+3)
      GO TO 350
C   NIEZGODNE RODZAJE
 500  CALL MERR ( 357,IPMEM(I+2) )
      GO TO 600
C   ROZNE TYPY
 550  CALL MERR ( 358, IPMEM(I+2) )
      GO TO 600
C   ROZNE DLUGOSCI LIST PARAMETROW II-GO RZEDU
C   JESLI KROTSZA LISTA JEST BLEDNA, TO NIE MA SYGNALIZACJI
 700  IF ( I.EQ.0 ) GO TO 750
C  K - KROTSZA
      KD = IPMEM(LPOR)
 710   IF ( KD.EQ.5.OR.KD.EQ.6) GO TO 800
       CALL MERR(359,NMP)
      GO TO 800
 750  KD = IPMEM(LISTE)
      GO TO 710
C
C   KONIE LIST II-GO RZEDU
C   KONTROLA TYPOW - JESLI FUNKCJE
 800  IF ( KIND.EQ.3 .OR. KIND.EQ.5 ) GO TO 400
      IF(IPMEM(LISTE+5).NE.IPMEM(LPOR+5)) GO TO 250
      IF(IPMEM(LISTE+6).NE.IPMEM(LPOR+6)) GO TO 250
      GO TO 400
C
C   NIEZGODNE DLUGOSCI LIST PARAMETROW I-GO RZEDU
C   JESLI LISTA KROTSZA JEST BLEDNA, TO NIE MA SYGNALIZACJI
 900  IF ( LISTE.EQ.0 ) GO TO 950
      IF(IAND(IPMEM(ELPOR),MERPF).EQ.0) CALL MERR(360,NM)
      RETURN
 950  IF(IAND(IPMEM(EL),MERPF).EQ.0) CALL MERR(360,NM)
       RETURN
      END
       INTEGER FUNCTION ICPROT ( IDPR )
C
C  ** * * * * * * * ** *** ** * * * * * * * * * * ** * * * * * * *
C    KOPIUJE PROTOTYP IDPR ( PROTOTYP PARAMETRU FORMALNEGO).
C    UAKTUALNIA SLOWNIK ZAMIANY ADRESOW PROTOTYPOW.
C    WYNIKIEM FUNKCJI JEST IDENTYFIKATOR UTWORZONEGO PROTOTYPU.
C   * * * * * * * * * ** * * * * * * * * * ** * * * * * * * * * * *
C
      IMPLICIT INTEGER ( A - Z )
C
C
C ..... ZMIENNE GLOBALNE
C
C.....
      LOGICAL  INSYS,  OWN
      COMMON /BLANK/ COM(278),
     X        LMEM  , LPMEM , IRECN , ISFIN , LPMSYS, LPML  , LPMF  ,
     X        NRINT , NRRE  , NRBOOL, NRCHR , NRCOR , NRPROC, NRTEXT,
     X        NRUNIV, NATTR , NRNONE, NBLSYS, NBLUS , NEMPTY, INSYS ,
     X        LOCAL , OWN   , OBJECT,
     X        IPMEM(5000)
C
C             COM    - OBSZAR KOMUNIKACYJNY STRUMIENI
C             LMEM   - (=5000) ROZMIAR CALEJ PAMIECI GLOWNEJ
C             LPMEM  - PODZIAL PAMIECI NA CZESCI  IPMEM  I  ISMEM
C             IRECN  - INDEKS SZCZYTU STOSU STALYCH REAL
C             ISFIN  - INDEKS SZCZYTU SLOWNIKA PROTOTYPOW
C
C             LPMSYS - INDEKS PIERWSZEGO SLOWA W IPMEM OBSZARU NIEPRZEZ-
C                      NACZONEGO NA PROTOTYPY SYSTEMOWE
C             LPML   - INDEKS PIERWSZEGO SLOWA OBSZARU WOLNEGO W IPMEM
C             LPMF   - INDEKS OSTATNIEGO SLOWA WOLNEGO OBSZARU W IPMEM
C
C
C
      COMMON / DGLOB/ INDICT,INDSPR,INDPR,IHBEG,LASTPR,INDPREF
C
C  **  **  **  **  ***  **  **  **  **  **  **  **  **  **  **  **  **
C    BLOK ZAWIERA DANE O PRZETWARZANYM PROTOTYPIE
C        INDICT  -  INDEKS PRZETWARZANEGO PROTTYPU W ISDICT
C        INDSPR  -  IDENTYFIKATOR PROTOTYPU W ISMEM
C        INDPR  -  IDENTYFIKATOR PROTOTYPU W IPMEM
C        IHBEG  -  ADRES PIERWSZEGO SLOWA TABLICY HASHU
C        IDPREF  -  IDENTYFIKATOR BEZPOSREDNIEGO PREFIKSU PRZETWARZANEGO
C                  PROTOTYPU
C        LASTPR  -  IDENTYFIKATOR POPRZEDNIO PRZETWARZANEGO PROTOTYPU
C
C
C
      COMMON  / MASKS /  MTP,MSPR,MOTHERS,MPAR,MASKTP,NOTTP,
     * MPROCES, MCOR, MERPF, MBLOCK,MHAND,MNOTVIR
C
C  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
C   MASKI I WZORCE:
C    MTP - MASKA DO WYCINANIA INFORMACJI DOTYCZACYCH TYPOW ZE SLOWA ZEROWEGO
C    MOTHERS -      --     --      --      --     --  INNYCH PROTOTYPOW
C    MPAR -    --    --    --    --    --    --       ZMIENNYCH I PARAMETROW
C    MSPR - MASKA DLA SYSPREF  ( DLA PROTOTYPOW SYNTAKTYCZNYCH )
C    MASKTP - ZAPRZECZENIE MASKI  MTP
C    NOTTP - WZORZEC DLA NIE-TYPU  ( 1 )
C    MPROCES - WZORZEC DLA PROCESU  ( 5 )
C    MCOR - WZORZEC DLA COROUTINY (7)
C    MERPF - MASKA DO ROZPOZNAWANIA BLEDNYCH LIST PARAMETROW
C    MBLOCK - WZORZEC DLA BLOKU  ( 0 PRZY PRZECIECIU Z  MOTHERS )
C
C
C
      COMMON /COPSIG/ BEGADR, IDHAND
C
C   BLOK SLUZACY DO KOMUNIKACJI Z PROCEDURA KOPIUJACA POSZCZEGOLNE PROTOTYPY
C   IDHAND - IDENTYFIKATOR HANDLERA
C   BEGADR - PIERWSZY ELEMENT SLOWNIKA ZAMIANY STARYCH ADRESOW NA NOWE
C    KAZDY ELEMENT SLOWNIKA ZAJMUJE 2 SLOWA: STARY ADRES, NOWY ADRES.
C    OSTATNI ELEMENT SLOWNIKA - LPML-2
C
C
C
      KIND = ISHFT ( IAND(IPMEM(IDPR),MPAR),-4)
      IF(KIND.GT.3) GO TO 400
      GO TO (100,200,300), KIND
C
C   TYP FORMALNY
 100  ICPROT = MGETM(5,341) + 2
C   DOLACZENIE DO LISTY NEXTDECL
      IPMEM(LASTPR+2) = ICPROT
      LASTPR = ICPROT
      GO TO 1000
C
C   FUNKCJA FORMALNA
 200  ICPROT = MGETM(10,341) + 5
      GO TO 500
C
C   PROCEDURA FORMALNA
 300  ICPROT = MGETM(7,341) + 2
C
C   KOPIOWAIE LISTY PARAMETROW II-GO RZEDU
 500  J = IPMEM(IDPR+4)
      IPMEM(ICPROT+3) = MGETM(J,341)
      IPMEM(ICPROT+4) = J
      IF(J.EQ.0) GO TO 1000
C
      DO 700 K=1,J
      II = IPMEM(IDPR+3) +K-1
      II = IPMEM(II)
C   II - IDENTYFIKATOR STAREGO PARAMETRU
C   I - ROZMIAR PROTOTYPU
      I = 6
      IJ=4
      KD=ISHFT(IAND(IPMEM(II),MPAR),-4)
      IF(KD.GT.4) GO TO 520
      IF(KD.EQ.2) GO TO 510
C   PROCEDURA LUB TYP
      I=5
      IJ=2
      GO TO 520
 510  I=7
C   IJ - IDENTYFIKATOR NOWEGO PROTOTYPU
 520  IJ = MGETM(I,341) + IJ
      IPMEM(IJ) = IPMEM(II)
      IPMEM(IJ-1) = ICPROT
      IPMEM(IJ-2) = IPMEM(II-2)
      IPMEM(IJ+1) = IPMEM(II+1)
      IF(KD.LT.5) GO TO  650
C   DLA ZMIENNEJ - WPISANIE TYPU
 550  I = IPMEM(II-3)
      IPMEM(IJ-4) = IPMEM(II-4)
      IPMEM(IJ-3) = I
C  SPRAWDZENIE, CZY JEST TO FORMALNY TYP LOKALNY
      I2 = LPML-2
      IF(I2.LT.BEGADR) GO TO 690
      DO 600 I1 = BEGADR,I2,2
      IF(IPMEM(I1).EQ.I) GO TO 610
 600  CONTINUE
C   NIE MA TYPU
      GO TO 690
 610  IPMEM(IJ-3) = IPMEM(I1+1)
      GO TO 690
C   DOLOZENIE ADRESU DO SLOWNIKA ( DLA ZMIENNEJ NIE WARTO )
 650  INSYS = .TRUE.
      I1 = MGETM(2,341)
      IPMEM(I1) = II
      IPMEM(I1+1) = IJ
      INSYS = .FALSE.
C   WSTAWIENIE ADRESU PROTOTYPU DO LISTY PARAMETROW
 690  I1 = IPMEM(ICPROT+3) +K-1
      IPMEM(I1) = IJ
 700  CONTINUE
C
      IF(KIND.EQ.3) GO TO 1000
C   FUNKCJA - USTAWIENIE IDENTYFIKATORA RESULT
      IPMEM(ICPROT-5) = IJ
      GO TO 800
C
C   ZMIENNE
 400  ICPROT = MGETM(6,341) + 4
C   KOPIOWANIE TYPU ZMIENNEJ LUB FUNKCJI
 800  IPMEM(ICPROT-4) = IPMEM(IDPR-4)
      I = IPMEM(IDPR-3)
      IPMEM(ICPROT-3) = I
C   SPRAWDZAMY, CZY TO JET TYP FORMALNY LOKALNY
      K = LPML-2
      IF(K.LT.BEGADR) GO TO 1000
C   JEST NIEPUSTY SLOWNIK ZAMIANY ADRESOW
      DO  820 J=BEGADR, K,2
      IF(IPMEM(J).EQ.I) GO TO 850
 820  CONTINUE
C   NIE MA TAKIEGO TYPU
      GO TO 870
 850  IPMEM(ICPROT-3) = IPMEM(J+1)
C   JESLI ZMIENNA TO NIE WSTAWIAMY DO NEXTDECL
 870  IF(KIND.GE.5) GO TO 1000
C   DLA FUNKCJI - WSTAWIENIE TYPU DO ATRYBUTU RESULT
      K = IPMEM(ICPROT-5)
      IPMEM(K-4) = IPMEM(ICPROT-4)
      IPMEM(K-3) = IPMEM(ICPROT-3)
C
C   UZUPELNIENIE SLOWA ZEROWEGO, ADRESU ATRUBUTU I USED
 1000 IPMEM(ICPROT-2) = IPMEM(IDPR-2)
      IPMEM(ICPROT-1) = IDHAND
      IPMEM(ICPROT)  = IPMEM(IDPR)
      IPMEM(ICPROT+1) = IPMEM(IDPR+1)
C
C   UZUPELNIENIE SLOWNIKA ZAMIANY ADRESOW
      INSYS = .TRUE.
      K = MGETM(2,341)
      IPMEM(K) = IDPR
      IPMEM(K+1) = ICPROT
      INSYS = .FALSE.
C
      RETURN
      END

