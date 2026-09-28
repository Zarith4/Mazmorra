#include "juego.h"
#include "juego.hxx"

int main()
{
    string comando;
    string direccion;
    cout<<endl;
    iniciarJuego();
    if(!cargarPartida(partida))
    {
        cout<<"No se pudo iniciar la partida"<<endl;
        return 0;
    }
    cout<<endl;
    cout<<"Partida iniciada"<<endl;
    cout<<"Escriba un comando para jugar"<<endl;
    cout<<"Comandos: board, stats, move, seek, attack, salir"<<endl;
    while(partida.terminado==0)
    {
        cout<<endl;
        cout<<"Comando: ";
        cin>>comando;
        if(comando=="board")
        {
            liberarPartida(partida);
            mostrarTablero();
            if(!cargarPartida(partida))
            {
                break;
            }
        }
        else if(comando=="stats")
        {
            liberarPartida(partida);
            mostrarEstadisticas();
            if(!cargarPartida(partida))
            {
                break;
            }
        }
        else if(comando=="move")
        {
            cin>>direccion;
            liberarPartida(partida);
            moverJugador(direccion.c_str());
            if(!cargarPartida(partida))
            {
                break;
            }
        }
        else if(comando=="seek")
        {
            liberarPartida(partida);
            explorarCofre();
            if(!cargarPartida(partida))
            {
                break;
            }
        }
        else if(comando=="attack")
        {
            liberarPartida(partida);
            atacar();
            if(!cargarPartida(partida))
            {
                break;
            }
        }
        else if(comando=="salir")
        {
            cout<<"Juego terminado"<<endl;
            liberarPartida(partida);
            break;
        }
        else
        {
            cout<<"Comando invalido"<<endl;
            cout<<"Use: board, stats, move, seek, attack o salir"<<endl;
        }
    }
    if(partida.terminado==1)
    {
        liberarPartida(partida);
    }
}
