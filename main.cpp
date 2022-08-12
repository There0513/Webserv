#include <unistd.h>
#include <stdio.h>

int main(int ac, char **av) {

    if (ac != 2) {
        cout << "ERROR: Wrong number of arguments\n" << endl; 
        return (1); 
    }

    try {
        parseConfig(av[1]);
    }
    catch {
        cerr << "ERROR: not able to parse file" << endl;
    }

    return (0);
}