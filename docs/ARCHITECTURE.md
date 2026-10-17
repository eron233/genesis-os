# Direção arquitetural do Gênesis

Sistema operacional independente, inicialmente x86-64.

- Boot: Assembly/C legado em ambiente Multiboot1 para validar o primeiro kernel; avaliar Limine/UEFI após o gate.
- Kernel principal futuro: Rust no_std, com limites de unsafe explícitos e verificáveis.
- Hardware: interfaces com escopo restrito. Não usar memória física como se fosse memória segura.
- Processos: isolamento de endereços com privilégios de CPU; syscalls, IPC e permissões mínimas.
- Drivers: virtualizados primeiro, físicos depois; mover serviços a userspace quando a base permitir.
- Desktop e Íris: processos opcionais fora do kernel, sem dependência de boot ou segredos embarcados.
- Metas de RAM e latência: comparar com Windows e Linux no mesmo hardware e tarefas equivalentes.
- Não declarar superioridade até benchmarks reproduzíveis.
