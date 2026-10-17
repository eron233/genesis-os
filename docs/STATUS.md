# Gênesis OS: laboratório, 2026-10-16

## Código publicado
Bootstrap x86-64, IDT de 32 exceções, stubs, diagnóstico serial, demonstrador de scheduler, verificador ELF e testes em `lab`. A `main` permanece apenas documental.

## Verificação local desta etapa
- Compilação/link ELF64 x86-64 com GCC/GNU ld: passou.
- Compilação/link com Clang/LLD: passou.
- 10 testes Python aprovados no artefato de desenvolvimento.
- Testes C de round-robin, codificação IDT e layout trap frame aprovados.
- Verificação estática ELF e Multiboot1 aprovada.
- `make smoke`: NÃO aprovado, pois o QEMU não está instalado neste ambiente.
- Sem demonstração de boot real nem prova de execução de `int3` em máquina virtual.
- Rust `no_std` ainda não integrado ou testado.
- Código da branch remota publicado, mas **não compilado novamente diretamente do checkout remoto** por indisponibilidade de rede Git no ambiente de testes. Os testes mencionados foram no pacote de trabalho local equivalente; diferenças de empacotamento remoto ainda dependem de checkout independente.

## Gates
1. Clonar `lab` e executar `make clean check`.
2. Instalar QEMU e executar `make smoke`, corrigindo qualquer falha.
3. Introduzir testes de exceções e limites de memória com evidências.
4. Introduzir toolchain Rust `no_std` reproduzível, então avançar para memória física e virtual.
5. Não promover para `main` antes do boot validado.
