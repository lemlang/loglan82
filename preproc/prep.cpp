
#include <qlist.h>
#include <qstring.h>
#include <stdio.h>
#include <qfile.h>
#include <qtstream.h>
#include <string.h>
#include <qdir.h>
#include <unistd.h>
#include <stdlib.h>

class IncFile
{
public:
 char filename[255];
 int start, len;
 IncFile(char *s,int st) {strcpy(filename,s);start=st;len=0;};
};

QList<IncFile> inc;

IncFile *findTrueLine(int ln, int *trueline)
{
 IncFile *pom,*pom1;
 QList<IncFile> pl;
 int c1=0,c2=0;


if (inc.isEmpty()) { *trueline=ln;return(NULL);} 
pl.clear();
pom=inc.first();
if (pom->start>ln) {*trueline=ln;return(NULL);}

while ( (pom!=NULL) && (ln>pom->start))
 {

  if (ln<pom->start+pom->len) {*trueline=ln-pom->start;
                               return(pom);}  
  pl.append(pom);
  pom=inc.next();
 }

 if (pom != NULL)
 {
  pom1=pl.first();c1=0;
  while (pom1!=NULL) { c1+=pom1->len;pom1=pl.next(); }
  *trueline=ln-c1; return(NULL);
 }
  else
 {
   pom1=inc.first();c1=0;
   while (pom1!=NULL) { c1+=pom1->len;pom1=inc.next(); }
   *trueline=ln-c1; return(NULL);
  }
 return(NULL);
}

int main(int argc,char **argv)
{
 char homedir[255];
 char mydir[255],ss[255],fname[255];
 QString poms,poms1,poms2;

 int currentline=1;
 int i,j,line,tline;

 
 
 inc.clear();
 poms.sprintf("%s",argv[1]);
 i=poms.findRev('/',poms.length()-1,FALSE);


//******* get home directory 
 if (i!=-1)
 {
   poms1=poms.left(i+1);
   strcpy(homedir,poms1.data());
  }
   else strcpy(homedir,"");

 poms.sprintf("%s",argv[0]);
 i=poms.findRev('/',poms.length()-1,FALSE);
 if (i!=-1) poms1=poms.left(i);
 else poms1.sprintf(".");
 strcpy(mydir,poms1.data());

 poms.sprintf("%s",argv[1]);
 i=poms.findRev(".log",poms.length()-1,FALSE);
 poms1=poms.left(i);
 strcpy(fname,poms1.data());
 

// ********************

 poms.sprintf("%s/.cmp00",mydir);
 poms1.sprintf("%s/cmp01.log",mydir);
 unlink(poms.data());
 unlink(poms1.data());

 QFile compfile(poms1.data());
 QFile srcfile(argv[1]);

  
  if (!compfile.open(IO_WriteOnly))
     {fprintf(stdout,"Cannot open temp file to write %s\n",poms1.data());exit(1);}

 if (!srcfile.open(IO_ReadOnly))
   {fprintf(stdout,"Cannot open file\n");exit(1);}

 QTextStream comps(&compfile);
 QTextStream src(&srcfile);

 
 while (!src.eof())
 {
  poms = src.readLine();
  i=poms.find("#include");
  if (i!=-1)
    {            // get include file
        i=poms.find('"');
        if (i!=-1) j=poms.find('"',i+1);
        if ((i!=-1) && (j!=-1))
        {
         IncFile *p;
         poms1=poms.mid(i+1,j-i-1);
         p=new IncFile((char*)poms1.ascii(),currentline);
         poms2.sprintf("%s%s",homedir,poms1.data());
         QFile pomf(poms2.data());
         if (!pomf.open(IO_ReadOnly))
          {
           fprintf(stdout,"Cannot open include file: %s\n",poms2.data());
           exit(1);
          }
         QTextStream pomstream(&pomf);
         while (!pomstream.eof())
         {
          poms1=pomstream.readLine();
          comps<<poms1.data();
          comps<<"\n";
          p->len++;
          currentline++;
          }
         pomf.close();
         inc.append(p);
        }
       }
  else
  {
   comps<<poms.data();
   comps<<"\n";
  }
    
  currentline++;
 }// eof


 srcfile.close(); 
 compfile.close();

 poms.sprintf("%s/cmp01.lcd",mydir);
 unlink(poms.data());

 sprintf(ss,"%s/loglan %s/cmp01 > %s/.cmp00",mydir,mydir,mydir);
 if (system(ss)==-1)
  {
  fprintf(stdout,"Cannot execute compiler\n");
  exit(1);
  }
 
 
 poms1.sprintf("%s.lcd",fname);

 if (QFile::exists(poms.data()))
  {
   rename(poms.data(),poms1.data());
   unlink(poms.data());
   fprintf(stdout,"Compile ok\n");
   sprintf(ss,"%s/gen %s",mydir,fname);
   if (system(ss)==-1)
   {
    fprintf(stdout,"Cannot execute generator\n");
    exit(1);
   }
   poms.sprintf("%s.ccd",fname);
   if (QFile::exists(poms.data())) {unlink(poms1.data());}
  }
 else
{ 
 fprintf(stdout,"Errors\n");
 
 poms.sprintf("%s/.cmp00",mydir);
 QFile err(poms.data());

 if (!err.open(IO_ReadOnly)) 
      { fprintf(stdout,"Cannot open error file\n");
        exit(1);
      }
 QTextStream errstream(&err);
 IncFile *fl;

 poms=errstream.readLine();
 i=poms.find("LOGLAN-82");
 while( (!errstream.eof()) && (i==-1)) 
   {
     poms=errstream.readLine();
     i=poms.find("LOGLAN-82");
    } // *** 
 
 while (!errstream.eof())
 {
  poms=errstream.readLine();
  i=poms.find("ERROR");
  if (i!=-1)
   {
    i=i-2;
    j=poms.findRev(' ',i);
    poms1=poms.mid(j+1,i-j);
    line=poms1.toInt();
    fl = findTrueLine(line,&tline);
    poms2=poms.right(poms.length()-i-1);
    if (fl!=NULL) fprintf(stdout,"%s: ",fl->filename);
    fprintf(stdout,"%d  %s\n",tline,poms2.data());    
    } 
  }
 err.close();
} // errors

 poms.sprintf("%s/cmp01.log",mydir);
 unlink(poms.data());
 poms.sprintf("%s/.cmp00",mydir);
 unlink(poms.data());                             
 
return(0);
}
