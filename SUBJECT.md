# Mini Server

#### Nome do executável

    mini_serv

#### Arquivos a enviar

    mini_serv.c

#### Funções permitidas:

`write` `close` `select` `socket` `accept` `listen` `send` `recv` `bind` `strstr` `malloc` `realloc` `free` `calloc` `bzero` `atoi` `sprintf` `strlen` `exit` `strcpy` `strcat` `memset`

---

Escreva um program que aguarde a ligação de clientes numa determinada porta em 127.0.0.1 e permita que os clientes comuniquem entre si.

Este programa receberá como primeiro argumento a porta à qual se deve ligar.

Se não for fornecido nenhum argumento, deve escrever em `stderr` *Wrong number of arguments* seguida por um *\\n* e uma saida com estado 1.

Se uma chamada de sistema devolver um erro antes de o programa começar a aceitar ligações, deve escrever em `stderr` *Fatal error* seguido por um *\\n* e uma saida com o estado 1.

Se não for possível alocar memória, deve escrever em `stderr` *Fatal error*, seguido de um *\\n*, e terminar com estado 1.

O seu programa deve ser não-bloqueante, mas os clientes podem ser preguiçosos e, se não lerem a mensagem, não deve desligá-los...

O seu programa não deve conter o pré-processador *#define*.

O seu programa deve ouvir apenas em 127.0.0.1.

O **fd** que receberá já estará configurado para que as funções `recv` ou `send` bloqueiem se a função `select` não tiver sido chamada antes de as chamar, mas não bloqueará caso contário.

##  Requisitos

### Quando o cliente se liga ao servidor:

-   Será atribuído um ID ao cliente. O primeiro cliente receberá o ID 0 e cada cliente receberá o ID do último cliente +1;
-   %d será substituído por este número;
-   Uma mensagem é enviada para todos os clientes que estiverem conectados ao servidor: *server: client %d just arrived.*.
-   Clients precisam ser capazes de enviar mensagens ao seu programa.
-   Uma única mensagem pode conter vários caracteres.
-   Quando o servidor receber uma mensagem, deve reenviá-la a todos os outros clientes com *client %d:* antes de cada linha!

### Os clientes devem poder enviar mensagens para o seu programa.

- As mensagens serão compostas apenas por caracteres imprimíveis; não é necessário verificar.
- Uma única mensagem pode conter vários caracteres.
- Quando o servidor receber uma mensagem, deve reenviá-la a todos os outros clientes com «cliente %d: » antes de cada linha!

### Quando um cliente se desliga do servidor:

- É enviada uma mensagem a todos os clientes que estavam ligados ao servidor: «servidor: o cliente %d acabou de sair».

---

Não são permitidos fugas de memória ou de fd.

Para o ajudar, encontrará o ficheiro main.c com o início de um servidor e, talvez, algumas funções úteis. (Tenha em atenção que este ficheiro utiliza funções proibidas ou escreve coisas que não devem constar no seu programa final).

-   **Aviso:** o nosso testador espera que envie as mensagens o mais rapidamente possível. Não utilize buffers desnecessários.

A avaliação pode demorar um pouco mais do que o habitual...

    Dica: pode usar o nc para testar o seu programa.
    Dica: deve usar o nc para testar o seu programa.
    Dica: Para testar, pode usar fcntl(fd, F_SETFL, O_NONBLOCK), mas use o select e NUNCA verifique EAGAIN (man 2 send).