// Os arquivos de cabeçalho
#include <allegro5/allegro.h>
#include <random>
#include<stdio.h>
#include<vector> 

// Atributos da tela
const int LARGURA_TELA = 640;
const int ALTURA_TELA = 480;
const float FPS = 20;

using namespace std;

 
int main(void)
{
	ALLEGRO_DISPLAY *janela = NULL;
	ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
	ALLEGRO_BITMAP* boneco = NULL;
	ALLEGRO_BITMAP* left_bound = NULL;
	ALLEGRO_BITMAP* right_bound = NULL;
	ALLEGRO_BITMAP* up_bound = NULL;
	ALLEGRO_BITMAP* down_bound=NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP* comida_im = NULL;



	int comida = 100;
	int pos_com_x,pos_com_y;
	bool comeu = false;


	vector<int> posicoes_x,posicoes_y;


	std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_int_distribution<> distr_x(LARGURA_TELA/15, LARGURA_TELA -LARGURA_TELA/15); // define the range

    std::uniform_int_distribution<> distr_y(ALTURA_TELA/15, ALTURA_TELA -ALTURA_TELA/15); // define the range

	




  // Flag que condicionará nosso looping
	int sair = 0;
 
	if (!al_init()){
		fprintf(stderr, "Falha ao inicializar a Allegro.\n");
    	return -1;
  	}
 
	janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
	if (!janela){

	fprintf(stderr, "Falha ao criar janela.\n");
	return -1;
	}
 
	// Configura o título da janela
	al_set_window_title(janela, "Snake10.1");
 
 

 
	// Alocamos o retângulo central da tela
	boneco = al_create_bitmap( (int)LARGURA_TELA / 15,(int) ALTURA_TELA / 15);
	if (!boneco){

    	fprintf(stderr, "Falha ao criar bitmap.\n");
    	al_destroy_display(janela);
    	return -1;

	}


	// Alocamos o retângulo central da tela
	left_bound = al_create_bitmap( (int)LARGURA_TELA / 15,(int) ALTURA_TELA );
	if (!left_bound){

    	fprintf(stderr, "Falha ao criar bitmap.\n");
    	al_destroy_display(janela);
    	return -1;

	}

	// Alocamos o retângulo central da tela
	up_bound = al_create_bitmap( LARGURA_TELA,(int) ALTURA_TELA/15 );
	if (!up_bound){

    	fprintf(stderr, "Falha ao criar bitmap.\n");
    	al_destroy_display(janela);
    	return -1;

	}

	// Alocamos o retângulo central da tela
	right_bound = al_create_bitmap( (int)LARGURA_TELA / 15,(int) ALTURA_TELA );
	if (!right_bound){

    	fprintf(stderr, "Falha ao criar bitmap.\n");
    	al_destroy_display(janela);
    	return -1;

	}

	// Alocamos o retângulo central da tela
	down_bound = al_create_bitmap( (int)LARGURA_TELA,(int) ALTURA_TELA/15 );
	if (!down_bound){

    	fprintf(stderr, "Falha ao criar bitmap.\n");
    	al_destroy_display(janela);
    	return -1;

	}
	

	// Alocamos o retângulo central da tela
	comida_im = al_create_bitmap( (int)LARGURA_TELA/15,(int) ALTURA_TELA/15 );
	if (!comida_im){

    	fprintf(stderr, "Falha ao criar bitmap.\n");
    	al_destroy_display(janela);
    	return -1;

	}


 
 	fila_eventos = al_create_event_queue();
	if (!fila_eventos){
    
		fprintf(stderr, "Falha ao inicializar o fila de eventos.\n");
    	al_destroy_display(janela);
    	return -1;
  	}

	if (!al_install_keyboard()){

    	fprintf(stderr, "Falha ao inicializar o teclado.\n");
    	return false;
	}

	timer = al_create_timer(1.0 / FPS);
	if(!timer) {
      	fprintf(stderr, "failed to create timer!\n");
      	return -1;
   	}

	al_register_event_source(fila_eventos, al_get_keyboard_event_source());
	al_register_event_source(fila_eventos, al_get_display_event_source(janela));
	al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
 


	int x=LARGURA_TELA / 2,y=ALTURA_TELA / 2;
	int tecla = 0;

	// Set to black background
	al_clear_to_color(al_map_rgb(0, 0, 0));

	// Draw red shape
	al_set_target_bitmap(boneco);
	al_clear_to_color(al_map_rgb(255, 0, 0));
	al_set_target_bitmap(al_get_backbuffer(janela));
	al_draw_bitmap(boneco, x,y, 0);
	

	al_start_timer(timer);
 



	al_flip_display();

	int count = 0;

	while (!sair){
		
    	while(!al_is_event_queue_empty(fila_eventos)){
        
			
			ALLEGRO_EVENT evento;
        	al_wait_for_event(fila_eventos, &evento);
        	if (evento.type == ALLEGRO_EVENT_KEY_DOWN){


            	switch (evento.keyboard.keycode){
            		
					case ALLEGRO_KEY_UP:
						if(tecla!=2)
              				tecla = 1;
              		
					break;
            		
					case ALLEGRO_KEY_DOWN:
              		
					  	if(tecla!=1)
					  		tecla = 2;

              		break;
            
					case ALLEGRO_KEY_LEFT:

						if(tecla!=4)
              				tecla = 3;

              		break;
            		
					case ALLEGRO_KEY_RIGHT:

						if(tecla!=3)
              				tecla = 4;
              		break;
            	}
        	}

			

        	
			if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){

				sair = true;
        	
			}
			if(evento.type == ALLEGRO_EVENT_TIMER){
				count++;

				al_clear_to_color(al_map_rgb(0, 0, 0));

				if(comeu || count==1){
					posicoes_x.push_back(x);
					posicoes_y.push_back(y);

					pos_com_x = distr_x(eng);
					pos_com_y = distr_y(eng);

					comeu = false;


					
				}
	
				switch (tecla){
				
					case 1:
						
						y-=5;

					break;

					case 2:
						y+=5;

					break;

					case 3:
						x-=5;

					break;
					
					case 4:
						x+=5;

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

				if(x>LARGURA_TELA-LARGURA_TELA/15||x<LARGURA_TELA/15) exit(0);
				if(y>ALTURA_TELA-ALTURA_TELA/15||y<ALTURA_TELA/15) exit(0);


				
				for(int i = 1; i<posicoes_x.size();i++){

					
					if(x==posicoes_x[i]&&y==posicoes_y[i]&&count>comida) exit(0);


				}


				int c_x = (x + LARGURA_TELA/30);
				int c_y = (y + ALTURA_TELA/30);

				int c_com_x = (pos_com_x + LARGURA_TELA/30);
				int c_com_y = (pos_com_y + ALTURA_TELA/30);
				

				if(x < c_com_x && x + LARGURA_TELA/15 >= c_com_x &&y < c_com_y && y + ALTURA_TELA/30 >= c_com_y){
					comeu = true;


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

    


    
  
 
  // Desaloca os recursos utilizados na aplicação
  al_destroy_bitmap(boneco);
  return 0;
}