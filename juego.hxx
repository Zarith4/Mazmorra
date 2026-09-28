#ifndef JUEGO_HXX
#define JUEGO_HXX
#include "juego.h"
Partida partida;

void Tablero(char **&tablero,int N)
{
    tablero=new char*[N];
    for(int i=0;i<N;i++)
    {
        tablero[i]=new char[N];
        for(int j=0;j<N;j++)
        {
            tablero[i][j]='-';
        }
    }
}

void liberarTablero(char **tablero,int N)
{
    if(tablero==NULL)
    {
        return;
    }
    for(int i=0;i<N;i++)
    {
        delete[] tablero[i];
    }
    delete[] tablero;
}

void iniciarPartida(Partida &partida)
{
    partida.N=0;
    partida.tablero=NULL;
    partida.enemigos=NULL;
    partida.cofres=NULL;
    partida.cantidadEnemigos=0;
    partida.cantidadCofres=0;
    partida.turno=0;
    partida.terminado=0;
    partida.gano=0;
    partida.salidaFila=0;
    partida.salidaColumna=0;
}

void liberarPartida(Partida &partida)
{
    if(partida.tablero!=NULL)
    {
        liberarTablero(partida.tablero, partida.N);
        partida.tablero=NULL;
    }
    if(partida.enemigos!=NULL)
    {
        delete[] partida.enemigos;
        partida.enemigos=NULL;
    }
    if(partida.cofres!=NULL)
    {
        delete[] partida.cofres;
        partida.cofres=NULL;
    }
}

void iniciarJugador(Partida &partida)
{
    cout<<"Ingrese el nombre del jugador: ";
    cin>>partida.jugador.nombre;
    partida.jugador.PV=PV_INICIAL;
    partida.jugador.PH=PH_INICIAL;
    partida.jugador.oro=0;
    partida.jugador.fila=partida.N / 2;
    partida.jugador.columna=partida.N / 2;
}

void generarSalida(Partida &partida)
{
    int lado=rand() % 4;
    if(lado==0)
    {
        partida.salidaFila=0;
        partida.salidaColumna=rand() % partida.N;
    }
    else if(lado==1)
    {
        partida.salidaFila=partida.N - 1;
        partida.salidaColumna=rand() % partida.N;
    }
    else if(lado==2)
    {
        partida.salidaFila=rand() % partida.N;
        partida.salidaColumna=0;
    }
    else
    {
        partida.salidaFila=rand() % partida.N;
        partida.salidaColumna=partida.N - 1;
    }
}

void reiniciarTablero(Partida &partida)
{
    for(int i=0;i<partida.N;i++)
    {
        for(int j=0;j<partida.N;j++)
        {
            partida.tablero[i][j]='-';
        }
    }
    partida.tablero
        [partida.salidaFila]
        [partida.salidaColumna]='#';
    for(int i=0;i<partida.cantidadCofres;i++)
    {
        if(partida.cofres[i].abierto==0)
        {
            partida.tablero[partida.cofres[i].fila][partida.cofres[i].columna]='?';
        }
        else
        {
            partida.tablero[partida.cofres[i].fila][partida.cofres[i].columna]='O';
        }
    }
    for(int i=0; i<partida.cantidadEnemigos;i++)
    {
        if(partida.enemigos[i].vivo==1)
        {
            if(partida.enemigos[i].descubierto==1)
            {
                partida.tablero[partida.enemigos[i].fila][partida.enemigos[i].columna]=partida.enemigos[i].tipo;
            }
            else
            {
                partida.tablero[partida.enemigos[i].fila][partida.enemigos[i].columna]='E';
            }
        }
        else
        {
            partida.tablero[partida.enemigos[i].fila][partida.enemigos[i].columna]='X';
        }
    }
    if(partida.terminado==0)
    {
        partida.tablero[partida.jugador.fila][partida.jugador.columna]='@';
    }
}

void mostrarTablero()
{
    if(!cargarPartida(partida))
    {
        cout<<"No hay ninguna partida guardada"<<endl;
        return;
    }
    cout<<endl;
    cout<<"TABLERO"<<endl;
    cout<<endl;
    for(int i=0;i<partida.N;i++)
    {
        for(int j=0;j<partida.N;j++)
        {
            cout<<partida.tablero[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
    liberarPartida(partida);
}

void mostrarEstadisticas()
{
    if(!cargarPartida(partida))
    {
        cout<<"No hay ninguna partida guardada"<<endl;
        return;
    }
    cout<<endl;
    cout<<"ESTADISTICAS"<<endl;
    cout<<"Jugador: "<<partida.jugador.nombre<<endl;
    cout<<"PV: "<<partida.jugador.PV<<endl;
    cout<<"PH: "<<partida.jugador.PH<<endl;
    cout<<"Oro: "<<partida.jugador.oro<<endl;
    cout<<"Turno: "<<partida.turno<<endl;
    liberarPartida(partida);
}

void moverJugador(const char *direccion)
{
    if(!cargarPartida(partida))
    {
        cout<<"No hay ninguna partida guardada"<<endl;
        return;
    }
    int nuevaFila=partida.jugador.fila;
    int nuevaColumna=partida.jugador.columna;
    if(strcmp(direccion,"up")==0)
    {
        nuevaFila--;
    }
    else if(strcmp(direccion,"down")==0)
    {
        nuevaFila++;
    }
    else if(strcmp(direccion, "left")==0)
    {
        nuevaColumna--;
    }
    else if(strcmp(direccion, "right")==0)
    {
        nuevaColumna++;
    }
    else
    {
        cout<<"Direccion invalida"<<endl;
        liberarPartida(partida);
        return;
    }

    if(!dentroTablero(nuevaFila, nuevaColumna, partida.N))
    {
        cout<<"No se puede salir del tablero"<<endl;
        liberarPartida(partida);
        return;
    }
    partida.jugador.fila=nuevaFila;
    partida.jugador.columna=nuevaColumna;
    partida.turno++;
    actualizarTurno(partida);
    comprobarFin(partida);
    if(partida.terminado==0)
    {
        reiniciarTablero(partida);
    }
    guardarPartida(partida);
    cout<<"Se movio correctamente"<<endl;
    liberarPartida(partida);
}
void explorarCofre()
{
    if(!cargarPartida(partida))
    {

        cout<<"No hay ninguna partida guardada"<<endl;
        return;
    }
    int posicion=buscarCofre(partida,partida.jugador.fila,partida.jugador.columna);
    if(posicion==-1)
    {
        cout<<"No hay ningun cofre aqui"<<endl;
        liberarPartida(partida);
        return;
    }
    if(partida.cofres[posicion].abierto==1)
    {
        cout<<"Este cofre ya esta abierto"<<endl;
        liberarPartida(partida);
        return;
    }
    int numero=rand() % 100;
    if(numero<50)
    {
        partida.cofres[posicion].tipo='O';
        partida.jugador.oro+=10;
        cout<<"Oro encontrado"<<endl;
        cout<<"+10 oro"<<endl;
    }
    else if(numero<80)
    {
        partida.cofres[posicion].tipo='P';
        partida.jugador.PV+=50;
        cout<<"Encontraste una pocion"<<endl;
        cout<<"+50 PV"<<endl;
    }
    else
    {
        partida.cofres[posicion].tipo='T';
        partida.jugador.PV-=30;
        cout<<"TRAMPA!!"<<endl;
        cout<< "-30 PV"<<endl;
    }
    partida.cofres[posicion].abierto=1;
    partida.turno++;
    actualizarTurno(partida);
    comprobarFin(partida);
    if(partida.terminado==0)
    {
        reiniciarTablero(partida);
    }
    guardarPartida(partida);
    liberarPartida(partida);
}

void atacar()
{
    if(!cargarPartida(partida))
    {
        cout<<"No hay ninguna partida guardada"<<endl;
        return;
    }
    int posicion=buscarEnemigo(partida,partida.jugador.fila,partida.jugador.columna);
    if(posicion==-1)
    {
        cout<<"No hay enemigos en esta posicion"<<endl;
        liberarPartida(partida);
        return;
    }
    partida.enemigos[posicion].descubierto=1;
    partida.enemigos[posicion].PV-=partida.jugador.PH;
    cout<<"Ataque a enemigo exitoso"<<endl;
    if(partida.enemigos[posicion].PV<=0)
    {
        partida.enemigos[posicion].PV=0;
        partida.enemigos[posicion].vivo=0;
        if(partida.enemigos[posicion].tipo=='G')
        {
            partida.jugador.PH+=2;
            cout<<"Goblin derrotado obtienes +2 PH"<<endl;
        }
        else if(partida.enemigos[posicion].tipo=='A')
        {
            partida.jugador.PH+=3;
            cout<<"Arquero derrotado obtienes +3 PH"<<endl;
        }
        else if(partida.enemigos[posicion].tipo=='J')
        {
            partida.jugador.PH+=5;
            cout<<"Jefe derrotado obtienes +5 PH"<<endl;
        }
    }
    else
    {
        cout<<"PV del enemigo: "<< partida.enemigos[posicion].PV<<endl;
    }
    partida.turno++;
    actualizarTurno(partida);
    comprobarFin(partida);
    if(partida.terminado==0)
    {
        reiniciarTablero(partida);
    }
    guardarPartida(partida);
    liberarPartida(partida);
}

void actualizarTurno(Partida &partida)
{
    descubrirEnemigos(partida);
    atacArqueros(partida);
    atacEnemigosMismaCasilla(partida);
    if(partida.turno % 2==0)
    {
        mJefe(partida);
    }
}

void atacArqueros(Partida &partida)
{
    for(int i=0;i<partida.cantidadEnemigos;i++)
    {
        if(partida.enemigos[i].vivo==1&&partida.enemigos[i].tipo=='A')
        {
            int diferenciaFila=abs(partida.enemigos[i].fila-partida.jugador.fila);
            int diferenciaColumna=abs(partida.enemigos[i].columna-partida.jugador.columna);
            if(diferenciaFila + diferenciaColumna==1)
            {
                partida.jugador.PV-=partida.enemigos[i].PH;
                cout<<"CUIDADO! Un arquero te ataco"<<endl;
                cout<<"-"<<partida.enemigos[i].PH<<" PV"<<endl;
            }
        }
    }
}

void atacEnemigosMismaCasilla(Partida &partida)
{
    for(int i=0;i<partida.cantidadEnemigos;i++)
    {
        if(partida.enemigos[i].vivo==1&&partida.enemigos[i].tipo!='A')
        {
            if(partida.enemigos[i].fila==partida.jugador.fila&&partida.enemigos[i].columna==partida.jugador.columna)
            {
                partida.jugador.PV-=partida.enemigos[i].PH;
                cout<<"CUIDADO! Un enemigo te ataco"<<endl;
                cout<<"-" << partida.enemigos[i].PH<<"PV"<<endl;
            }
        }
    }
}

void mJefe(Partida &partida)
{
    int posicion=-1;
    for(int i=0;i<partida.cantidadEnemigos;i++)
    {
        if(partida.enemigos[i].tipo=='J'&&partida.enemigos[i].vivo==1)
        {
            posicion=i;
            break;
        }
    }
    if(posicion==-1)
    {
        return;
    }
    int fila=partida.enemigos[posicion].fila;
    int columna=partida.enemigos[posicion].columna;
    int nuevaFila=fila;
    int nuevaColumna=columna;
    if(fila<partida.salidaFila)
    {
        nuevaFila++;
    }
    else if(fila>partida.salidaFila)
    {
        nuevaFila--;
    }
    else if(columna<partida.salidaColumna)
    {
        nuevaColumna++;
    }
    else if(columna>partida.salidaColumna)
    {
        nuevaColumna--;
    }

    if(dentroTablero(nuevaFila, nuevaColumna, partida.N)&&!(nuevaFila==partida.salidaFila&&nuevaColumna==partida.salidaColumna)&&!posicionOcupada(partida,nuevaFila,nuevaColumna)&&!hayCofre(partida,nuevaFila,nuevaColumna))
    {
        partida.enemigos[posicion].fila=nuevaFila;
        partida.enemigos[posicion].columna=nuevaColumna;
    }
}

void descubrirEnemigos(Partida &partida)
{
    for(int i=0;i<partida.cantidadEnemigos;i++)
    {
        if(partida.enemigos[i].vivo==1)
        {
            int distanciaFila=abs(partida.enemigos[i].fila-partida.jugador.fila);
            int distanciaColumna=abs(partida.enemigos[i].columna-partida.jugador.columna);
            if(distanciaFila + distanciaColumna<=1)
            {
                partida.enemigos[i].descubierto=1;
            }
        }
    }
}

void comprobarFin(Partida &partida)
{
    if(partida.jugador.PV<=0)
    {
        partida.jugador.PV=0;
        partida.terminado=1;
        partida.gano=0;
        cout<<endl;
        cout<<"GAME OVER"<<endl;
        guardarReporte(partida.jugador,partida.gano);
        return;
    }
    if(partida.jugador.fila==partida.salidaFila&&partida.jugador.columna==partida.salidaColumna)
    {
        partida.terminado=1;
        partida.gano=1;
        cout<<endl;
        cout<<"LLEGASTE A LA SALIDA"<<endl;
        cout<<"GANASTE!!"<<endl;
        guardarReporte(partida.jugador, partida.gano);
    }
}
bool guardarPartida(Partida &partida)
{
    Estado estado;
    estado.N=partida.N;
    estado.cantidadE=partida.cantidadEnemigos;
    estado.cantidadC=partida.cantidadCofres;
    estado.turno=partida.turno;
    estado.terminado=partida.terminado;
    estado.gano=partida.gano;
    estado.salidaFila=partida.salidaFila;
    estado.salidaColumna=partida.salidaColumna;
    ofstream archivoEstado(ARCHIVO_ESTADO,ios::binary);
    ofstream archivoTablero(ARCHIVO_TABLERO,ios::binary);
    ofstream archivoJugador(ARCHIVO_JUGADOR,ios::binary);
    ofstream archivoEnemigos(ARCHIVO_ENEMIGOS,ios::binary);
    ofstream archivoCofres(ARCHIVO_COFRES,ios::binary);
    if(!archivoEstado||!archivoTablero||!archivoJugador||!archivoEnemigos||!archivoCofres)
    {
        return false;
    }
    archivoEstado.write((char*)&estado,sizeof(Estado));
    archivoJugador.write((char*)&partida.jugador,sizeof(Jugador));
    for(int i=0;i<partida.N;i++)
    {
        archivoTablero.write(partida.tablero[i],sizeof(char) * partida.N);
    }
    archivoEnemigos.write((char*)partida.enemigos,sizeof(Enemigo) * partida.cantidadEnemigos);
    archivoCofres.write((char*)partida.cofres,sizeof(Cofre) * partida.cantidadCofres);
    archivoEstado.close();
    archivoTablero.close();
    archivoJugador.close();
    archivoEnemigos.close();
    archivoCofres.close();
    return true;
}

bool cargarPartida(Partida &partida)
{
    ifstream archivoEstado(ARCHIVO_ESTADO,ios::binary);
    if(!archivoEstado)
    {
        return false;
    }
    Estado estado;
    archivoEstado.read((char*)&estado,sizeof(Estado));
    archivoEstado.close();
    iniciarPartida(partida);
    partida.N=estado.N;
    partida.cantidadEnemigos=estado.cantidadE;
    partida.cantidadCofres=estado.cantidadC;
    partida.turno=estado.turno;
    partida.terminado=estado.terminado;
    partida.gano=estado.gano;
    partida.salidaFila=estado.salidaFila;
    partida.salidaColumna=estado.salidaColumna;
    Tablero(partida.tablero, partida.N);
    partida.enemigos=new Enemigo[partida.cantidadEnemigos];
    partida.cofres=new Cofre[partida.cantidadCofres];
    ifstream archivoTablero(ARCHIVO_TABLERO,ios::binary);
    ifstream archivoJugador(ARCHIVO_JUGADOR,ios::binary);
    ifstream archivoEnemigos(ARCHIVO_ENEMIGOS,ios::binary);
    ifstream archivoCofres(ARCHIVO_COFRES,ios::binary);

    if(!archivoTablero||!archivoJugador||!archivoEnemigos||!archivoCofres)
    {
        liberarPartida(partida);
        return false;
    }
    for(int i=0;i<partida.N;i++)
    {
        archivoTablero.read(partida.tablero[i],sizeof(char) * partida.N);
    }
    archivoJugador.read((char*)&partida.jugador,sizeof(Jugador));
    archivoEnemigos.read((char*)partida.enemigos,sizeof(Enemigo) * partida.cantidadEnemigos);
    archivoCofres.read((char*)partida.cofres,sizeof(Cofre) * partida.cantidadCofres);
    archivoTablero.close();
    archivoJugador.close();
    archivoEnemigos.close();
    archivoCofres.close();
    return true;
}


void generarEnemigos(Partida &partida,int cantidadGoblins,int cantidadArqueros)
{
    partida.cantidadEnemigos=cantidadGoblins+cantidadArqueros+1;
    partida.enemigos=new Enemigo[partida.cantidadEnemigos];
     for(int i=0;i<partida.cantidadEnemigos;i++)
     {
        partida.enemigos[i].vivo=0;
     }

int posicion=0;
    for(int i=0;i<cantidadGoblins;i++)
    {
        int fila;
        int columna;
        do
        {
            fila=rand() % partida.N;
            columna=rand() % partida.N;
        }
        while
        (
        posicionOcupada(partida, fila, columna)||(fila==partida.jugador.fila&&columna==partida.jugador.columna)||(fila==partida.salidaFila&&columna==partida.salidaColumna));
        partida.enemigos[posicion].tipo='G';
        partida.enemigos[posicion].fila=fila;
        partida.enemigos[posicion].columna=columna;
        partida.enemigos[posicion].PV=PV_GOBLIN;
        partida.enemigos[posicion].PH=PH_GOBLIN;
        partida.enemigos[posicion].vivo=1;
        partida.enemigos[posicion].descubierto=0;
        posicion++;
    }
    for(int i=0;i<cantidadArqueros;i++)
    {
        int fila;
        int columna;
        do
        {
            fila=rand() % partida.N;
            columna=rand() % partida.N;
        }
        while
    (
        posicionOcupada(partida, fila, columna)||(fila==partida.jugador.fila&&columna==partida.jugador.columna)||(fila==partida.salidaFila&&columna==partida.salidaColumna));
        partida.enemigos[posicion].tipo='A';
        partida.enemigos[posicion].fila=fila;
        partida.enemigos[posicion].columna=columna;
        partida.enemigos[posicion].PV=PV_ARQUERO;
        partida.enemigos[posicion].PH=PH_ARQUERO;
        partida.enemigos[posicion].vivo=1;
        partida.enemigos[posicion].descubierto=0;
        posicion++;
    }
    int fila;
    int columna;
    do
    {
        fila=rand() % partida.N;
        columna=rand() % partida.N;
    }
    while
    (
    posicionOcupada(partida, fila, columna)||(fila==partida.jugador.fila&&columna==partida.jugador.columna)||(fila==partida.salidaFila&&columna==partida.salidaColumna));
    partida.enemigos[posicion].tipo='J';
    partida.enemigos[posicion].fila=fila;
    partida.enemigos[posicion].columna=columna;
    partida.enemigos[posicion].PV=PV_JEFE;
    partida.enemigos[posicion].PH=PH_JEFE;
    partida.enemigos[posicion].vivo=1;
    partida.enemigos[posicion].descubierto=0;
}

void generarCofres(Partida &partida,int cantidad)
{
    partida.cantidadCofres=cantidad;
    partida.cofres=new Cofre[cantidad];
    for(int i=0;i<cantidad;i++)
    {
        int fila;
        int columna;
        do
        {
            fila=rand() % partida.N;
            columna=rand() % partida.N;
        }
        while
        (
        posicionOcupada(partida, fila, columna)||hayCofre(partida, fila, columna)||(fila==partida.jugador.fila&&columna==partida.jugador.columna)||(fila==partida.salidaFila&&columna==partida.salidaColumna));
        partida.cofres[i].fila=fila;
        partida.cofres[i].columna=columna;
        partida.cofres[i].tipo='?';
        partida.cofres[i].abierto=0;
    }
}

int Puntaje(Jugador &jugador,int gano)
{
    int B=0;
    if(gano==1)
    {
        B=100;
    }
    int P=B+(jugador.oro *2)+(jugador.PV*2)+(jugador.PH*5);
    return P;
}

void guardarReporte(Jugador &jugador,int gano)
{
    ofstream archivo(ARCHIVO_REPORTE,ios::app);
    if(!archivo)
    {
        return;
    }
    archivo<<"REPORTE DE PARTIDA"<<endl;
    archivo<<"Jugador: "<< jugador.nombre<<endl;
    if(gano==1)
    {
        archivo<<"Resultado: GANO!!"<<endl;
    }
    else
    {
        archivo<<"Resultado: PERDIO :("<<endl;
    }
    archivo<<"Oro: "<<jugador.oro<<endl;
    archivo<<"PV: "<<jugador.PV<<endl;
    archivo<<"PH: "<<jugador.PH<<endl;
    archivo<<"Puntaje: "<<Puntaje(jugador, gano)<<endl;
    archivo<<endl;
    archivo.close();
}

bool dentroTablero(int fila,int columna,int N)
{
    if(fila>=0&&fila<N&&columna>=0&&columna<N)
    {
        return true;
    }
    return false;
}

bool posicionOcupada(Partida &partida,int fila,int columna)
{
    for(int i=0;i<partida.cantidadEnemigos;i++)
    {
        if(partida.enemigos[i].vivo==1&&partida.enemigos[i].fila==fila&&partida.enemigos[i].columna==columna)
        {
            return true;
        }
    }
    return false;
}

bool hayEnemigoVivo(Partida &partida,int fila,int columna)
{
    for(int i=0;i<partida.cantidadEnemigos;i++)
    {
        if(partida.enemigos[i].vivo==1&&partida.enemigos[i].fila==fila&&partida.enemigos[i].columna==columna)
        {
            return true;
        }
    }
    return false;
}

bool hayCofre(Partida &partida,int fila,int columna)
{
    for(int i=0;i<partida.cantidadCofres;i++)
    {
        if(partida.cofres[i].fila==fila&&partida.cofres[i].columna==columna)
        {
            return true;
        }
    }
    return false;
}


int buscarEnemigo(Partida &partida,int fila,int columna)
{
    for(int i=0;i<partida.cantidadEnemigos;i++)
    {
        if(partida.enemigos[i].fila==fila&&partida.enemigos[i].columna==columna &&partida.enemigos[i].vivo==1)
        {
            return i;
        }
    }
    return -1;
}

int buscarCofre(Partida &partida,int fila,int columna)
{
    for(int i=0;i<partida.cantidadCofres;i++)
    {
        if(partida.cofres[i].fila==fila&&partida.cofres[i].columna==columna)
        {
            return i;
        }
    }
    return -1;
}

void iniciarJuego()
{
    srand(time(NULL));
    iniciarPartida(partida);
    cout<<"     JUEGO DE LA MAZMORRA     "<<endl;
    cout<<"Ingrese el tamano del tablero: ";
    cin>>partida.N;
    if(partida.N < 5)
    {
        cout<<"El tablero debe ser de minimo 5x5"<<endl;
        return;
    }
    Tablero(partida.tablero, partida.N);
    iniciarJugador(partida);
    generarSalida(partida);
    int cantidadGoblins;
    int cantidadArqueros;
    int cantidadCofres;
    cout<<"Cantidad de goblins: "<<endl;
    cin>>cantidadGoblins;
    cout<<"Cantidad de arqueros: "<<endl;
    cin>>cantidadArqueros;
    cout<<"Cantidad de cofres: "<<endl;
    cin>>cantidadCofres;
    generarEnemigos(partida,cantidadGoblins,cantidadArqueros);
    generarCofres(partida,cantidadCofres);
    partida.turno=0;
    partida.terminado=0;
    partida.gano=0;
    reiniciarTablero(partida);
    if(guardarPartida(partida))
    {
        cout<<endl;
        cout<<"Partida creada correctamente"<<endl;
    }
    else
    {
        cout<<"Error al guardar la partida"<<endl;
    }
    liberarPartida(partida);
}
#endif
