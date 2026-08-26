#include <assert.h>
#include <stdio.h>

#include "inifiles.h"

int main(void)
{
    char arquivo[] = "teste.ini";
    char secaoGeral[] = "Geral";
    char secaoRede[] = "Rede";
    char chaveNome[] = "nome";
    char chavePorta[] = "porta";
    char chaveMascara[] = "mascara";
    char chaveEndereco[] = "endereco";
    char valor[ TAM_STRING ] = { 0 };
    FILE *fixture = fopen(arquivo, "w");

    assert(fixture != NULL);
    fprintf(fixture, "[Geral]\n");
    fprintf(fixture, "nome=teste-inifiles\n\n");
    fprintf(fixture, "[Rede]\n");
    fprintf(fixture, "porta=8080\n");
    fprintf(fixture, "mascara=0xFF00FF00\n");
    fclose(fixture);

    TIniFile ini(arquivo);

    assert(ini.ReadString(secaoGeral, chaveNome, valor));
    printf("nome = %s\n", valor);
    assert(ini.ReadInteger(secaoRede, chavePorta, -1) == 8080);
    printf("porta = %d\n", ini.ReadInteger(secaoRede, chavePorta, -1));
    assert((unsigned int)ini.ReadHexadecimal(secaoRede, chaveMascara, 0) == 0xFF00FF00u);
    printf("mascara = 0x%08X\n", (unsigned int)ini.ReadHexadecimal(secaoRede, chaveMascara, 0));

    assert(!ini.ReadString(secaoRede, chaveEndereco, valor));
    assert(ini.WriteString(secaoRede, chaveEndereco, (char *)"127.0.0.1"));
    assert(ini.WriteInteger(secaoRede, chavePorta, 9090));

    printf("\nConteudo carregado:\n");
    ini.PrintIniFile();
    printf("\nO arquivo sera atualizado ao finalizar o teste.\n");

    return 0;
}