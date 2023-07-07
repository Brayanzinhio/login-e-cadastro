#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define TAMNOME 60
#define TAMSENHA 30
#define MIN 6 // mínimo de caracteres em uma senha ou nome
#define TENTATIVA 5

typedef struct {
    char Nome[TAMNOME];
    char Senha[TAMSENHA];
} dados;

void cadastro();
void login();
int verifica_Login(char senha[], char nome[]);
void limpaVetor(char vetor[]);
void tratarErroArquivo(FILE* arquivo, const char* mensagem);

int main() {
    setlocale(LC_ALL, "Portuguese");

    int escolha;

    printf("    Sistema login   \n");
    printf("\nEscolha uma opção:\n");
    printf("[1] Cadastrar-se\n");
    printf("[2] Fazer login\n");
    printf("[3] Sair\n");

    scanf("%i", &escolha);

    switch (escolha) {
        case 1:
            cadastro();
            break;
        case 2:
            login();
            break;
        case 3:
            printf("Saindo...");
            break;
        default:
            printf("Escolha inválida.");
            break;
    }

    return 0;
}

void cadastro() {
    dados usuario_cadastro;
    FILE* Cadastros;

    Cadastros = fopen("cadastros.dat", "ab");
    tratarErroArquivo(Cadastros, "falha ao abrir o arquivo");

    fflush(stdin);
    printf("    Cadastrar novo usuario  \n");
    printf("Nome:");
    fgets(usuario_cadastro.Nome, TAMNOME, stdin);
    usuario_cadastro.Nome[strcspn(usuario_cadastro.Nome, "\n")] = '\0';

    if (verifica_Login("**", usuario_cadastro.Nome) == 10) {
        fclose(Cadastros);
        printf("O nome %s já existe.\n", usuario_cadastro.Nome);
        exit(1);
    }

    printf("Senha:");
    fgets(usuario_cadastro.Senha, TAMSENHA, stdin);
    usuario_cadastro.Senha[strcspn(usuario_cadastro.Senha, "\n")] = '\0';

    printf("Confirme a senha:");
    char confirmacao[TAMSENHA];
    fgets(confirmacao, TAMSENHA, stdin);
    confirmacao[strcspn(confirmacao, "\n")] = '\0';

    if (strlen(usuario_cadastro.Nome) < MIN || strlen(usuario_cadastro.Senha) < MIN) {
        fclose(Cadastros);
        printf("\nSenha e/ou nome inválidos!\n(Senhas e nomes devem ter mais de %i caracteres).\n", MIN);
        exit(1);
    }

    if (strcmp(usuario_cadastro.Senha, confirmacao) != 0) {
        fclose(Cadastros);
        printf("\nAs senhas não correspondem.\n");
        exit(1);
    }

    fwrite(&usuario_cadastro, sizeof(dados), 1, Cadastros);
    printf("\nSalvo com sucesso!\n");
    fclose(Cadastros);
    exit(1);
}

void login() {
    dados usuario_login;
    int i = 0;
    FILE* Login;

    Login = fopen("cadastros.dat", "rb");
    tratarErroArquivo(Login, "falha ao abrir o arquivo");

    fflush(stdin);
    printf("    Login  \n");
    printf("Nome:");
    fgets(usuario_login.Nome, TAMNOME, stdin);
    usuario_login.Nome[strcspn(usuario_login.Nome, "\n")] = '\0';

    while (verifica_Login(usuario_login.Senha, usuario_login.Nome) == 10 && i != TENTATIVA) {
        printf("Senha incorreta para %s, restam %i tentativas.\n", usuario_login.Nome, TENTATIVA - i);
        limpaVetor(usuario_login.Senha);
        printf("Senha:");
        fgets(usuario_login.Senha, TAMSENHA, stdin);
        usuario_login.Senha[strcspn(usuario_login.Senha, "\n")] = '\0';
        i++;
    }

    if (verifica_Login(usuario_login.Senha, usuario_login.Nome) == 0) {
        fclose(Login);
        printf("Logado com sucesso.\n");
        printf("Bem-vindo(a), %s!\n", usuario_login.Nome);
        exit(1);
    } else if (verifica_Login(usuario_login.Senha, usuario_login.Nome) == 1) {
        fclose(Login);
        printf("Usuário %s inexistente.\n", usuario_login.Nome);
        exit(1);
    } else {
        printf("Número limite de tentativas atingido.\n");
        exit(1);
    }
}

int verifica_Login(char senha[], char nome[]) {
    dados _usuario2;
    FILE* Verifica2;

    Verifica2 = fopen("cadastros.dat", "rb");
    tratarErroArquivo(Verifica2, "falha ao abrir o arquivo");

    while (fread(&_usuario2, sizeof(dados), 1, Verifica2)) {
        if (strcmp(_usuario2.Nome, nome) == 0 && strcmp(_usuario2.Senha, senha) == 0) {
            fclose(Verifica2);
            return 0;
        } else if (strcmp(_usuario2.Nome, nome) == 0 && strcmp(_usuario2.Senha, senha) != 0) {
            fclose(Verifica2);
            return 10;
        }
    }

    fclose(Verifica2);
    return 1;
}

void limpaVetor(char vetor[]) {
    for (int i = 0; i < strlen(vetor); i++) {
        vetor[i] = '\0';
    }
}

void tratarErroArquivo(FILE* arquivo, const char* mensagem) {
    if (arquivo == NULL) {
        printf("%s\n", mensagem);
        exit(1);
    }
}
