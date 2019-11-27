#include <iostream>
#include <allegro5/allegro.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector> 
#include <sstream>
#include <stdint.h>
#include "../../lib/de150.cpp"
#include <omp.h>
using namespace std;

// Atributos da tela
#define LARGURA_TELA 250
#define ALTURA_TELA 250
#define FPS 10

#define KEY_0 14
#define KEY_1 13
#define KEY_2 11
#define KEY_3 7

#define UP_BUTTON 1
#define DOWN_BUTTON 2
#define LEFT_BUTTON 3
#define RIGHT_BUTTON 4



	class Snake {
	private:
		enum OPS {HEX_RIGHT,HEX_LEFT,RED_LEDS,GREEN_LEDS,BUTTONS,SWITCHES};

		uint32_t number;
		uint32_t zero;
		uint32_t zerol;
		uint32_t onel;
		uint32_t numberSwitch;
		uint32_t numberButton;
		
		ALLEGRO_DISPLAY *janela;
		ALLEGRO_EVENT_QUEUE *fila_eventos;
		ALLEGRO_BITMAP* boneco;
		ALLEGRO_BITMAP* left_bound;
		ALLEGRO_BITMAP* right_bound;
		ALLEGRO_BITMAP* up_bound;
		ALLEGRO_BITMAP* down_bound;
		ALLEGRO_TIMER *timer;
		ALLEGRO_BITMAP* comida_im;

		void allegroSetup() {
			janela = NULL;
			fila_eventos = NULL;
			boneco = NULL;
			left_bound = NULL;
			right_bound = NULL;
			up_bound = NULL;
			down_bound=NULL;
			timer = NULL;
			comida_im = NULL;

			if (!al_init()){
				fprintf(stderr, "Falha ao inicializar a Allegro.\n");
		    //return -1;
		  	}
		 
			janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
			if (!janela){

		  	fprintf(stderr, "Falha ao criar janela.\n");
		  	//return -1;
			}
		 
			// Configura o título da janela
			al_set_window_title(janela, "Snake10.1");


		 
			// Alocamos o retângulo central da tela
			boneco = al_create_bitmap( (int)LARGURA_TELA / 15,(int) ALTURA_TELA / 15);
			if (!boneco){

		    	fprintf(stderr, "Falha ao criar bitmap.\n");
		    	al_destroy_display(janela);
		    	//return -1;

			}


			// Alocamos o retângulo central da tela
			left_bound = al_create_bitmap( (int)LARGURA_TELA / 15,(int) ALTURA_TELA );
			if (!left_bound){

		    	fprintf(stderr, "Falha ao criar bitmap.\n");
		    	al_destroy_display(janela);
		    	//return -1;

			}

			// Alocamos o retângulo central da tela
			up_bound = al_create_bitmap( LARGURA_TELA,(int) ALTURA_TELA/15 );
			if (!up_bound){

		    	fprintf(stderr, "Falha ao criar bitmap.\n");
		    	al_destroy_display(janela);
		    	//return -1;

			}

			// Alocamos o retângulo central da tela
			right_bound = al_create_bitmap( (int)LARGURA_TELA / 15,(int) ALTURA_TELA );
			if (!right_bound){

		    	fprintf(stderr, "Falha ao criar bitmap.\n");
		    	al_destroy_display(janela);
		    	//return -1;

			}

			// Alocamos o retângulo central da tela
			down_bound = al_create_bitmap( (int)LARGURA_TELA,(int) ALTURA_TELA/15 );
			if (!down_bound){

		    	fprintf(stderr, "Falha ao criar bitmap.\n");
		    	al_destroy_display(janela);
		    	//return -1;

			}
			

			// Alocamos o retângulo central da tela
			comida_im = al_create_bitmap( (int)LARGURA_TELA/15,(int) ALTURA_TELA/15 );
			if (!comida_im){

		    	fprintf(stderr, "Falha ao criar bitmap.\n");
		    	al_destroy_display(janela);
		    	//return -1;

			}


		 
		 	fila_eventos = al_create_event_queue();
			if (!fila_eventos){
		    
				fprintf(stderr, "Falha ao inicializar o fila de eventos.\n");
		    	al_destroy_display(janela);
		    	//return -1;
		  	}

			if (!al_install_keyboard()){

		    	fprintf(stderr, "Falha ao inicializar o teclado.\n");
		    	//return false;
			}

			timer = al_create_timer(1.0 / FPS);
			if(!timer) {
		      	fprintf(stderr, "failed to create timer!\n");
		      	//return -1;
		   	}

			al_register_event_source(fila_eventos, al_get_keyboard_event_source());
			al_register_event_source(fila_eventos, al_get_display_event_source(janela));
			al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
		}

		void manager() {
			int tid;
			#pragma omp parallel num_threads(2) private(tid)
			{
			    tid = omp_get_thread_num();

			    if (tid == 0) {
			    	cout << "entrou aq" << endl;
					system("python py-midi/py-midi.py");
				} else {
					game();
				}
			}
		}

		void game() {
			De150 de150("world!");
			de150.writeDisplays(00, 00, 0000);

			al_register_event_source(fila_eventos, al_get_keyboard_event_source());
			al_register_event_source(fila_eventos, al_get_display_event_source(janela));
			al_register_event_source(fila_eventos, al_get_timer_event_source(timer));

			int x=LARGURA_TELA / 2,y=ALTURA_TELA / 2;
			//int tecla = 0;

			// Set to black background
			al_clear_to_color(al_map_rgb(0, 0, 0));

			// Draw red shape
			al_set_target_bitmap(boneco);
			al_clear_to_color(al_map_rgb(255, 0, 0));
			al_set_target_bitmap(al_get_backbuffer(janela));
			al_draw_bitmap(boneco, x,y, 0);

			al_start_timer(timer);

			al_flip_display();

			int comida = 0;
			int pos_com_x,pos_com_y;
			int comeu = 0;

			vector<int> posicoes_x,posicoes_y;
			
		  	// Flag que condicionará nosso looping
			int sair = 0;
		 

			int count = 0;
			int tecla=0;
			uint32_t tecla_apert = 0;
			while (!sair){
				tecla_apert = de150.readButtonInput();
				tecla_apert = de150.readButtonInput();

				printf("%d\n",tecla_apert);

				if (tecla_apert<15 && tecla_apert>0){

					switch (tecla_apert){
						case KEY_3:
							if(tecla!=RIGHT_BUTTON)
								tecla = LEFT_BUTTON;
						break;

						case KEY_2:
							if(tecla!=UP_BUTTON)
								tecla = DOWN_BUTTON;
						break;

						case KEY_1:
							if(tecla!=DOWN_BUTTON)
								tecla = UP_BUTTON;
						break;

						case KEY_0:
							if(tecla!=LEFT_BUTTON)
								tecla = RIGHT_BUTTON;
						break;
					}
				}

				//printf("tecla == %d \n", tecla);
				while(!al_is_event_queue_empty(fila_eventos)){

					ALLEGRO_EVENT evento;
					al_wait_for_event(fila_eventos, &evento);


					if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
						sair = true;
					}

					if(evento.type == ALLEGRO_EVENT_TIMER){
						count++;

						al_clear_to_color(al_map_rgb(0, 0, 0));

						if(comeu || count==1){
							posicoes_x.push_back(x);
							posicoes_y.push_back(y);

							int min_x = LARGURA_TELA/15,min_y = ALTURA_TELA/15;
							int max_x = LARGURA_TELA - LARGURA_TELA/15,max_y = ALTURA_TELA - ALTURA_TELA/15; 

							pos_com_x = (rand()%(max_x+1-min_x)) + min_x;
							pos_com_y = (rand()%(max_y+1-min_y)) + min_y;

							comeu = 0;
						}

						int cinco = 5;

						switch (tecla) {

							case UP_BUTTON:
							//y-=5;
							__asm__("subl %%ebx, %%eax;" : "=a" (y) : "a" (y) , "b" (cinco));
							break;

							case DOWN_BUTTON:
							// y+=5;
							__asm__("addl %%ebx, %%eax;" : "=a" (y) : "a" (cinco) , "b" (y));
							break;

							case LEFT_BUTTON:
							//x-=5;
							__asm__("subl %%ebx, %%eax;" : "=a" (x) : "a" (x) , "b" (cinco));
							break;

							case RIGHT_BUTTON:
							//x+=5;
							__asm__("addl %%ebx, %%eax;" : "=a" (x) : "a" (cinco) , "b" (x));
							break;
						}


						for(int i =  posicoes_x.size()-1;i > 0;i--){
							posicoes_y[i] = posicoes_y[i-1];
							posicoes_x[i] = posicoes_x[i-1];
						}

						posicoes_x[0] = x;
						posicoes_y[0] = y;


						for(int i=0;i<posicoes_y.size();i++){
							al_set_target_bitmap(boneco);
							al_clear_to_color(al_map_rgb(255, 0, 0));

							al_set_target_bitmap(al_get_backbuffer(janela));
							al_draw_bitmap(boneco, posicoes_x[i],posicoes_y[i], 0);
						}

						al_set_target_bitmap(left_bound);
						al_clear_to_color(al_map_rgb(0, 255, 0));
						al_set_target_bitmap(al_get_backbuffer(janela));
						al_draw_bitmap(left_bound, 0,0, 0);

						al_set_target_bitmap(right_bound);
						al_clear_to_color(al_map_rgb(0, 255, 0));
						al_set_target_bitmap(al_get_backbuffer(janela));
						al_draw_bitmap(right_bound, LARGURA_TELA-LARGURA_TELA/15,0, 0);

						al_set_target_bitmap(up_bound);
						al_clear_to_color(al_map_rgb(0, 255, 0));
						al_set_target_bitmap(al_get_backbuffer(janela));
						al_draw_bitmap(up_bound,0,0, 0);

						al_set_target_bitmap(down_bound);
						al_clear_to_color(al_map_rgb(0, 255, 0));
						al_set_target_bitmap(al_get_backbuffer(janela));
						al_draw_bitmap(down_bound,0,ALTURA_TELA - ALTURA_TELA/15, 0);

						if(x>LARGURA_TELA-LARGURA_TELA/15||x<LARGURA_TELA/15)
							exit(0);
						if(y>ALTURA_TELA-ALTURA_TELA/15||y<ALTURA_TELA/15)
							exit(0);


						for(int i = 1; i<posicoes_x.size()&&tecla!=0;i++){
							if(x==posicoes_x[i]&&y==posicoes_y[i]&&count>comida)
								exit(0);
						}

						int c_x = (x + LARGURA_TELA/30);
						int c_y = (y + ALTURA_TELA/30);

						int c_com_x = (pos_com_x + LARGURA_TELA/30);
						int c_com_y = (pos_com_y + ALTURA_TELA/30);


						if(x < c_com_x && x + LARGURA_TELA/15 >= c_com_x &&y < c_com_y && y + ALTURA_TELA/30 >= c_com_y){
							comeu = 1;
							comida++;
							
							de150.writeDisplays(comida, 0, 0);
						}

						if(!comeu){
							al_set_target_bitmap(comida_im);
							al_clear_to_color(al_map_rgb(0,0, 255));
							al_set_target_bitmap(al_get_backbuffer(janela));
							al_draw_bitmap(comida_im,pos_com_x,pos_com_y, 0);
						}

						al_flip_display();
					}
				}
			}

			// close(dev);
			de150.closeBoard();
			// Desaloca os recursos utilizados na aplicação
			al_destroy_bitmap(boneco);
			system("sudo killall python");
		}		

	public:
		Snake(string s) {
			cout << "Hello, " << s << endl;
			
			number = 0x40794079;
			zero = 0x40404040;
			zerol = 0;
			onel = 1;
			numberSwitch = 0;
			numberButton = 20;
			allegroSetup();
			game();
		}
	};
