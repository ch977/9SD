#include"SUtil.h"
int _compareFloat(const void * a,const void *b)
{
return (*(float *)a>*(float *)b)?1:-1;
}

/*For removing the '\r' '\n' char at the end of a string */
void chomp(char *pszString)
{
	int nLen=strlen(*pszString);
	while(pszString[nLen-1]=='\r'||pszString[nLen-1]=='\n')
		--nLen;
	pszString[nLen]=0;
}

/*for reading song melody database associated information,such as 
	name ,singer and so on*/
int SReadMelodyDBInfoFile(char *sFileName, char ** &psStrRead)
{
	int iTotalFile=0;
	FILE *fp=fopen(sFileName,"r");
	if(NULL==fp)
	{
		return ERROR_CODE_FILE_CANNOT_OPEN;
	}
	char szRmpStr[LINEREAD];
	if(fgets(szTmpStr,sizeof(char)*LINEREAD,fp)!=NULL)
		iTotalFile=atoi(szTmpStr);
	else
	{
		return ERROR_CODE_MODELINFO_WRONG_FORMAT;
	}
	
	psStrRead=new char *[iTotalFile];
	for (int i=0;i<iTotalFile;i++)
	{	psStrRead[i]=new char[LINEREAD];
		fgets(psStrRead[i],sizeof(char)*LINEREAD,fp);
		chomp(psStrRead[i]);

	}
	fclose(fp);
	return iTotalFIle;
	}