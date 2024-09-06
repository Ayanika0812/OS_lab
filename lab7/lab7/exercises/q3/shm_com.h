#define SHM_COM_H

#define TEXT_SZ 256

struct shared_use_st {
    char message[TEXT_SZ];
    int data_available;  // Flag to indicate if data is available
};


