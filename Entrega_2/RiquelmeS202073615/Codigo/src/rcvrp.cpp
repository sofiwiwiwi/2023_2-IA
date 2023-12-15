#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <cmath>
#include <chrono>
#include <cstring>
#include <string>

#include "restrictions.cpp"
using namespace std;
using namespace std::chrono;

//lee todo el archivo
void read_instance(string nombre_archivo, int &n, long double &riesgo, vector< nodo > &nodos){
    ifstream archivo(nombre_archivo, ios::in);
    archivo >> n >> riesgo;
    nodos.resize(n);

    for(int i = 0; i < n; i++){
        archivo >> nodos[i].demanda;
    }

    for(int i = 0; i < n; i++){
        archivo >> nodos[i].coordenada_x >> nodos[i].coordenada_y;
    }
    archivo.close();
}

//obtiene distancia total de la ruta del camion x
long double get_distance_route_x(vector < vector < arco > > &rutas, vector < vector < long double > > &distancias, int &x){
    long double total_distancia = 0;
    if(rutas[x].size() == 0) return 1000000000;
    for(int i = 0; i < rutas[x].size() - 1; i++){
        int id_nodo_actual = rutas[x][i].id_nodo_actual;
        int id_siguiente_nodo = rutas[x][i + 1].id_nodo_actual;
        total_distancia += distancias[id_nodo_actual][id_siguiente_nodo];
    }
    return total_distancia;
}

//funcion de evaluacion de la distancia total
long double evaluation_function(vector < vector < arco > > &rutas, vector < vector < long double > > &distancias){
    long double total = 0;
    for(int i = 1; i < rutas.size(); i++){
        long double distancia = get_distance_route_x(rutas, distancias, i);
        if (distancia != 1000000000){
            total += distancia;
        }
    }
    return total;
}

//calcula la distancia entre dos nodos
void distance_calculator(vector < nodo > &nodos, vector < vector < long double > > &distancias){
    for(int i = 0; i < nodos.size(); i++){
        for(int j = 0; j < nodos.size(); j++){
            long double distancia_x = nodos[i].coordenada_x - nodos[j].coordenada_x;
            long double distancia_y = nodos[i].coordenada_y - nodos[j].coordenada_y;
            distancias[i][j] = 
                sqrt(distancia_x*distancia_x + distancia_y*distancia_y);
        }
    }
}

//escribe la solución 
void write_solution(vector < vector < arco > > &rutas, vector < vector < long double > > &distancias, string &nombre_instancia){
    int amount = 0;
    long double total_distancia = 0;
    set< int > nodos_atentidos;
    for(int i = 1; i < rutas.size(); i++) {
      if(rutas[i].size() != 0) {
        amount++;
        total_distancia += get_distance_route_x(rutas, distancias, i);
        for(int j = 0; j < rutas[i].size(); j++) {
          if(rutas[i][j].id_nodo_actual != 0)
            nodos_atentidos.insert(rutas[i][j].id_nodo_actual);
        }
      }
    }

    ofstream archivo_salida(nombre_instancia, ios::out | ios::app);
    archivo_salida << total_distancia << "\n";
    archivo_salida << amount << "\n";
    for(set< int >::iterator itr = nodos_atentidos.begin(); itr != nodos_atentidos.end(); itr++) {
      archivo_salida << *itr << " ";
    }
    archivo_salida << "\n";
    for(int i = 1; i < rutas.size(); i++){
        if(rutas[i].size() == 0) continue;
        archivo_salida << get_distance_route_x(rutas, distancias, i) << " " << rutas[i][rutas[i].size() - 1].riesgo << " ";
        for(int j = 0; j < rutas[i].size() - 1; j++){
            archivo_salida << rutas[i][j].id_nodo_actual << "->";
        }
        archivo_salida << rutas[i][rutas[i].size() - 1].id_nodo_actual << "\n";
    }
}

 /* Busca una solución de manera greedy donde la funcion miope es combinar las dos
rutas que generen la menor calidad parcial*/
void initial_solution(vector < nodo > &nodos, vector < vector < long double > > &distancias, 
                      long double &riesgo, int &n, vector < vector< int > > &existe_ruta, 
                      vector < vector < arco > > &rutas){
    for(int i = 1; i < n; i++){
        existe_ruta[0][i] = i;
        existe_ruta[i][0] = i;
        rutas[i].push_back(arco(0, 0, 0));
        rutas[i].push_back(arco(nodos[i].demanda, 0, i));
        rutas[i].push_back(arco(rutas[i][1].dinero, rutas[i][1].riesgo + distancias[i][0] * rutas[i][1].dinero, 0));
    }

    long double calidad_actual = evaluation_function(rutas, distancias);
    for(int l = 0; l < n; l++){
        for(int i = 1; i < n; i++){
            for(int j = 1; j < n; j++){
                //compara dos rutas distintas
                if(i != j && rutas[i].size() > 0 && rutas[j].size() > 0){
                    int nodo_ruta_i = rutas[i][rutas[i].size() - 2].id_nodo_actual; //nodo justo antes del deposito
                    int nodo_ruta_j = rutas[j][1].id_nodo_actual; //nodo justo despues del deposito
                    long double distancia_i = get_distance_route_x(rutas, distancias, i); //calcula la distancia total de las rutas hasta el momento
                    long double distancia_j = get_distance_route_x(rutas, distancias, j);
                    long double nuevo_arco = distancias[nodo_ruta_i][nodo_ruta_j]; //calcula la distancia del nuevo arco que se formaría si se combinan las dos rutas
                    long double nueva_distancia = distancia_i + distancia_j - distancias[0][nodo_ruta_i] - distancias[nodo_ruta_j][0] + nuevo_arco; //calula da distancia nueva
                    if(nueva_distancia < (distancia_i + distancia_j)){
                        vector< vector < arco > > backup_rutas = rutas; // si no se pueden combinar, volvemos a las rutas anteriores
                        rutas[i].pop_back(); //sacar deposito de la ruta a la cual vamos a combinar otra
                        for(int k = 1; k < rutas[j].size(); k++){
                            int nodo_nuevo = rutas[j][k].id_nodo_actual;
                            int nodo_anterior = rutas[i][rutas[i].size() - 1].id_nodo_actual;
                            existe_ruta[nodo_nuevo][nodo_anterior] = i;
                            rutas[i].push_back(arco(rutas[i][rutas[i].size() - 1].dinero + nodos[nodo_nuevo].demanda, 
                                            rutas[i][rutas[i].size() - 1].dinero*distancias[nodo_anterior][nodo_nuevo] 
                                            + rutas[i][rutas[i].size() - 1].riesgo, nodo_nuevo)); //agrega nueva ruta con la info necesaria
                        }
                        rutas[j].clear(); //borra la ruta anterior
                        if(!restriction_checker(rutas, riesgo)){
                            rutas = backup_rutas; // si no pasa la restriccion, deshacemos el cambio
                        }
                        else{
                            long double calidad_nueva = evaluation_function(rutas, distancias);
                            if(calidad_nueva < calidad_actual){
                                calidad_actual = calidad_nueva;
                            }
                            else{
                                rutas = backup_rutas; // si no mejora la calidad, deshacemos el cambio
                            }
                        }
                    }
                }
            }
        }
    }
}

bool neighbor(vector < nodo > &nodos, vector < vector < long double > > &distancias,
              vector< vector< arco > > &rutas,
              int c1, int nodo_c1, int c2, vector< int > &nodos_c2, long double riesgo) {
  //caso para intercambiar clientes de un mismo camion
  if(c1 == c2) {
    //buscamos quien esta primero
    int position_c1 = 0; 
    for(int i = 0; i < rutas[c1].size() && rutas[c1][i].id_nodo_actual != nodo_c1; i++, position_c1 = i);

    int position_c2 = 0;
    for(int i = 0; i < rutas[c1].size() && rutas[c1][i].id_nodo_actual != nodos_c2[0]; i++, position_c2 = i);
   
    if(position_c1 < position_c2) { //caso de que el subconjunto está después del nodo a intercambiar
      vector< arco > new_c1;
      for(int i = 0; i < position_c1; i++) {//agregamos nodos hasta la posicion del nodo a intercambiar
        new_c1.push_back(rutas[c1][i]);
      }
      //agregamos los que si se van a intercambiar
      for(int i = 0; i < nodos_c2.size(); i++) {
        int nodo_nuevo = nodos_c2[i];
        int nodo_anterior = new_c1[new_c1.size() - 1].id_nodo_actual;
        new_c1.push_back(arco(new_c1[new_c1.size() - 1].dinero + nodos[nodo_nuevo].demanda,
                              new_c1[new_c1.size() - 1].dinero * distancias[nodo_anterior][nodo_nuevo] + new_c1[new_c1.size() - 1].riesgo,
                              nodo_nuevo));
      }   

      int pos_nod = 1;
      //agregar todos los que no se van a intercambiar
      for(int i = position_c1 + 1; i < rutas[c1].size(); i++) {
        if(i == position_c2) {
          int nodo_nuevo = nodo_c1;
          int nodo_anterior = new_c1[new_c1.size() - 1].id_nodo_actual;
          new_c1.push_back(arco(new_c1[new_c1.size() - 1].dinero + nodos[nodo_nuevo].demanda,
                        new_c1[new_c1.size() - 1].dinero * distancias[nodo_anterior][nodo_nuevo] + new_c1[new_c1.size() - 1].riesgo,
                        nodo_nuevo));
        } else {
          int nodo_nuevo = rutas[c2][i].id_nodo_actual;
          if(pos_nod < nodos_c2.size() && nodos_c2[pos_nod] == nodo_nuevo) {
            pos_nod++;
            continue;
          }
          int nodo_anterior = new_c1[new_c1.size() - 1].id_nodo_actual;
          new_c1.push_back(arco(new_c1[new_c1.size() - 1].dinero + nodos[nodo_nuevo].demanda,
                                new_c1[new_c1.size() - 1].dinero * distancias[nodo_anterior][nodo_nuevo] + new_c1[new_c1.size() - 1].riesgo,
                                nodo_nuevo));
        }
      }
      rutas[c1] = new_c1;//se guarda la nueva ruta
    } else { //caso de que el subconjunto está antes que el nodo a intercambiar
      vector< arco > new_c1;
      for(int i = 0; i < position_c2; i++) {
        new_c1.push_back(rutas[c1][i]);
      }
    //primero se añaden los nodos que están antes del primer elemento del subconjunto
      int nodo_nuevo = nodo_c1;
      int nodo_anterior = new_c1[new_c1.size() - 1].id_nodo_actual;
      new_c1.push_back(arco(new_c1[new_c1.size() - 1].dinero + nodos[nodo_nuevo].demanda,
                    new_c1[new_c1.size() - 1].dinero * distancias[nodo_anterior][nodo_nuevo] + new_c1[new_c1.size() - 1].riesgo,
                    nodo_nuevo));

      //agregar los nodos que están entre primer elemento del subconjunto y el nodo a intercambiar 
      int pos_nod = 1;
      for(int i = position_c2 + 1; i < position_c1; i++) {
        int nodo_nuevo = rutas[c2][i].id_nodo_actual;
        if(pos_nod < nodos_c2.size() && nodos_c2[pos_nod] == nodo_nuevo) {
          pos_nod++;
          continue;
        }
        //agregar nodo a intercambiar
        int nodo_anterior = new_c1[new_c1.size() - 1].id_nodo_actual;
        new_c1.push_back(arco(new_c1[new_c1.size() - 1].dinero + nodos[nodo_nuevo].demanda,
                              new_c1[new_c1.size() - 1].dinero * distancias[nodo_anterior][nodo_nuevo] + new_c1[new_c1.size() - 1].riesgo,
                              nodo_nuevo));
      }
      //agrega todos los nodos del subconjunto a intercambiar 
      for(int i = 0; i < nodos_c2.size(); i++) {
        int nodo_nuevo = nodos_c2[i];
        int nodo_anterior = new_c1[new_c1.size() - 1].id_nodo_actual;
        new_c1.push_back(arco(new_c1[new_c1.size() - 1].dinero + nodos[nodo_nuevo].demanda,
                              new_c1[new_c1.size() - 1].dinero * distancias[nodo_anterior][nodo_nuevo] + new_c1[new_c1.size() - 1].riesgo,
                              nodo_nuevo));
      }
      //agrega todos los qu faltan
      for(int i = position_c1 + 1; i < rutas[c2].size(); i++) {
        int nodo_nuevo = rutas[c2][i].id_nodo_actual;
        if(pos_nod < nodos_c2.size() && nodos_c2[pos_nod] == nodo_nuevo) {
          pos_nod++;
          continue;
        }
        int nodo_anterior = new_c1[new_c1.size() - 1].id_nodo_actual;
        new_c1.push_back(arco(new_c1[new_c1.size() - 1].dinero + nodos[nodo_nuevo].demanda,
                              new_c1[new_c1.size() - 1].dinero * distancias[nodo_anterior][nodo_nuevo] + new_c1[new_c1.size() - 1].riesgo,
                              nodo_nuevo));
      }
      rutas[c1] = new_c1;//actualizamos ruta
    }
    return restriction_checker(rutas, riesgo);
  }

  vector< arco > new_c1;
  int position = 1;
  for(int i = 0; rutas[c1][i].id_nodo_actual != nodo_c1; i++, position = i + 1) {
    new_c1.push_back(rutas[c1][i]);
  }

  for(int i = 0; i < nodos_c2.size(); i++) {
    int nodo_nuevo = nodos_c2[i];
    int nodo_anterior = new_c1[new_c1.size() - 1].id_nodo_actual;
    new_c1.push_back(arco(new_c1[new_c1.size() - 1].dinero + nodos[nodo_nuevo].demanda,
                          new_c1[new_c1.size() - 1].dinero * distancias[nodo_anterior][nodo_nuevo] + new_c1[new_c1.size() - 1].riesgo,
                          nodo_nuevo));
  }

  for(int i = position; i < rutas[c1].size(); i++) {
    int nodo_nuevo = rutas[c1][i].id_nodo_actual;
    int nodo_anterior = new_c1[new_c1.size() - 1].id_nodo_actual;
    new_c1.push_back(arco(new_c1[new_c1.size() - 1].dinero + nodos[nodo_nuevo].demanda,
                          new_c1[new_c1.size() - 1].dinero * distancias[nodo_anterior][nodo_nuevo] + new_c1[new_c1.size() - 1].riesgo,
                          nodo_nuevo));
  }

  vector< arco > new_c2;
  position = 1;
  for(int i = 0; rutas[c2][i].id_nodo_actual != nodos_c2[0]; i++, position = i + 1) {
    new_c2.push_back(rutas[c2][i]);
  }

  int nodo_nuevo = nodo_c1;
  int nodo_anterior = new_c2[new_c2.size() - 1].id_nodo_actual;
  new_c2.push_back(arco(new_c2[new_c2.size() - 1].dinero + nodos[nodo_nuevo].demanda,
                        new_c2[new_c2.size() - 1].dinero * distancias[nodo_anterior][nodo_nuevo] + new_c2[new_c2.size() - 1].riesgo,
                        nodo_nuevo));

  int pos_nod = 1;
  for(int i = position; i < rutas[c2].size(); i++) {
    int nodo_nuevo = rutas[c2][i].id_nodo_actual;
    if(pos_nod < nodos_c2.size() && nodos_c2[pos_nod] == nodo_nuevo) {
      pos_nod++;
      continue;
    }
    int nodo_anterior = new_c2[new_c2.size() - 1].id_nodo_actual;
    new_c2.push_back(arco(new_c2[new_c2.size() - 1].dinero + nodos[nodo_nuevo].demanda,
                          new_c2[new_c2.size() - 1].dinero * distancias[nodo_anterior][nodo_nuevo] + new_c2[new_c2.size() - 1].riesgo,
                          nodo_nuevo));
  }

  rutas[c1] = new_c1;
  rutas[c2] = new_c2;
  return restriction_checker(rutas, riesgo);
}

void hill_climbing(vector < nodo > &nodos, vector < vector < long double > > &distancias,
                   vector< vector< arco > > &rutas, long double riesgo) {
  bool local = true;
  vector< int > nodos_c2;
  while(local) {
    vector< vector< arco > > backup = rutas;
    long double calidad_actual = evaluation_function(rutas, distancias);
    bool update = false; //variable que determina estado de local
    for(int camion = 1; camion < rutas.size(); camion++) { //elegimos un camion
      if(rutas[camion].size() > 0) {
        for(int k = 1; k < rutas[camion].size() - 1; k++) {//pasamos por todos los nodos del camion
          int cliente = rutas[camion][k].id_nodo_actual;
          for(int camion_2 = 1; camion_2 < rutas.size(); camion_2++) {//pasa por todos los otros camiones
            if(rutas[camion_2].size() > 0) {
              for(int S = 0; S < (1 << (rutas[camion_2].size() - 2)); S++) {//obtenemos todos los subconjuntos con un shift a la izquierda en binario
                vector< int > subset;
                for(int j = 0; j < rutas[camion_2].size() - 2; j++) {//pasamos por las posiciones del binario
                  if((S & (1 << j)) && rutas[camion_2][j + 1].id_nodo_actual != cliente) {//si el bit es uno, el cliente es parte de la ruta, con un and en binario
                    subset.push_back(rutas[camion_2][j + 1].id_nodo_actual);//agrega los que son parte del subconjunto
                  }
                }
                if(subset.size() >= 1 && neighbor(nodos, distancias, rutas, camion, cliente, camion_2, subset, riesgo)) {
                  long double calidad_nueva = evaluation_function(rutas, distancias);
                  if(calidad_nueva < calidad_actual) {
                    update = true;
                    break; // si mejora hacemos break
                  } else {
                    rutas = backup;
                  }
                } else {
                  rutas = backup;
                }
              }
            }
            if(update) break;
          }
          if(update) break;
        }
        if(update) break;
      }
    }
    local = update;
  }
}

//parametros del main: argumentos para pasar en la ejecución para cambiar la instancia
//argsc: cantidad de argumentos en la línea de comando
//argsv: los argumentos a escribir en la línea de comando 
int main(int argsc, char* argv[]) {
    auto start = high_resolution_clock::now();

    // concatene la carpeta con el archivo
    string file = argv[1];
    file += "/";
    file += argv[2];
    file += "/";
    file += argv[3];

    int n;
    long double riesgo;
    vector<nodo> nodos;

    read_instance(file, n, riesgo, nodos);

    vector< vector < long double > > distancias(n, vector < long double > (n,0));//vector de vectores de tamaño n, donde en cada posicion hay un vector de long doubles de tamaño n que parten en 0
    distance_calculator(nodos, distancias);

    vector< vector < int > > existe_ruta (n, vector < int > (n,0));
    vector< vector < arco > > rutas (n);
    initial_solution(nodos, distancias, riesgo, n, existe_ruta, rutas);
    hill_climbing(nodos, distancias, rutas, riesgo);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    string set_instancia = argv[2];

    string instancia = argv[3];
    instancia.pop_back();
    instancia.pop_back();
    instancia.pop_back();
    instancia.pop_back();
    instancia += ".out";
    if (set_instancia[4] == 'V'){
        set_instancia[5] = '_';
    }
    instancia = set_instancia + "_" + instancia;

    ofstream archivo_salida(instancia, ios::out );
    archivo_salida << duration.count() / 1000000.0 << endl;
    write_solution(rutas, distancias, instancia);
    archivo_salida.close();
    return 0;
}