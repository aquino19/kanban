# project-kanban
Felipe Marcelo Aquino da Costa

          +-------------------+
          |   ItemKanban      |
          +-------------------+
          | virtual mostrar() |
          | virtual moverParaColuna() |
          +-------^-----------+
                  ^
                  |
                  | 1
          +-------+-------+
          |     Tarefa    |
          +---------------+
          | - descricao: std::string   |
          | - status: std::string      |
          +---------------------------+
          | + Tarefa(descricao: const std::string&) |
          | + getDescricao(): std::string           |
          | + getStatus(): std::string              |
          | + setStatus(novoStatus: const std::string&) |
          | + mostrar() override                     |
          | + moverParaColuna(novaColuna: const std::string&) override |
          +---------------------------+
                          ^
                          |
          +---------------+--------------+
          |           Coluna             |
          +---------------+--------------+
          | - nome: std::string          |
          | - itens: std::vector<ItemKanban*> |
          +-------------------------------+
          | + Coluna(nome: const std::string&) |
          | + adicionarItem(item: ItemKanban*) |
          | + removerItem(descricao: const std::string&) : bool |
         
