#include <iostream>
#include <vector>
#include <assert.h>

class ListGraph;

class MatrixGraph;

class IGraph {
public:

    virtual ~IGraph() {}

    IGraph() {};

    IGraph(IGraph *_oth) {};

    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const = 0;

    virtual void GetNextVertices(int vertex, std::vector<int> &vertices) const = 0;

    virtual void GetPrevVertices(int vertex, std::vector<int> &vertices) const = 0;

    virtual void fillVertices(ListGraph *graph) = 0;

    virtual void fillVertices(MatrixGraph *graph) = 0;
};


class ListGraph : public IGraph {
    std::vector<std::vector<int>> verticesGraph;
public:
    ListGraph() {}

    ListGraph(IGraph *graph) {
        graph->fillVertices(this);
    }

    void fillVertices(ListGraph *graph) override {
        graph->verticesGraph = this->verticesGraph;
    }

    void fillMatrixGraph(MatrixGraph *graphM, ListGraph *graphL);

    void fillVertices(MatrixGraph *graph) override {
        fillMatrixGraph(graph, this);
    }

    ListGraph(ListGraph *graph) {
        verticesGraph = graph->verticesGraph;
    }

    void operator=(const ListGraph *graph) {
        verticesGraph = graph->verticesGraph;
    }

    virtual void AddEdge(int from, int to) override {
        assert(from > 0 && to > 0);

        int verticesMax = from >= to ? from : to;
        if (verticesMax > VerticesCount()) {
            for (int i = VerticesCount(); i < verticesMax; ++i) {
                verticesGraph.push_back(std::vector<int>{});
            }
        }
        verticesGraph[from - 1].push_back(to - 1);
    }


    int VerticesCount() const override {
        return verticesGraph.size();
    }

    void GetNextVertices(int vertex, std::vector<int> &vertices) const override {
        for (int i = 0; i < verticesGraph[vertex].size(); ++i) {
            vertices.push_back(verticesGraph[vertex][i]);
        }
    }

    void GetPrevVertices(int vertex, std::vector<int> &vertices) const override {
        for (int i = 0; i < VerticesCount(); ++i) {
            for (int j = 0; j < verticesGraph[i].size(); ++j) {
                if (vertex == verticesGraph[i][j]) {
                    vertices.push_back(i);
                }
            }
        }
    };

    void show() {
        std::cout << "ListGraph:" << std::endl << std::endl;

        for (int i = 0; i < VerticesCount(); ++i) {
            std::vector<int> nextVertices;
            GetNextVertices(i, nextVertices);

            std::cout << i + 1 << " -> ";
            for (int j = 0; j < nextVertices.size(); ++j) {
                std::cout << ((j > 0) ? ", " : "") << nextVertices[j] + 1;
            }
            std::cout << std::endl;

            std::vector<int> prevVertices;
            GetPrevVertices(i, prevVertices);
            std::cout << i + 1 << " <- ";
            for (int j = 0; j < prevVertices.size(); ++j) {
                std::cout << ((j > 0) ? ", " : "") << prevVertices[j] + 1;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    };
};


class MatrixGraph : public IGraph {
    std::vector<std::vector<int>> verticesGraph;
public:
    MatrixGraph() {}

    MatrixGraph(IGraph *graph) {
        graph->fillVertices(this);
    }

    void fillVertices(ListGraph *graph) override {
        for (int i = 0; i < VerticesCount(); ++i) {
            std::vector<int> nextVertices;
            GetNextVertices(i, nextVertices);
            for (int j = 0; j < nextVertices.size(); ++j) {
                graph->AddEdge(i + 1, nextVertices[j] + 1);
            }

        }

    }

    void fillVertices(MatrixGraph *graph) override {
        graph->verticesGraph = this->verticesGraph;
    }

    MatrixGraph(MatrixGraph *graph) {
        verticesGraph = graph->verticesGraph;
    }

    void operator=(const MatrixGraph *graph) {
        verticesGraph = graph->verticesGraph;
    }

    void AddEdge(int from, int to) override {
        assert(from > 0 && to > 0);

        int verticesMax = from >= to ? from : to;
        for (int i = 0; i < verticesMax; ++i) {
            if (VerticesCount() < verticesMax) {
                verticesGraph.push_back(std::vector<int>{0});
            }
            while (verticesGraph[i].size() < verticesMax) {
                verticesGraph[i].push_back(0);
            }
        }

        verticesGraph[from - 1][to - 1] = 1;

/*
        for (int i = 0; i < matrix.size(); ++i) {
            for (int j = 0; j < matrix[i].size(); ++j) {
                std::cout << matrix[i][j];
            }
            std::cout << std::endl;
        }
        std::cout << "-----------------" << std::endl;
*/

    }

    int VerticesCount() const override {
        return verticesGraph.size();
    }

    void GetNextVertices(int vertex, std::vector<int> &vertices) const override {
        for (int i = 0; i < verticesGraph[vertex].size(); ++i) {
            if (verticesGraph[vertex][i] == 1) {
                vertices.push_back(i);
            }
        }
    }

    void GetPrevVertices(int vertex, std::vector<int> &vertices) const override {
        for (int i = 0; i < VerticesCount(); ++i) {
            if (verticesGraph[i][vertex] == 1) {
                vertices.push_back(i);
            }
        }
    }

    void show() {
        std::cout << "MatrixGraph:" << std::endl << std::endl;
        for (int i = 0; i < VerticesCount(); ++i) {
            std::vector<int> nextVertices;
            GetNextVertices(i, nextVertices);

            std::cout << i + 1 << " -> ";
            for (int j = 0; j < nextVertices.size(); ++j) {
                std::cout << ((j > 0) ? ", " : "") << nextVertices[j] + 1;
            }
            std::cout << std::endl;

            std::vector<int> prevVertices;
            GetPrevVertices(i, prevVertices);
            std::cout << i + 1 << " <- ";
            for (int j = 0; j < prevVertices.size(); ++j) {
                std::cout << ((j > 0) ? ", " : "") << prevVertices[j] + 1;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};

void ListGraph::fillMatrixGraph(MatrixGraph *graphM, ListGraph *graphL) {
    for (int i = 0; i < graphL->VerticesCount(); ++i) {
        std::vector<int> nextVertices;
        graphL->GetNextVertices(i, nextVertices);
        for (int j = 0; j < nextVertices.size(); ++j) {
            graphM->AddEdge(i + 1, nextVertices[j] + 1);
        }
    }
}

int main() {

    ListGraph *lg = new ListGraph();
    lg->AddEdge(1, 2);
    lg->AddEdge(2, 3);
    lg->AddEdge(3, 4);
    lg->AddEdge(4, 1);
    lg->AddEdge(1, 3);

    ListGraph *lg2 = new ListGraph(lg);
    ListGraph *lg3 = lg2;


    std::cout << "lg:" << std::endl;
    lg->show();
    std::cout << "lg2:" << std::endl;
    lg2->show();
    std::cout << "lg3:" << std::endl;
    lg3->show();

    MatrixGraph *mg = new MatrixGraph();
    mg->AddEdge(1, 2);
    mg->AddEdge(2, 3);
    mg->AddEdge(3, 4);
    mg->AddEdge(4, 1);
    mg->AddEdge(1, 3);

    MatrixGraph *mg2 = new MatrixGraph(mg);
    MatrixGraph *mg3 = mg2;

    std::cout << "mg:" << std::endl;
    mg->show();
    std::cout << "mg2:" << std::endl;
    mg2->show();
    std::cout << "mg3:" << std::endl;
    mg3->show();

    IGraph *lg0 = lg;
    IGraph *mg0 = mg;

    ListGraph *lg4 = new ListGraph(mg0);
    MatrixGraph *mg4 = new MatrixGraph(lg0);


    std::cout << "lg4:" << std::endl;
    lg4->show();
    std::cout << "mg4:" << std::endl;
    mg4->show();

    return 0;
}
