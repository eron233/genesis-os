# Gênesis OS

Kernel experimental independente, inicialmente x86-64. O sistema operacional Gênesis não é o aplicativo Íris e não contém memória, credenciais ou funcionalidades privadas da Íris.

## Estado atual

**Branch `lab`:** código-fonte Multiboot1 + transição para modo longo de 64 bits, paginação provisória de 1 GiB, console VGA/COM1, IDT experimental de 32 vetores, handlers de exceção e um teste de `int3`. Inclui um demonstrador de algoritmo round-robin, **sem** escalonamento real, processos ou isolamento.

**Branch `main`:** somente escopo documental até a homologação.

Nenhuma versão é adequada para instalação em hardware pessoal. Não há sistema de arquivos, rede, drivers gráficos, processos de usuário ou UEFI nativo.

## Compilar no Linux x86-64

Requer `make`, Python 3, GCC + binutils ou Clang + LLD.

```bash
make clean check
# alternativa:
make clean check CC=clang LD=ld.lld
```

O comando verifica ELF/Multiboot e roda testes unitários **no computador de desenvolvimento**. Isso não comprova que o kernel inicializa.

Para testar o boot real, instale `qemu-system-x86` e execute:

```bash
make smoke
make run
```

O smoke exige os marcadores `GENESIS_BOOT_STAGE0`, `GENESIS_IDT_CHECK: OK`, `GENESIS_SCHEDULER_CHECK: OK` e `GENESIS_KERNEL_READY` pela COM1. Se não houver QEMU, falha explicitamente.

## Arquitetura

Novo código de sistema tenderá a Rust `no_std`, com C/Assembly em bootstrap e interfaces específicas. Nenhum módulo Rust está integrado ou homologado ainda. A arquitetura final será definida por benchmarks, isolamento e manutenção, não por preferência ideológica de linguagem.

Veja `docs/ARCHITECTURE.md`, `docs/STATUS.md` e `AGENTS.md`.

## Isolamento e promoção

Sem GitHub Actions, sem rebase e sem atualizações de produção. A Íris privada permanece em `eron233/jarvis_core`. Alterações devem ser validadas em `lab`, depois promovidas apenas com evidência e autorização.
