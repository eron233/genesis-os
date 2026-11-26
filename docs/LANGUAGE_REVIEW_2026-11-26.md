# Revisão das linguagens do Gênesis OS
Data: 2026-11-26. Base lab: a06ef08397d3d063652f6705d924a9bc02534558.
A main contém apenas documentação. Em lab: bootstrap Assembly x86-64 Multiboot1, C freestanding, IDT de exceções, serial, VGA, demonstrador round-robin, ferramentas e testes Python. O scheduler atual guarda IDs, não troca contexto. Não houve boot validado em QEMU conforme STATUS.md. Não é um OS utilizável ainda.

## Linguagens por fronteira
- Assembly: entrada do boot, transição de modo, stubs de interrupção e troca de contexto quando implementada. Manter mínimo e auditável.
- C freestanding: preservar o estágio 0, serial, IDT e scheduler como referência; não reescrever sem paridade.
- Rust no_std: direção principal já prevista em ARCHITECTURE.md. Novos componentes após validação de toolchain e ABI, preferir código seguro e sem alocação no início. Unsafe somente em fronteiras documentadas (MMIO, tabelas de páginas, interrupções) com invariantes, ownership e testes.
- Python: ferramentas de build, verificação ELF e testes, nunca dependência do kernel.
- C++: apenas avaliar em serviços de userspace/bibliotecas maduras; não introduzir exceções, RTTI ou runtime C++ no kernel sem justificativa.
- Íris: processo opcional e separado do kernel, sem dependência de boot, sem segredos privados embutidos. Sem importação de código privado da Íris.

## Sequência e critérios de aceite
0. Validar o estágio C existente em checkout independente: make clean check, make smoke (QEMU). Não avançar main sem evidências seriais reais.
1. Experimento isolado em experiments/rust-abi: make rust-check, alvo x86_64-unknown-none, sem dependências externas, sem heap, panic abort, code-model large, sem red zone. Confere compilação de objeto e link C/Rust e ausência de símbolos indefinidos. Não altera genesis.elf. Em host com Rust, executar também rustc --edition=2021 --test experiments/rust-abi/probe.rs -o build/rust_abi_tests e ./build/rust_abi_tests (testes host, não boot). Fixar versão exata do toolchain após validação real; não instalar automaticamente.
2. ABI e linker: validar layouts repr(C), tipos de largura fixa, alinhamento, convenção SysV e flags de compilação. Introduzir panic handler e símbolos de runtime necessários em etapa própria. Testar link final e boot misto, sem remover bootstrap C.
3. Prioridade: mapa de memória real a partir do bootloader, alocador de páginas físicas, endereçamento virtual, proteção W^X/NX conforme suporte e page-fault, guard pages, TSS/IST, interrupções; testar invariantes e falhas. O mapeamento temporário atual de 1 GiB não comprova isolamento.
4. Processos em ring 3, context switch real, syscalls e IPC; scheduler Rust somente após testes de comportamento, latência e concorrência. Drivers virtuais antes de hardware real; permissões mínimas.
5. Userspace, sistema de arquivos, drivers reais, rede, desktop, integração opcional Íris. Comparar com Linux/Windows em mesmo hardware e workloads equivalentes (boot, RAM ociosa, latência, energia, IO, estabilidade). Não alegar superioridade sem dados.

## Estado desta revisão
Probe Rust/C e alvo opt-in incluídos. Rust/Cargo e QEMU não estão disponíveis no ambiente desta conversa; Git direto não resolveu github.com. Portanto não foi compilado nem executado o probe Rust, não houve boot e não se mudou a linguagem do kernel ativo. C/Assembly preservados. Esta proposta permanece fora de lab/main até validação. Sem GitHub Actions, force push ou publicação.
