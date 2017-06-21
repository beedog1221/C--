#ifndef __markov_press__
#define __markov_press__

//#include <boost/algorithm/string.hpp>
//#include <boost/lexical_cast.hpp>
//#include <boost/bind.hpp>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

namespace markov_press {

#define POS(x) (::islower(x) ? x-97+1 : 0)
#define CHAR(x) (0 < x && x <= 26 ? x+97-1 : 0)
#define ATTEMPTS 20

// prereq srand
std::string gen_word(const double freq[27][27][27])
{
    std::string word;

    for (int p0=0,b1=0,b2=0;; b2=b1,b1=p0) {
        double f = static_cast<double>(rand()) / RAND_MAX;
        
        double sum = 0.0;
        for (int k=0; k<27; ++k) {
            sum += freq[b2][b1][k];

            if (sum > f) {
                p0 = k;
                break;
            }
        }

        if (CHAR(p0) == 0) {
            break;
        }
        word.push_back(CHAR(p0));
    }

    return word;
}

// input is line data; column 1 is the word, column 2 is the freqency
void fill_stochastic_matrix(std::istream& is, double freq[27][27][27])
{
    while (is.good()) {
        char buf[256];
        is.getline(buf, 256);

        std::vector<std::string> tokens;
        std::string line(buf);
        boost::algorithm::split(tokens, line, boost::algorithm::is_space(), boost::algorithm::token_compress_on);
        if (tokens.size() >= 2) {
            std::string word = tokens[0];
            double frequency = boost::lexical_cast<double>(tokens[1]);

            const char* last = word.c_str() + word.length();
            const char* notalpha = std::find_if(word.c_str(), last, boost::bind(::isalpha, _1) == 0);
            if (notalpha != last) {
                break;
            }

            std::transform(word.begin(), word.end(), word.begin(), ::tolower);

            for (int i=0,p0=0,b1=0,b2=0; i<=(int)word.length(); ++i,b2=b1,b1=p0) {
                p0 = POS(word[i]);
                freq[b2][b1][p0] += frequency;
            }
        }
    }

    // normalize
    for (int i=0; i<27; ++i) {
        for (int j=0; j<27; ++j) {
            double sum = 0.0;
            for (int k=0; k<27; ++k) {
                sum += freq[i][j][k];
            }

            for (int k=0; k<27; ++k) {
                 if (sum > 0) {
                     freq[i][j][k] = freq[i][j][k] / sum;   
                 }
                 else {
                     freq[i][j][k] = 1.0/27;   
                 }
            }
        }
    }
}

void print_stochastic_matrix(const double freq[27][27][27], std::ostream& os)
{
    os << "double freq[27][27][27] = { " << std::endl;
    for (int i=0; i<27; ++i) {
        os << "    {" <<  std::endl;
        for (int j=0; j<27; ++j) {
            os << "        { ";
            for (int k=0; k<27; ++k) {
                os << freq[i][j][k] << ", ";
            }
            os << "}," << std::endl;
        }
        os << "    }," << std::endl;
    }
    os << "};" << std::endl;
}

}; // namespace __markov_press__

#endif // __markov_press__

