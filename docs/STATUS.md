# Gênesis OS: P0 de laboratório, 2026-10-16

## Evidência local nesta sessão
- GCC/GNU ld: compilação, linking ELF64, testes de host e verificações estáticas aprovados.
- Clang/LLD: mesmos testes aprovados.
- Fonte implementada localmente: IDT inicial com 32 vetores, stubs de exceção, handler serial e autoteste `int3`.
- 10 testes Python aprovados; testes C do escalonador, codificação IDT e layout trap frame aprovados.
- `make smoke`: **não aprovado**. QEMU indisponível no ambiente de desenvolvimento.
- Rust ainda não instalado no ambiente; nenhum módulo Rust compilado ou validado.
- Nenhum boot real, IRQ, TSS/IST, driver, usuário ou API de sistema foi homologado.

## Limite de publicação
O código compilado encontra-se no pacote de laboratório gerado na conversa; seu upload ao GitHub ainda precisa ser verificado. Este documento não afirma que os fontes da build local estão sincronizados com a branch remota.

## Próximo gate
Executar teste real em QEMU, corrigir falhas, testar exceções e só então começar a arquitetura de memória.
