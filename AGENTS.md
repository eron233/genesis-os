# Gênesis OS: regras de desenvolvimento

- Projeto independente de Íris, Nexus-world e RR; não alterar outros repositórios.
- Branch `lab` para experimentos. `main` só após compilação, boot real e homologação.
- Um kernel compilado não é um kernel inicializado: verificar QEMU com saída serial.
- Preferir Rust no_std para novos componentes, após toolchain e linking misto validados.
- Assembly e C são permitidos nos estágios de boot e interfaces de arquitetura.
- Interfaces de hardware perigosas devem permanecer pequenas, explícitas e auditáveis.
- Não importar memórias, chaves, pesos, runtime ou funções privadas da Íris.
- Não usar GitHub Actions, git rebase, publicação de produção ou force push.
- Não anunciar compatibilidade, desempenho ou segurança sem testes reproduzíveis.
- Registar mudanças e provas em documentação técnica, sem relatórios dispersos.
