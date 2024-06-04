#ifndef COMMON_H
#define COMMON_H

#define OK 0
#define FAIL -1

extern int num_states;
extern int num_symbols;
extern char* symbols;

typedef struct machine_conf_t {
    
    int num_states;
    int num_symbols;

    int **table;           
    char *symbols; // The alphabet
    int first_state;

    // Data for each state
    char **labels; 
    int *accept; // 1 if acceptance state, 0 otherwise

} machine_conf_t;

char *get_char(char c);
void errorMessage(char *message);

#endif // COMMON_H
