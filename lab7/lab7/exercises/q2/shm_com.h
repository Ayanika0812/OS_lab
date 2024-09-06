
#define SHM_COM_H

#define TEXT_SZ 2

struct shared_use_st {
    char letter;
    char next_letter;
    int written_by_parent;
};

