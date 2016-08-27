# Comunicação TCP
Projeto de comunicação entre clientes e servidor, utilizando o protocolo TCP.

O cliente envia mensagens para o servidor, o qual transformará em maiúsculo e retornará para o cliente.

## Funcionamento
### Cliente
O cliente envia para o servidor as mensagens que o usuário inserir no terminal.

### Servidor
O objetivo do servidor é enviar novamente para o cliente todas as mensagens que recebeu. Porém, a mensagem será retornada em maiúsculo.

### Exemplo
1. Usuário digitou "Hello world!"
2. Cliente enviou "Hello world!" para o servidor
3. Servidor alterou a mensagem para "HELLO WORLD!"
4. Servidor envia a nova mensagem para o cliente
5. O cliente mostra a mensagem na tela
