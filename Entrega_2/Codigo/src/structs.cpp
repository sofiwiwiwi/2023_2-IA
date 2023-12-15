#ifndef STRUCTS
#define STRUCTS

struct nodo {
    long double coordenada_x;
    long double coordenada_y;
    int demanda;
    nodo(long double x, long double y, int demand){
        coordenada_x = x;
        coordenada_y = y;
        demanda = demand;
    }
    nodo(){}
};

struct arco {
    int dinero;
    long double riesgo;
    int id_nodo_actual;
    
    arco(int money, long double risk, int id){
        dinero = money;
        riesgo = risk;
        id_nodo_actual = id;
    }
    arco(){}
};
#endif