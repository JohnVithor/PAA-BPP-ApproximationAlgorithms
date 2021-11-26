// driver.cpp

#include <chrono>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <map>
#include <algorithm>
#include <random>

typedef struct Solution
{
    size_t k;
    size_t N;
    size_t* items_bins;
} Solution;

Solution* not_implemented(size_t C, size_t N, size_t* items) {
    std::cout << "Não implementado ainda" << std::endl;
    exit(1);
}

bool decreasing(size_t i, size_t j) { return (i>j); }

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

Solution* worst_fit(size_t C, size_t N, size_t* items) {
    Solution *s = new Solution;
    s->k = 1;
    s->N = N;
    s->items_bins = new size_t[N];
    size_t* S = new size_t[N];
    std::fill_n(S,N,0);
    size_t smallest_content_i = N;
    size_t smallest_content = C;
    for (size_t i = 0; i < N; ++i)
    {
        smallest_content_i = N;
        smallest_content = C;
        for (size_t j = 0; j < s->k; ++j)
        {
            if (S[j] + items[i] <= C && S[j] < smallest_content) {
                smallest_content_i = j;
                smallest_content = S[j];
            }
        }
        if (smallest_content_i != N) {
            S[smallest_content_i] += items[i];
            s->items_bins[i] = smallest_content_i+1;
        } else {
            S[s->k] = items[i];
            ++s->k;
            s->items_bins[i] = s->k;
        }
    }
    delete[] S;
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

Solution* best_fit(size_t C, size_t N, size_t* items) {
    Solution *s = new Solution;
    s->k = 1;
    s->N = N;
    s->items_bins = new size_t[N];
    size_t* S = new size_t[N];
    std::fill_n(S,N,0);
    size_t largest_content_i = N;
    size_t largest_content = 0;
    for (size_t i = 0; i < N; ++i)
    {
        largest_content_i = N;
        largest_content = 0;
        for (size_t j = 0; j < s->k; ++j)
        {
            if (S[j] + items[i] <= C && S[j] > largest_content) {
                largest_content_i = j;
                largest_content = S[j];
            }
        }
        if (largest_content_i != N) {
            S[largest_content_i] += items[i];
            s->items_bins[i] = largest_content_i+1;
        } else {
            S[s->k] = items[i];
            ++s->k;
            s->items_bins[i] = s->k;
        }
    }
    delete[] S;
    return s;
}

Solution* next_fit_decreasing(size_t C, size_t N, size_t* items) {
    std::sort(items, items+N, decreasing);
    return next_fit(C, N, items);
}

Solution* worst_fit_decreasing(size_t C, size_t N, size_t* items) {
    std::sort(items, items+N, decreasing);
    return worst_fit(C, N, items);
}

Solution* first_fit_decreasing(size_t C, size_t N, size_t* items) {
    std::sort(items, items+N, decreasing);
    return first_fit(C, N, items);
}

Solution* best_fit_decreasing(size_t C, size_t N, size_t* items) {
    std::sort(items, items+N, decreasing);
    return best_fit(C, N, items);
}

int main(int argc, char const *argv[]) {
    if (argc != 4) {
        std::cout << "O programa deve receber o caminho para arquivo com os dados da instância. Seguido da sigla de identificação do algoritmo a ser utilizado:" << std::endl;
        std::cout << "NF, WF, FF, BF, NFD, WFD, FFD ou BFD" << std::endl;
        std::cout << "E por fim a seed para realizar o shuffle dos elementos antes de aplicar o algoritmo escolhido (Os items já estão ordenados nos arquivos)" << std::endl;
        exit(1);
    }
    std::ifstream fileSource(argv[1]);

    using pfunc = Solution* (*)(size_t, size_t, size_t*);
    std::map<std::string, pfunc> funcMap; 

    funcMap["NF"] = next_fit;
    funcMap["WF"] = worst_fit;
    funcMap["FF"] = first_fit;
    funcMap["BF"] = best_fit;
    funcMap["NFD"] = next_fit_decreasing;
    funcMap["WFD"] = worst_fit_decreasing;
    funcMap["FFD"] = first_fit_decreasing;
    funcMap["BFD"] = best_fit_decreasing;

    std::map<std::string, pfunc>::iterator it = funcMap.find(argv[2]);
    if (it == funcMap.end()){
        std::cout << "A sigla: '" << argv[2] << "' não é válida, por favor informe uma entre as seguintes:" << std::endl;
        std::cout << "NF, WF, FF, BF, NFD, WFD, FFD ou BFD" << std::endl;
        return 1;
    }
    size_t N = 0;
    size_t C = 0;
    size_t* items = nullptr;
    if (!fileSource) {
        std::cerr << "Erro: Arquivo não pode ser aberto - " << argv[1] << std::endl;
        return 1;
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

    std::shuffle(items, items+N, std::default_random_engine(std::strtoul(argv[3], nullptr, 10)));

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    Solution* s = it->second(C, N, items);

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