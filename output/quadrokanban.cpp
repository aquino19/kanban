///FELIPE MARCELO AQUINO DA COSTA
/// MATRÍCULA: 20200049662
/// LP1

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

/// Classe abstrata para itens do quadro Kanban
class ItemKanban {
public:
    virtual ~ItemKanban() {}
    virtual void mostrar() const = 0;
    virtual void moverParaColuna(const std::string& novaColuna) = 0;
};

/// Classe que representa uma tarefa no quadro Kanban
class Tarefa : public ItemKanban {
private:
    std::string descricao;
    std::string status;

public:
    // Construtor da classe Tarefa
    Tarefa(const std::string& descricao) : descricao(descricao), status("A fazer") {}

    // Métodos de acesso às informações da tarefa
    std::string getDescricao() const {
        return descricao;
    }

    std::string getStatus() const {
        return status;
    }

    void setStatus(const std::string& novoStatus) {
        status = novoStatus;
    }

    // Método para mostrar informações da tarefa
    void mostrar() const override {
        std::cout << "Tarefa: " << descricao << " - Status: " << status << std::endl;
    }

    // Método para mover a tarefa para outra coluna
    void moverParaColuna(const std::string& novaColuna) override {
        status = novaColuna;
    }
};

/// Classe que representa uma coluna no quadro Kanban
class Coluna {
private:
    std::string nome;
    std::vector<ItemKanban*> itens;

    // Função privada auxiliar para ordenar as tarefas com base na descrição usando Quick Sort
    void ordenarTarefasRecursivo(int esquerda, int direita) {
        if (esquerda >= direita) {
            return;
        }

        int i = esquerda;
        int j = direita;
        std::string pivo = dynamic_cast<Tarefa*>(itens[(esquerda + direita) / 2])->getDescricao();

        while (i <= j) {
            while (dynamic_cast<Tarefa*>(itens[i])->getDescricao() < pivo) {
                i++;
            }

            while (dynamic_cast<Tarefa*>(itens[j])->getDescricao() > pivo) {
                j--;
            }

            if (i <= j) {
                std::swap(itens[i], itens[j]);
                i++;
                j--;
            }
        }

        if (esquerda < j) {
            ordenarTarefasRecursivo(esquerda, j);
        }

        if (i < direita) {
            ordenarTarefasRecursivo(i, direita);
        }
    }

public:
    // Construtor da classe Coluna
    Coluna(const std::string& nome) : nome(nome) {}

    // Método para adicionar um item à coluna
    void adicionarItem(ItemKanban* item) {
        itens.push_back(item);
    }

    // Método para remover um item da coluna, baseado na descrição da tarefa
    bool removerItem(const std::string& descricao) {
        for (auto it = itens.begin(); it != itens.end(); ++it) {
            if (dynamic_cast<Tarefa*>(*it) && dynamic_cast<Tarefa*>(*it)->getDescricao() == descricao) {
                delete *it;
                itens.erase(it);
                return true;
            }
        }
        return false;
    }

    // Método público para ordenar as tarefas na coluna usando Quick Sort
    void ordenarTarefas() {
        ordenarTarefasRecursivo(0, itens.size() - 1);
    }

    // Método para mostrar os itens da coluna
    void mostrarItens() const {
        std::cout << "=== " << nome << " ===" << std::endl;
        for (const auto& item : itens) {
            item->mostrar();
        }
    }

    // Método para obter os itens da coluna
    const std::vector<ItemKanban*>& getItens() const {
        return itens;
    }
};

/// Classe que representa o quadro Kanban
class QuadroKanban {
public:
    // Colunas do quadro
    Coluna aFazer;
    Coluna emAndamento;
    Coluna concluido;

    /// Construtor da classe QuadroKanban
    QuadroKanban() : aFazer("A fazer"), emAndamento("Em andamento"), concluido("Concluído") {}

    /// Destrutor da classe QuadroKanban para liberar memória dos itens
    ~QuadroKanban() {
        for (const auto& item : aFazer.getItens()) {
            delete item;
        }
        for (const auto& item : emAndamento.getItens()) {
            delete item;
        }
        for (const auto& item : concluido.getItens()) {
            delete item;
        }
    }

    /// Método para adicionar uma tarefa à coluna "A fazer"
    void adicionarTarefa(const std::string& descricao) {
        Tarefa* tarefa = new Tarefa(descricao);
        aFazer.adicionarItem(tarefa);
    }

    /// Método para mover uma tarefa de uma coluna para outra
    void moverTarefa(const std::string& descricao, const std::string& novaColuna) {
        Tarefa* tarefaEncontrada = nullptr;
        std::vector<Coluna*> colunas = {&aFazer, &emAndamento, &concluido};

        // Percorrer as colunas para encontrar a tarefa
        for (Coluna* coluna : colunas) {
            for (auto item : coluna->getItens()) {
                Tarefa* tarefa = dynamic_cast<Tarefa*>(item);
                if (tarefa && tarefa->getDescricao() == descricao) {
                    tarefaEncontrada = tarefa;
                    break;
                }
            }
            // Se a tarefa foi encontrada, sai do loop externo
            if (tarefaEncontrada) {
                break;
            }
        }

        // Se a tarefa foi encontrada, mover para a nova coluna
        if (tarefaEncontrada) {
            // Remover a tarefa da coluna atual
            if (aFazer.removerItem(descricao)) {
                emAndamento.adicionarItem(tarefaEncontrada);
                tarefaEncontrada->moverParaColuna(novaColuna);
            } else if (emAndamento.removerItem(descricao)) {
                concluido.adicionarItem(tarefaEncontrada);
                tarefaEncontrada->moverParaColuna(novaColuna);
            } else if (concluido.removerItem(descricao)) {
                std::cout << "A tarefa já foi concluída e não pode ser movida." << std::endl;
            } else {
                std::cout << "Tarefa não encontrada no quadro Kanban." << std::endl;
            }
        } else {
            std::cout << "Tarefa não encontrada no quadro Kanban." << std::endl;
        }
    }

    /// Método para mostrar o quadro Kanban
    void mostrarQuadro() const {
        aFazer.mostrarItens();
        emAndamento.mostrarItens();
        concluido.mostrarItens();
    }
};

/// Função principal (main)
int main() {
    QuadroKanban quadro;

    quadro.adicionarTarefa("Tarefa 1");
    quadro.adicionarTarefa("Tarefa 2");
    quadro.adicionarTarefa("Tarefa 3");

    int opcao;
    std::string descricao;
    std::string novaColuna;

    while (true) {
        quadro.mostrarQuadro();

        std::cout << "\nOpções:\n";
        std::cout << "1. Adicionar Tarefa\n";
        std::cout << "2. Mover Tarefa\n";
        std::cout << "3. Sair\n";
        std::cout << "Escolha uma opção: ";
        std::cin >> opcao;

        if (opcao == 1) {
            std::cout << "Digite a descrição da nova tarefa: ";
            std::cin.ignore();
            std::getline(std::cin, descricao);
            quadro.adicionarTarefa(descricao);
        } else if (opcao == 2) {
            std::cout << "Digite a descrição da tarefa que deseja mover: ";
            std::cin.ignore();
            std::getline(std::cin, descricao);

            std::cout << "Digite a coluna de destino (A fazer, Em andamento ou Concluído): ";
            std::getline(std::cin, novaColuna);

            // Corrigindo a coluna de destino para letras minúsculas para evitar problemas de comparação
            std::transform(novaColuna.begin(), novaColuna.end(), novaColuna.begin(), ::tolower);

            // Mover a tarefa para a coluna de destino
            quadro.moverTarefa(descricao, novaColuna);
        } else if (opcao == 3) {
            break;
        } else {
            std::cout << "Opção inválida. Digite novamente.\n";
        }
    }

    return 0;
}