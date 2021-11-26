// next_fit.cpp

#include <chrono>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>

typedef struct Solution
{
    size_t k;
    size_t N;
    size_t* items_bins;
} Solution;


Solution* next_fit(size_t C, size_t N, size_t* items) {
    Solution *s = new Solution;
    s->k = 1;
    s->N = N;
    s->items_bins = new size_t[N];
    size_t S = 0;
    for (size_t i = 0; i < N; ++i)
    {
        if (S + items[i] > C) {
            ++s->k;
            S = 0;
        }
        s->items_bins[i] = s->k;
        S += items[i];
    }
    
    return s;
}

Solution* first_fit(size_t C, size_t N, size_t* items) {
    Solution *s = new Solution;
    s->k = 1;
    s->N = N;
    s->items_bins = new size_t[N];
    size_t* S = new size_t[N];
    std::fill_n(S,N,0);
    bool packed = false;
    for (size_t i = 0; i < N; ++i)
    {
        packed = false;
        for (size_t j = 0; j < s->k; ++j)
        {
            if (S[j] + items[i] <= C) {
                S[j] += items[i];
                s->items_bins[i] = j+1;
                packed = true;
                break;
            }
        }
        if (!packed) {
            S[s->k] = items[i];
            ++s->k;
            s->items_bins[i] = s->k;
        }
    }
    delete[] S;
    return s;
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        std::cout << "O programa deve receber apenas o caminho para arquivo com os dados da instância." << std::endl;
        exit(1);
    }
    std::ifstream fileSource(argv[1]);

    size_t N = 0;
    size_t C = 0;
    size_t* items = nullptr;
    if (!fileSource) {
        std::cerr << "Erro: Arquivo não pode ser aberto - " << argv[1] << std::endl;
        exit(-1);
    } else {
        fileSource >> N;
        fileSource >> C;

        items = new size_t[N];

        for (size_t i = 0; i < N; ++i)
        {
            fileSource >> items[i];
        }
        fileSource.close();
    }

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    Solution* s = first_fit(C, N, items);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << std::left;
    for (size_t i = 0; i < s->N; ++i)
    {
        std::cout << "Item " << std::setw(4) << i+1 << " de valor " << std::setw(4) << items[i] << " na bin " << std::setw(4) << s->items_bins[i] << std::endl;
    }
    std::cout << "Número de bins: " << s->k << std::endl;
    std::cout << "Número de items: " << s->N << std::endl;    
    std::cout << "Tempo em nanosegundos: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() << std::endl;

    delete[] items;
    delete[] s->items_bins;
    delete s;
    return 0;
}