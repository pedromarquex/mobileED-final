#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <math.h>
#include <conio.h>

#define N 50



//estrutura para os apps.
typedef struct{
    int id;
    char name[50];
    int tam;
}app;


//estrutura da LLV.
typedef struct{
    int IA, FA, IL, FL;
    app v[N];
}LLV;

//estrutura do nó para a LLSE
typedef struct{
    app info;
    int prox;
}no;

//estrutura do nó para a LLDE
typedef struct{
    int ant;
    app info;
    int prox;
}noh;

//estrutura para a LLSE
typedef struct{
    int IL;
    no v[N];
    int disp;
    int count;		//para guardar a quantidade de elementos
}LLSE;

typedef struct{
    app info;
	struct celula *prox;		
}celula;

typedef struct {
	int count;			//Para guardar a quantidade de celulas empilhadas
	celula *topo;		//Para definir a celular que indicará o topo da pilha
}PILHA;

//estrutura para a LLDE
typedef struct{
    int IL;
    noh v[N];
    int disp;
    int FL;
}LLDE;

//estrutura para a fila, derivada da LLDE
typedef struct{
    int IF;
    noh v[4];
    int disp;
    int FF;
    int count;
}FILA;

//protótipo das funções:
void imprime_loja(LLV *_stru);
void inicializa_llv(LLV *_stru);
void cabecalho();
void carrega_loja(LLV *_stru);
int aloca_no_llse(LLSE *_stru);
void inicializa_llse(LLSE *_stru);
void instalar(LLSE *_destino, LLV *_origem, int id);
void store(LLV *_loja, LLSE *_instalados, FILA *_fila, app vetor[9]);
void myapps(LLSE *_instalados, LLDE *_rodando, FILA *_fila, LLV *_loja, PILHA *_pilha, app vetor[9]);
void imprime_instalados(LLSE *_instalados);
void desinstala(LLSE *_instalados, int id);
void libera_no(LLSE *_stru, int ind);
void inicializa_llde(LLDE *_stru);
int aloca_noh_llde(LLDE *_stru);
void executa(LLSE *_instalados, LLDE *_rodando, int id);
void apprum(LLDE *_rodando);
void killapp(LLDE *_rodando,int id);
void inicializa_fila(FILA *_stru);
int fila_vazia(FILA *_fila);
int fila_cheia(FILA *_fila);
int aloca_noh_fila(FILA *_stru);
void insere_fila(LLV *_loja, FILA *_fila, int id, LLSE *_instalados, app vetor[9]);
void fila_remover(FILA *_fila);
void imprime_fila(FILA *_fila);
void AppPilha(PILHA *_pilha);
void init_pilha(PILHA *_pilha );
int pilha_vazia(PILHA *_pilha);
int pilha_destrutor(PILHA *_pilha);
int len_pilha(PILHA *_pilha);
int pilha_push(PILHA *_pilha, LLDE *_rodando, int id);
int pilha_pop(PILHA *_pilha);
void mostra_pilha(PILHA *_pilha);
void ini_worka(app vetor[9]);
void insere_worka(app vetor[9], LLSE *_instalados, int id);
void mostra_work(app vetor[9]);



int main(){
    int opcao;
    system("color A");
setlocale(LC_ALL, "");

//estruturas que guardarão as informações dos apps
PILHA pilha;
FILA fila_inst;
LLV loja;	//guardará os apps disponiveis na loja
LLSE instalados; //guardará os apps que foram "instalados" a partir da loja
LLDE rodando;	//guardará os apps que estão em "execução"
app worka[9];

//ponteiros para cada estrutura de apps
PILHA *_pilha;
FILA *_fila_inst;
LLV *_loja;
LLSE *_instalados;
LLDE *_rodando;

//atribuição dos endereços para os ponteiros
_pilha = &pilha;
_fila_inst = &fila_inst;
_loja = &loja;
_instalados = &instalados;
_rodando = &rodando;


//inicializando cada estrutura como uma estrutura vazia
init_pilha(_pilha);
inicializa_fila(_fila_inst);
inicializa_llv(_loja);
inicializa_llse(_instalados);
inicializa_llde(_rodando);
ini_worka(worka);


//carregando os apps do arquivo para a loja
carrega_loja(_loja);

do{
    cabecalho();
    //mostrarWork(worka);
    mostra_work(worka);
    printf("\n");
    printf("|| 1 - StoreED |");
    printf("| 2 - MeusAppsED |");
    printf("| 3 - AppRumED |");
    printf("| 4 - AppPilha ||");
    printf("0 - Sair\n");
    printf("\n\n");
    imprime_fila(_fila_inst);
    printf("\n\n");

    printf("Digite uma opção: ");
    scanf("%d", &opcao);

    switch(opcao){
        case 1:
        //StoreED(aplic, MyApps, worka)
        store(_loja, _instalados, _fila_inst, worka);
        break;


        case 2:
        //MyAppsED(MyApps, AppRum, worka);
        myapps(_instalados, _rodando, _fila_inst, _loja, _pilha, worka);

        break;


        case 3:
        //AppRumED(AppRum);
		apprum(_rodando);
        break;
    
    	case 4:
    	AppPilha(_pilha);
    	break;

        case 0:
        	pilha_destrutor(_pilha);
            printf("\n\nObrigado por utilizar os nossos serviços!\n\n");
            getch();
            exit(0);
        break;
    }


}while(opcao != 0);







return 0;
}

//função que inicializa uma LLV indicando que está vazia.
void inicializa_llv(LLV *_stru){

    _stru->IL = -1;   //utilizando -1 para mostrar que a lista está vazia
    _stru->FL = -1;   //utilizando -1 para mostrar que a lista está vazia

    //como o IL == FL == -1, sabemos que a lista está vazia
    _stru->IA = 0;    //o IA é constante
    _stru->FA = N-1;  //O FA é constante

return;
}

//função que carrega a os apps disponiveis no arquivo para a estrutura loja do main()
void carrega_loja(LLV *_stru){
    int i, pos, k=0;
    FILE *lista;
    app aux;

    lista = fopen("lista.txt", "r");

        if(lista == NULL){
            printf("Não foi possível ler o arquivo.");
            getch();
            exit(EXIT_FAILURE);
        }
        else{
            //enquanto o que for lido do arquivo for diferente de EOF (final de arquivo)
            //cada linha do arquivo vai ser lido para uma posição da LLV loja do main(). -- FAZER
            while((fscanf(lista, "%d %s %d", &aux.id, aux.name, &aux.tam)) != EOF){
            printf("\tLoading ");
            for(i=0; i<=k; i++){
                printf("##");
            }
            k++;
            printf(" %d%%\n\n\n", k*5);
            if(k*5 >= 100){
                system("pause");
            }
            cabecalho();

            if((_stru->IA == _stru->IL) && (_stru->FA == _stru->FL)){
            printf("LOCAL DE DESTINO CHEIO! NO CARREGA LOJA \n\n\n");
            system("pause");
            return;
            }

            if((_stru->IL == -1) && (_stru->FL == -1)){
                _stru->v[N/2].id = aux.id;
                _stru->v[N/2].tam = aux.tam;
                strcpy(_stru->v[N/2].name, aux.name);
                _stru->IL = N/2;
                _stru->FL = N/2;
                continue;
            }

            else{


        //NO COMEÇO
        if(_stru->v[_stru->IL].tam <= aux.tam){
            //SE HOUVER ESPAÇO NO COMEÇO
            if(_stru->IL > _stru->IA){
                _stru->IL--;
                _stru->v[_stru->IL].id = aux.id;
                _stru->v[_stru->IL].tam = aux.tam;
                strcpy(_stru->v[_stru->IL].name, aux.name);
                continue;
                    }
            if(_stru->IL == _stru->IA){
                //MOVIMENTAÇÃO
                for(i=_stru->FL; i<=_stru->IL; i--){
                    _stru->v[i+1].id = _stru->v[i].id;
                    _stru->v[i+1].tam = _stru->v[i].tam;
                    strcpy(_stru->v[i+1].name, _stru->v[i].name);
                    }
                _stru->IL--;
                _stru->v[_stru->IL].id = aux.id;
                _stru->v[_stru->IL].tam = aux.tam;
                strcpy(_stru->v[_stru->IL].name, aux.name);

                continue;
                    }
                }
        //NO FINAL
        if(_stru->v[_stru->FL].tam >= aux.tam){
            if((_stru->FL < _stru->FA)){
                _stru->FL++;
                _stru->v[_stru->FL].id = aux.id;
                _stru->v[_stru->FL].tam = aux.tam;
                strcpy(_stru->v[_stru->FL].name, aux.name);

                continue;
                }
            if((_stru->FL == _stru->FA)){
                //MOVIMENTAÇÃO
                for(i=_stru->IL; i<=_stru->FL; i++){
                    _stru->v[i-1].id = _stru->v[i].id;
                    _stru->v[i-1].tam = _stru->v[i].tam;
                    strcpy(_stru->v[i-1].name, _stru->v[i].name);
                    }
                _stru->FL++;
                _stru->v[_stru->FL].id = aux.id;
                _stru->v[_stru->FL].tam = aux.tam;
                strcpy(_stru->v[_stru->FL].name, aux.name);

                continue;
                }
            }

        //NO MEIO
        if((_stru->v[_stru->IL].tam > aux.tam) && (_stru->v[_stru->FL].tam < aux.tam)){
                //DEFINIR A POSIÇÃO
                for(i=_stru->IL; i<=_stru->FL; i++){
                    if(_stru->v[i+1].tam < aux.tam){
                        pos = i;
                        break;
                        }
                    }
                if((pos - _stru->IL) < (_stru->FL - pos)){
                    i = _stru->IL;
                    while(_stru->v[i].tam > aux.tam){
                        _stru->v[i-1].id = _stru->v[i].id;
                        _stru->v[i-1].tam = _stru->v[i].tam;
                        strcpy(_stru->v[i-1].name, _stru->v[i].name);
                        i++;
                        }
                    _stru->v[pos].id = aux.id;
                    _stru->v[pos].tam = aux.tam;
                    strcpy(_stru->v[pos].name, aux.name);
                    _stru->IL--;

                    continue;
                }
                if((pos - _stru->IL) > (_stru->FL - pos)){
                    i = _stru->FL;
                    while(_stru->v[i].tam < aux.tam){
                        _stru->v[i+1].id = _stru->v[i].id;
                        _stru->v[i+1].tam = _stru->v[i].tam;
                        strcpy(_stru->v[i+1].name, _stru->v[i].name);
                        i--;
                        }
                    _stru->v[pos+1].id = aux.id;
                    _stru->v[pos+1].tam = aux.tam;
                    strcpy(_stru->v[pos+1].name, aux.name);
                    _stru->FL++;

                    continue;
                    }
                else{
                    i = _stru->IL;
                    while(_stru->v[i].tam > aux.tam){
                        _stru->v[i-1].id = _stru->v[i].id;
                        _stru->v[i-1].tam = _stru->v[i].tam;
                        strcpy(_stru->v[i-1].name, _stru->v[i].name);
                        i++;
                        }
                    _stru->v[pos].id = aux.id;
                    _stru->v[pos].tam = aux.tam;
                    strcpy(_stru->v[pos].name, aux.name);
                    _stru->IL--;

                    continue;
                    }
                }

        }//FINAL DO PRIMEIRO ELSE
    }//FINAL DO WHILE DA LEITURA
}//FINAL DO ELSE DO NULL



    fclose(lista);


}//FINAL DA FUNÇÃO



void cabecalho(){
    system("cls");
    printf("\t------------------------------------------------------------\n");
    printf("\t\t\t\tMobileED\n");
    printf("\t------------------------------------------------------------\n");
    printf("\n");
}

void imprime_loja(LLV *_stru){
    int i;

    for(i=_stru->IL; i<=_stru->FL; i++){
        printf("ID: %2d   \t||\t", _stru->v[i].id);
        printf("NAME: %15s \t||\t", _stru->v[i].name);
        printf("TAM: %5d\t", _stru->v[i].tam);
        printf("|| INDEX: %d", i);
        printf("\n--------------------------------------------------\n");
    }


}
void inicializa_llse(LLSE *_stru){
    int i;
    _stru->IL = -1; //para informar que a lista está vazia
    _stru->disp = 1;    //o primeiro disponivel
    _stru->count = 0;	//quantidade inicial de elementos

    for(i=0; i<N; i++){
        _stru->v[i].prox = i+1;
    }
    _stru->v[N-1].prox = -1; //sendo o último, não tem próximo
    return;
}

void inicializa_llde(LLDE *_stru){
	int i;
	_stru->IL = -1;	//para informar que a lista está vazia
	_stru->FL = -1;
	_stru->disp = 1;

	for(i=0; i<N; i++){
		_stru->v[i].ant = i-1;
		_stru->v[i].prox = i+1;
	}
	_stru->v[0].ant == -3;	//sendo o primeiro, nao tem anterior
	_stru->v[N-1].prox == -5;	//sendo o último, nao tem próximo
	return;
}
void inicializa_fila(FILA *_stru){
	int i;
	_stru->IF = -1;	//para informar que a lista está vazia
	_stru->FF = -1;
	_stru->disp = 0;

	for(i=0; i<4; i++){
		_stru->v[i].ant = i-1;
		_stru->v[i].prox = i+1;
	}
	_stru->v[0].ant == -3;	//sendo o primeiro, nao tem anterior
	_stru->v[3].prox == -5;	//sendo o último, nao tem próximo
	_stru->count = 0;
	return;
}

void libera_no(LLSE *_stru, int ind){
    _stru->v[ind].prox = _stru->disp;
    _stru->disp = ind;
}

int aloca_no_llse(LLSE *_stru){
    int d;
    if(_stru->disp == -5){
        printf("LISTA CHEIA NA LLSE\n\n ");
        system("pause");
        return 0;
    }
    d = _stru->disp;
    _stru->disp = _stru->v[_stru->disp].prox;
    return d;
}

int aloca_noh_llde(LLDE *_stru){
	int novo;
	if(_stru->disp == -5){
		printf("LISTA CHEIA NA LLDE\n\n");
		system("pause");
		return 0;
	}
	novo = _stru->disp;
	_stru->disp = _stru->v[_stru->disp].prox;
	_stru->v[_stru->disp].ant = -3;
	
	return novo;
}
int aloca_noh_fila(FILA *_stru){
	int novo;
	if((_stru->disp == -5) || (_stru->disp == -3)){
		printf("LISTA CHEIA NA FILA\n\n");
		system("pause");
		return 0;
	}
	novo = _stru->disp;
	_stru->disp = _stru->v[_stru->disp].prox;
	_stru->v[_stru->disp].ant = -3;
	
	return novo;
}

void instalar(LLSE *_destino, LLV *_origem, int id){
    int d, i=0;
    i = _destino->IL;
    if(_destino->IL != -1){
		while(1){
			if(_destino->v[i].info.id == id){
				return;
			}
			if(_destino->v[i].prox == -5){
				break;
			}
			else{
				i = _destino->v[i].prox;
			}
		}
	}
	
    

    d = aloca_no_llse(_destino);
    for(i=_origem->IL; i<=_origem->FL; i++){

        if(_origem->v[i].id == id){
            _destino->v[d].info.id = _origem->v[i].id;
            _destino->v[d].info.tam = _origem->v[i].tam;
            strcpy(_destino->v[d].info.name, _origem->v[i].name);
  	    }
	}
    if(d != 0){
        //SE LISTA ESTÁ VAZIA
        if(_destino->IL == -1){
			_destino->v[d].prox = -5; //ultimo nó
			_destino->IL = d;
			_destino->count++;
			return;
		}
		else{
			//NO INICIO
			if(_destino->v[d].info.tam >= _destino->v[_destino->IL].info.tam){

				_destino->v[d].prox = _destino->IL;
				_destino->IL = d;
				_destino->count++;
				return;
			}
			else{
                int ult;
                i = _destino->IL;
				//PARA ACHAR O INDICE DO ULTIMO ITEM DA LISTA
				while(1){
					if(_destino->v[i].prox == -5){
						break;
					}
					else{
						   i = _destino->v[i].prox;
					}

				}
				ult = i;
				//NO FINAL
				if(_destino->v[ult].info.tam > _destino->v[d].info.tam){
					_destino->v[d].prox = -5; //ULTIMO DA LISTA
					_destino->v[ult].prox = d;
					_destino->count++;
					return;
				}
				else{
					int pos;
					i = _destino->IL;
					while(1){
						if(_destino->v[_destino->v[i].prox].info.tam > _destino->v[d].info.tam){
							i = _destino->v[i].prox;
						}
						else{
							break;
						}

					}
					pos = i;
					_destino->v[d].prox = _destino->v[pos].prox;
					_destino->v[pos].prox = d;
					_destino->count++;
					return;

				}
			}
		}
    }

}//FIM DA FUNÇÃO

void desinstala(LLSE *_instalados, int id){
	int atual, i, ult;

	if(_instalados->count == 1){
		inicializa_llse(_instalados);
		return;
	}
	else{
		//COMEÇO
		if(_instalados->v[_instalados->IL].info.id == id){
			atual = _instalados->IL;
			_instalados->IL = _instalados->v[atual].prox;
			libera_no(_instalados, atual);
			_instalados->count--;
		}
		else{
			i = _instalados->IL;
			while(1){
				if(_instalados->v[_instalados->v[i].prox].prox == -5){
					break;
				}
				else{
					i = _instalados->v[i].prox;
				}
			}
			ult = _instalados->v[i].prox;
			//FINAL
			if(_instalados->v[ult].info.id == id){
				atual = _instalados->v[i].prox;
				_instalados->v[i].prox = _instalados->v[atual].prox;
				libera_no(_instalados, atual);
				_instalados->count--;
			}
			else{
				i = _instalados->IL;
				int p, pos;
				while(1){
					if(_instalados->v[_instalados->v[i].prox].info.id == id){
						break;
					}
					else{
						i = _instalados->v[i].prox;
					}
				}
				p = i;
				pos = _instalados->v[i].prox;
				_instalados->v[p].prox = _instalados->v[pos].prox;
				_instalados->v[pos].prox = _instalados->disp;
				_instalados->disp = pos;
				_instalados->count--;
			}
		}
	}

}

void store(LLV *_loja, LLSE *_instalados, FILA *_fila, app vetor[9]){
	int id, opc, i, loja=0, instalado=0;
	while(1){
		cabecalho();
		printf("\n------StoreED----------\n");
		imprime_loja(_loja);
		printf("|| 1 - Instalar |");
    	printf("0 - Sair\n");
    	printf("Digite uma opção: ");
    	scanf("%d", &opc);

		   	if(opc == 1){
			printf("Digite o índice do App: ");
			scanf("%d", &id);
			//VERIFICANDO SE O APP INDICADO ESTÁ NA LOJA
			for(i=_loja->IL; i<=_loja->FL; i++){
				if(_loja->v[i].id == id){
				//NESTE CASO O APP ESTÁ NA LOJA
					loja = 1;
			}
			}
			if(loja == 0){
				printf("O App de índice %d não está disponível na loja\n\n\n", id);
				system("pause");
			}
			if(loja == 1){
				i = _instalados->IL;
				//VERIFICANDO SE O APP JÁ ESTÁ INSTALADO
				//LISTA VAZIA
				if(_instalados->IL == -1){
	   			 	 insere_fila(_loja, _fila, id, _instalados, vetor);
				}
				else{
					do{
						if(_instalados->v[i].info.id == id){
						   	//NESTE CASO O APP JA ESTÁ INSTALADO
							instalado = 1;
						}
						i = _instalados->v[i].prox;
					}while(_instalados->v[i].prox != -5);
					if(instalado == 1){
						printf("O App de índice %d já está instalado\n\n\n", id);
						system("pause");
						return;
					}
					if(instalado == 0){
						insere_fila(_loja, _fila, id, _instalados, vetor);
					}
				}

			}
		}
		if(opc == 0){
			break;
		}
	}
	return;
}

void myapps(LLSE *_instalados, LLDE *_rodando, FILA *_fila, LLV *_loja, PILHA *_pilha, app vetor[9]){
	int opc, i, id, instalado, rodando;
	
	while(1){
		if(_fila->count == 0){
			break;
		}
		else{
			instalar(_instalados, _loja, _fila->v[_fila->IF].info.id);
			insere_worka(vetor, _instalados, _fila->v[_fila->IF].info.id);
			fila_remover(_fila);
		}
	}

	while(1){
		instalado = 0;
		cabecalho();
		printf("\n------MeusAppsED----------\t\t\t Count: %d\n", _instalados->count);
		imprime_instalados(_instalados);
		printf("1 - Executar ||");
		printf(" 2 - Desinstalar ||");
		printf(" 0 - voltar\n");

    	printf("Digite uma opção: ");
    	scanf("%d", &opc);

    	if(opc == 1){
			//Executar
			printf("Digite o ID do APP: ");
			scanf("%d", &id);

			if(_instalados->count == 0){
				printf("O APP de ID %d não está instalado\n\n\n", id);
				system("pause");
			}
			else{
				i = _instalados->IL;
				while(1){
					if(_instalados->v[i].info.id == id){
						instalado = 1;
						break;
					}
					if(_instalados->v[i].prox == -5){
						break;
					}
					else{
						i = _instalados->v[i].prox;
					}

				}
				if(instalado == 0){
					printf("O APP de ID %d não está instalado\n\n\n", id);
					system("pause");
				}
				if(instalado == 1){
					rodando = 0;
					if(_rodando->IL == -1){
						executa(_instalados, _rodando, id);
					}
					else{
						i = _rodando->IL;
						while(1){
							if(_rodando->v[i].info.id == id){
								rodando = 1;
								break;
							}
							if(_rodando->v[i].prox == -5){
								break;
							}
							else{
								i = _rodando->v[i].prox;
							}
		
						}
						if(rodando == 0){
							executa(_instalados, _rodando, id);
						}
						if(rodando == 1){
							pilha_push(_pilha, _rodando, id);
						}
					}
						
					
				}
			}
			
		}
		if(opc == 2){
			//Desinstalar
			printf("Digite o ID do APP: ");
			scanf("%d", &id);

			if(_instalados->count == 0){
				printf("O APP de ID %d não está instalado\n\n\n", id);
				system("pause");
			}
			else{
				//VERIFICANDO SE ESTÁ INSTALADO
				i = _instalados->IL;
				while(1){
					if(_instalados->v[i].info.id == id){
						instalado = 1;
						break;
					}
					if(_instalados->v[i].prox == -5){
						break;
					}
					else{
						i = _instalados->v[i].prox;
					}

				}
				
				//VERIFICANDO SE ESTÁ RODANDO
				if(_rodando->IL != -1){
					i = _rodando->IL;
					while(1){
						if(_rodando->v[i].info.id == id){
							rodando = 1;
							break;
						}
						if(_rodando->v[i].prox == -5){
							break;
						}
						else{
							i = _rodando->v[i].prox;
						}
	
					}
				}
				
				
				if(instalado == 0){
					printf("O APP de ID %d não está instalado\n\n\n", id);
					system("pause");
				}
				if(instalado == 1){
					desinstala(_instalados, id);
					for(i=0; i<9; i++){
						if(vetor[i].id == id){
							vetor[i].id = -1;
						}
					}
				}
				if(rodando == 1){
					killapp(_rodando, id);
				}
			}
		}
		if(opc == 0){
			break;
		}
	}
	return;
}

void imprime_instalados(LLSE *_instalados){
	int i;

	if(_instalados->IL == -1){
		return;
	}
	i = _instalados->IL;

	while(1){
		printf("\nID: %2d   \t||\t", _instalados->v[i].info.id);
        printf("NAME: %15s \t||\t", _instalados->v[i].info.name);
        printf("TAM: %4d\t", _instalados->v[i].info.tam);
        printf("|| INDEX: %d, PROX = %d", i, _instalados->v[i].prox);
        printf("\n--------------------------------------------------\n");

        if(_instalados->v[i].prox == -5){
			break;
		}
		else{
			i = _instalados->v[i].prox;
		}
	}
}


void executa(LLSE *_instalados, LLDE *_rodando, int id){
	int i, novo, elem;
	
	//DEFINIR O ELEMENTO
	i = _instalados->IL;
	while(1){
		if(_instalados->v[i].info.id == id){
			elem = i;
			break;
		}
		else{
			i = _instalados->v[i].prox;
		}
	}
	novo = aloca_noh_llde(_rodando);
	//COPIANDO OS VALORES
	_rodando->v[novo].info.id = _instalados->v[elem].info.id;
	_rodando->v[novo].info.tam = _instalados->v[elem].info.tam;
	strcpy(_rodando->v[novo].info.name, _instalados->v[elem].info.name);
	
	
	//LISTA VAZIA
	if((_rodando->IL == -1) && (_rodando->FL == -1)){
		
		_rodando->IL = novo;
		_rodando->FL = novo;
		
		_rodando->v[novo].ant = -3;
		_rodando->v[novo].prox = -5;
		return;
		
	}
	else{
		if(_instalados->v[elem].info.tam > _rodando->v[_rodando->IL].info.tam){
			_rodando->v[novo].prox = _rodando->IL;
			_rodando->v[_rodando->IL].ant = novo;
			_rodando->IL = novo;
			_rodando->v[_rodando->disp].ant == -3;
			
			return;
		}
		if(_instalados->v[elem].info.tam < _rodando->v[_rodando->FL].info.tam){
			_rodando->v[_rodando->FL].prox = novo;
			_rodando->v[novo].ant = _rodando->FL;
			_rodando->v[novo].prox = -5;
			_rodando->FL = novo;
			return;
		}
		else{
			int pos;
			i = _rodando->IL;
			while(1){
				if(_rodando->v[_rodando->v[i].prox].info.tam < _instalados->v[elem].info.tam){
					pos = i;
					break;
				}
				else{
					i = _rodando->v[i].prox;				
				}
			}
			
			_rodando->v[novo].prox = _rodando->v[pos].prox;
			_rodando->v[_rodando->v[pos].prox].ant = novo;
			_rodando->v[novo].ant = pos;
			_rodando->v[pos].prox = novo;
		}
	}
	
}

void imprime_rodando(LLDE *_rodando){
	int i;
	if((_rodando->IL == -1) && (_rodando->FL == -1)){
		return;
	}
	
	i = _rodando->IL;
	while(1){
		printf("\nID: %2d   \t||\t", _rodando->v[i].info.id);
        printf("NAME: %15s \t||\t", _rodando->v[i].info.name);
        printf("TAM: %4d\t", _rodando->v[i].info.tam);
        printf("|| INDEX: %d, PROX = %d, ANT = %d", i, _rodando->v[i].prox, _rodando->v[i].ant);
        printf("\n--------------------------------------------------\n");

        if(_rodando->v[i].prox == -5){
			break;
		}
		else{
			i = _rodando->v[i].prox;
		}
	}
}
void apprum(LLDE *_rodando){
	int opc, rodando, id, i;
	while(1){
		rodando = 0;
		cabecalho();
		printf("\n------AppRumED----------\t\t\t IL: %d , FL : %d\n\n\n", _rodando->IL, _rodando->FL);
		
		imprime_rodando(_rodando);
		
		printf(" 1 - Parar App ||");
		printf(" 2 - Parar Tudo ||");
		printf(" 0 - voltar\n");

    	printf("Digite uma opção: ");
    	scanf("%d", &opc);
    	
    	if(opc == 1){
			printf("Digite o ID do APP: ");
			scanf("%d", &id);

			if((_rodando->IL == -1) && (_rodando->FL == -1)){
				printf("O APP de ID %d não está sendo executado\n\n\n", id);
				system("pause");
			}
			else{
				i = _rodando->IL;
				while(1){
					if(_rodando->v[i].info.id == id){
						rodando = 1;
						break;
					}
					if(_rodando->v[i].prox == -5){
						break;
					}
					else{
						i = _rodando->v[i].prox;
					}

				}
				if(rodando == 0){
					printf("O APP de ID %d não está sendo executado\n\n\n", id);
					system("pause");
				}
				if(rodando == 1){
					killapp(_rodando, id);
				}
			}
		}
		if(opc == 2){
			inicializa_llde(_rodando);
		}
		if(opc == 0){
			break;
		}
	}
	return;
}

void killapp(LLDE *_rodando, int id){
	int i, disp;
	
	if(_rodando->IL == _rodando->FL){
		inicializa_llde(_rodando);
		return;
	}
	else{
		if(_rodando->v[_rodando->IL].info.id == id){
			_rodando->IL = _rodando->v[_rodando->IL].prox;
			disp = _rodando->disp;
			_rodando->disp = _rodando->v[_rodando->IL].ant;
			_rodando->v[_rodando->IL].ant = -3;
			_rodando->v[_rodando->disp].prox = disp;
			_rodando->v[disp].ant = _rodando->disp;
		}
		if(_rodando->v[_rodando->FL].info.id == id){
			disp = _rodando->disp;
			_rodando->FL = _rodando->v[_rodando->FL].ant;
			_rodando->v[_rodando->FL].prox = -5;
			_rodando->v[_rodando->disp].prox = disp;
			_rodando->v[disp].ant = _rodando->disp;
			_rodando->v[_rodando->disp].ant = -3;
		}
		else{
			int pos;
			i = _rodando->IL;
			while(1){
				if(_rodando->v[i].info.id == id){
					pos = i;
					break;
				}
				else{
					i = _rodando->v[i].prox;
				}
			}
			disp = _rodando->disp;
			_rodando->v[_rodando->v[pos].ant].prox = _rodando->v[pos].prox;
			_rodando->v[_rodando->v[pos].prox].ant = _rodando->v[pos].ant;
			_rodando->v[disp].ant = pos;
			_rodando->v[pos].prox = disp;
			_rodando->disp = pos;
			_rodando->v[_rodando->disp].ant = -3;
			
		}
	}
	
}

void AppPilha(PILHA *_pilha){
	int opc;
	while(1){
		cabecalho();
		mostra_pilha(_pilha);
		printf(" 1 - Desempilhar ||");
		printf(" 2 - Destruir pilha ||");
		printf(" 0 - voltar\n");

    	printf("Digite uma opção: ");
    	scanf("%d", &opc);
		
		if(opc == 1){
			pilha_pop(_pilha);
			continue;
		}	
		if(opc == 2){
			pilha_destrutor(_pilha);
			continue;
		}
		if(opc == 0){
			break;
		}
	}
}

//função para inicializar a pilha
void init_pilha(PILHA *_pilha ){
	_pilha->topo = NULL;	//para indicar que está vazia
	_pilha->count = 0;	//como está vazia, nao possui nenhum elemento
}

//função para indicar que a pilha está vazia
int pilha_vazia(PILHA *_pilha){	
	if(_pilha->topo == NULL){
		//indicando que está vazia
		return 1;
	}
	else{
		//indicando que não está vazia
		return 0;
	}
}

//função para destruir a pilha
int pilha_destrutor(PILHA *_pilha){
	int aux;
	celula *_temp;
	if(pilha_vazia(_pilha)){
		return 0;
	}
	else{
		while(1){
			if(_pilha->topo == NULL){
				break;
			}
			else{
				_temp = _pilha->topo;
				_temp->prox = NULL;
				
				_pilha->topo = _pilha->topo->prox;
				free(_temp);
			}
		}
		_pilha->count = 0;
		return 1;
	}
}

//função que retorna o tamanho da pilha
int len_pilha(PILHA *_pilha){
	return _pilha->count;
}

//função para empilhar
int pilha_push(PILHA *_pilha, LLDE *_rodando, int id){
	int i;
	celula *nova_celula = (celula*) malloc(sizeof(celula));
	if(nova_celula == NULL){
		return 0;
	}
	else{
		//PARA DEFINIR O ELEMENTO
		i = _rodando->IL;
		while(1){
			if(_rodando->v[i].info.id == id){
				break;
			}
			else{
				i = _rodando->v[i].prox;
			}
		}
		//COPIANDO AS INFORMAÇÕES
		nova_celula->info.id = _rodando->v[i].info.id;
		nova_celula->info.tam = _rodando->v[i].info.tam;
		strcpy(nova_celula->info.name, _rodando->v[i].info.name);
		
		nova_celula->prox = _pilha->topo;
		
		_pilha->topo = nova_celula;
		
		_pilha->count++;
		return 1;
	}
}

int pilha_pop(PILHA *_pilha){
	celula *temp;
	
	if(pilha_vazia(_pilha) == 1){
		return 0;
	}
	else{
		temp = _pilha->topo;
		_pilha->topo = _pilha->topo->prox;
		
		temp->prox = NULL; //medida de segurança
		
		free(temp);
		_pilha->count--;
		
		return 1;
	}
}


void mostra_pilha(PILHA *_pilha){
	celula *_cursor;
	if(pilha_vazia(_pilha) == 1){
		printf("Nada para mostrar\n\n");
		return;
	}
	else{
		printf("ID: %2d NAME: %15s <--- TOPO\n", _pilha->topo->info.id, _pilha->topo->info.name);
		_cursor = _pilha->topo->prox;
		if(_cursor == NULL){
			return;
		}
		else{
			while(1){
				printf("ID: %2d NAME: %15s \n", _cursor->info.id, _cursor->info.name);
				if(_cursor->prox == NULL){
					break;
				}
				else{
					_cursor = _cursor->prox;
				}
			}
		}
			
	}
}

void insere_fila(LLV *_loja, FILA *_fila, int id, LLSE *_instalados, app vetor[9]){
	int i, novo, elem;
	
	//DEFINIR O ELEMENTO
	for(i=_loja->IL; i<=_loja->FL; i++){
		if(_loja->v[i].id == id){
			elem = i;
			break;
		}
	}
	
	if(fila_cheia(_fila) == 1){
		instalar(_instalados, _loja, _fila->v[_fila->IF].info.id);
		insere_worka(vetor, _instalados, _fila->v[_fila->IF].info.id);
		fila_remover(_fila);
		
		novo = aloca_noh_fila(_fila);
		if(fila_vazia(_fila) == 1){

			//copiando valores
			_fila->v[novo].info.id = _loja->v[elem].id;
			_fila->v[novo].info.tam = _loja->v[elem].tam;
			strcpy(_fila->v[novo].info.name, _loja->v[elem].name);
			
			_fila->IF = novo;
			_fila->FF = novo;
			
			_fila->v[novo].ant = -3;
			_fila->v[novo].prox = -5;
			_fila->count++;
			return;
		}
		else{
			_fila->v[novo].info.id = _loja->v[elem].id;
			_fila->v[novo].info.tam = _loja->v[elem].tam;
			strcpy(_fila->v[novo].info.name, _loja->v[elem].name);
			
			_fila->v[_fila->FF].prox = novo;
			_fila->v[novo].ant = _fila->FF;
			_fila->v[novo].prox = -5;
			_fila->FF = novo;
			_fila->count++;
		}
	}
	else{
		novo = aloca_noh_fila(_fila);
		if(fila_vazia(_fila) == 1){

			//copiando valores
			_fila->v[novo].info.id = _loja->v[elem].id;
			_fila->v[novo].info.tam = _loja->v[elem].tam;
			strcpy(_fila->v[novo].info.name, _loja->v[elem].name);
			
			_fila->IF = novo;
			_fila->FF = novo;
			
			_fila->v[novo].ant = -3;
			_fila->v[novo].prox = -5;
			_fila->count++;
			return;
		}
		else{
			_fila->v[novo].info.id = _loja->v[elem].id;
			_fila->v[novo].info.tam = _loja->v[elem].tam;
			strcpy(_fila->v[novo].info.name, _loja->v[elem].name);
			
			_fila->v[_fila->FF].prox = novo;
			_fila->v[novo].ant = _fila->FF;
			_fila->v[novo].prox = -5;
			_fila->FF = novo;
			_fila->count++;
		}
	}
	
}

int fila_cheia(FILA *_fila){
	if(_fila->count == 4){
		return 1;
	}
	else{
		return 0;
	}
}
int fila_vazia(FILA *_fila){
	//_stru->IF = -1;	//para informar que a lista está vazia
	//_stru->FF = -1;	
	if(_fila->count == 0){
		return 1;
	}
	else{
		return 0;
	}
}

//função para instalar
void fila_remover(FILA *_fila){
	int novo, i=0;
	
	if(fila_vazia(_fila) == 1){
		return;
	}
	else{
		if(_fila->count == 1){
			inicializa_fila(_fila);
		}
		else{
			int disp;
			_fila->IF = _fila->v[_fila->IF].prox;
			disp = _fila->disp;
			_fila->disp = _fila->v[_fila->IF].ant;
			_fila->v[_fila->IF].ant = -3;
			_fila->v[_fila->disp].prox = disp;
			_fila->v[disp].ant = _fila->disp;
			_fila->count--;
			return;
		}
			
	}
	
	
	
}

void imprime_fila(FILA *_fila){
	int i;
	
	if(fila_vazia(_fila) == 1){
		return;
	}
	else{
		i = _fila->IF;
		printf("IF: %d        FF: %d          count: %d\n", _fila->IF, _fila->FF, _fila->count);
		while(1){
			printf("id = %d\n", _fila->v[i].info.id);
			if(_fila->v[i].prox == -5){
				break;
			}
			else{
				i = _fila->v[i].prox;
			}
		}
	}
	return;
}

void ini_worka(app vetor[9]){
    int i;
    for(i=0; i<9; i++){
        vetor[i].id = -1;
    }

}

void mostra_work(app vetor[9]){
    int i=0, count=0;
        for(i=0; i<9; i++){
            if(vetor[i].id != -1){
            printf("|| %15s || ", vetor[i].name);
            if((count == 2) || (count == 5) || (count == 8)){
				printf("\n-------------------------------------------------------------------------\n");
			}
            count++;
            }
        }
        printf("\n\n");

}

void insere_worka(app vetor[9], LLSE *_instalados, int id){
	int i, elem;
	
	i = _instalados->IL;
	while(1){
	
		if(_instalados->v[i].info.id == id){
			elem = i;
			break;
		}
		if(_instalados->v[i].prox == -5){
			break;
		}
		else{
			i = _instalados->v[i].prox;
		}
	}

	
	for(i=0; i<9; i++){
		if(vetor[i].id == -1){
			vetor[i].id = _instalados->v[elem].info.id;
			vetor[i].tam = _instalados->v[elem].info.tam;
			strcpy(vetor[i].name, _instalados->v[elem].info.name);
			break;
		}
	}
	return;
}
