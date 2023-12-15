#include <iostream>
#include <vector>

#include "structs.cpp"

using namespace std;


bool restriction_1(vector < vector < arco > > &rutas_camiones){
    for(int i = 1; i < rutas_camiones.size(); i++){
        if(rutas_camiones[i].size() > 0 && rutas_camiones[i][0].id_nodo_actual != rutas_camiones[i][rutas_camiones[i].size() - 1].id_nodo_actual) return false;
    }
    return true;
}

bool restriction_2(vector < vector < arco > > &rutas_camiones){
    if(rutas_camiones[1].size() > 0 && rutas_camiones[1][0].id_nodo_actual == 0) return true;
    return false;
}

// bool restriction_3(vector < vector < arco > > &rutas_camiones){
//     for(int i = 1; i < rutas_camiones.size() - 1; i++){
//         if(rutas_camiones[i + 1].size() > 0){
//             if(rutas_camiones[i].size() == 0) return false;
//             if(rutas_camiones[i][rutas_camiones[i].size() - 1].id_nodo_actual != 0) return false;
//         }
//     }
//     return true;
// }
//el vector existe_ruta define explícitamente la restriccion 4 ya que solo permite una ruta por arco
//para la restrccion 5 el vector se construye de tal forma que nunca se va a violar esa restriccion


bool restriction_6(vector < vector < arco > > &rutas_camiones){
    for(int i = 1; i < rutas_camiones.size(); i++){
        if(rutas_camiones[i].size() > 0){
            if(rutas_camiones[i][0].dinero != 0) return false;
        }
    }    
    return true;
}

//en las instancias no se incluye la capacidad de cada vehiculo

bool restriction_9(vector < vector < arco > > &rutas_camiones){
    for(int i = 1; i < rutas_camiones.size(); i++){
        if(rutas_camiones[i].size() > 0){
            if(rutas_camiones[i][0].riesgo != 0) return false;
        }
    }    
    return true;
}

bool restriction_10_11(vector < vector < arco > > &rutas_camiones, long double &riesgo){
    for(int i = 1; i < rutas_camiones.size(); i++){
        if(rutas_camiones[i].size() > 0 && rutas_camiones[i][rutas_camiones[i].size() - 1].riesgo > riesgo) return false;
    }
    return true;
}

bool restriction_checker(vector < vector < arco > > &rutas_camiones, long double &riesgo){
    if(restriction_1(rutas_camiones) &&
       restriction_2(rutas_camiones) &&
       //restriction_3(rutas_camiones) &&
       restriction_6(rutas_camiones) &&
       restriction_9(rutas_camiones) &&
       restriction_10_11(rutas_camiones, riesgo)) return true;
    else return false;
}