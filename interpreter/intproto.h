unsigned alarm ( unsigned );
double prandom ( void );

void openrc ( word,virtaddr *,word * );
void raise_signal ( word,word,word *,word * );
void openobj ( word,word *,word * );
void slopen ( word,virtaddr *,word *,word * );
void errsignal ( int );
void typep ( word,word,word *,word * );
void copy ( virtaddr *,virtaddr * );
void qua ( virtaddr *,word );
void standard ( word );
void disp ( virtaddr * );
void gkill ( virtaddr * );
void typref ( virtaddr *,word );
void go ( word,word );
void goloc ( word,word );
void typed ( word,word,word,word,virtaddr * );
void term ( void );
void wind ( void );
void trace ( word );
void inner ( word );
void backhd ( virtaddr *,word * );
void backbl ( virtaddr *,word * );
void backpr ( virtaddr *,word * );
void back ( virtaddr *,word *,word );
void detach ( void );
void attach ( virtaddr * );
void fin ( word,virtaddr *,word * );
void heads ( virtaddr *,word );
void resume ( virtaddr * );
void passivate ( int );
void enable ( word,word );
void evaluaterpc ( word );
void disable ( word,word );
void rpc_accept ( word );
void rpc3 ( void );
void popmask ( word );
void askprot ( virtaddr * );
bool member ( virtaddr *,word * );
word virtprot ( word );
word loadt ( word,word );
bool is ( virtaddr *,word );
bool inl ( virtaddr *,word );
word shift ( word,word );
void execute ( void );
void abend ( char * );
void addext ( char *,char * );
void decode ( void );
void runsys ( void );
void schedule ( void );
void msginterrupt ( message * );
void loosen ( void );
void update ( word, word );
void compactify ( void );
void moveparams ( word pix, word am, message *msg, int par1, int dir );
void sendmsg1 ( message * );
word getnode ( word );
void endprocess ( int );
word entier ( double );
char *asciiz ( virtaddr * );
void ranset ( void );
void moveblock ( char *,char *, word );
void request ( word, word *, word * );
word memavail ( void );
void newarry ( word, word, word, virtaddr *, word * );
void initprocess ( word,word,procaddr * );
void activate ( word );
void reset ( word );
void pushmask ( word );
void trapmsg ( void );
void rpc2 ( void );
void endrun ( int );
void loadfile ( word,word *,word *,FILE ** );
word directio ( virtaddr *,word,int ( * ) (),FILE * );
void nonstandard ( word );
void genfileobj ( bool,word,char *,virtaddr *,word * );
void rewrite ( word );
void delete ( virtaddr * );
bool testeof ( FILE * );
bool testeoln ( FILE * );
char *tempfilename ( void );
word readint ( FILE * );
double readreal ( FILE * );
void writeint ( word, word, FILE * );
void writereal ( double, word, word, FILE * );
void writestring ( word, word, FILE * );
void readln ( FILE * );
void senderr ( int, procaddr * );
void rpcend ( message * );
void rpc1 ( message * );
void handle_message ( MESSAGE* );
int check_port ( MESSAGE* );
void send_to_kernel ( MESSAGE* );
void writeln_str ( char* );
void write_str ( char* );
void write_char ( char );
char read_char();
void read_str ( char* );
