#include "markov_press.h"
#include <iostream>
#include <fstream>

int main()
{
    double freq[27][27][27];

    std::ifstream ifs("dist.male.first");
    markov_press::fill_stochastic_matrix(ifs, freq);
    ifs.close();

    srand(time(0));
    for (int i=0; i<10; ++i) {
        std::cout << markov_press::gen_word(freq) << std::endl;
    }
}

