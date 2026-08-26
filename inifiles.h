#ifndef __inifiles_H__
#define __inifiles_H__
//---------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_STRING 255

typedef struct PROFILE PROFILE;
struct PROFILE {
    char *pstVarValor;
    PROFILE *prox;
};

typedef struct SECTION SECTION;
struct SECTION {
    char *pstNome;
    PROFILE *VarValores;
    SECTION *prox;
};
//---------------------------------------------------------------------------
class TIniFile
{
private:    // User declarations
    SECTION *pLstArquivo, *lsttmpSecao;
    PROFILE *lsttmpProfile;
    bool bAlterado;
    char *pstNomeArquivo;
    void AdicionaSecao(char *pstNomeSecao);
    void AdicionaVariavel(char *pstVariavel);
    void RemoveSecao(void);
    void RemoveProfile(SECTION *pstDelete);
    bool FindSection(char *pstSection);
    bool FindVariavel(char *pstVariavel);
    void LimpaString(char *pstStr);
public:     // User declarations
    TIniFile(char *pstArquivo);
    ~TIniFile(void);
    void PrintIniFile(void);
    bool WriteString(char *pstSecao, char *pstVariavel, char *pstValor);
    bool WriteInteger(char *pstSecao, char *pstVariavel, int nValor);
    bool ReadString(char *pstSecao, char *pstVariavel, char *pstValor);
    int ReadInteger(char *pstSecao, char *pstVariavel, int nDefault);
    int ReadHexadecimal(char *pstSecao, char *pstVariavel, int nDefault);
};
//---------------------------------------------------------------------------
TIniFile::TIniFile(char *pstArquivo)
{
 char *pstLinha = (char *) calloc(TAM_STRING,sizeof(char));
 char *pstTemp = (char *) calloc(TAM_STRING,sizeof(char));
 FILE *fp = NULL;
 int k;

 pstNomeArquivo = pstArquivo;

 pLstArquivo = NULL;
 lsttmpSecao = NULL;
 lsttmpProfile = NULL;
 if((fp=fopen(pstNomeArquivo,"rt")) != NULL) {
     while(!feof(fp)) {
         memset(pstLinha,0x00,TAM_STRING*sizeof(char));
         if(fgets(pstLinha,TAM_STRING,fp) == NULL)
             break;
         LimpaString(pstLinha);
         char *pstFimSecao = strrchr(pstLinha,']');
         k = pstFimSecao ? (int)(pstFimSecao-pstLinha) : 0;
         if(*pstLinha == '[' && k>0) {
             memset(pstTemp,0x00,TAM_STRING*sizeof(char));
             strncpy(pstTemp,pstLinha+1,k-1);
             AdicionaSecao(pstTemp);
         }
         else {
             if(lsttmpSecao && strlen(pstLinha) > 2) {
                 memset(pstTemp,0x00,TAM_STRING*sizeof(char));
                 strcpy(pstTemp,pstLinha);
                 AdicionaVariavel(pstTemp);
             }
         }
     }
  fclose(fp);
 }
 free(pstLinha);
 free(pstTemp);
 lsttmpSecao = NULL;
 bAlterado = false;
}
//---------------------------------------------------------------------------
TIniFile::~TIniFile(void)
{
 FILE *fp;
 
 if(bAlterado) {
     if(pLstArquivo) {
         fp=fopen(pstNomeArquivo,"wt");
         while(pLstArquivo) {
             fprintf(fp,"[%s]\n",pLstArquivo->pstNome);
             if(pLstArquivo->VarValores)  {
                 while(pLstArquivo->VarValores) {
                     fprintf(fp,"%s\n",pLstArquivo->VarValores->pstVarValor);
                     RemoveProfile(pLstArquivo);
                 }
             }
             RemoveSecao();
             fprintf(fp,"\n");
         }
         fclose(fp);
     }
 }
 else {
     while(pLstArquivo) {
         if(pLstArquivo->VarValores)
             while(pLstArquivo->VarValores)
                 RemoveProfile(pLstArquivo);
         RemoveSecao();
     }
 }
}
//---------------------------------------------------------------------------
void TIniFile::AdicionaSecao(char *pstNomeSecao)
{
 SECTION *p, *lp;

 lp = (SECTION *) calloc(1,sizeof(SECTION));
 lp->pstNome = (char *) calloc(strlen(pstNomeSecao)+1,sizeof(char));
 strcpy(lp->pstNome,pstNomeSecao);
 lp->prox = NULL;
 lp->VarValores = NULL;

 if(pLstArquivo == NULL)
     pLstArquivo = lp;
 else {
     for(p = pLstArquivo; p->prox; p = p->prox);
     p->prox = lp;
 }
 lsttmpSecao = lp;
}
//---------------------------------------------------------------------------
void TIniFile::AdicionaVariavel(char *pstVariavel)
{
 PROFILE *p, *lp;

 lp = (PROFILE *) calloc(1,sizeof(PROFILE));
 lp->pstVarValor = (char *) calloc(strlen(pstVariavel)+1,sizeof(char));
 strcpy(lp->pstVarValor,pstVariavel);
 lp->prox = NULL;

 if(lsttmpSecao->VarValores == NULL)
     lsttmpSecao->VarValores = lp;
 else {
     for(p = lsttmpSecao->VarValores; p->prox; p = p->prox);
     p->prox = lp;
 }
}
//---------------------------------------------------------------------------
void TIniFile::PrintIniFile(void)
{
 SECTION *lstSecao;
 PROFILE *lstProfile;

 for(lstSecao = pLstArquivo;lstSecao;lstSecao=lstSecao->prox) {
     printf("[%s]\n",lstSecao->pstNome);
     if(lstSecao->VarValores) 
         for(lstProfile=lstSecao->VarValores;lstProfile;lstProfile=lstProfile->prox)
             printf("\t%s\n",lstProfile->pstVarValor);
 }
}
//---------------------------------------------------------------------------
void TIniFile::RemoveSecao(void)
{
 SECTION *lp;

 lp = pLstArquivo;
 pLstArquivo = lp->prox; 
 free(lp->pstNome);
 free(lp);
}
//---------------------------------------------------------------------------
void TIniFile::RemoveProfile(SECTION *pstDelete)
{
 PROFILE *lp = pstDelete->VarValores;

 pstDelete->VarValores = lp->prox; 
 free(lp->pstVarValor);
 free(lp);
}
//---------------------------------------------------------------------------
bool TIniFile::WriteString(char *pstSecao, char *pstVariavel, char *pstValor)
{
 bAlterado = true;
 char *pstTemp = (char *) calloc(TAM_STRING,sizeof(char));

 sprintf(pstTemp,"%s=%s",pstVariavel,pstValor);
 if(FindSection(pstSecao))
    if(FindVariavel(pstVariavel))
        strcpy(lsttmpProfile->pstVarValor,pstTemp);
    else
        AdicionaVariavel(pstTemp);
 else {
    AdicionaSecao(pstSecao);
    AdicionaVariavel(pstTemp);
 }
 free(pstTemp);
 return true;
}
//---------------------------------------------------------------------------
bool TIniFile::WriteInteger(char *pstSecao, char *pstVariavel, int nValor)
{
 bAlterado = true;
 char *pstTemp = (char *) calloc(TAM_STRING,sizeof(char));
    
 sprintf(pstTemp,"%s=%d",pstVariavel,nValor);
 if(FindSection(pstSecao))
    if(FindVariavel(pstVariavel))
        strcpy(lsttmpProfile->pstVarValor,pstTemp);
    else
        AdicionaVariavel(pstTemp);
 else {
    AdicionaSecao(pstSecao);
    AdicionaVariavel(pstTemp);
 }
 free(pstTemp);
 return true;
}
//---------------------------------------------------------------------------
bool TIniFile::ReadString(char *pstSecao, char *pstVariavel, char *pstValor)
{
 bool bRet = false;
 char *pstTemp = (char *) calloc(TAM_STRING,sizeof(char));
 int nCounter;

 if(FindSection(pstSecao))
     if(FindVariavel(pstVariavel)) {
        nCounter = strchr(lsttmpProfile->pstVarValor,'=') - lsttmpProfile->pstVarValor + 1;
        strncpy(pstValor,lsttmpProfile->pstVarValor+nCounter,strlen(lsttmpProfile->pstVarValor)-nCounter);
		pstValor[strlen(lsttmpProfile->pstVarValor)-nCounter]=0x00;
        bRet = true;
     }
 free(pstTemp);

 return bRet;
}
//---------------------------------------------------------------------------
int TIniFile::ReadInteger(char *pstSecao, char *pstVariavel, int nDefault)
{
 int nInteger = nDefault;
 char *pstValor;

 if(FindSection(pstSecao) && FindVariavel(pstVariavel)) {
     pstValor = strchr(lsttmpProfile->pstVarValor, '=');
     if(pstValor)
         nInteger = (int)strtol(pstValor + 1, NULL, 10);
 }

 return nInteger;
}
//---------------------------------------------------------------------------
int TIniFile::ReadHexadecimal(char *pstSecao, char *pstVariavel, int nDefault)
{
 int nInteger = nDefault;
 char *pstValor;

 if(FindSection(pstSecao) && FindVariavel(pstVariavel)) {
     pstValor = strchr(lsttmpProfile->pstVarValor, '=');
     if(pstValor)
         nInteger = (int)strtol(pstValor + 1, NULL, 0);
 }
 return nInteger;
}
//---------------------------------------------------------------------------
bool TIniFile::FindSection(char *pstSection)
{
 bool bRet = false;
 SECTION *lp;

 for(lp=pLstArquivo;lp;lp=lp->prox)
     if(!strcmp(pstSection,lp->pstNome)) {
       lsttmpSecao = lp;
       bRet = true;
       break;
     }
 return bRet;
}
//---------------------------------------------------------------------------
bool TIniFile::FindVariavel(char *pstVariavel)
{
 bool bRet = false;
 PROFILE *lp;
 
 lsttmpProfile = NULL;

 for(lp=lsttmpSecao->VarValores;lp;lp=lp->prox)
     if(strncmp(lp->pstVarValor, pstVariavel, strlen(pstVariavel)) == 0 &&
        lp->pstVarValor[strlen(pstVariavel)] == '=' && lp->pstVarValor[0] != ';') {
       lsttmpProfile = lp;
       bRet = true;
       break;
     }
 return bRet;
}
//---------------------------------------------------------------------------
void TIniFile::LimpaString(char *pstStr)
{
 int nCounter;
 char *pstFimLinha;
 
 pstFimLinha = strrchr(pstStr,'\r');
 if(pstFimLinha) {
     nCounter = (int)(pstFimLinha - pstStr);
     pstStr[nCounter] = 0x00;
 }
 pstFimLinha = strrchr(pstStr,'\n');
 if(pstFimLinha) {
     nCounter = (int)(pstFimLinha - pstStr);
     pstStr[nCounter] = 0x00;
 }

}
//---------------------------------------------------------------------------
#endif
