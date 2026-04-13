# PROJETOS COM GTK
projetos pessoais feitos em tempo livre que utilizam da biblioteca gtk3.0 para criação de GUI simples.
para compilação, os programas necessitam do GTK3.0 instalado na maquina

## [CALCULADORA](./CALCULADORA)
para executar utilize os comandos:

gcc calculadora.c -o calculadora $(pkg-config --cflags --libs gtk+-3.0)

./calculadora.c

## [JOGO DA VELHA](./JOGO_DA_VELHA)
para executar utilize os comandos:

gcc velha.c -o velha $(pkg-config --cflags --libs gtk+-3.0)

./velha.c

## [LOJA](./LOJA)
para executar utilize os comandos:

gcc loja.c -o loja $(pkg-config --cflags --libs gtk+-3.0)

./loja.c

